---
title: "Forense en bases de datos: registros, transacciones y correlación"
tags:
  - nota
  - course
  - curso
  - materia
  - guia-de-lectura
  - forense-bases-de-datos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "09"
author: Jordy
start: 2026-10-05
end: 2026-10-07
created_at: 2026-10-04
aliases:
  - "Forense en bases de datos: registros, transacciones y correlación"
  - "Materia y guía de lectura - Forense en bases de datos"
---
# Forense en bases de datos: registros, transacciones y correlación

```toc
```

## De la red a los sistemas de información

En la Lección 07 se reconstruyó una secuencia con lo que **registró el propio equipo**. En la Lección 08 se usó un **sensor de red**, que ve las comunicaciones, pero no lo que ocurre dentro de los sistemas. Esta lección mira el lugar donde finalmente quedan los datos de una organización: la **base de datos** y las aplicaciones que la usan.

Cuando un dato cambia (una cuenta bancaria, un precio, un correo de cliente), la pregunta forense ya no es qué paquetes viajaron, sino:

> ¿Qué solicitud cambió el dato, en qué transacción, si el cambio quedó realmente guardado y qué puede atribuirse a una cuenta del sistema y no a una persona?

Tres ideas ordenan la lección:

1. un sistema con base de datos deja rastros en **varias fuentes**, cada una con su propio reloj, formato y punto de vista;
2. las fuentes se unen por **identificadores** (solicitud, transacción), no por parecido ni por cercanía en el tiempo;
3. un registro muestra lo que hizo una **cuenta** o una **conexión**; llegar a una persona exige evidencia adicional.

## 1. Dónde deja rastros una base de datos

| Fuente | Qué conserva | Quién la escribe | Límite principal |
| --- | --- | --- | --- |
| Registro de la aplicación | Solicitudes de los usuarios: quién, desde dónde, qué ruta, qué resultado | El servidor de aplicación | Solo lo que el programador decidió anotar; suele estar en hora local |
| Registro del motor | Conexiones, sentencias SQL, errores, inicio y fin de transacciones | El motor (PostgreSQL, MySQL, SQL Server) | Depende de la configuración: por defecto registra poco |
| Tabla de auditoría | Cambios de datos con valor anterior y nuevo | Un *trigger* o la propia aplicación | Solo cubre las tablas auditadas y confía en lo que la aplicación declara |
| Datos persistidos | El estado actual de las tablas | El motor | Muestra el resultado, no la historia |
| Registro de transacciones (WAL en PostgreSQL) | Los cambios físicos, para recuperar la base tras una falla | El motor | Formato interno; no registra usuarios de la aplicación ni solicitudes; se recicla |
| Respaldos | Estados anteriores de la base | El proceso de respaldo | Solo los momentos en que se tomó cada copia |
| Configuración | Qué se registraba y con qué zona horaria | El administrador | Puede haber cambiado después del incidente |

En una investigación se combinan. La aplicación dice **quién pidió** el cambio; el motor dice **qué sentencia** se ejecutó y si se confirmó; la auditoría dice **qué valor cambió**; los datos persistidos confirman **qué quedó**. Ninguna fuente sola responde todas las preguntas.

Antes de analizar, se registra la configuración vigente de cada fuente: qué nivel de detalle tenía el registro, en qué zona horaria escribía, cuánto tiempo se conservaba y desde cuándo existe. Una ausencia en el registro solo significa algo si la fuente estaba configurada para registrarlo.

## 2. Cómo llega una escritura a la base

![Diagrama del recorrido de una escritura: el usuario envía una solicitud HTTP a la aplicación, la aplicación ejecuta SQL en PostgreSQL con una cuenta técnica y un trigger guarda la auditoría en la misma transacción; debajo, lo que registra cada fuente y los identificadores que las unen](recorrido-escritura-fuentes.svg)

