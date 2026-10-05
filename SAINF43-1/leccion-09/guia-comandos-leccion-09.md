---
title: "Guía práctica de comandos Lección 09 - Registros y base de datos con sqlite3"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "09"
author: Jordy
start: 2026-10-05
end: 2026-10-07
created_at: 2026-10-04
aliases:
  - "Guía práctica de comandos Lección 09 - Registros y base de datos con sqlite3"
---
# Guía práctica de comandos Lección 09 - Registros y base de datos con sqlite3

## Cómo usar esta guía

Esta guía explica los comandos de la práctica del caso `BD-CASO03`: qué hace cada uno, qué significa cada opción, cómo leer su salida y qué hacer si algo falla. La actividad ya trae los comandos que necesitas; esta guía es para entenderlos y para resolver problemas.

Ejecuta los bloques en orden y lee la salida antes de avanzar. Todos se ejecutan en una terminal de Kali.

```text
carpetas compartidas -> manifiesto OK -> leer registros (grep) -> consultar la base (sqlite3, solo SELECT)
-> convertir horas a UTC (date) -> manifiesto OK otra vez -> respaldar la bitácora
```

Ningún comando de esta guía modifica la evidencia. Los registros se leen con `grep`, `head` y `wc`, y la base se abre con `sqlite3` en modo de solo lectura. La evidencia queda siempre en `/media/sf_LAB09_ENTREGA/evidencia/`: no la copies dentro de Kali.

## Cómo leer los comandos

En los bloques aparecen símbolos que se repiten. No son parte del programa: le dicen a la terminal qué hacer.

| Símbolo | Qué significa |
| --- | --- |
| `NOMBRE=valor` | Guarda un valor en una variable. No lleva espacios alrededor del `=`. |
| `$NOMBRE` o `"$NOMBRE"` | Usa el valor guardado. Las comillas dobles evitan problemas si la ruta tuviera espacios. |
| `~` | Tu carpeta personal, por ejemplo `/home/kali`. |
| `\|` | Envía la salida del comando de la izquierda al de la derecha. |
| `tee archivo.txt` | Muestra la salida en pantalla **y** la guarda en ese archivo. |
| `( ... )` | Ejecuta lo de adentro aparte: un `cd` dentro del paréntesis no cambia la carpeta de la terminal. |
| `&&` | Ejecuta lo de la derecha solo si lo de la izquierda funcionó. |
| `\|\|` | Ejecuta lo de la derecha solo si lo de la izquierda falló. |
| `;` | Separa dos comandos en la misma línea; se ejecutan uno tras otro. |
| `'texto'` | Texto literal: la terminal no interpreta nada de lo que hay adentro. |
| `"texto"` | Texto en el que sí se reemplazan las variables (`$EV`, `$BD`). |
| `2>/dev/null` | Oculta los mensajes de error de ese comando. |

Cada bloque que guarda archivos empieza con `cd "$LAB"`. Es intencional: si ejecutas el bloque desde otra carpeta, `tee` falla con `No such file or directory` y te quedas sin registro. Si dudas de dónde estás, escribe `pwd`.

### Los comandos que vas a usar

No hay que memorizarlos; esta tabla es para consultarla cuando aparezca uno que no reconozcas.

| Comando | Qué hace |
| --- | --- |
| `command -v` | Muestra dónde está instalado un programa. Si no imprime nada, falta. |
| `mkdir -p` | Crea carpetas, sin error si ya existen. |
| `cd` | Cambia la carpeta de trabajo. |
| `pwd` | Muestra en qué carpeta estás. |
| `cp` | Copia archivos. Con `-n`, no reemplaza un archivo que ya existe. |
| `chmod u+w` | Da permiso de escritura a tu usuario sobre un archivo. |
| `touch` | Crea un archivo vacío. Aquí se usa para probar si una carpeta admite escritura. |
| `rm -f` | Borra un archivo, sin preguntar ni avisar si no existe. |
| `sha256sum -c` | Comprueba archivos contra un manifiesto de hashes. |
| `wc -l` | Cuenta las líneas de un archivo. |
| `head -3` | Muestra las tres primeras líneas de un archivo. |
| `grep -n` | Busca líneas que contengan un texto y muestra su número de línea. |
| `sqlite3` | Consola para consultar una base SQLite. |
| `date` | Muestra o convierte una fecha y hora. |
| `cmp -s` | Compara dos archivos byte a byte, sin mostrar nada; solo informa si son iguales. |
| `echo` | Escribe un texto en pantalla. |

