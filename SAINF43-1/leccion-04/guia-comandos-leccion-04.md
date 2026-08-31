---
title: "Guía técnica Lección 04 - Adquisición del pendrive EV-01"
tags:
  - nota
  - course
  - curso
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "04"
author: Jordy
start: 2026-08-31
end: 2026-09-02
created_at: 2026-08-29
aliases:
  - "Guía técnica Lección 04 - Adquisición del pendrive EV-01"
---
# Guía técnica Lección 04 - Adquisición del pendrive EV-01

## Cómo usar esta guía

Ejecuta los bloques en orden. Lee la explicación y comprueba la salida antes de avanzar. Los comandos suponen que el paquete ya fue copiado a `~/forense/LAB-04/`.

Esta guía cubre solo la adquisición y preservación del pendrive `EV-01` del caso `PCL-2026-018`. No incluye comandos para listar, abrir ni recuperar archivos del volumen: esa etapa no está autorizada en este encargo. El contexto completo está en la `actividad-leccion-04.md`.

```text
fuente RAW -> /dev/nbd0 en modo RO -> Guymager -> E01 preservada
-> ewfverify -> comparación de contenido -> copia de trabajo
```

## Cómo leer los comandos

En los bloques aparecen algunos símbolos que se repiten. No son parte del comando: le dicen a la terminal qué hacer con él.

| Símbolo | Qué significa |
| --- | --- |
| `sudo` | Ejecuta con permisos de administrador. Solo lo llevan las órdenes que tocan dispositivos. |
| `\` al final de una línea | El comando sigue en la línea siguiente. Cópialo completo, no lo cortes ahí. |
| `\|` | Envía la salida del comando de la izquierda al de la derecha. |
| `tee archivo.txt` | Muestra la salida en pantalla **y** la guarda en ese archivo. |
| `tee -a archivo.txt` | Igual, pero agrega al final en vez de reemplazar. |
| `2>&1` | Guarda también los mensajes de error, que normalmente no pasan por `\|`. |
| `{ ... }` | Agrupa varias órdenes para que su salida vaya junta al mismo archivo. |
| `cd ruta` | Cambia la carpeta de trabajo. |

Cada bloque empieza con su propio `cd`. Es intencional: si lo ejecutas parado en otra carpeta, `tee` falla con `No such file or directory`, el comando principal se ejecuta igual y te quedas sin registro sin darte cuenta. Si dudas de dónde estás, escribe `pwd`.

### Los comandos que vas a usar

Ninguno hay que memorizarlo; esta tabla está para consultarla cuando aparezca uno que no reconozcas.

| Comando | Qué hace |
| --- | --- |
| `which` | Muestra dónde está instalado un programa. Si no imprime nada, falta. |
| `ls -l` | Lista archivos con su tamaño en bytes, permisos y fecha. |
| `ls -lR` | Igual, recorriendo también las subcarpetas. |
| `ls -A` | Lista solo los nombres. Si no muestra nada, la carpeta está vacía. |
| `cat` | Muestra el contenido de un archivo de texto. |
| `nano` | Editor de texto. Guarda con `Ctrl+O`, `Enter`; sale con `Ctrl+X`. |
| `mkdir -p` | Crea carpetas, sin error si ya existen. |
| `cp` | Copia archivos. |
| `chown` | Cambia el dueño de un archivo. |
| `date`, `timedatectl` | Fecha y hora actuales; la segunda incluye la zona horaria. |
| `realpath` | Imprime la ruta completa de un archivo o carpeta. |
| `grep` | Busca líneas que contengan un texto dentro de un archivo. |
| `echo` | Escribe un texto en pantalla. Se usa para mostrar mensajes de resultado. |
| `test -w` | Pregunta si un archivo admite escritura. No imprime nada por sí solo. |
| `udevadm settle` | Espera a que el sistema termine de reconocer un dispositivo recién conectado. |
| `md5sum`, `sha1sum`, `sha256sum` | Calculan el hash de un archivo. Con `-c` comprueban un manifiesto ya existente. |
| `lsblk` | Lista los discos y dispositivos conectados. |
| `blockdev --getro` | Responde si un dispositivo está en solo lectura: `1` sí, `0` no. |
| `blockdev --getsize64` | Tamaño del dispositivo en bytes. |
| `findmnt` | Indica si una carpeta tiene algo montado. |
| `modprobe` | Carga un módulo del núcleo. Aquí, el que crea `/dev/nbd0`. |
| `qemu-nbd` | Conecta o desconecta un archivo de imagen como si fuera un disco. |
| `umount` | Desmonta lo que se haya montado. |
| `guymager` | La herramienta gráfica de adquisición. |
| `ewfinfo` | Muestra los datos del caso guardados dentro de una imagen E01. |
| `ewfverify` | Recorre la E01 y comprueba que no está dañada. |
| `ewfmount` | Muestra el contenido de una E01 como si fuera un archivo normal. |

### Vocabulario

| Término | Qué es |
| --- | --- |
| Imagen RAW | Archivo que contiene los bytes del pendrive, uno tras otro, sin ninguna estructura extra. |
| Dispositivo de bloque | Nombre bajo `/dev/` que el sistema usa para hablarle a un disco. Aquí será `/dev/nbd0`. |
| Modo RO | *Read Only*, solo lectura: el sistema rechaza cualquier escritura hacia ese dispositivo. |
| E01 | Formato de imagen forense. Guarda el contenido comprimido más datos del caso y sus propios controles. |
| Contenedor | El archivo `.E01` en sí. Distinto del contenido que guarda dentro. |
| Montar | Hacer que el contenido de una imagen aparezca como una carpeta del sistema para poder leerlo. |
| Manifiesto | Archivo de texto con hashes que permite comprobar después si algo cambió. |

## 1. Comprobar herramientas y estructura

El primer comando busca cada herramienta y muestra dónde está instalada. El segundo lista el contenido del paquete, carpeta por carpeta.

```bash
which guymager qemu-nbd ewfinfo ewfverify ewfmount blockdev lsblk md5sum sha1sum sha256sum