1. El usuario inicia sesión en la aplicación desde su equipo. La aplicación le asigna una **sesión** y anota su dirección IP.
2. Cada acción del usuario es una **solicitud**. Muchas aplicaciones le asignan un identificador propio (`request_id`).
3. La aplicación no abre una conexión nueva por cada usuario: toma una conexión libre de un **pool** (conjunto de conexiones ya abiertas), y todas usan la misma **cuenta técnica** de la base, por ejemplo `crm_app`.
4. Para que la base sepa a nombre de quién trabaja, la aplicación puede declarar el usuario y la solicitud al inicio de la transacción, por ejemplo con `SET LOCAL app.usuario = 'tvera'`. `SET LOCAL` dura solo hasta el fin de esa transacción.
5. Un *trigger* (procedimiento que la base ejecuta automáticamente ante un `INSERT`, `UPDATE` o `DELETE`) copia esos valores y el cambio a una **tabla de auditoría**.

Consecuencias forenses:

- **La base ve a la aplicación, no al usuario.** La cuenta es la técnica y la dirección de origen es la del servidor de aplicación.
- **El usuario de la auditoría es una declaración.** Proviene de lo que la aplicación dijo; si la aplicación fue manipulada o una sesión fue robada, el dato sigue pareciendo correcto.
- **Una conexión del pool atiende a muchos usuarios.** El número de proceso (PID) de esa conexión identifica la conexión, no a la persona.

## 3. Transacciones: intento no es cambio

Una **transacción** agrupa sentencias que deben aplicarse todas o ninguna. En SQL:

```sql
BEGIN;
UPDATE clientes SET correo = 'nuevo@ejemplo.invalid' WHERE id = 10;
COMMIT;      -- confirma: el cambio queda guardado
-- o bien
ROLLBACK;    -- revierte: es como si el UPDATE no hubiera ocurrido
```

Si una sentencia falla dentro de la transacción (por ejemplo, viola una restricción), PostgreSQL deja la transacción en estado de error y solo admite `ROLLBACK`. Sin `BEGIN` explícito, PostgreSQL trabaja en **autocommit**: cada sentencia es una transacción que se confirma sola, sin que aparezca una línea `COMMIT` en el registro.

| Lo que se observa | Lectura forense |
| --- | --- |
| `UPDATE` seguido de error y `ROLLBACK` | Hubo un **intento**; no hay cambio persistido |
| `UPDATE` seguido de `COMMIT` | Se pidió confirmar; se corrobora con la auditoría y los datos actuales |
| `UPDATE` sin `COMMIT` ni `ROLLBACK` en la ventana | Transacción pendiente o registro incompleto; no se concluye nada sin más datos |
| `UPDATE` sin `BEGIN` ni `COMMIT` | Posible autocommit: revisar si el dato cambió |
| La aplicación respondió `200` | La aplicación **declara** éxito; se confirma en la base |
| La aplicación respondió `500` | Algo falló, pero no dice qué ni si alguna sentencia quedó confirmada |

La auditoría escrita por un *trigger* forma parte de la misma transacción: si hay `ROLLBACK`, su fila también desaparece. Por eso la ausencia de fila de auditoría es coherente con un intento revertido, y su presencia es coherente con un cambio confirmado.

Una línea `COMMIT` en el registro dice que se **ordenó** confirmar. El cambio persistido se corrobora con la auditoría y con los datos conservados: ninguna línea aislada es prueba suficiente.

### El identificador de transacción

PostgreSQL asigna un **identificador de transacción** (`xid`) cuando la transacción escribe por primera vez. Una transacción que solo lee no recibe `xid`, y el registro lo muestra como `0`. Por eso, en un registro, las líneas de `BEGIN` o `SELECT` pueden mostrar `xid=0` y la línea de error o de `COMMIT` de esa misma transacción, un número. En la tabla de auditoría el mismo valor suele guardarse como `txid`.

## 4. El registro del motor: PostgreSQL

PostgreSQL decide qué registrar según su configuración:

| Parámetro | Qué controla | Valores frecuentes |
| --- | --- | --- |
| `log_statement` | Qué sentencias se anotan | `none` (por defecto), `ddl`, `mod` (cambios de datos), `all` |
| `log_line_prefix` | Qué datos encabezan cada línea | Combinación de escapes como `%m`, `%p`, `%u` |
| `log_timezone` | Zona horaria de las horas del registro | `UTC` o una zona local |
| `log_min_duration_statement` | Registrar sentencias lentas | Milisegundos; `-1` lo desactiva |

