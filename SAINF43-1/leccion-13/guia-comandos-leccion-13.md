---
title: "Guía práctica de comandos Lección 13 - Análisis estático y recuperación autorizada"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "13"
author: Jordy
start: 2026-11-03
end: 2026-11-09
created_at: 2026-10-29
aliases:
  - "Guía práctica de comandos Lección 13 - Análisis estático y recuperación autorizada"
---

# Guía práctica de comandos Lección 13 - Análisis estático y recuperación autorizada

Esta guía acompaña la actividad de la Lección 13. Explica, en el mismo orden de la actividad, qué hace cada comando y cada parámetro, qué resultado se espera y qué límite tiene. El análisis estático examina un archivo **sin ejecutarlo**; la recuperación se hace **solo** con la lista de candidatos autorizada.

El paquete del caso está en `/media/sf_LAB13_ENTREGA/` (solo lectura). Se trabaja en `~/forense/LAB-13/AR-CASO06/` y se respalda en `/media/sf_LAB13_SALIDA/`. Ningún comando requiere red ni privilegios de administrador.

> [!warning] Seguridad
> No se ejecuta la muestra, no se abre con doble clic y no se sube a servicios en línea (VirusTotal u otros): subirla la publica. La recuperación usa únicamente `candidatos-autorizados.txt`; no se generan diccionarios ni se aplica fuerza bruta abierta.

## Herramientas

| Comando | Uso en la práctica |
| --- | --- |
| `findmnt` | Muestra cómo está montada una carpeta (solo lectura o escritura). |
| `sha256sum` | Calcula el hash SHA-256 de un archivo o lo compara con un manifiesto (`-c`). |
| `cp` | Copia archivos; con opciones evita sobrescribir y conserva fechas. |
| `chmod` | Cambia permisos; aquí quita el permiso de escritura. |
| `file` | Identifica el tipo de archivo por su contenido, no por su extensión. |
| `head` | Muestra el comienzo de un archivo; con `-c` cuenta bytes. |
| `od` | Muestra los bytes de un archivo en hexadecimal o como caracteres. |
| `stat` | Informa propiedades del sistema de archivos: tamaño, fechas y permisos. |
| `strings` | Extrae las secuencias de texto legible de un archivo binario. |
| `grep` | Filtra líneas o fragmentos que coinciden con un patrón. |
| `unzip` | Extrae un ZIP; con `-P` entrega la contraseña. |
| `date` | Muestra la fecha y hora con el formato indicado. |
| `tee` | Muestra la salida en pantalla y la guarda en un archivo al mismo tiempo. |

Comprueben que estén disponibles:

```bash
command -v findmnt sha256sum file od stat strings grep unzip
```

`command -v` imprime la ruta de cada herramienta encontrada. Si falta alguna, avisen al docente antes de empezar.

### Dos piezas que se repiten

- **`| tee archivo`**: el símbolo `|` (tubería) entrega la salida de un comando como entrada del siguiente. `tee` la muestra en pantalla y la guarda en `archivo`. Con `tee -a` agrega al final del archivo en lugar de reemplazarlo. El archivo guardado es el que se cita en la ficha.
- **`( cd carpeta && comando )`**: los paréntesis abren una subshell. El `cd` ocurre solo dentro de ella, así que la terminal sigue en la carpeta del caso. `&&` ejecuta el segundo comando solo si el primero tuvo éxito.

## Paso 1. Preparar las rutas

```bash
LAB=~/forense/LAB-13
COMPARTIDA=/media/sf_LAB13_ENTREGA
SALIDA=/media/sf_LAB13_SALIDA
CASO="$LAB/AR-CASO06"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_anexos}
cd "$CASO"
cp -n "$COMPARTIDA/documentos/actividad-leccion-13.md" 00_control/ficha-l13.md
chmod u+w 00_control/ficha-l13.md
```

| Parte | Qué hace |
| --- | --- |
| `LAB=...`, `COMPARTIDA=...` | Guardan rutas en variables. Después se usan como `"$COMPARTIDA"`; las comillas evitan errores si una ruta tuviera espacios. |
| `mkdir -p` | Crea las carpetas. `-p` crea también las intermedias y no falla si ya existen. |
| `{00_control,01_copias_trabajo,...}` | Expansión de llaves: la terminal la convierte en una ruta por cada nombre. |
| `cd "$CASO"` | Entra en la carpeta del caso. Todas las rutas siguientes son relativas a ella. |
| `cp -n` | Copia la actividad como ficha. `-n` (*no clobber*) no reemplaza un archivo que ya exista, así no se pierde una ficha avanzada si repiten el bloque. |
| `chmod u+w` | Da permiso de escritura al dueño (`u`) sobre la ficha. La copia hereda los permisos del paquete, que es de solo lectura; sin este paso no podrían guardar sus respuestas. |