ls -lR ~/forense/LAB-04
```

Deben aparecer diez rutas, una por herramienta, y estos archivos:

```text
fuente/PENDRIVE_ADMIN_01.raw
notas/PENDRIVE_ADMIN_01-ficha-fuente.txt
notas/PENDRIVE_ADMIN_01-fuente.md5
notas/PENDRIVE_ADMIN_01-fuente.sha1
notas/PENDRIVE_ADMIN_01-fuente.sha256
```

Si falta una herramienta, ejecuta esto:
```bash
sudo apt install -y guymager qemu-utils ewf-tools sleuthkit ntfs-3g
```

Pero si falta un archivo, informa al docente. No uses otro archivo como sustituto.

Comprueba en qué estado te fue entregada la fuente. El comando `test -w` pregunta si el archivo admite escritura y muestra un mensaje según la respuesta:

```bash
cd ~/forense/LAB-04

ls -l fuente/PENDRIVE_ADMIN_01.raw

test -w fuente/PENDRIVE_ADMIN_01.raw \
  && echo 'ATENCIÓN: la fuente admite escritura; informa al docente' \
  || echo 'OK: la fuente no admite escritura'
```

Lo esperable es `-r--r--r--` y el mensaje `OK`. **No ejecutes `chmod` por tu cuenta.** Si la fuente admite escritura, regístralo y avísale al docente: modificar los permisos de una evidencia recibida es una acción sobre la evidencia, y solo se realiza con instrucción y quedando anotada en la bitácora.

Hay dos niveles de protección y conviene no confundirlos:

| Nivel | Qué protege | Cómo se comprueba |
| --- | --- | --- |
| Permiso del archivo | Escrituras accidentales del usuario sobre el RAW | `ls -l`, `test -w` |
| Solo lectura del dispositivo | Cualquier escritura a través del dispositivo que adquiere Guymager | `blockdev --getro` devuelve `1` |

El primero es una barrera adicional y conveniente. El segundo es **el control que sostiene tu afirmación** de que no escribiste sobre el original, y es el que debes evidenciar en E2. Una fuente con permisos de escritura pero expuesta con `qemu-nbd --read-only` sigue estando protegida durante la adquisición; lo que no puedes hacer es omitir la comprobación del dispositivo confiando en el permiso del archivo.

## 2. Registrar el inicio

Deja constancia de la hora, la zona horaria y quiénes trabajan. Toda bitácora forense parte por ahí.

```bash
cd ~/forense/LAB-04