Escapes de `log_line_prefix` que conviene reconocer:

| Escape | Significado |
| --- | --- |
| `%m` | Hora con milisegundos |
| `%p` | PID del proceso que atiende la conexión |
| `%u` | Usuario de la base |
| `%d` | Nombre de la base |
| `%a` | Nombre de la aplicación que declaró la conexión |
| `%x` | Identificador de transacción (`0` si no tiene) |
| `%h` | Dirección del cliente de la base |
| `%c` | Identificador de la sesión de la base |

Con `log_line_prefix = '%m [%p] %u@%d app=%a xid=%x '`, una línea se lee así:

```text
2026-10-01 14:22:12.520 UTC [2210] crm_app@crm app=crm xid=55121 LOG:  statement: COMMIT
```

| Fragmento | Significado |
| --- | --- |
| `2026-10-01 14:22:12.520 UTC` | Hora en UTC (`%m`) |
| `[2210]` | PID de la conexión (`%p`) |
| `crm_app@crm` | Usuario de la base y nombre de la base (`%u@%d`) |
| `app=crm` | Aplicación declarada (`%a`) |
| `xid=55121` | Identificador de transacción (`%x`) |
| `statement: COMMIT` | Sentencia registrada |

Lo que el registro del motor **no** contiene: el usuario final de la aplicación (salvo que aparezca en una sentencia como `SET LOCAL`), la IP del equipo del usuario, ni el contenido anterior de un dato.

## 5. El registro de la aplicación

Una línea típica:

```text
2026-10-01T11:22:12.540-03:00 INFO req=Q-881 session=K-31 user=tvera method=POST path=/clientes/7731/correo status=200 ms=31
```

| Campo | Qué aporta | Cuidado |
| --- | --- | --- |
| Hora con desfase (`-03:00`) | Momento en hora local, convertible a UTC | Muchas aplicaciones escriben al **terminar** la solicitud |
| `req` | Identificador de la solicitud | Solo es útil si la aplicación lo propaga a la base |
| `session` | Agrupa las solicitudes de un inicio de sesión | Una sesión robada sigue mostrando el mismo usuario |
| `user` | Cuenta de la aplicación | Es una cuenta, no una persona |
| `status` | Resultado declarado por la aplicación | No confirma lo que quedó en la base |
| `ms` | Duración de la solicitud | Permite estimar cuándo empezó: hora de la línea menos la duración |

## 6. La tabla de auditoría

Una auditoría por *trigger* suele guardar: hora, identificador de transacción, usuario de la base, usuario y solicitud declarados por la aplicación, dirección del cliente de la base, objeto afectado, campo, valor anterior y valor nuevo.

### ¿Qué hora guarda?

PostgreSQL ofrece varias funciones de hora y cada una significa algo distinto:

| Función | Devuelve |
| --- | --- |
| `now()` o `transaction_timestamp()` | Inicio de la **transacción** |
| `statement_timestamp()` | Inicio de la **sentencia** actual |
| `clock_timestamp()` | Hora real en el instante de la llamada |

Por eso la hora de la auditoría puede ser anterior al `COMMIT` del registro del motor: se tomó al ejecutar el `UPDATE`, no al confirmar. Antes de comparar horas, hay que saber qué función usa cada fuente.

## 7. El tiempo: normalizar antes de comparar

Cada fuente tiene su reloj y su formato:

| Formato | Ejemplo | Zona |
| --- | --- | --- |
| ISO 8601 con desfase | `2026-10-01T11:22:12.540-03:00` | Hora local; el desfase indica cuántas horas está detrás o delante de UTC |
| ISO 8601 con `Z` | `2026-10-01T14:22:12.515Z` | UTC |
| Texto con zona | `2026-10-01 14:22:12.520 UTC` | UTC |

Para convertir una hora con desfase a UTC se **resta el desfase**; como el desfase es negativo, restar `-03:00` equivale a **sumar tres horas**: `11:22:12.540-03:00` equivale a `14:22:12.540 UTC`. En Kali:

```bash
date -u -d '2026-10-01T11:22:12.540-03:00' '+%F %T.%3N UTC'
```

En Chile continental (`America/Santiago`), la hora oficial es UTC-04:00 en invierno y UTC-03:00 en verano. En 2026 el cambio ocurrió el 6 de septiembre: un caso de agosto y uno de octubre tienen desfases distintos. Nunca aplique el desfase de memoria; tómelo del registro o de la ficha del caso.

### Orden temporal y orden lógico

Aun en UTC, horas de servidores distintos no se comparan al milisegundo:

- los relojes tienen una **desviación** respecto del servidor de tiempo (NTP). Si la ficha informa una desviación menor a 50 ms, una diferencia de 30 ms entre dos servidores no fija cuál evento ocurrió primero;
- la aplicación escribe **al terminar**: su línea puede quedar después del `COMMIT` aunque la solicitud haya empezado antes.

Cuando las horas no bastan, el orden se apoya en la **secuencia lógica** (no hay `COMMIT` sin `BEGIN`; la respuesta sigue a la solicitud) y en los identificadores compartidos.

## 8. Preservar mientras se analiza

Una base de datos es evidencia frágil: abrirla con un programa común puede modificarla.

- SQLite crea archivos auxiliares (`-journal`, `-wal`, `-shm`) junto a la base y puede reescribir su cabecera al abrirla en modo normal.
- Un servidor en producción sigue recibiendo cambios mientras se consulta.

Prácticas mínimas:

1. trabajar sobre una **copia** o una extracción entregada, nunca sobre producción;
2. calcular el **hash** antes de analizar y comprobarlo al terminar;
3. abrir en **solo lectura**: en SQLite, `sqlite3 -readonly "file:base.sqlite?immutable=1"`. `immutable=1` declara que el archivo no puede cambiar y evita archivos auxiliares;
4. ejecutar solo **consultas `SELECT`** y registrar cada consulta usada;
5. documentar lo que hizo el propio equipo investigador. Una consulta de un administrador durante la investigación también queda en el registro del motor y no debe confundirse con el incidente.

## 9. Consultas de solo lectura

Basta un subconjunto pequeño de SQL:

| Necesidad | Consulta |
| --- | --- |
| Ver las tablas | `.tables` (comando de `sqlite3`) |
| Ver las columnas | `.schema clientes` o `SELECT * FROM clientes LIMIT 1;` |
| Contar filas | `SELECT count(*) FROM auditoria;` |
| Filtrar un objeto | `SELECT * FROM auditoria WHERE cliente_id = 7731;` |
| Ordenar por tiempo | `... ORDER BY ts_utc;` |
| Elegir columnas | `SELECT id, correo FROM clientes WHERE id = 7731;` |

En los registros de texto, `grep -n` muestra el número de línea, que es la forma de citarlos:

```bash
grep -n 'Q-881' app.log
grep -n ' 14:22:' postgresql.log
```

Cite siempre la fuente y su ubicación: «registro del motor, línea 12» o «tabla `auditoria`, fila `id = 41`».

## 10. Correlacionar fuentes

Correlacionar es demostrar que dos registros hablan **del mismo evento**. La fuerza de la relación depende del dato que las une:

| Tipo de relación | Dato que une | Ejemplo | Fuerza |
| --- | --- | --- | --- |
| **Directa** | Identificador del mismo evento | Mismo `request_id` en la aplicación y en la auditoría; mismo `xid` en el motor y en la auditoría | Alta, si el identificador es único |
| **Por valor** | Un dato que puede repetirse en eventos distintos | La misma dirección IP, cuenta bancaria, correo o identificador de cliente | Media: puede coincidir por otras razones |
| **Solo temporal** | Cercanía en el tiempo | «La respuesta llegó 30 ms después» | Baja: sirve para buscar, no para concluir |

El identificador de un **objeto** (el cliente `7731`) dice que dos registros tratan del mismo objeto, no que sean la misma transacción. Un mismo cliente puede cambiar varias veces en un día.

