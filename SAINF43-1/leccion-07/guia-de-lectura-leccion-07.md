---
title: "Forense de sistemas operativos: artefactos, eventos y línea de tiempo"
tags:
  - nota
  - course
  - curso
  - materia
  - guia-de-lectura
  - sistemas-operativos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "07"
author: Jordy
start: 2026-09-21
end: 2026-09-23
created_at: 2026-09-18
aliases:
  - "Forense de sistemas operativos: artefactos, eventos y línea de tiempo"
  - "Materia y guía de lectura - Forense de sistemas operativos"
---
# Forense de sistemas operativos: artefactos, eventos y línea de tiempo

```toc
```

## Ruta de lectura previa

Antes de la clase, prioriza las secciones 3, 4, 5, 7, 8, 9, 10 y las seis preguntas de la sección 14. La lectura completa queda como material de consulta. No es necesario memorizar los Event ID ni leer anticipadamente toda la guía de comandos: durante el laboratorio se usará como referencia paso a paso.

Al terminar la preparación debes poder explicar, con tus propias palabras:

1. por qué se conserva la hora original además de la normalizada;
2. qué campo permite unir dos eventos de una misma sesión;
3. por qué una cuenta no identifica por sí sola a una persona;
4. qué diferencia existe entre hecho observado e inferencia.

## Del encargo al sistema operativo

En la Lección 06 se decidió qué fuentes preservar y bajo qué condiciones podían aceptarse. Ahora existe una copia verificada y la pregunta cambia:

> ¿Qué hechos observables puede sostener esta copia, mediante qué artefactos, y hasta dónde llega la inferencia antes de convertirse en suposición?

El análisis de sistemas operativos no consiste en abrir archivos interesantes. Consiste en reconocer que un sistema en uso deja **registros paralelos** de la misma actividad, producidos por componentes distintos, con relojes y niveles de detalle distintos, y en usar esa redundancia para distinguir lo que ocurrió de lo que se supone que ocurrió.

Tres ideas ordenan toda la lección:

1. un artefacto es **el rastro que deja un componente del sistema**, no el hecho en sí;
2. dos artefactos que coinciden en un campo concreto valen más que diez que coinciden solo en la hora;
3. la línea de tiempo es un **instrumento de análisis**, no la conclusión del caso.

## 1. Qué registra un sistema operativo y por qué

Un sistema operativo no fue diseñado para la investigación. Registra para funcionar: para volver a abrir un archivo rápido, para reanudar una sesión, para depurar una falla, para cumplir una política de auditoría. Esa es su ventaja y su límite.

| Motivo del registro | Artefacto típico | Consecuencia para el analista |
| --- | --- | --- |
| Rendimiento | Prefetch, cachés | Existe aunque nadie lo pidió; puede vaciarse o estar desactivado |
| Funcionalidad | Sistema de archivos, Registro de Windows, tareas programadas | Es fiable en su propósito, no en el nuestro |
| Auditoría | Registro de eventos de Windows (EVTX) | Depende de la política configurada antes del hecho |
| Telemetría añadida | Sysmon, EDR, antivirus | Solo existe si alguien lo instaló y lo configuró |

De aquí se deduce una regla práctica: **la ausencia de un artefacto casi nunca demuestra la ausencia del hecho**. Puede significar que la función estaba desactivada, que la retención venció, que el artefacto se limpió o que el sistema se apagó antes de escribirlo.

### Capas de un examen de sistema operativo

| Capa | Pregunta que responde | Ejemplos |
| --- | --- | --- |
| Física / imagen | ¿Qué copia estoy examinando y está íntegra? | Segmentos E01, manifiesto SHA-256 |
| Volumen | ¿Qué particiones existen y dónde empieza cada una? | Tabla de particiones, sector inicial |
| Sistema de archivos | ¿Qué archivos hay, con qué metadatos? | NTFS, MFT, marcas de tiempo |
| Sistema operativo | ¿Qué cuentas, servicios, tareas y configuración existían? | Registro de Windows, tareas, perfiles |
| Aplicación | ¿Qué hizo un programa concreto? | EVTX de PowerShell, historial, navegador |
| Usuario | ¿Qué actividad puede asociarse a una sesión? | Inicios de sesión, archivos recientes |

Saltarse capas produce errores típicos: interpretar una ruta sin saber en qué partición está, o afirmar que un programa se ejecutó sin comprobar qué componente lo registró.