date --iso-8601=seconds | tee registros/inicio.txt

timedatectl | tee -a registros/inicio.txt

nano registros/identificacion-pareja.txt
```

En `nano` escribe los nombres de ambos integrantes y el número de la estación. Guarda con `Ctrl+O`, `Enter`, y sal con `Ctrl+X`.

## 3. Leer y verificar la fuente

Lee la ficha del caso y anota el tamaño real del archivo.

```bash
cd ~/forense/LAB-04

cat notas/PENDRIVE_ADMIN_01-ficha-fuente.txt \
  | tee registros/ficha-leida.txt

ls -l fuente/PENDRIVE_ADMIN_01.raw \
  | tee registros/tamano-fuente-antes.txt
```

En la salida de `ls -l`, el número grande es el tamaño en bytes. Debe ser exactamente:

```text
536870912
```

La fuente llega con tres manifiestos. Compruébalos todos sobre el mismo archivo:

```bash
cd ~/forense/LAB-04

md5sum -c notas/PENDRIVE_ADMIN_01-fuente.md5 \
  | tee registros/verificacion-fuente-antes.txt
  
sha1sum -c notas/PENDRIVE_ADMIN_01-fuente.sha1 \
  | tee -a registros/verificacion-fuente-antes.txt
  
sha256sum -c notas/PENDRIVE_ADMIN_01-fuente.sha256 \
  | tee -a registros/verificacion-fuente-antes.txt
```

Las tres líneas deben terminar en `OK`. Si tu Kali está en español mostrará `La suma coincide`; es la misma respuesta. Guarda además los valores para poder compararlos entre sí:

```bash
cd ~/forense/LAB-04

md5sum fuente/PENDRIVE_ADMIN_01.raw \
  | tee registros/algoritmos-fuente.txt
  
sha1sum fuente/PENDRIVE_ADMIN_01.raw \
  | tee -a registros/algoritmos-fuente.txt
  
sha256sum fuente/PENDRIVE_ADMIN_01.raw \
  | tee -a registros/algoritmos-fuente.txt
```

Observa que los tres valores tienen longitudes distintas, 32, 40 y 64 caracteres hexadecimales, y que ninguno se parece a los otros. Son tres resúmenes independientes del **mismo objeto**: coincidir en los tres refuerza la comprobación, pero ninguno describe el contenido ni el significado del archivo.

No continúes si el tamaño discrepa o si alguno de los tres manifiestos no devuelve `OK`.

## 4. Exponer la fuente como dispositivo de solo lectura

Carga el módulo NBD y comprueba primero que `/dev/nbd0` no esté conectado ni montado.

Si `qemu-nbd` responde `Failed to open /dev/nbd0: No such file or directory`, el módulo no está cargado: vuelve a ejecutar `modprobe` antes de continuar. No significa que te hayas equivocado de dispositivo.

```bash
sudo modprobe nbd max_part=8
lsblk -o NAME,PATH,SIZE,TYPE,FSTYPE,MOUNTPOINTS,RO /dev/nbd0
```

Si muestra tamaño, particiones o punto de montaje de otro caso, detente. No desconectes un dispositivo que no identificaste.

Expón el RAW en modo de solo lectura y registra el dispositivo:

```bash
cd ~/forense/LAB-04

sudo qemu-nbd --read-only --format=raw --connect=/dev/nbd0 \
  ~/forense/LAB-04/fuente/PENDRIVE_ADMIN_01.raw
  
sudo udevadm settle

DISPOSITIVO_EVIDENCIA=/dev/nbd0

echo "$DISPOSITIVO_EVIDENCIA" \
  | tee registros/dispositivo-fuente.txt
  