| Carpeta | Propósito |
| --- | --- |
| `00_control` | Ficha de entrega (`ficha-l13.md`) y resultados de controles de integridad |
| `01_copias_trabajo` | Copia verificada de la muestra sobre la que se examina |
| `02_salidas` | Resultados guardados de cada comando |
| `03_anexos` | Manifiesto de hashes que respalda la ficha |

Las variables existen solo en la terminal actual. Si abren otra terminal, repitan el bloque (`mkdir -p` y `cp -n` no dañan lo ya hecho).

## Paso 1. Comprobar el entorno

**Decisión:** confirmar que el paquete no se puede modificar y que la carpeta de salida admite escritura, sin probarlo creando archivos dentro del paquete.

```bash
findmnt -no TARGET,OPTIONS -T "$COMPARTIDA"
test -w "$SALIDA" && echo 'OK: salida con escritura' || echo 'ERROR: salida sin escritura'
```

| Parte | Qué hace |
| --- | --- |
| `findmnt -T ruta` | Busca el punto de montaje que contiene esa ruta. |
| `-n` | Omite la fila de títulos. |
| `-o TARGET,OPTIONS` | Muestra solo dos columnas: dónde está montado y con qué opciones. |
| `test -w ruta` | Responde éxito si se puede escribir en la ruta. No escribe nada. |
| `&& ... \|\| ...` | Si la prueba tiene éxito, imprime `OK`; si falla, imprime `ERROR`. |

**Resultado esperado:** la primera línea incluye `ro` (*read only*) entre las opciones; la segunda muestra `OK`.

**Si falla:** deténganse y avisen al docente.

## Paso 1. Verificar la integridad

**Pregunta:** ¿la muestra y el contenedor tienen exactamente los mismos bytes que cuando se prepararon?

```bash
(cd "$COMPARTIDA/muestra" && sha256sum -c --ignore-missing ../notas/AR-CASO06.sha256) | tee 00_control/verificacion-recibidos.txt
(cd "$COMPARTIDA/recuperacion" && sha256sum -c --ignore-missing ../notas/AR-CASO06.sha256) | tee -a 00_control/verificacion-recibidos.txt
```

| Parte | Qué hace |
| --- | --- |
| `cd "$COMPARTIDA/muestra"` | Entra en la carpeta del archivo, porque el manifiesto nombra los archivos sin ruta. |
| `sha256sum -c manifiesto` | Lee cada línea del manifiesto (`hash  nombre`), recalcula el hash del archivo y lo compara. Imprime `nombre: OK` o `nombre: FAILED`. |
| `--ignore-missing` | No informa los archivos del manifiesto que no están en esa carpeta. |
| `../notas/` | `..` sube una carpeta: desde `muestra/` llega a `notas/`. |
| `tee` y `tee -a` | La primera línea crea el archivo de verificación; la segunda agrega al final. |

**Por qué `--ignore-missing`:** el manifiesto lista la muestra y el contenedor, pero cada carpeta contiene solo uno. Sin la opción, el archivo ausente aparece como `FAILED open or read` aunque el material esté correcto. Por eso hay que confirmar que aparezcan **dos** líneas `OK` en total: si falta una, falta un archivo.

**Qué demuestra:** que cada archivo tiene los mismos bytes que la referencia del manifiesto.

**Qué no demuestra:** completitud de la fuente, origen auténtico ni intención.

**Si aparece `FAILED`:** deténganse, no recalculen el manifiesto y pidan una copia nueva.

## Paso 1. Crear la copia de trabajo

**Decisión:** examinar una copia identificada y dejar intacta la muestra recibida.

```bash
cp -n --preserve=timestamps "$COMPARTIDA/muestra/actualizacion-seguridad.pdf" 01_copias_trabajo/
sha256sum "$COMPARTIDA/muestra/actualizacion-seguridad.pdf" 01_copias_trabajo/actualizacion-seguridad.pdf | tee 00_control/copia.txt
chmod a-w 01_copias_trabajo/*
```

| Parte | Qué hace |
| --- | --- |
| `cp -n` | Copia sin reemplazar una copia que ya exista. |
| `--preserve=timestamps` | Conserva en la copia las fechas del original. Sin esta opción, la copia tendría la fecha y hora en que se copió. |
| `sha256sum original copia` | Sin `-c`, calcula y muestra el hash de cada archivo indicado, uno por línea. |
| `chmod a-w` | `a` = todos (dueño, grupo y otros); `-w` = quitar escritura. Reduce el riesgo de alterar la copia por error. |
| `*` | Aplica el cambio a todos los archivos de la carpeta. |