### Vocabulario

| Término | Qué es |
| --- | --- |
| Registro (*log*) | Archivo de texto donde un programa anota, línea por línea, lo que hizo. |
| Base de datos SQLite | Base completa guardada en un solo archivo. Se consulta con `sqlite3` sin instalar un servidor. |
| Tabla, fila y columna | Una tabla guarda filas (registros) con las mismas columnas (campos). |
| Consulta `SELECT` | Pregunta a la base que solo **lee** datos. |
| URI | Forma de escribir la ruta de un archivo con opciones, como `file:ruta?immutable=1`. |
| Manifiesto | Archivo con los hashes de referencia de la evidencia. |
| UTC | Hora universal coordinada; referencia común para comparar horas de distintas fuentes. |
| Desfase | Diferencia con UTC escrita al final de una hora, por ejemplo `-03:00`. |

## Preparación previa: comprobar las herramientas

```bash
command -v sqlite3 sha256sum grep date
sqlite3 --version
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `command` | `command -v` | Busca cada programa nombrado y muestra su ruta. |
|  | `sqlite3 sha256sum grep date` | Los cuatro programas que usa la práctica. Deben aparecer cuatro rutas. |
| `sqlite3` | `--version` | Muestra la versión de SQLite. Regístrala en la bitácora. |

Si falta `sqlite3`, avisa al docente. Se instala antes de aislar la red con:

```bash
sudo apt update
sudo apt install -y sqlite3
```

| Parte | Explicación |
| --- | --- |
| `sudo` | Ejecuta con permisos de administrador. Es el único lugar de la guía donde se usa. |
| `apt update` | Actualiza la lista de programas disponibles. |
| `apt install -y sqlite3` | Instala la consola de SQLite; `-y` responde «sí» a la confirmación. |

**Precaución:** no ejecutes `sqlite3` con `sudo`. Leer una base no requiere privilegios.

## 1. Preparar el trabajo y verificar la evidencia

Es el bloque del paso 1 de la actividad:

```bash
LAB=~/forense/LAB-09
COMPARTIDA=/media/sf_LAB09_ENTREGA
RECUPERACION=/media/sf_LAB09_SALIDA
EV="$COMPARTIDA/evidencia"
BD="file:$EV/BD-CASO03.sqlite?immutable=1"
sql() { [ -n "$BD" ] || { echo "ERROR: falta definir BD (ver variables del paso 1)" >&2; return 1; }; sqlite3 -readonly -header -column "$BD" "$@"; }
mkdir -p "$LAB"/{notas,entrega}
cd "$LAB"
(cd "$EV" && sha256sum -c ../notas/BD-CASO03.sha256) | tee notas/verificacion-inicial.txt
cp -n "$COMPARTIDA/documentos/actividad-leccion-09.md" entrega/bitacora-equipo.md
chmod u+w entrega/bitacora-equipo.md
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| Variables | `LAB=~/forense/LAB-09` | Carpeta de trabajo dentro de Kali. Aquí se guardan las salidas y la bitácora. |
|  | `COMPARTIDA=/media/sf_LAB09_ENTREGA` | Carpeta compartida con la evidencia, en solo lectura. VirtualBox antepone `sf_` al nombre. |
|  | `RECUPERACION=/media/sf_LAB09_SALIDA` | Carpeta compartida con escritura, para respaldar la bitácora en Windows. |
|  | `EV="$COMPARTIDA/evidencia"` | Atajo para la carpeta donde están los tres archivos de evidencia. |
|  | `BD="file:...?immutable=1"` | Ruta de la base en formato URI. `immutable=1` declara que el archivo no puede cambiar: SQLite no lo bloquea ni crea archivos auxiliares (`-journal`, `-wal`, `-shm`) junto a él. |
| `sql() { ... }` | `sql()` | Define una **función** llamada `sql`: un atajo que podrás usar como si fuera un comando. |
|  | `[ -n "$BD" ] \|\| { ...; return 1; }` | Comprueba que `BD` tenga una ruta. Si está vacía, muestra `ERROR: falta definir BD` y no consulta nada: sin esta protección, `sqlite3` abriría una base vacía y respondería `no such table`. |
|  | `sqlite3` | Abre la base y ejecuta la consulta indicada. |
|  | `-readonly` | Abre la base en solo lectura: cualquier intento de modificarla falla. |
|  | `-header` | Muestra los nombres de las columnas en la primera línea. |
|  | `-column` | Alinea la salida en columnas para leerla mejor. |
|  | `"$@"` | Pasa a `sqlite3` lo que escribas después de `sql`, por ejemplo la consulta. |
| `mkdir` | `-p` | Crea las carpetas que falten sin error si ya existen. |
|  | `"$LAB"/{notas,entrega}` | Crea dos carpetas: `notas` para las salidas y `entrega` para la bitácora. |
| `cd` | `"$LAB"` | Se ubica en la carpeta de trabajo. |
| `( ... )` | `cd "$EV" && sha256sum -c ...` | Entra a la carpeta de evidencia y comprueba los archivos. Al cerrar el paréntesis, la terminal sigue en `$LAB`. |
| `sha256sum` | `-c` | Modo **comprobar**: calcula el hash de cada archivo y lo compara con el del manifiesto. |
|  | `../notas/BD-CASO03.sha256` | El manifiesto, que está en la carpeta `notas` del paquete, al lado de `evidencia`. |
| `tee` | `notas/verificacion-inicial.txt` | Guarda una copia del resultado para citarla en la bitácora. |
| `cp` | `-n` | Copia la actividad como bitácora **solo si todavía no existe**: nunca reemplaza un trabajo empezado. |
| `chmod` | `u+w` | Da permiso de escritura a tu usuario. Sin esto, la copia podría quedar de solo lectura, como el original. |