sudo blockdev --getsize64 "$DISPOSITIVO_EVIDENCIA" \
  | tee registros/tamano-dispositivo.txt
  
sudo blockdev --getro "$DISPOSITIVO_EVIDENCIA" \
  | tee registros/solo-lectura.txt
  
lsblk -o NAME,PATH,SIZE,TYPE,FSTYPE,MOUNTPOINTS,RO "$DISPOSITIVO_EVIDENCIA" \
  | tee registros/lsblk-fuente.txt
```

Antes de abrir Guymager, deben cumplirse todas estas condiciones:

| Control | Valor esperado |
| --- | --- |
| Nombre | `/dev/nbd0` |
| Tamaño | `536870912` bytes o `512M` |
| Tipo | `disk` |
| Sistema de archivos | `ntfs` |
| Punto de montaje | vacío |
| `blockdev --getro` | `1` |
| Columna `RO` de `lsblk` | `1` |

Si una condición falla, no selecciones ningún dispositivo en Guymager.

## 5. Preparar el destino

Comprueba que la carpeta donde irá la imagen esté vacía y guarda su ruta completa: Guymager la pedirá escrita entera.

```bash
cd ~/forense/LAB-04

mkdir -p adquisicion trabajo registros

ls -A adquisicion

realpath adquisicion | tee registros/ruta-destino.txt
```

`ls -A` no debe mostrar nada: la carpeta tiene que estar vacía. Si aparecen archivos, no los sobrescribas e informa al docente.

`realpath` imprime la ruta completa de la carpeta. Cópiala tal cual en Guymager, que la pide escrita entera.

## 6. Adquirir con Guymager

Abre Guymager:

```bash
sudo guymager
```

Busca el dispositivo indicado por:

```bash
cat ~/forense/LAB-04/registros/dispositivo-fuente.txt
```

Selecciona esa fila, verifica tamaño `512 MiB`, haz clic derecho y elige `Acquire image`. No selecciones el disco de Kali.

Configura:

| Campo           | Valor                                                       |
| --------------- | ----------------------------------------------------------- |
| Formato         | `Expert Witness Format`                                     |
| Case number     | `PCL-2026-018`                                              |
| Evidence number | `EV-01`                                                     |
| Examiner        | Identificador de la pareja                                  |
| Description     | `Pendrive EV-01 rotulado RESPALDO ADMIN`                    |
| Notes           | `Fuente expuesta mediante qemu-nbd en modo de solo lectura` |
| Image directory | Valor de `registros/ruta-destino.txt`                       |
| Image filename  | `PENDRIVE_ADMIN_01`                                         |
| Info filename   | `PENDRIVE_ADMIN_01`                                         |

El nombre lleva **guion bajo**, no guion. Guymager solo acepta `a-z`, `A-Z`, `0-9` y `_` en los nombres de imagen: si escribes un guion, lo elimina sin avisar y el archivo termina llamándose distinto de como creías. Comprueba la ruta que aparece en el campo *Image file* de la ventana principal antes de iniciar.

En hashes y verificación:

1. marca `Calculate MD5`, `Calculate SHA-1` y `Calculate SHA-256`;
2. marca `Verify image after acquisition`;
3. deja `Re-read source after acquisition` **sin** marcar: esa comparación la harás tú en el paso 10;
4. usa la compresión rápida predeterminada;
5. no es necesario dividir una fuente de 512 MiB.

Los tres quedan registrados, pero no en el mismo lugar. El formato E01 (EnCase 6) solo tiene espacio para MD5 y SHA-1 en su sección de digest; el SHA-256 se guarda en el archivo `.info` que Guymager escribe junto a la imagen. Lo verás en el paso 9.

Antes de iniciar, operador y verificador confirman en voz alta dispositivo, tamaño, modo RO y destino. Presiona `Start` y espera la finalización de adquisición y verificación.

## 7. Registrar la salida y desconectar la fuente

Cierra Guymager y corrige la propiedad de los archivos solo si quedaron asignados a `root`:

```bash
cd ~/forense/LAB-04