Una **matriz de correlación** registra cada relación con sus dos evidencias, el dato de unión, el tipo, la confianza y lo que falta para confirmarla:

| Evidencia A | Evidencia B | Dato de unión | Tipo | Confianza | Qué falta |
| --- | --- | --- | --- | --- | --- |
| Aplicación, línea N | Motor, líneas M a P | `request_id` | Directa | Alta | Nada esencial |

## 11. Atribución: cuenta, conexión y persona

| Lo que muestran los registros | Formulación correcta | Formulación incorrecta |
| --- | --- | --- |
| `user=tvera` en la aplicación | «La sesión de la cuenta `tvera`…» | «Tamara Vera cambió…» |
| `crm_app` en el motor | «La cuenta técnica de la aplicación…» | «Un usuario de base de datos desconocido…» |
| PID `2210` | «La conexión 2210 del pool…» | «El computador 2210…» |
| `client_addr` del servidor de aplicación | «La base recibió la sentencia desde el servidor de aplicación» | «El usuario se conectó desde esa IP» |
| `src` de la aplicación | «La sesión se originó desde la dirección X» | «El equipo de la persona X lo hizo» |

Para pasar de una cuenta a una persona se necesitan fuentes de otro tipo: inicio de sesión en el directorio (Active Directory), VPN, autenticación multifactor, registros del equipo, cámaras o testimonios. Siempre se propone una **explicación alternativa**: credenciales robadas, sesión abierta en un equipo comprometido, una tarea automatizada o un error de operación.

### El control del cambio también es evidencia

Muchas organizaciones exigen una **solicitud de cambio** aprobada para modificar datos sensibles (datos bancarios, precios, permisos). Comparar el cambio registrado con las solicitudes de cambio aprobadas permite distinguir un cambio respaldado de uno sin respaldo. La ausencia de solicitud no prueba fraude, pero es un hecho relevante que debe declararse.

## 12. Hecho, inferencia y límites

Los ejemplos provienen del caso resuelto de la sección 15.

| Nivel | Ejemplo |
| --- | --- |
| Dato observado | La auditoría, fila 41, registra `correo` de `info@canelo.example` a `contacto@canelo.example` con `txid 55121` |
| Relación | El registro del motor muestra `COMMIT` con `xid=55121` y `request_id` `Q-881` en la misma transacción |
| Inferencia | La solicitud `Q-881` de la sesión de `tvera` cambió el correo del cliente 7731 y el cambio quedó persistido |
| Explicación alternativa | Otra persona usó la sesión de `tvera` o sus credenciales |
| Conclusión acotada | El cambio quedó guardado por una solicitud de la cuenta `tvera`; los registros no identifican a la persona |

### Lo que los registros de una base no pueden mostrar

| Límite | Formulación correcta |
| --- | --- |
| Persona | «La cuenta `tvera` realizó la solicitud», no «Tamara Vera cambió el dato». |
| Intención | «El correo se cambió», no «se cambió para desviar mensajes». |
| Origen real | «La sesión provino de `10.20.4.18`», no «la persona estaba en su puesto». |
| Cobertura | «No se registró en las fuentes entregadas», no «no ocurrió». |
| Exactitud del reloj | «Ocurrió dentro de la misma transacción», no «ocurrió exactamente 5 ms antes». |

## 13. Mejoras de trazabilidad

Una investigación también descubre qué faltaba registrar. Una mejora útil indica **qué dato agrega** y **qué pregunta permitiría responder**:

| Mejora | Dato que agrega | Pregunta que responde |
| --- | --- | --- |
| Guardar la IP y la sesión del usuario en la auditoría | Origen de la solicitud junto al cambio | ¿Desde qué sesión y equipo se hizo el cambio, sin depender de otra fuente? |
| Incluir `%c` o el identificador de solicitud en `log_line_prefix` | Identificador en cada línea del motor | ¿Qué sentencias pertenecen a una solicitud? |
| Exigir solicitud de cambio y segunda aprobación para datos sensibles | Número de solicitud y aprobador | ¿El cambio estaba autorizado y por quién? |
| Centralizar registros con retención suficiente | Registros fuera del servidor afectado | ¿Qué ocurrió antes de la retención local? |
| Alertar cambios sensibles seguidos de operaciones críticas | Evento correlacionado | ¿Se usó un dato recién modificado? |