**Por qué se hace:** antes de analizar se demuestra que la evidencia es la misma que entregó TI. Si algo cambia durante el análisis, la verificación final lo detectará.

### Cómo leer la salida de `sha256sum -c`

| Salida | Interpretación válida | Lo que no demuestra |
| --- | --- | --- |
| `BD-CASO03.sqlite: OK` | El hash calculado coincide con el del manifiesto. | Que el contenido sea verdadero o completo. |
| `app-erp.log: FAILED` | El archivo no coincide con el registrado. | La causa de la diferencia ni quién la produjo. |
| `No such file or directory` | Falta un archivo o la carpeta no está montada. | Que la evidencia haya sido alterada. |

Resultado esperado: tres líneas terminadas en `OK`. Ante un `FAILED`, detente y avisa al docente: **registra, aísla y escala**.

### Comprobar los permisos de las carpetas

El docente confirma que la evidencia rechaza la escritura y que la salida la admite. Este bloque hace la misma prueba:

```bash
cd "$LAB"
if touch "$COMPARTIDA/.prueba" 2>/dev/null; then rm -f "$COMPARTIDA/.prueba"; echo 'ERROR: LAB09_ENTREGA admite escritura'; else echo 'OK: LAB09_ENTREGA rechaza escritura'; fi | tee notas/carpeta-evidencia.txt
if touch "$RECUPERACION/.prueba" 2>/dev/null; then rm -f "$RECUPERACION/.prueba"; echo 'OK: LAB09_SALIDA admite escritura'; else echo 'ERROR: LAB09_SALIDA no admite escritura'; fi | tee notas/carpeta-salida.txt
```

| Parte | Explicación |
| --- | --- |
| `if ...; then ...; else ...; fi` | Si el primer comando funciona, ejecuta lo de `then`; si falla, lo de `else`. |
| `touch "$COMPARTIDA/.prueba"` | Intenta crear un archivo vacío y oculto. En la carpeta de evidencia **debe fallar**. |
| `2>/dev/null` | Oculta el mensaje de error del intento, porque el resultado lo informa el `echo`. |
| `rm -f` | Si el archivo de prueba llegó a crearse, lo borra de inmediato. |
| `echo 'OK: ...'` o `echo 'ERROR: ...'` | Informa el resultado en una línea. |

**Precaución:** no uses `findmnt` para esta comprobación. Con anfitrión Windows, Kali puede mostrar ambas carpetas como `rw` aunque VirtualBox tenga marcada la opción **Solo lectura**. Lo que vale es el resultado de la prueba.

### Comando que no debes ejecutar

```text
sha256sum "$EV"/* > "$EV"/../notas/BD-CASO03.sha256
```