sudo chown "$USER" adquisicion/PENDRIVE_ADMIN_01.*

ls -l adquisicion | tee registros/inventario-adquisicion.txt
```

Guymager corre con `sudo`, así que los archivos quedan a nombre de `root`. `chown "$USER"` te devuelve la propiedad; `$USER` es tu nombre de usuario, que la terminal ya conoce.

Desconecta el dispositivo NBD y comprueba que quedó libre:

```bash
cd ~/forense/LAB-04

cat registros/dispositivo-fuente.txt

sudo qemu-nbd --disconnect /dev/nbd0

lsblk -o NAME,PATH,SIZE,TYPE,FSTYPE,MOUNTPOINTS,RO /dev/nbd0
```

El `cat` te recuerda qué dispositivo anotaste en el paso 4. Debe decir `/dev/nbd0`; si dijera otra cosa, no desconectes nada y avisa al docente. Tras desconectar, `lsblk` debe mostrar tamaño `0B`.

## 8. Confirmar que la fuente no cambió

Vuelve a comprobar el manifiesto de la fuente. Si sigue dando `OK`, adquirir no la modificó: es la prueba central de todo el procedimiento.

```bash
cd ~/forense/LAB-04

ls -l fuente/PENDRIVE_ADMIN_01.raw \
  | tee registros/tamano-fuente-despues.txt
  
sha256sum -c notas/PENDRIVE_ADMIN_01-fuente.sha256 \
  | tee registros/verificacion-fuente-despues.txt
```

Aquí basta un solo algoritmo. En la fase 1 se reconciliaron los tres manifiestos sobre el mismo archivo; a partir de ese punto SHA-256 es el control externo elegido y repetir MD5 y SHA-1 no agrega garantía. Poder justificar esa elección forma parte del ejercicio.

Un resultado `OK` sustenta que el archivo fuente mantiene los mismos bytes que al ser preparado. No demuestra por sí solo que la E01 sea correcta; eso se comprueba a continuación.

## 9. Leer y verificar la E01

`ewfinfo` muestra los datos del caso guardados dentro de la imagen. `ewfverify` la recorre entera y recalcula sus hashes internos para confirmar que no está dañada.

```bash
cd ~/forense/LAB-04
ewfinfo adquisicion/PENDRIVE_ADMIN_01.E01 \
  | tee registros/ewfinfo.txt
ewfverify adquisicion/PENDRIVE_ADMIN_01.E01 \
  2>&1 | tee registros/ewfverify-adquisicion.txt
```

Revisa en `ewfinfo`:

- número de caso;
- número de evidencia;
- examinador;
- tamaño del medio;
- descripción y notas.

En el apartado *Digest hash information* verás **solo MD5 y SHA1**, aunque marcaras los tres. No es un error tuyo: el formato E01 es antiguo y solo tiene espacio para esos dos hashes. El SHA-256 no cabe ahí.

Por eso MD5 y SHA-1 siguen apareciendo en herramientas forenses actuales aunque sean algoritmos viejos: los formatos de imagen los siguen exigiendo. Y por eso en esta lección el SHA-256 va aparte, en los manifiestos que acompañan a la evidencia.

El SHA-256 sí se calculó. Guymager lo guardó en el archivo `.info` que dejó junto a la imagen:

```bash
cd ~/forense/LAB-04
grep -E 'hash|Hash' adquisicion/PENDRIVE_ADMIN_01.info \
  | tee registros/info-hashes.txt
```

Compara esos tres valores con los que calculaste sobre el RAW en el paso 3. Deben coincidir uno a uno, porque Guymager hasheó exactamente los mismos bytes que contiene el archivo fuente.

Fíjate además en el tamaño del archivo `.E01`:

```bash
cd ~/forense/LAB-04
ls -l adquisicion/
```

La `.E01` pesa muchísimo menos que los 512 MiB de la fuente, porque el formato comprime y el pendrive está casi todo vacío. Anota los dos tamaños: son la prueba más simple de que el RAW y el `.E01` no son el mismo objeto.

`ewfverify` debe finalizar con `SUCCESS`. Si falla, conserva la salida y no crees la copia de trabajo.

## 10. Comparar la fuente con el contenido adquirido

Crea un punto de montaje y expón la E01:

```bash
sudo mkdir -p /mnt/pendrive-admin-ewf