## 2. Sistemas de archivos y metadatos

### Qué es un sistema de archivos

Un sistema de archivos organiza el espacio de un volumen en estructuras que permiten localizar contenido: una tabla de descriptores y un mapa de espacio ocupado. En Windows moderno el sistema habitual es **NTFS**.

| Estructura de NTFS | Función | Valor forense |
| --- | --- | --- |
| `$MFT` | Tabla maestra: un registro por archivo o carpeta | Nombre, tamaño, marcas de tiempo, ubicación de los datos |
| Atributo `$STANDARD_INFORMATION` | Marcas de tiempo que muestra el sistema | Es el que suelen modificar las herramientas de usuario |
| Atributo `$FILE_NAME` | Marcas asociadas al nombre dentro del directorio | Se actualiza con menos operaciones; sirve para contrastar |
| `$LogFile`, `$UsnJrnl` | Registro de transacciones y diario de cambios | Puede mostrar operaciones recientes, incluso sobre archivos borrados |
| Espacio no asignado | Bloques liberados pero no sobrescritos | Puede conservar contenido de archivos eliminados |

### Las marcas de tiempo MACB

Cada archivo tiene un conjunto de marcas. La convención `MACB` se lee así:

| Sigla | Significado | Qué la cambia con frecuencia |
| --- | --- | --- |
| M (*Modified*) | Última modificación del contenido | Guardar cambios |
| A (*Accessed*) | Último acceso | Abrir, indexar, analizar; en Windows puede estar desactivada |
| C (*Changed / MFT entry*) | Último cambio de los metadatos | Renombrar, mover, cambiar permisos |
| B (*Birth*) | Creación del registro en este volumen | Crear o copiar hacia este volumen |

Consecuencias que se deben memorizar:

- **Copiar un archivo no conserva su historia.** La copia nace en el destino: su marca de creación es la de la copia, no la del original.
- **Una creación posterior a la modificación es normal** cuando el archivo se copió desde otro volumen, y no implica manipulación.
- **El acceso puede estar deshabilitado.** Windows suele limitar la actualización de la marca de acceso por rendimiento; que no cambie no prueba que nadie abrió el archivo.
- **La marca es del sistema de archivos donde está el objeto**, no del hecho investigado. Si se examina una exportación, las marcas describen la exportación.

### Archivos eliminados

Eliminar un archivo normalmente no borra su contenido: marca el registro como disponible y libera los bloques. Mientras no se sobrescriban, el contenido puede recuperarse total o parcialmente. Por eso existen dos operaciones distintas:

| Operación | Qué hace | Qué se puede afirmar |
| --- | --- | --- |
| Recuperación por metadatos | Usa el registro de la `$MFT` que aún existe | Nombre, tamaño y ubicación declarados; el contenido puede estar parcialmente sobrescrito |
| Tallado (*carving*) | Busca firmas de formato en el espacio no asignado | Que existe una estructura compatible con ese formato; el nombre y la fecha originales pueden haberse perdido |

Un archivo recuperado por tallado no viene con su ruta ni con sus marcas de tiempo. Presentarlo como si las tuviera es un error de método.

## 3. El tiempo: relojes, zonas y normalización

El tiempo es la principal causa de conclusiones equivocadas en esta etapa.

### Cuatro problemas distintos

| Problema | Descripción | Control |
| --- | --- | --- |
| Zona horaria | El mismo instante se escribe como `04:45Z` o `00:45-04:00` | Registrar la zona de cada fuente y convertir a una referencia única |
| Horario de verano | El desfase de una zona cambia según la fecha | Convertir con la fecha real del evento, no con el desfase de hoy |
| Deriva del reloj | El equipo puede adelantar o atrasar respecto de una referencia | Buscar un evento de sincronización y declarar la incertidumbre |
| Resolución y formato | Segundos, milisegundos, cien nanosegundos, epoch | Conservar el valor original íntegro |

En Chile, `America/Santiago` usa **UTC-04:00** en invierno y **UTC-03:00** en horario de verano. Un evento del 21 de agosto y otro del 21 de septiembre del mismo año pueden requerir conversiones distintas. Aplicar el mismo desfase a todo el caso es un error frecuente y silencioso.

### Regla de registro

Se conserva siempre el valor original **y** el normalizado:

```text
Hora original: 2026-08-21T04:45:31.490935Z   (fuente: Security.evtx, UTC)
Hora normalizada: 2026-08-21 00:45:31.490935 -04:00 (America/Santiago)
```

Nunca se reemplaza el valor original por el convertido. Si más tarde se descubre que la zona declarada era incorrecta, el dato original permite rehacer la conversión; el convertido, no.

### Orden no es causa

Que un evento aparezca antes que otro no demuestra que lo haya provocado. La secuencia es una condición necesaria de la causalidad, no una prueba de ella. Una correlación se sostiene por **campos de unión**, no por cercanía en el reloj.

## 4. Cuentas, identificadores y sesiones

### Cuenta no es persona

Un evento registra la **cuenta** que el sistema usó, no a la persona que estaba frente al teclado. Entre la cuenta y la persona hay supuestos: credencial no compartida, sesión no secuestrada, ausencia de automatización. Esos supuestos se verifican con otras fuentes (control de acceso físico, MFA, cámaras, testimonios), no con el sistema operativo.

### Identificadores que sí sirven para unir eventos

| Identificador | Qué identifica | Uso en correlación |
| --- | --- | --- |
| SID (`S-1-5-21-...-1003`) | Una cuenta de forma única y estable | Une eventos aunque la cuenta se renombre |
| Logon ID (`0x85020b`) | Una sesión de inicio de sesión concreta | Une todo lo ocurrido dentro de esa sesión |
| PID / identificador de proceso | Una ejecución concreta de un programa | Une proceso padre e hijo; se reutiliza con el tiempo |
| Nombre de objeto | Tarea, grupo, archivo o servicio afectado | Une el evento con el artefacto en disco |

El **Logon ID** es el campo más útil de la lección: permite decir «esto ocurrió dentro de la misma sesión» sin depender del reloj. Los SID conocidos ayudan a leer rápido: `S-1-5-18` es la cuenta `SYSTEM` del equipo y `S-1-5-19` / `S-1-5-20` son servicios locales; un SID terminado en `-500` corresponde al administrador integrado, y los que terminan en `-1000` o más son cuentas creadas localmente.

Una cadena de autenticación puede contener **dos** Logon ID válidos. El `SubjectLogonId` de 4688/4648 identifica la sesión que solicita usar otras credenciales y puede repetirse como `SubjectLogonId` en el 4624 resultante. El `TargetLogonId` de 4624/4634 identifica la nueva sesión creada para la cuenta objetivo. El identificador solicitante y el de la nueva sesión no deben confundirse: la unión fuerte usa `SubjectLogonId` entre 4688, 4648 y 4624, y `TargetLogonId` entre 4624 y 4634.

Windows también puede generar dos 4624 casi simultáneos para una misma autenticación cuando crea tokens vinculados, por ejemplo uno elevado y otro limitado. En ese caso, `TargetLinkedLogonId` relaciona ambos registros. Cada 4624 debe emparejarse con el 4634 que conserve su propio `TargetLogonId`; no se mezclan los identificadores de los dos tokens.

### Tipos de inicio de sesión

El campo `LogonType` describe **cómo** se inició la sesión:

| Tipo | Significado |
| ---: | --- |
| 2 | Interactivo, en la consola del equipo |
| 3 | Por red (por ejemplo, recurso compartido) |
| 4 | Por tarea programada (*batch*) |
| 5 | Por servicio |
| 7 | Desbloqueo de la estación |
| 10 | Interactivo remoto (escritorio remoto) |
| 11 | Interactivo con credenciales en caché |

Confundir el tipo cambia la historia por completo: un tipo 3 no demuestra presencia física, y un tipo 2 no demuestra acceso remoto.

## 5. El registro de eventos de Windows

### Estructura