## 14. Errores frecuentes

| Error | Por qué ocurre | Corrección |
| --- | --- | --- |
| Comparar horas sin normalizar | Cada fuente «se ve» bien por separado | Pasar todo a UTC con el desfase de cada registro |
| Concluir éxito por el código HTTP | El `200` parece definitivo | Confirmar en el motor, la auditoría y los datos |
| Concluir que nada cambió por un `500` | El error parece total | Revisar si alguna sentencia quedó confirmada |
| Tratar el PID como identificador de persona o equipo | Es un número estable durante un rato | Usarlo solo para seguir la secuencia de una conexión |
| Leer `xid=0` como «sin transacción» | El cero parece «nada» | Significa que aún no se había asignado identificador |
| Atribuir a una persona desde el usuario de la aplicación | El nombre de usuario parece una firma | Hablar de la cuenta y buscar otras fuentes |
| Ordenar eventos por diferencias de milisegundos | Los números parecen exactos | Considerar la desviación de relojes y el orden lógico |
| Abrir la base sin modo de solo lectura | Es el comportamiento por defecto | Usar `-readonly` e `immutable=1` y comprobar el hash |
| Unir registros solo porque son cercanos en el tiempo | La coincidencia parece obvia | Buscar un identificador común o declarar la relación como temporal |
| Confundir la consulta del investigador con el incidente | Aparece en el mismo registro | Documentar las acciones propias y su hora |

## 15. Ejemplo resuelto - caso CANELO SPA (ficticio)

CANELO SPA detectó que el correo de contacto del cliente `7731` cambió sin aviso. Se pide reconstruir el cambio con tres fuentes del sistema de clientes, verificadas por hash. El caso ocurre el 1 de octubre de 2026 (UTC-03:00). Los servidores informan desviación de reloj menor a 50 ms.

### Fuentes

Registro de la aplicación (hora local):

```text
1  2026-10-01T11:20:05.100-03:00 INFO auth user=tvera result=success src=10.20.4.18 session=K-31
2  2026-10-01T11:21:40.310-03:00 ERROR req=Q-880 session=K-31 user=tvera method=POST path=/clientes/7731/correo status=500 ms=22 error=db_unique_violation
3  2026-10-01T11:22:12.540-03:00 INFO req=Q-881 session=K-31 user=tvera method=POST path=/clientes/7731/correo status=200 ms=31
```

Registro del motor (UTC):

```text
1  2026-10-01 14:21:40.290 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: BEGIN
2  2026-10-01 14:21:40.291 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: SET LOCAL app.usuario='tvera'; SET LOCAL app.request_id='Q-880'
3  2026-10-01 14:21:40.294 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: UPDATE clientes SET correo='ventas@canelo.example' WHERE id=7731
4  2026-10-01 14:21:40.296 UTC [2210] crm_app@crm app=crm xid=55120 ERROR:  duplicate key value violates unique constraint "clientes_correo_key"
5  2026-10-01 14:21:40.297 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: ROLLBACK
6  2026-10-01 14:22:12.511 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: BEGIN
7  2026-10-01 14:22:12.512 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: SET LOCAL app.usuario='tvera'; SET LOCAL app.request_id='Q-881'
8  2026-10-01 14:22:12.515 UTC [2210] crm_app@crm app=crm xid=0 LOG:  statement: UPDATE clientes SET correo='contacto@canelo.example' WHERE id=7731
9  2026-10-01 14:22:12.520 UTC [2210] crm_app@crm app=crm xid=55121 LOG:  statement: COMMIT
```

Tabla `auditoria`, filas del cliente 7731:

| id | ts_utc | txid | db_user | app_usuario | request_id | client_addr | campo | valor_anterior | valor_nuevo |
| ---: | --- | ---: | --- | --- | --- | --- | --- | --- | --- |
| 41 | `2026-10-01T14:22:12.515Z` | 55121 | `crm_app` | `tvera` | `Q-881` | `10.20.1.5` | `correo` | `info@canelo.example` | `contacto@canelo.example` |