sudo ewfmount ~/forense/LAB-04/adquisicion/PENDRIVE_ADMIN_01.E01 /mnt/pendrive-admin-ewf
  
ls -lh /mnt/pendrive-admin-ewf
```

Debes ver un archivo llamado `ewf1`. No es un archivo real en disco: es el contenido de la E01 mostrado como si fuera el pendrive completo. Su tamaño debe ser `536870912`, igual que la fuente.

Ahora calcula los tres hashes sobre los dos objetos. Son seis órdenes, en pares: primero la fuente, después `ewf1`.

```bash
cd ~/forense/LAB-04
{
  md5sum    fuente/PENDRIVE_ADMIN_01.raw
  sudo md5sum    /mnt/pendrive-admin-ewf/ewf1
  
  sha1sum   fuente/PENDRIVE_ADMIN_01.raw
  sudo sha1sum   /mnt/pendrive-admin-ewf/ewf1
  
  sha256sum fuente/PENDRIVE_ADMIN_01.raw
  sudo sha256sum /mnt/pendrive-admin-ewf/ewf1
} | tee registros/comparacion-tres-algoritmos.txt
```

Obtendrás seis líneas. Compáralas **de dos en dos**: la 1 con la 2, la 3 con la 4, la 5 con la 6. Los valores de cada par tienen que ser idénticos.

```text
línea 1  md5 del RAW      ┐ deben ser iguales
línea 2  md5 de ewf1      ┘
línea 3  sha1 del RAW     ┐ deben ser iguales
línea 4  sha1 de ewf1     ┘
línea 5  sha256 del RAW   ┐ deben ser iguales
línea 6  sha256 de ewf1   ┘
```

Copia los seis valores a la tabla de la fase 3. Si un solo par no coincide, detente y avisa al docente: significaría que la imagen no contiene lo mismo que la fuente, y no debes crear la copia de trabajo.

Desmonta la capa EWF:

```bash
cd ~/forense/LAB-04
sudo umount /mnt/pendrive-admin-ewf
findmnt /mnt/pendrive-admin-ewf || echo 'EWF desmontada'
```

Un error frecuente: comparar `sha256sum fuente/PENDRIVE_ADMIN_01.raw` con `sha256sum adquisicion/PENDRIVE_ADMIN_01.E01`. No tiene sentido. El primero es el pendrive completo; el segundo es el archivo comprimido que lo guarda, y ni siquiera pesan lo mismo. La comparación válida es siempre RAW contra `ewf1`.

## 11. Crear el manifiesto de contenedores

Si la fuente fuera grande, Guymager partiría la imagen en varios archivos: `.E01`, `.E02`, y así. Aquí solo hay uno, pero se escribe `PENDRIVE_ADMIN_01.E*` para que el comando los tome todos sin importar cuántos sean. El `*` significa «cualquier terminación».

```bash
cd ~/forense/LAB-04

(cd adquisicion && \
  sha256sum PENDRIVE_ADMIN_01.E* > ../registros/PENDRIVE_ADMIN_01-contenedores.sha256)
  
(cd adquisicion && \
  sha256sum -c ../registros/PENDRIVE_ADMIN_01-contenedores.sha256) \
  | tee registros/verificacion-contenedores-adquisicion.txt
```

El manifiesto identifica los archivos contenedores preservados. No reemplaza los controles EWF internos.

## 12. Crear y verificar la copia de trabajo

Copia la imagen a `trabajo/` y comprueba la copia contra el manifiesto que creaste en el paso anterior.

```bash
cd ~/forense/LAB-04

cp --preserve=timestamps adquisicion/PENDRIVE_ADMIN_01.E* trabajo/
(cd trabajo && \
  sha256sum -c ../registros/PENDRIVE_ADMIN_01-contenedores.sha256) \
  | tee registros/verificacion-copia-trabajo.txt
  