**Resultado esperado:** dos líneas con el **mismo** hash. Si difieren, la copia no es fiel: bórrenla y repitan.

## Paso 2.1. Determinar el tipo real por firma

**Pregunta:** ¿el archivo es realmente lo que dice su extensión?

```bash
file 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/tipo.txt
head -c 2 01_copias_trabajo/actualizacion-seguridad.pdf | od -An -tx1 | tee 02_salidas/magic.txt
```

| Parte | Qué hace |
| --- | --- |
| `file` | Lee los primeros bytes (*magic bytes*) y los compara con una base de firmas conocidas. Ignora la extensión. |
| `head -c 2` | Toma solo los 2 primeros bytes del archivo (`-c` cuenta bytes, no líneas). |
| `od` | *Octal dump*: muestra bytes que no se pueden leer como texto. |
| `-An` | Oculta la columna de posición (*address none*). |
| `-tx1` | Formato hexadecimal (`x`), un byte por grupo (`1`). |

**Resultado esperado:** `file` informa un ejecutable `PE32` y `od` muestra `4d 5a`, que en ASCII son las letras `MZ`.

| Firma inicial | Tipo que sugiere |
| --- | --- |
| `4d 5a` (`MZ`) | Ejecutable de Windows (PE/DOS) |
| `25 50 44 46` (`%PDF`) | Documento PDF |
| `50 4b 03 04` (`PK..`) | Contenedor ZIP (también `.docx`, `.xlsx`) |
| `7f 45 4c 46` (`.ELF`) | Ejecutable de Linux |

**Límite:** una extensión incorrecta es un **hecho observable** (discrepancia), no una prueba de intención maliciosa. Algunos archivos legítimos también se renombran. `file` no ejecuta el archivo.

## Paso 2.2. Revisar tamaño y metadatos

**Decisión:** registrar lo que informa el sistema de archivos, sin abrir el contenido como programa.

```bash
stat -c '%n  %s bytes  modificado %y' 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/metadatos.txt
```

| Parte | Qué hace |
| --- | --- |
| `stat` | Muestra la información que el sistema de archivos guarda sobre el archivo. |
| `-c 'formato'` | Imprime solo los campos indicados, con el texto que se escriba entre ellos. |
| `%n` | Nombre del archivo. |
| `%s` | Tamaño en bytes. |
| `%y` | Fecha y hora de la última modificación del contenido. |

**Resultado esperado:** nombre, `794 bytes` y la fecha de modificación del original, conservada gracias a `--preserve=timestamps`.

**Límite:** `stat` **no** lee metadatos internos como autor, firma digital o fecha de compilación. Si no se examinaron, se registran como «no examinado con estas herramientas», no como «ausente». En una investigación real, herramientas como `exiftool` amplían los metadatos internos y se registra su versión.

## Paso 2.3. Extraer y clasificar cadenas

**Pregunta:** ¿qué texto legible contiene el archivo y qué sugiere?

```bash
strings -n 6 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/cadenas.txt
```

| Parte | Qué hace |
| --- | --- |
| `strings` | Recorre el archivo e imprime cada secuencia de caracteres imprimibles, una por línea. No ejecuta ni interpreta el archivo. |
| `-n 6` | Solo muestra secuencias de 6 o más caracteres. Con un número menor aparecen muchos fragmentos sin sentido; con uno mayor se pierden cadenas cortas. |

**Resultado esperado:** rutas, una clave de registro, un dominio, nombres de funciones de Windows y mensajes.

Clasifiquen cada resultado:

| Clase | Qué es | Ejemplo de clase |
| --- | --- | --- |
| **Fuerte** | Específico de un comportamiento concreto | Una ubicación de autoarranque del registro; un dominio externo |
| **Contextual** | Capacidad posible, no comportamiento observado | Nombres de funciones de red, registro o archivos, encontrados como texto |
| **Débil o señuelo** | Común en software legítimo | `kernel32.dll`, nombres de fabricantes, mensajes genéricos |

**Límite:** una cadena aislada no es un veredicto. Que aparezca el nombre de una función indica que el archivo **contiene** ese texto; no demuestra que la importe ni que la use. Una clave propia de la aplicación que solo termina en `Run` no es una ubicación de autoarranque.

**Dominios del laboratorio:** el caso usa el sufijo reservado `.invalid`, que nunca resuelve, en lugar de dominios reales. Analícenlo como si fuera un dominio real; que no resuelva es una convención del laboratorio, no una prueba de que sea señuelo.