La tabla `clientes` muestra hoy `correo = contacto@canelo.example` para el id 7731.

### Línea de tiempo normalizada

| Hito | Fuente | Hora UTC | Identificadores | Dato |
| --- | --- | --- | --- | --- |
| Inicio de sesión | App L1 | `14:20:05.100` | `K-31` | `tvera` desde `10.20.4.18` |
| Primer intento rechazado | Motor L1 a L5; app L2 | Motor `14:21:40.290` a `.297`; app `14:21:40.310` | `Q-880`, PID 2210, xid 55120 | Correo duplicado, `ROLLBACK`; app `500` |
| Segundo intento | Motor L8 | `14:22:12.515` | `Q-881`, PID 2210 | Escribe `contacto@canelo.example` |
| Auditoría | `auditoria` id 41 | `14:22:12.515` | `Q-881`, txid 55121 | Valor anterior y nuevo |
| Confirmación | Motor L9; `clientes` id 7731 | `14:22:12.520` | xid 55121 | `COMMIT`; el dato actual coincide |
| Respuesta | App L3 | `14:22:12.540` | `Q-881` | `200`; empezó cerca de `.509` (`ms=31`) |

La respuesta aparece 20 ms después del `COMMIT` porque la aplicación escribe al terminar. Esa diferencia es menor que la desviación declarada, así que el orden entre esas dos líneas se apoya en la lógica (la aplicación responde después de que la base confirma), no en los milisegundos.

### Matriz de correlación

| Evidencia A | Evidencia B | Dato de unión | Tipo | Confianza | Qué falta |
| --- | --- | --- | --- | --- | --- |
| App L2 (`Q-880`) | Motor L2 a L5 | `request_id` `Q-880`; secuencia del PID 2210 | Directa | Alta | Nada esencial |
| Motor L9 (`COMMIT`) | Auditoría id 41 | `xid` = `txid` 55121; `Q-881` | Directa | Alta | Código del *trigger* |
| Auditoría id 41 | `clientes` id 7731 | Correo `contacto@canelo.example` | Por valor | Alta, junto con lo anterior | Confirmar que no hubo cambios posteriores |
| Sesión `K-31` (`10.20.4.18`) | Equipo de un trabajador | Dirección IP | Por valor | Baja | Asignación de IP del día, inicio de sesión en el directorio |

### Conclusión de la etapa

> La solicitud `Q-880` de la sesión `K-31` (cuenta `tvera`) intentó cambiar el correo del cliente 7731 y fue revertida por una restricción (motor L1 a L5, app L2). La solicitud `Q-881` de la misma sesión lo cambió a `contacto@canelo.example` y el cambio quedó persistido: `COMMIT` con xid 55121 (motor L9), auditoría id 41 con el mismo identificador y dato actual coincidente. Los registros no identifican a la persona que usaba la cuenta; se recomienda revisar el directorio y la asignación de la dirección `10.20.4.18`.

El texto separa intento de cambio, cita cada fuente y declara qué falta. No califica el cambio como fraude ni lo atribuye a una persona.

## 16. Preparación para la actividad

En la práctica se trabajará sobre `BD-CASO03`, con un registro de aplicación, un registro de PostgreSQL y una extracción SQLite. Conviene llegar con estas respuestas claras:

1. ¿Por qué el código HTTP de la aplicación no basta para saber si un dato cambió?
2. ¿Qué identificador une la aplicación con el motor y cuál une el motor con la auditoría?
3. ¿Por qué la base registra la cuenta técnica y la IP del servidor de aplicación, y no al usuario?
4. ¿Qué significa `xid=0` en una línea del registro del motor?
5. ¿Cómo se convierte `10:15:00.000-03:00` a UTC?
6. ¿Por qué una diferencia de 20 ms entre dos servidores no fija el orden de dos eventos?
7. ¿Qué diferencia hay entre una relación directa, una por valor y una solo temporal?
8. ¿Qué falta para atribuir un cambio a una persona?