| Parte | Explicación |
| --- | --- |
| `sha256sum "$EV"/*` | Calcularía hashes nuevos de los archivos actuales. |
| `>` | **Reemplazaría** el manifiesto recibido por uno nuevo. |
| Consecuencia | Se perdería la referencia de TI: un archivo alterado aparecería como `OK`. |

Un manifiesto se verifica; no se vuelve a calcular. En este laboratorio la carpeta es de solo lectura y el comando fallaría, pero en un caso real podría borrar la referencia.

## 2. Mirar las fuentes antes de buscar

Opcional, pero recomendable: en dos minutos se ve qué contiene cada fuente.

```bash
cd "$LAB"
wc -l "$EV/app-erp.log" "$EV/postgresql.log"
head -3 "$EV/app-erp.log"
head -3 "$EV/postgresql.log"
sql ".tables"
sql ".schema auditoria_proveedores"
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `wc` | `-l` | Cuenta las líneas de cada archivo y muestra el total. |
| `head` | `-3` | Muestra las tres primeras líneas: sirve para ver el formato de la hora y de los campos. |
| `sql` | `".tables"` | Lista las tablas de la base. Los comandos que empiezan con punto son de la consola `sqlite3`, no de SQL. |
|  | `".schema auditoria_proveedores"` | Muestra cómo se creó la tabla: el nombre y el tipo de cada columna. |

**Qué observar:** el registro de la aplicación termina sus horas en `-03:00` (hora local) y el de PostgreSQL en `UTC`. Antes de comparar horas habrá que convertirlas.

## 3. Extraer los hitos

Son los comandos del paso 2 de la actividad:

```bash
cd "$LAB"
grep -n 'R-5102\|R-5103' "$EV/app-erp.log" | tee notas/app-solicitudes.txt
grep -n ' 13:03:\| 13:04:' "$EV/postgresql.log" | tee notas/pg-cambio.txt
sql "SELECT * FROM auditoria_proveedores WHERE proveedor_id=184 ORDER BY ts_utc;" | tee notas/auditoria-184.txt
sql "SELECT id,banco,cuenta FROM proveedores WHERE id=184;" | tee notas/proveedor-184.txt
sql "SELECT * FROM pagos WHERE id=9921;" | tee notas/pago-9921.txt
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `grep` | `-n` | Antepone el **número de línea** a cada resultado. Ese número es el que se cita: «app L6». |
|  | `'R-5102\\|R-5103'` | Busca líneas que contengan `R-5102` **o** `R-5103`. Dentro de las comillas, `\\|` significa «o». |
|  | `"$EV/app-erp.log"` | Archivo donde se busca. |
| `grep` | `' 13:03:\\| 13:04:'` | Busca las líneas de esos dos minutos. El espacio inicial evita coincidencias con otros números que contengan `13:03`. |
| `sql` | `SELECT *` | Pide todas las columnas. |
|  | `FROM auditoria_proveedores` | Tabla consultada. |
|  | `WHERE proveedor_id=184` | Solo las filas del proveedor que interesa. |
|  | `ORDER BY ts_utc` | Ordena por hora, de la más antigua a la más reciente. |
|  | `;` | Marca el fin de la consulta. |
| `sql` | `SELECT id,banco,cuenta FROM proveedores` | Muestra solo esas tres columnas: el dato **conservado hoy** en la extracción. |
| `sql` | `WHERE id=9921` | Selecciona el pago por su identificador. |
| `tee` | `notas/...txt` | Guarda cada salida para citarla después sin repetir la consulta. |

**Por qué se hace:** cada fuente aporta una parte del hecho. La aplicación dice qué solicitud se hizo y qué respondió; PostgreSQL, qué sentencia se ejecutó y si se confirmó; la auditoría y las tablas, qué valor cambió y qué quedó guardado.

### Cómo leer una línea de la aplicación

```text
6:2026-09-15T10:03:15.433-03:00 ERROR req=R-5102 session=S-7Q2 user=mrojas method=POST path=/proveedores/184/cuenta status=500 ms=37 error=db_check_violation
```