## Paso 2.4. Observar la estructura con una segunda vista

**Decisión:** contrastar herramientas para no depender de una sola.

```bash
strings -n 4 01_copias_trabajo/actualizacion-seguridad.pdf | grep -oE '\.(text|data|rsrc)' | tee 02_salidas/secciones.txt
od -An -c 01_copias_trabajo/actualizacion-seguridad.pdf | sed -n '1,4p'
```

| Parte | Qué hace |
| --- | --- |
| `strings -n 4` | Baja el mínimo a 4 caracteres, porque los nombres de sección son cortos (`.text` tiene 5). |
| `grep -E` | Usa expresiones regulares extendidas, que admiten `( )` y `\|` sin escaparlos. |
| `grep -o` | Imprime solo el fragmento que coincide, no la línea completa. |
| `'\.(text\|data\|rsrc)'` | Un punto literal (`\.` porque `.` solo significa «cualquier carácter») seguido de `text`, `data` o `rsrc`. |
| `od -c` | Muestra cada byte como carácter. Los bytes no imprimibles aparecen como `\0` (cero) o como número octal (por ejemplo, `220`). |
| `sed -n '1,4p'` | `-n` no imprime nada por defecto; `1,4p` imprime solo las líneas 1 a 4. |

**Resultado esperado:** tres líneas, `.text`, `.data` y `.rsrc`, y una vista de bytes que empieza con `M   Z`. En `od`, un `*` indica que se omitieron líneas iguales a la anterior.