## 17. Glosario

| Término | Definición |
| --- | --- |
| Autocommit | Modo en que cada sentencia se confirma sola, sin `BEGIN` ni `COMMIT` explícitos. |
| Cuenta técnica | Cuenta de la base que usa una aplicación para todas sus conexiones. |
| `COMMIT` | Orden que confirma una transacción y hace persistentes sus cambios. |
| Correlación | Demostración de que registros de fuentes distintas corresponden al mismo evento. |
| Desfase | Diferencia entre la hora local y UTC, por ejemplo `-03:00`. |
| Desviación de reloj | Diferencia entre el reloj de un servidor y la hora de referencia. |
| Extracción | Copia de tablas o registros preparada para el análisis, separada del sistema en uso. |
| Identificador de solicitud | Valor que la aplicación asigna a cada petición (`request_id`). |
| Inmutable (`immutable=1`) | Opción de SQLite que declara que el archivo no puede cambiar y evita archivos auxiliares. |
| PID | Número del proceso que atiende una conexión a la base. |
| Pool de conexiones | Conjunto de conexiones abiertas que la aplicación reutiliza para distintos usuarios. |
| Registro del motor | Archivo donde la base anota conexiones, sentencias y errores según su configuración. |
| Restricción | Regla de la base que rechaza datos inválidos, como un valor duplicado. |
| `ROLLBACK` | Orden que revierte una transacción; sus cambios no quedan guardados. |
| `SET LOCAL` | Sentencia de PostgreSQL que fija un valor solo hasta el fin de la transacción. |
| Solicitud de cambio | Registro formal que autoriza modificar un dato o un sistema. |
| Tabla de auditoría | Tabla que guarda quién cambió qué dato, cuándo, y sus valores anterior y nuevo. |
| Transacción | Grupo de sentencias que se aplican todas o ninguna. |
| *Trigger* | Procedimiento que la base ejecuta automáticamente ante un cambio de datos. |
| UTC | Tiempo universal coordinado, referencia común para comparar horas. |
| WAL | Registro de escritura anticipada de PostgreSQL, usado para recuperar la base tras una falla. |
| `xid` / `txid` | Identificador de transacción asignado por PostgreSQL cuando la transacción escribe. |

## Fuentes

- [PostgreSQL 16 - Error Reporting and Logging](https://www.postgresql.org/docs/16/runtime-config-logging.html). `log_statement`, `log_line_prefix` y `log_timezone`.
- [PostgreSQL 16 - Transactions](https://www.postgresql.org/docs/16/tutorial-transactions.html). `BEGIN`, `COMMIT` y `ROLLBACK`.
- [PostgreSQL 16 - Date/Time Functions](https://www.postgresql.org/docs/16/functions-datetime.html). Diferencia entre `now()`, `statement_timestamp()` y `clock_timestamp()`.
- [PostgreSQL 16 - Trigger Functions](https://www.postgresql.org/docs/16/plpgsql-trigger.html). Funcionamiento de los *triggers* de auditoría.
- [PostgreSQL 16 - SET](https://www.postgresql.org/docs/16/sql-set.html). Alcance de `SET LOCAL`.
- [PostgreSQL 16 - Write-Ahead Logging](https://www.postgresql.org/docs/16/wal-intro.html). Propósito del WAL.
- [SQLite - URI Filenames](https://www.sqlite.org/uri.html). Parámetros `mode=ro` e `immutable=1`.
- [SQLite - Command Line Shell](https://www.sqlite.org/cli.html). Comandos `.tables` y `.schema`.
- [NIST SP 800-92 - Guide to Computer Security Log Management](https://csrc.nist.gov/pubs/sp/800/92/final). Gestión, sincronización y conservación de registros.
- [NIST SP 800-86 - Guide to Integrating Forensic Techniques into Incident Response](https://csrc.nist.gov/pubs/sp/800/86/final). Examen de datos de aplicaciones.
- [RFC 3339 - Date and Time on the Internet: Timestamps](https://www.rfc-editor.org/rfc/rfc3339). Formato de fecha y hora con desfase.