Windows escribe eventos en archivos `.evtx` ubicados en `C:\Windows\System32\winevt\Logs\`. Cada evento tiene proveedor, identificador numérico, nivel, marca de tiempo en UTC y un conjunto de campos propios del identificador.

| Canal | Archivo | Contenido |
| --- | --- | --- |
| Security | `Security.evtx` | Autenticación, gestión de cuentas, auditoría de procesos y objetos |
| System | `System.evtx` | Servicios, controladores, apagados e inicios |
| Application | `Application.evtx` | Errores y mensajes de programas |
| PowerShell operativo | `Microsoft-Windows-PowerShell%4Operational.evtx` | Bloques de script y actividad del motor |
| Tareas programadas | `Microsoft-Windows-TaskScheduler%4Operational.evtx` | Registro y ejecución de tareas |

El símbolo `%4` en el nombre representa la barra `/` del nombre del canal; no es un error del archivo.

### Lo que condiciona su contenido

- **La política de auditoría.** Si la auditoría de creación de procesos no estaba activada, no existirán eventos `4688` aunque los procesos se hayan ejecutado.
- **La captura de la línea de comandos.** Es una opción adicional: sin ella, `4688` registra el ejecutable pero no los argumentos.
- **El tamaño y la rotación.** Cada canal tiene un tamaño máximo; al llenarse, los eventos antiguos se sobrescriben. La ventana real puede ser de horas en un equipo activo.
- **El borrado deliberado.** Vaciar un registro deja su propia huella (evento `1102` en Security), pero el contenido se pierde.

Por eso, antes de concluir «no hay evidencia de X», se documenta si la fuente podía haber registrado X.

### Los identificadores de esta lección

|   ID | Canal      | Hecho que registra                | Campos clave                                                               | Error habitual                                                                  |
| ---: | ---------- | --------------------------------- | -------------------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| 4624 | Security   | Inicio de sesión correcto         | `TargetUserName`, `TargetLogonId`, `LogonType`                             | Suponer presencia física sin mirar `LogonType`                                  |
| 4625 | Security   | Intento de inicio fallido         | `TargetUserName`, `Status`, `SubStatus`, `LogonType`                       | Leer un fallo aislado como ataque                                               |
| 4634 | Security   | Cierre de sesión                  | `TargetLogonId`                                                            | Interpretarlo como fin de toda actividad del usuario                            |
| 4648 | Security   | Uso de credenciales explícitas    | Cuenta solicitante, cuenta objetivo, proceso                               | Leerlo como acceso exitoso: solo registra el intento con credenciales indicadas |
| 4688 | Security   | Creación de proceso               | `NewProcessName`, `ParentProcessName`, `SubjectLogonId`, línea de comandos | Revisar todos los procesos en vez de partir de eventos de referencia            |
| 4698 | Security   | Creación de una tarea programada  | Nombre de tarea, XML, identidad de ejecución                               | No distinguir quién la creó de con qué identidad se ejecutará                   |
| 4720 | Security   | Creación de una cuenta de usuario | Cuenta creadora, cuenta creada, SID nuevo                                  | Confundir creador con creado                                                    |
| 4732 | Security   | Incorporación a un grupo local    | `MemberSid`, grupo                                                         | Suponer que el grupo es Administradores sin leerlo                              |
| 4104 | PowerShell | Bloque de script ejecutado        | `ScriptBlockText`, `ScriptBlockId`                                         | Atribuirlo a una cuenta: el evento no siempre trae Logon ID                     |

Los códigos de estado de `4625` son informativos: `0xC000006D` indica usuario incorrecto o información de autenticación inválida y el subestado `0xC000006A` precisa contraseña incorrecta, mientras que `0xC0000064` indica que la cuenta no existe. Chainsaw puede conservar `FailureReason: %%2313`; ese valor es un identificador de recurso que Windows normalmente presenta como «usuario desconocido o contraseña incorrecta», no un tercer código independiente. Para distinguir las causas se leen juntos `Status` y `SubStatus`. Un fallo con contraseña incorrecta sobre una cuenta existente no es lo mismo que una enumeración de cuentas.

### Telemetría complementaria

**Sysmon** es un componente que se instala aparte y registra con más detalle: creación de procesos con hash y línea de comandos completa, conexiones de red por proceso, carga de controladores y cambios en el Registro. Cuando existe, resuelve muchas ambigüedades de `4688`. Cuando no existe, no se puede inventar su ausencia como pérdida de evidencia: se documenta que la organización no tenía esa capacidad.

La regla es la misma para EDR y antivirus: **son fuentes de valor alto y disponibilidad incierta**. Su presencia se verifica antes de planificar el análisis.

## 6. Artefactos de ejecución y persistencia

### Prefetch

Windows guarda en `C:\Windows\Prefetch\` archivos `NOMBRE.EXE-XXXXXXXX.pf` para acelerar arranques posteriores. Cada archivo conserva el nombre del ejecutable, un contador de ejecuciones y marcas de las últimas ejecuciones.

| Permite afirmar | No permite afirmar |
| --- | --- |
| Que un ejecutable con ese nombre se ejecutó en este sistema | Qué cuenta lo ejecutó |
| Un número aproximado de ejecuciones y sus últimas marcas | Que no se ejecutó nada más: puede estar desactivado o limpiado |

En servidores suele estar deshabilitado. Su ausencia no refuta un evento `4688`.

### Tareas programadas

Una tarea se almacena como archivo XML bajo `C:\Windows\System32\Tasks\` y también queda inscrita en el Registro. El XML declara el disparador (al iniciar sesión, a una hora, al arrancar), la acción (programa y argumentos) y la identidad con la que se ejecutará.

Es un artefacto doblemente útil: **corrobora** el evento `4698` y muestra la **intención operativa** de la tarea, es decir, qué haría si se cumple su disparador. Que exista una tarea no demuestra que se haya ejecutado; para eso se revisa su canal operativo o la evidencia de la acción.

### Registro de Windows

El Registro es la base de configuración del sistema. Para esta lección bastan cuatro ideas:

| Ubicación conceptual | Contenido | Uso |
| --- | --- | --- |
| `SAM` | Cuentas locales, SID, último inicio, contador de accesos | Contrastar la existencia de una cuenta creada |
| `SYSTEM` | Servicios, zona horaria del equipo, dispositivos conectados | Confirmar la zona con la que Windows mostraba las horas |
| `SOFTWARE` | Programas instalados, configuración del sistema | Contexto de las aplicaciones observadas |
| `NTUSER.DAT` de cada perfil | Actividad por usuario: programas ejecutados, rutas recientes | Asociar actividad a un perfil, no a una persona |

Las claves de ejecución automática (*Run*) son uno de los mecanismos de persistencia más comunes junto con las tareas programadas y los servicios. En esta lección se nombran, no se analizan en profundidad.

### Otros artefactos que se estudian más adelante

Archivos LNK y *Jump Lists* (apertura de documentos), Papelera de reciclaje, Amcache y ShimCache (evidencia de presencia y ejecución), historial del navegador y `$UsnJrnl`. Se mencionan para que se reconozca su existencia; el examen exhaustivo excede el tiempo de la clase.

## 7. Trabajar sobre la copia sin alterarla

Una imagen forense se analiza **montada en modo de solo lectura** o mediante herramientas que leen la estructura sin montarla. Nunca se arranca el sistema contenido en la imagen: iniciar Windows escribe en el disco, modifica marcas de tiempo, rota registros y destruye parte de lo que se quiere observar.

| Paso | Propósito |
| --- | --- |
| Verificar el manifiesto de la copia | Confirmar que se analiza lo mismo que se preservó |
| Exponer la imagen como dispositivo | Leer el contenido sin desempaquetar 50 GiB |
| Leer la tabla de particiones | Conocer el sector inicial y el tipo de cada partición |
| Montar la partición en solo lectura | Impedir escrituras accidentales |
| Comprobar las opciones de montaje | Verificar que realmente está en modo `ro` |
| Exportar copias de los artefactos | Analizar sin tocar la partición montada |
| Desmontar en orden inverso | Evitar dejar el dispositivo bloqueado |

Al exportar un artefacto se registra su **ruta interna dentro de la imagen** y se calcula un hash de la copia exportada. La ruta interna identifica la fuente; el hash identifica el objeto analizado. Sin esos dos datos, un hallazgo no es trazable.

Un detalle que suele confundir: al montar NTFS desde Linux, los permisos que muestra el sistema son una representación del controlador, no los permisos reales de Windows, y no indican que la evidencia esté montada para escritura. La comprobación válida es la opción de montaje.

## 8. Correlación: unir hechos con fundamento

Correlacionar es demostrar que dos registros se refieren a la misma actividad. No basta con que estén cerca en el tiempo.

### Fuerza de una correlación

| Nivel | Base de la unión | Ejemplo |
| --- | --- | --- |
| Fuerte | Identificador compartido y secuencia coherente | `4648`, `4624` y `4634` con el mismo Logon ID |
| Media | Objeto compartido y proximidad temporal estrecha | `4688` de `schtasks.exe` y `4698` con el mismo nombre de tarea |
| Débil | Solo cercanía temporal | Un proceso cualquiera y un inicio de sesión en el mismo minuto |

La diferencia entre fuerte y media es la posibilidad de una explicación alternativa. Dos eventos separados por fracciones de segundo y con el mismo nombre de objeto admiten pocas alternativas; dos eventos separados por un minuto sin campo común admiten muchas.

### Plantilla de correlación

```text
Evidencia A:
Evidencia B:
Campo de unión:
Relación observada:
Inferencia:
Confianza:
Explicación alternativa o dato faltante:
```

El último campo es el que distingue un análisis de una narración. Si no se puede escribir ninguna explicación alternativa, probablemente no se buscó.

### Corroboración

Corroborar es buscar en una fuente **independiente** un rastro que debería existir si la inferencia es correcta. Si un evento afirma que se creó una tarea, el archivo XML de esa tarea debería estar en disco. Si aparece, la inferencia gana respaldo; si no aparece, se documenta la ausencia y se explican sus causas posibles. Una corroboración que usa el mismo registro que se quiere confirmar no corrobora nada.

## 9. La línea de tiempo

### Qué es y qué no es

Una línea de tiempo es una secuencia ordenada de hechos observados, cada uno con su fuente, su hora original, su hora normalizada y su identificador. **No es** el relato del incidente: es el material con el que se construye ese relato.

| Tipo | Contenido | Uso |
| --- | --- | --- |
| Línea de tiempo del sistema de archivos | Marcas MACB de los archivos | Ver actividad sobre archivos en una ventana |
| Línea de tiempo de eventos | Registros EVTX seleccionados | Ver sesiones, procesos y cambios administrativos |
| Línea de tiempo combinada (*super timeline*) | Múltiples artefactos normalizados en un solo eje | Investigaciones amplias; requiere filtrado cuidadoso |

Una línea combinada generada sin filtro produce cientos de miles de entradas y oculta el hallazgo en lugar de mostrarlo. La ventana de análisis se acota **antes** de generar.

### Cómo se construye

1. Definir la ventana temporal y justificarla con un hecho de referencia.
2. Seleccionar los eventos de referencia: los que marcan cambios administrativos o fallos, no los más numerosos.
3. Buscar, alrededor de ellos, los eventos relacionados por cuenta, sesión y objeto.
4. Normalizar todas las horas a una zona declarada y conservar la original.
5. Ordenar por hora normalizada, nunca por identificador de evento.
6. Anotar, en cada fila, qué es hecho observado y qué es inferencia.
7. Registrar los vacíos: intervalos sin datos y campos ausentes.

### Regla de lectura

```text
evento observado != intención
cuenta != persona
proximidad temporal != causalidad
artefacto ausente != acción inexistente
línea de tiempo != conclusión
```

## 10. De hecho observado a conclusión

Cada afirmación de un informe pertenece a un nivel distinto, y mezclarlos es el error más caro de la disciplina.

| Nivel | Ejemplo en esta lección |
| --- | --- |
| Dato observado | El evento `4720` registra la creación de la cuenta `respaldo` a las `01:08:12 -04:00` |
| Relación | El evento `4732` incorpora a un grupo administrativo el mismo SID creado por `4720` |
| Inferencia | Una sesión con privilegios creó una cuenta y la elevó en poco más de un minuto |
| Explicación alternativa | Un procedimiento de soporte autorizado pudo producir la misma secuencia |
| Conclusión acotada | Dentro de la ventana analizada, la secuencia observada es compatible con la creación de un acceso persistente; la evidencia disponible no identifica a la persona que operó la cuenta |

### Niveles de confianza

| Nivel | Cuándo se usa |
| --- | --- |
| Alta | Varias fuentes independientes, campos de unión explícitos y sin explicación alternativa razonable |
| Media | Una fuente sólida o varias unidas por objeto y tiempo, con alternativas posibles pero menos probables |
| Baja | Un solo registro, campos ausentes o unión basada solo en proximidad temporal |

La confianza se justifica con la evidencia, no con la impresión del analista. Bajar la confianza cuando corresponde es una señal de rigor, no de debilidad.

## 11. Límites de interpretación

| Límite | Formulación correcta |
| --- | --- |
| Identidad | «La actividad se registró bajo la cuenta X», no «X hizo». |
| Intención | «Se creó una tarea que ejecutaría Y al iniciar sesión», no «para ocultar su rastro». |
| Cobertura | «Dentro de la ventana analizada y de los canales disponibles», no «no ocurrió nada más». |
| Causalidad | «Ocurrió después de», no «a causa de». |
| Ausencia | «No se localizó el artefacto Z», no «la acción no existió». |
| Alcance | «El examen se limitó a los artefactos autorizados», no «se revisó todo el equipo». |

Estos límites no debilitan el informe: lo hacen defendible ante una revisión independiente, que es el criterio de la norma ISO/IEC 27042.

## 12. Errores frecuentes

| Error | Por qué ocurre | Corrección |
| --- | --- | --- |
| Ordenar por identificador de evento | Se lee la tabla como catálogo | Ordenar siempre por hora normalizada |
| Aplicar un desfase horario único a todo el caso | Se ignora el horario de verano | Convertir con la fecha real de cada evento |
| Reemplazar la hora original por la convertida | Se busca una tabla más limpia | Conservar ambas columnas |
| Revisar todos los procesos creados | No se eligieron eventos de referencia | Partir de fallos, cuentas, grupos y tareas |
| Confundir creador y creado en `4720` | Ambos son cuentas | Leer el sujeto y el objetivo por separado |
| Leer `4648` como acceso concedido | El nombre sugiere éxito | Registra uso de credenciales explícitas, no resultado |
| Negar una ejecución por falta de Prefetch | Se supone que el artefacto siempre existe | Documentar la ausencia y sus causas posibles |
| Copiar una línea de comandos que no está en el evento | Se completa desde la expectativa | Escribir `no registrado` |
| Analizar sobre la imagen montada con escritura | Se omite la comprobación del montaje | Verificar las opciones antes de leer |
| Exponer una contraseña visible en un comando | Se transcribe literalmente | Reemplazar por `[REDACTADO]` y dejar constancia |

## 13. Ejemplo resuelto - caso ALERCE LTDA (ficticio)

Se autoriza examinar una copia verificada del equipo `ALR-PC-12` para responder: **¿qué actividad administrativa se registró entre las 14:00 y las 15:00 del 3 de julio de 2026?** El equipo usa `America/Santiago` (UTC-04:00 en esa fecha) y el canal Security registra en UTC.

### Selección

| # | Hora original | Hora normalizada | ID | Cuenta / unión | Objeto |
| ---: | --- | --- | ---: | --- | --- |
| 1 | `18:12:04Z` | `14:12:04 -04:00` | 4625 | objetivo `jvargas`; tipo 2 | Fallo, subestado contraseña incorrecta |
| 2 | `18:12:41Z` | `14:12:41 -04:00` | 4624 | objetivo `jvargas`; `0x41c3a`; tipo 2 | Inicio correcto en consola |
| 3 | `18:20:10Z` | `14:20:10 -04:00` | 4688 | `jvargas`; `0x41c3a` | `net.exe`, padre `cmd.exe` |
| 4 | `18:20:11Z` | `14:20:11 -04:00` | 4720 | creador `jvargas`; objetivo `mantencion` | SID nuevo `...-1007` |
| 5 | `18:21:55Z` | `14:21:55 -04:00` | 4732 | `MemberSid ...-1007` | Grupo `Administradores` |
| 6 | `18:44:02Z` | `14:44:02 -04:00` | 4634 | `jvargas`; `0x41c3a` | Cierre de la sesión |

### Correlación principal

```text
Evidencia A: 4688 de net.exe, 14:20:10 -04:00, SubjectLogonId 0x41c3a
Evidencia B: 4720 de creación de la cuenta mantencion, 14:20:11 -04:00
Campo de unión: cuenta jvargas y sesión 0x41c3a; el SID creado reaparece en 4732
Relación observada: dentro de una misma sesión se ejecuta net.exe y se crea una cuenta que
  un minuto después se incorpora a un grupo administrativo