**Por qué `-o` y sin `^`:** `strings` puede pegar al nombre de la sección un byte vecino (por ejemplo, `` `.data``). Si el patrón exigiera que la línea empiece con el punto (`^\.`), se perderían dos de las tres secciones.

`.text`, `.data` y `.rsrc` son **secciones** típicas de un ejecutable de Windows (PE): código, datos y recursos.

**Diferencias entre herramientas:** `file` resume el tipo; `strings` extrae texto; `od` muestra bytes crudos. Pueden discrepar. Se registran todas las vistas; no se elige en silencio la conveniente. Verificar la tabla de importación o el uso real de una función exige una herramienta que interprete la estructura PE, como Ghidra, que esta práctica no usa.

## Paso 2.5. Consultar la referencia ATT&CK

```bash
cat "$COMPARTIDA/notas/referencia-attack.txt"
```

`cat` muestra el contenido completo del archivo. La estación no tiene red, así que se usa esta ficha local en lugar del sitio de MITRE. Incluye técnicas que **no** corresponden a la muestra: elegir una exige justificarla con un indicador observado.

| Sobreafirmación | Redacción acotada |
| --- | --- |
| «El archivo instala persistencia.» | «La muestra **contiene** el texto de una clave de autoarranque, **compatible con** persistencia.» |
| «Se comunica con el atacante.» | «La muestra **contiene** un dominio externo y nombres de funciones de red; no se verificó que los use.» |

## Paso 3. Revisar el alcance y registrar el inicio

```bash
ZIP="$COMPARTIDA/recuperacion/AR-CASO06-contenedor.zip"
cat "$COMPARTIDA/caso/candidatos-autorizados.txt" | tee 02_salidas/alcance-recuperacion.txt
date '+inicio %F %T' | tee 02_salidas/recuperacion.txt
```

| Parte | Qué hace |
| --- | --- |
| `ZIP=...` | Guarda la ruta del contenedor para no repetirla en cada intento. |
| `cat ... \| tee` | Muestra la lista autorizada y guarda una copia como evidencia del alcance. |
| `date '+formato'` | Imprime la fecha con el formato indicado. El texto que no empieza con `%` se copia tal cual. |
| `%F` | Fecha como `AAAA-MM-DD`. |
| `%T` | Hora como `HH:MM:SS`. |

En la lista, las líneas que empiezan con `#` son comentarios: explican el alcance, no son candidatos.

## Paso 3. Probar los candidatos autorizados

**Pregunta:** ¿cuál de los candidatos autorizados abre el contenedor?

Prueben los candidatos uno por vez, en el orden de la lista. Empiecen por el primero y cambien la contraseña en cada intento:

```bash
unzip -P 'respaldo-andes' "$ZIP" -d 02_salidas/recuperado
```

| Parte | Qué hace |
| --- | --- |
| `unzip` | Extrae el contenido de un archivo ZIP. |
| `-P 'contraseña'` | Entrega la contraseña en el mismo comando, sin que `unzip` la pida. Las comillas simples evitan que la terminal interprete caracteres especiales. |
| `"$ZIP"` | El contenedor que se intenta abrir. |
| `-d 02_salidas/recuperado` | Carpeta donde se extrae; se crea si no existe. |

**Cómo leer la respuesta:**

- `skipping: texto-control.txt  incorrect password`: el candidato no sirve y no se extrae nada. Anótenlo en la ficha y prueben el siguiente.
- `inflating: 02_salidas/recuperado/texto-control.txt`: el candidato es correcto. Deténganse; no prueben los restantes.

Después del acierto, registren el término:

```bash
date '+término %F %T' | tee -a 02_salidas/recuperacion.txt
```

`tee -a` agrega la línea al final de `recuperacion.txt`, debajo de la hora de inicio.

**Qué demuestra:** acceso al contenido del contenedor.

**Qué no demuestra:** autoría, intención ni licitud del contenido recuperado.

**Si la lista se agota sin éxito:** es un **resultado válido**. No amplíen la lista ni generen candidatos: detengan y escalen.

> [!important] Diferencia clave
> Probar una **lista acotada y autorizada** de contraseñas no es lo mismo que la fuerza bruta abierta. El alcance lo fija la autorización; salir de esa lista convierte una recuperación legítima en un acceso no autorizado.

## Paso 3. Verificar el texto de control

```bash
cat 02_salidas/recuperado/texto-control.txt
sha256sum 02_salidas/recuperado/texto-control.txt | tee -a 02_salidas/recuperacion.txt
unzip -v | head -1
```

| Parte | Qué hace |
| --- | --- |
| `cat` | Muestra el texto recuperado. |
| `sha256sum archivo \| tee -a` | Calcula el hash del texto y lo agrega al registro de la recuperación. |
| `unzip -v` | Sin un archivo ZIP, muestra la versión e información de compilación de `unzip`. |
| `head -1` | Conserva solo la primera línea, que contiene la versión. |

**Resultado esperado:** el SHA-256 coincide con el valor que publica el docente, y la versión de `unzip` queda anotada en la ficha.

**Qué demuestra:** que el contenido recuperado es exactamente el esperado. No demuestra quién lo creó ni para qué.

## Paso 4. Guardar, respaldar y verificar

```bash
sha256sum 00_control/*.txt 02_salidas/*.txt 02_salidas/recuperado/texto-control.txt | tee 03_anexos/hashes-salidas.txt
cp -r 00_control 02_salidas 03_anexos "$SALIDA/"
(cd "$SALIDA" && sha256sum -c 03_anexos/hashes-salidas.txt)
```

| Parte | Qué hace |
| --- | --- |
| `*.txt` | Selecciona todos los archivos `.txt` de esa carpeta. |
| `sha256sum ... \| tee 03_anexos/hashes-salidas.txt` | Crea un manifiesto propio con el hash de cada salida. |
| `cp -r` | Copia carpetas completas con su contenido (`-r`, recursivo). |
| `(cd "$SALIDA" && sha256sum -c ...)` | Desde la carpeta de respaldo, comprueba que cada archivo copiado coincide con el manifiesto. |

**Resultado esperado:** todos los archivos del manifiesto terminan en `OK` dentro de la carpeta de salida.

La ficha `00_control/ficha-l13.md` no entra en el manifiesto porque termina en `.md` y se sigue editando hasta el final.

## Interpretar sin exceder las fuentes

| Observación | Afirmación respaldada | Afirmación que excede la fuente |
| --- | --- | --- |
| `file` reporta PE32 con extensión `.pdf` | El tipo real no corresponde a la extensión | El archivo es malware |
| `strings` muestra una clave de autoarranque | La muestra contiene el texto de una ubicación de autoarranque | La persistencia se instaló y ejecutó |
| `strings` muestra nombres de funciones de red | La muestra contiene nombres asociados a red; sus imports y su uso no se verificaron | La muestra contactó un servidor |
| `strings` muestra una clave propia que termina en `Run` | Contiene una clave de la aplicación | Es una clave de arranque automático |
| `stat` no muestra autor ni firma | Esos metadatos no se examinaron con estas herramientas | El archivo no tiene autor ni firma |
| La lista recupera el contenedor | El contenido quedó accesible | El contenido es ilícito o lo creó una persona concreta |

## Errores que obligan a detenerse o corregir el método

- Continuar tras un `FAILED` del manifiesto.
- Ejecutar o abrir la muestra, o subirla a un servicio público.
- Analizar la muestra recibida en vez de la copia verificada.
- Tratar una cadena o una extensión como veredicto de malware.
- Ampliar la lista de candidatos o seguir probando después del acierto.
- Declarar una capacidad potencial como comportamiento observado.
- Guardar una ficha sin las salidas y los hashes que permiten reproducirla.