ewfverify trabajo/PENDRIVE_ADMIN_01.E01 \
  | tee registros/ewfverify-copia-trabajo.txt
```

Comprueba la separación:

```bash
ls -l adquisicion trabajo
```

A partir de este punto, cualquier exploración posterior se realiza sobre `trabajo/PENDRIVE_ADMIN_01.E01`.

## 13. Registrar el cierre

Comprueba que no quedan dispositivos conectados ni carpetas montadas, y anota la hora de término.

```bash
cd ~/forense/LAB-04

sudo blockdev --getsize64 /dev/nbd0 \
  | tee registros/tamano-nbd-final.txt
  
findmnt | grep 'pendrive-admin' \
  || echo 'No quedan montajes del caso'
  
date --iso-8601=seconds | tee registros/fin.txt
```

El tamaño final de `/dev/nbd0` debe ser `0`, señal de que la fuente quedó desconectada.

## Qué hash corresponde a cada objeto

| Comando | Objeto |
| --- | --- |
| `md5sum`, `sha1sum` y `sha256sum` sobre `fuente/PENDRIVE_ADMIN_01.raw` | El mismo archivo fuente, resumido con tres algoritmos distintos. |
| `sha256sum fuente/PENDRIVE_ADMIN_01.raw` | Archivo fuente entregado. |
| `sha256sum /mnt/pendrive-admin-ewf/ewf1` | Contenido lógico expuesto desde E01. |
| `sha256sum adquisicion/PENDRIVE_ADMIN_01.E01` | Archivo contenedor E01. |
| `ewfverify ...E01` | Contenido EWF y controles internos. |
| `sha256sum -c ...contenedores.sha256` | Igualdad byte a byte de los contenedores copiados. |

## Diagnóstico

| Problema                                      | Comprobación                                       | Acción                                                |
| --------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------- |
| `/dev/nbd0` ya está ocupado                   | Revisar `lsblk` y el caso asociado                 | Detener e informar; no desconectar evidencia ajena.   |
| `blockdev --getro` devuelve `0`               | Revisar `--read-only`                              | Desconectar y repetir antes de adquirir.              |
| Guymager muestra 50 GiB                       | Se seleccionó otra fuente                          | Cancelar; el pendrive mide 512 MiB.                   |
| El botón `Start` no se habilita               | Revisar ruta absoluta y nombres                    | Completar campos obligatorios.                        |
| No aparece `.E01`                             | Revisar destino y estado de Guymager               | No crear archivos manualmente.                        |
| `ewfverify` falla                             | Revisar todos los segmentos y salida               | Detener y documentar.                                 |
| Un manifiesto de la fuente falla              | Comprobar cuál de los tres y el tamaño del archivo | No exponer ni adquirir; informar al docente.          |
| RAW y `ewf1` difieren                         | Revisar fuente, E01 y tamaño                       | No duplicar hasta resolver.                           |
| Solo un algoritmo discrepa entre RAW y `ewf1` | Repetir el cálculo sobre el objeto correcto        | Verificar que no se hasheó `.E01` en lugar de `ewf1`. |
| Manifiesto falla en `trabajo/`                | Revisar copia y segmentos                          | Conservar ambos conjuntos y documentar.               |
| No se puede desmontar                         | Salir del punto de montaje                         | `cd ~/forense/LAB-04` y reintentar.                   |

## Lista final

- [ ] Los manifiestos MD5, SHA-1 y SHA-256 de la fuente verificaron `OK` antes de adquirir.
- [ ] La fuente volvió a verificar `OK` después de la adquisición.

- [ ] El dispositivo seleccionado era `/dev/nbd0`, 512 MiB y `RO 1`.
- [ ] Guymager creó la E01 con identificadores correctos.
- [ ] `ewfverify` validó la adquisición.
- [ ] RAW y `ewf1` coincidieron en los tres algoritmos.

- [ ] Se generó el manifiesto de contenedores.
- [ ] La copia de trabajo verificó contra ese manifiesto.
- [ ] `ewfverify` validó la copia de trabajo.
- [ ] No quedan montajes ni dispositivos de bucle del caso.