Inferencia: la cuenta nueva fue creada y elevada desde esa sesión interactiva
Confianza: alta para la secuencia; media para el mecanismo, porque 4688 no registró la línea
  de comandos en este equipo
Explicación alternativa o dato faltante: un procedimiento de soporte autorizado produce la
  misma secuencia; falta el registro de solicitudes de cambio para descartarlo
```

### Corroboración y vacíos

El SID `...-1007` aparece en la base de cuentas locales, lo que confirma que la cuenta existía al momento de la copia. No se localizó Prefetch de `net.exe`; la función está deshabilitada en el equipo según su configuración, de modo que la ausencia no refuta la ejecución. El fallo previo de las `14:12:04` es un único intento y no sustenta por sí solo una hipótesis de acceso no autorizado.

### Conclusión de la etapa

> Entre las `14:12` y las `14:44` del 3 de julio de 2026 se registró una sesión interactiva asociada a la cuenta `jvargas`, precedida por un intento fallido con contraseña incorrecta. Dentro de esa sesión se creó la cuenta local `mantencion` y se incorporó a un grupo administrativo un minuto después. La evidencia disponible no permite identificar a la persona que operó la cuenta, ni determinar si la creación respondió a un procedimiento autorizado. Se recomienda obtener el registro de solicitudes de cambio y la telemetría del agente de seguridad para el mismo intervalo.

El texto informa hechos, relaciones, una inferencia acotada y lo que falta. No atribuye intención ni identidad.

## 14. Preparación para la actividad

Durante la práctica se trabajará sobre una copia verificada de `WIN10-CASO01`, con dos canales de eventos y un artefacto obligatorio de corroboración. PowerShell y Prefetch quedan disponibles como ampliación. Conviene llegar con estas respuestas claras:

1. ¿Qué campo une dos eventos de la misma sesión?
2. ¿Qué registra `4648` y qué no registra?
3. ¿Cuál es la diferencia entre la cuenta creadora y la cuenta creada en `4720`?
4. ¿Por qué no se ordena la línea de tiempo por identificador de evento?
5. ¿Qué se escribe cuando un campo esperado no aparece en el evento?
6. ¿Qué permite afirmar la presencia de un archivo de tarea programada y qué no?

## 15. Glosario

| Término | Definición |
| --- | --- |
| Artefacto | Rastro que un componente del sistema deja como consecuencia de su funcionamiento. |
| Canal de eventos | Archivo o flujo donde un proveedor de Windows escribe sus registros. |
| Campo de unión | Dato compartido por dos registros que permite afirmar que se refieren a la misma actividad. |
| Corroboración | Confirmación de una inferencia mediante una fuente independiente. |
| Hora normalizada | Valor convertido a una zona horaria declarada para comparar fuentes distintas. |
| Logon ID | Identificador de una sesión de inicio de sesión dentro de un equipo. |
| MACB | Conjunto de marcas de modificación, acceso, cambio de metadatos y creación. |
| Persistencia | Mecanismo que permite que una acción vuelva a ejecutarse tras un reinicio o inicio de sesión. |
| Prefetch | Artefacto de rendimiento de Windows que conserva rastros de ejecución de programas. |
| SID | Identificador único y estable de una cuenta o grupo. |
| Solo lectura | Modo de acceso que impide escribir sobre la evidencia montada. |
| Tallado | Recuperación de contenido a partir de firmas de formato en espacio no asignado. |
| Ventana de análisis | Intervalo temporal acotado y justificado sobre el que se realiza el examen. |

## Fuentes

- [Microsoft Learn - Advanced security audit policy settings](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/advanced-security-audit-policy-settings). Qué eventos se generan y bajo qué política.
- [Microsoft Learn - Evento 4624](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-4624). Campos y tipos de inicio de sesión correcto.
- [Microsoft Learn - Evento 4625](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-4625). Estados, subestados y tipos de inicio fallido.
- [Microsoft Learn - Evento 4648](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-4648). Uso de credenciales explícitas y campos de correlación.
- [Microsoft Learn - Sysmon](https://learn.microsoft.com/sysinternals/downloads/sysmon). Telemetría complementaria de procesos, red y Registro.
- [NIST SP 800-86 - Guide to Integrating Forensic Techniques into Incident Response](https://csrc.nist.gov/pubs/sp/800/86/final). Proceso de examen, análisis y uso de datos del sistema operativo.
- [ISO/IEC 27042:2015 - Analysis and interpretation of digital evidence](https://www.iso.org/standard/44406.html). Validez del método, repetibilidad y revisión independiente.
- [Eric Zimmerman Tools](https://ericzimmerman.github.io/). Herramientas de análisis de artefactos de Windows y su documentación.
- [Chainsaw](https://github.com/WithSecureLabs/chainsaw). Búsqueda y filtrado de eventos EVTX desde línea de comandos.
- [The Sleuth Kit](https://sleuthkit.org/sleuthkit/docs.php). Estructuras de volumen y sistema de archivos.
- [Plaso / log2timeline](https://plaso.readthedocs.io/). Construcción de líneas de tiempo combinadas y sus límites prácticos.