| Fragmento | Significado |
| --- | --- |
| `6:` | Número de línea que agregó `grep -n`. No es parte del registro. |
| `2026-09-15T10:03:15.433-03:00` | Hora local con su desfase. La línea se escribe **al terminar** la solicitud. |
| `req=` | Identificador de la solicitud. |
| `session=` y `user=` | Sesión y cuenta de la aplicación. Una cuenta no es una persona. |
| `method=` y `path=` | Acción pedida y recurso afectado. |
| `status=` | Código que respondió la aplicación. No confirma por sí solo lo que quedó en la base. |
| `ms=` | Duración de la solicitud en milisegundos. |

### Cómo leer una línea de PostgreSQL

```text
12:2026-09-15 13:04:02.198 UTC [4410] erp_app@erp app=erp-prov xid=88232 LOG:  statement: COMMIT
```

| Fragmento | Significado |
| --- | --- |
| `12:` | Número de línea agregado por `grep -n`. |
| `2026-09-15 13:04:02.198 UTC` | Hora en UTC. |
| `[4410]` | PID de la conexión del pool. Identifica la conexión, no a una persona. |
| `erp_app@erp` | Cuenta técnica de la aplicación y nombre de la base. |
| `app=erp-prov` | Aplicación que abrió la conexión. |
| `xid=88232` | Identificador de transacción; `0` si la transacción aún no escribía. |
| `statement: COMMIT` | Sentencia registrada. |

Las líneas `SET LOCAL app.usuario=...; SET LOCAL app.request_id=...` muestran qué usuario y qué solicitud declaró la aplicación para esa transacción. Una línea `ERROR` seguida de `ROLLBACK` indica un intento revertido.

### Cómo leer la salida de `sqlite3`

Con `-header -column`, la primera línea trae los nombres de las columnas, la segunda es una línea de guiones y cada línea siguiente es una fila. Para citar una fila, usa la tabla y su columna `id`: «auditoría, fila `id = N`». Una celda vacía significa que el valor no existe (`NULL`).

Si la fila es tan ancha que no cabe en la pantalla, usa `-line`: cada columna aparece en su propia línea.

```bash
sqlite3 -readonly -line "$BD" "SELECT * FROM auditoria_proveedores WHERE proveedor_id=184 ORDER BY ts_utc;"
```

| Parte | Explicación |
| --- | --- |
| `-line` | Muestra cada columna en su propia línea (`nombre = valor` o `nombre: valor`, según la versión de SQLite) y separa las filas con una línea en blanco. |
| `-readonly` y `"$BD"` | La misma protección que usa el atajo `sql`. |

### Comando que no debes ejecutar

```text
sqlite3 /media/sf_LAB09_ENTREGA/evidencia/BD-CASO03.sqlite
```

| Parte | Explicación |
| --- | --- |
| Ruta sin `file:` ni `immutable=1` | SQLite abriría la base en modo normal: puede intentar crear archivos auxiliares o actualizar el archivo. |
| Sin `-readonly` | Una sentencia equivocada (`UPDATE`, `DELETE`) intentaría modificar la evidencia. |

Usa siempre el atajo `sql` o escribe `-readonly` y `"$BD"`.

## 4. Convertir horas a UTC

El registro de la aplicación está en hora local. Para compararlo con PostgreSQL y la base, conviértelo:

```bash
date -u -d '2026-09-15T10:04:02.231-03:00' '+%F %T.%3N UTC'
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `date` | `-d '...'` | Interpreta la fecha y hora indicadas en lugar de usar la hora actual. |
|  | `'2026-09-15T10:04:02.231-03:00'` | Hora copiada **completa, con su desfase**, desde el registro. |
|  | `-u` | Muestra el resultado en UTC. |
|  | `'+%F %T.%3N UTC'` | Formato de salida: `%F` es la fecha, `%T` la hora, `%3N` los milisegundos. Queda igual que en PostgreSQL. |

Resultado esperado: `2026-09-15 13:04:02.231 UTC`.

**Regla manual:** para pasar de `-03:00` a UTC se **suman** tres horas. `10:03:15.433-03:00` equivale a `13:03:15.433 UTC`.

**Precaución:** si copias la hora sin el desfase, `date` usa la zona horaria de Kali y el resultado puede quedar errado en horas sin ningún aviso.

La aplicación escribe cada línea al **terminar** la solicitud. Restando el valor de `ms=` a la hora de la línea se estima cuándo empezó.

## 5. Consultas de apoyo para unir las fuentes

Útiles para el paso 3 de la actividad y para justificar el tipo de cada relación:

```bash
cd "$LAB"
grep -n '\[4410\]' "$EV/postgresql.log" | grep 'app.usuario' | tee notas/pid-4410.txt
grep -n 'S-7Q2' "$EV/app-erp.log" | tee notas/sesion.txt
sql "SELECT * FROM solicitudes_cambio;" | tee notas/solicitudes.txt
grep -n 'R-5240' "$EV/app-erp.log"; grep -n '\[4502\]' "$EV/postgresql.log"
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `grep` | `'\[4410\]'` | Líneas de la conexión 4410. Los corchetes se escriben `\[` y `\]` porque en `grep` tienen un significado especial. |
|  | `\| grep 'app.usuario'` | De esas líneas, deja solo las que declaran un usuario: muestra quiénes usaron esa conexión. |
| `grep` | `'S-7Q2'` | Todas las líneas de esa sesión: inicio, solicitudes y cierre. |
| `sql` | `SELECT * FROM solicitudes_cambio` | Solicitudes de cambio registradas, para distinguir cambios respaldados de cambios sin respaldo. |
| `grep` | `'R-5240'` y `'\[4502\]'` | La solicitud y la conexión que aprobaron el lote del pago. |
| `;` | entre los dos `grep` | Ejecuta ambas búsquedas, una tras otra. |

### Escribir tu propia consulta

Para la ampliación, la estructura es siempre la misma:

```sql
SELECT columnas FROM tabla WHERE condición ORDER BY columna;
```

| Parte | Ejemplo | Explicación |
| --- | --- | --- |
| `SELECT` | `SELECT proveedor_id, campo` | Columnas que quieres ver; `*` muestra todas. |
| `FROM` | `FROM auditoria_proveedores` | Tabla consultada. |
| `WHERE` | `WHERE campo='cuenta'` | Condición que deben cumplir las filas. El texto va entre comillas simples. |
| `ORDER BY` | `ORDER BY ts_utc` | Orden de las filas. |

Escribe la consulta entre comillas dobles después de `sql` y termínala con `;`: `sql "SELECT ... ;"`. Así las comillas simples del texto quedan dentro sin conflicto.

## 6. Verificar al terminar y respaldar

Guarda la bitácora en el editor y ejecuta el bloque del paso 4 de la actividad:

```bash
cd "$LAB"
(cd "$EV" && sha256sum -c ../notas/BD-CASO03.sha256) | tee notas/verificacion-final.txt
cp "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/"
cmp -s "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/bitacora-equipo.md" && echo 'OK: respaldo' || echo 'ERROR: revisar copia con el docente'
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `sha256sum` | `-c` (segunda vez) | Demuestra que el análisis no modificó la evidencia. Debe repetir los tres `OK`. |
| `cp` | `... "$RECUPERACION/"` | Copia solo la bitácora a la carpeta compartida con Windows. Sin `-n`: reemplaza una copia anterior. |
| `cmp` | `-s` | Compara ambas copias byte a byte sin mostrar diferencias. |
|  | `&& echo 'OK: respaldo'` | Si son idénticas, lo informa. |
|  | `\|\| echo 'ERROR: ...'` | Si difieren o falta una, avisa. |

**Por qué se hace:** los PC del laboratorio se restablecen al reiniciarse. Con el mensaje `OK`, la bitácora ya está en Windows, pero todavía no está a salvo: ábrela desde la carpeta `LAB09_SALIDA` y respáldala en tu pendrive, correo o aula virtual antes de apagar. No copies la evidencia.

## Si abres otra terminal

Las variables y el atajo `sql` solo existen en la terminal donde se definieron. En una terminal nueva, ejecuta:

```bash
LAB=~/forense/LAB-09
COMPARTIDA=/media/sf_LAB09_ENTREGA
RECUPERACION=/media/sf_LAB09_SALIDA
EV="$COMPARTIDA/evidencia"
BD="file:$EV/BD-CASO03.sqlite?immutable=1"
sql() { [ -n "$BD" ] || { echo "ERROR: falta definir BD (ver variables del paso 1)" >&2; return 1; }; sqlite3 -readonly -header -column "$BD" "$@"; }
cd "$LAB"
```

## Si continúas en otra clase

1. Copia de nuevo `LAB09_ENTREGA` al anfitrión y configura las dos carpetas compartidas.
2. Coloca en la carpeta `LAB09_SALIDA` de Windows la bitácora que respaldaste.
3. En Kali, ejecuta el bloque del paso 1 y luego recupera tu bitácora:

```bash
cp "$RECUPERACION/bitacora-equipo.md" "$LAB/entrega/bitacora-equipo.md"
```

El paso 1 usa `cp -n`, que no reemplaza una bitácora existente; por eso aquí se usa `cp` sin `-n`, que sí la reemplaza con tu trabajo guardado.

## Qué llevar a la bitácora

| Salida guardada en `notas/` | Qué registrar |
| --- | --- |
| `verificacion-inicial.txt` y `verificacion-final.txt` | Tres `OK` al comenzar y al terminar. |
| `app-solicitudes.txt` | Número de línea, hora local, solicitud y código de respuesta. |
| `pg-cambio.txt` | Números de línea, hora UTC, PID, `xid`, sentencia, error, `ROLLBACK` o `COMMIT`. |
| `auditoria-184.txt` y `proveedor-184.txt` | Fila de auditoría (`id`, `txid`, solicitud, valor anterior y nuevo) y dato conservado. |
| `pago-9921.txt` | Cuenta destino, aprobador, hora y transacción del pago. |
| `pid-4410.txt`, `sesion.txt` y `solicitudes.txt` | Apoyo para clasificar relaciones y limitar la atribución. |

No pegues salidas completas en la bitácora: cita la fuente y la línea o la fila.

## Si aparece un error

| Situación | Qué revisar |
| --- | --- |
| `sqlite3: command not found` | Avisa al docente. Mientras tanto, la pareja vecina puede ejecutar las consultas y tú registras la salida. |
| `sql: command not found` | Abriste otra terminal. Ejecuta el bloque «Si abres otra terminal». |
| `unable to open database file` | La variable `BD` está vacía o mal escrita, o la carpeta compartida no está montada. Revisa con `echo "$BD"`. |
| `attempt to write a readonly database` | Ejecutaste algo distinto de `SELECT`. La base está protegida; corrige la consulta. |
| `Parse error: near ...` | Falta una coma, una comilla o el `;` final. Copia la consulta de nuevo desde esta guía. |
| `ERROR: falta definir BD (ver variables del paso 1)` | La variable `BD` está vacía: definiste `sql` sin las variables. Ejecuta el bloque «Si abres otra terminal» completo y repite la consulta. |
| `no such table` en **todas** las consultas, a veces con `Parse error in 5th command line argument` | Mismo problema, si usaste `sqlite3` directamente o una definición antigua de `sql`: `BD` está vacía. Revisa con `echo "$BD"` y ejecuta el bloque «Si abres otra terminal». |
| `no such table` o `no such column` en una sola consulta | Nombre mal escrito. Revisa con `sql ".tables"` o `sql ".schema tabla"`. |
| `grep` no muestra nada | Revisa las comillas y que `EV` esté definida (`echo "$EV"`). |
| `tee: notas/...: No such file or directory` | Ejecutaste el bloque desde otra carpeta. Escribe `cd "$LAB"` y repítelo. |
| `date: invalid date` | Copia la hora completa con el desfase, entre comillas simples. |
| Un archivo muestra `FAILED` | Detente e informa al docente. No calcules un manifiesto nuevo. |
| `ERROR: LAB09_ENTREGA admite escritura` | Detente. Apaga Kali, marca **Solo lectura** en esa carpeta compartida de VirtualBox y repite el paso 1. |
| `ERROR: LAB09_SALIDA no admite escritura` | La carpeta de salida debe configurarse con escritura y sin **Solo lectura**. |
| No existe `/media/sf_LAB09_ENTREGA` | Apaga Kali y revisa en VirtualBox el nombre y el montaje automático de la carpeta compartida. |

## Comprobación final

- [ ] La verificación inicial y la final muestran tres `OK`.
- [ ] Cité cada hito con su fuente y su número de línea o fila.
- [ ] Convertí a UTC las horas de la aplicación con su desfase.
- [ ] Solo ejecuté consultas `SELECT` con el atajo `sql` o con `-readonly`.
- [ ] La bitácora está en `LAB09_SALIDA` (`OK: respaldo`) y la respaldé fuera del PC antes de apagar.
