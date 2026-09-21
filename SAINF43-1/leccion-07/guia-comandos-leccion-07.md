---
title: "Guía práctica de comandos Lección 07 - Evidencia de Windows"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "07"
author: Jordy
start: 2026-09-21
end: 2026-09-23
created_at: 2026-08-21
aliases:
  - "Guía práctica de comandos Lección 07 - Evidencia de Windows"
---

# Guía práctica de comandos Lección 07 - Evidencia de Windows

## Objetivo y alcance

Usa esta guía como apoyo para los comandos de la actividad. Al finalizar tendrás la copia E01 verificada desde una carpeta compartida de **solo lectura**, Windows montado también en **solo lectura**, `Security.evtx` exportado, las búsquedas por Event ID guardadas, la imagen desmontada y la bitácora recuperada en el PC anfitrión.

Los segmentos permanecen en `/media/sf_LAB07_ENTREGA/`; no los copies dentro de Kali. Las salidas se guardan en `~/forense/LAB-07/`. No inicies la máquina Windows.

## Preparación previa: comprobar las herramientas

Al iniciar la sesión, comprueba las herramientas:

```bash
command -v ewfmount mmls ntfs-3g chainsaw sha256sum
chainsaw --version
```

Si falta alguna herramienta, ejecuta lo siguiente:

```bash
sudo apt update
sudo apt install -y ewf-tools sleuthkit ntfs-3g chainsaw
command -v ewfmount mmls ntfs-3g chainsaw sha256sum
chainsaw --version
```

| Parte                     | Explicación                                                                                                |
| ------------------------- | ---------------------------------------------------------------------------------------------------------- |
| `command -v ...`          | Muestra la ruta de cada ejecutable. Si falta una ruta, la instalación no está completa.                    |
| `chainsaw --version`      | Confirma que Chainsaw puede ejecutarse y muestra la versión instalada.                                     |
| `sudo apt update`         | Actualiza la lista de paquetes disponibles; no instala herramientas. Solo se usa si falta una herramienta. |
| `sudo apt install -y ...` | Instala `ewf-tools`, Sleuth Kit, el controlador NTFS y Chainsaw. `-y` confirma la instalación solicitada.  |

Después de instalar y comprobar las herramientas, restablece la red interna aislada del laboratorio antes de acceder a la evidencia. No habilites Internet mientras la E01 esté montada.

## Variables que usa esta guía

Los comandos guardan rutas y parámetros en variables para no repetirlos. Una variable se escribe `NOMBRE=valor` al definirla y `$NOMBRE` al usarla. **Solo existen en la terminal donde se definieron:** si abres una ventana nueva o cierras la sesión, se pierden y los comandos fallarán con rutas vacías.

| Variable                         | Dónde se define                           | Qué contiene                                                      |
| -------------------------------- | ----------------------------------------- | ----------------------------------------------------------------- |
| `LAB`                            | Paso 1                                    | Carpeta de trabajo dentro de Kali.                                |
| `COMPARTIDA`                     | Paso 1                                    | Carpeta compartida de solo lectura con la evidencia.              |
| `SALIDA`                         | Paso 1                                    | Carpeta compartida con escritura donde se recupera la bitácora.   |
| `SECTOR_INICIAL_WINDOWS`         | Paso 2, al cargar `WIN10-CASO01-caso.env` | Sector donde comienza la partición de Windows.                    |
| `TAMANO_SECTOR`                  | Paso 2, al cargar el mismo archivo        | Tamaño de cada sector, en bytes.                                  |
| `ZONA_CASO`                      | Paso 2, al cargar el mismo archivo        | Zona horaria del equipo analizado.                                |
| `OFFSET_BYTES`                   | Paso 3                                    | Desplazamiento de la partición, calculado con las dos anteriores. |
| `EVTX_ORIGEN`                    | Paso 4                                    | Ruta del registro dentro de la imagen montada.                    |
| `SEC`                            | Paso 4                                    | Ruta de la copia exportada que analizará Chainsaw.                |
| `CAMPO_TIEMPO`, `DESDE`, `HASTA` | Paso 5                                    | Campo de fecha del evento y límites de la ventana del caso.       |
| `PS_ORIGEN`, `POW`               | Paso 8                                    | Rutas del registro de PowerShell; solo para la ampliación.        |

Las tres variables del paso 2 no las escribes tú: provienen del archivo `WIN10-CASO01-caso.env` que entrega el docente con los parámetros reales del caso.

### Si abres otra terminal

Vuelve a definir las rutas y a cargar los parámetros antes de continuar:

```bash
LAB=~/forense/LAB-07
COMPARTIDA=/media/sf_LAB07_ENTREGA
SALIDA=/media/sf_LAB07_SALIDA
cd "$LAB"
. "$COMPARTIDA/notas/WIN10-CASO01-caso.env"
echo "sector=$SECTOR_INICIAL_WINDOWS  bytes_por_sector=$TAMANO_SECTOR  zona=$ZONA_CASO"
```

Las variables que se definen más adelante, como `SEC` u `OFFSET_BYTES`, se recuperan repitiendo la línea del paso correspondiente. No es necesario volver a montar ni a exportar nada.

## 1. Comprobar las carpetas compartidas y preparar el trabajo

```bash
LAB=~/forense/LAB-07
COMPARTIDA=/media/sf_LAB07_ENTREGA
SALIDA=/media/sf_LAB07_SALIDA

mkdir -p "$LAB"/{exportados/evtx,notas,entrega}
sudo mkdir -p /mnt/win10-caso01-ewf /mnt/win10-caso01-windows
cd "$LAB"
pwd
findmnt -T "$COMPARTIDA" | tee notas/carpeta-evidencia.txt
findmnt -T "$SALIDA" | tee notas/carpeta-salida.txt
test -r "$COMPARTIDA/evidencia/WIN10-CASO01.E01" && echo 'OK: E01 accesible' || echo 'ERROR: E01 no accesible'
test -w "$SALIDA" && echo 'OK: carpeta de salida escribible' || echo 'ERROR: carpeta de salida sin escritura'
cp -n "$COMPARTIDA/documentos/plantilla-bitacora-leccion-07.md" entrega/bitacora-equipo.md
```

| Parte | Explicación |
| --- | --- |
| `LAB=...`, `COMPARTIDA=...` y `SALIDA=...` | Guardan las tres rutas usadas durante la sesión. Si abres otra terminal, vuelve a definirlas. |
| `mkdir -p` | Crea las carpetas locales que falten sin borrar las existentes. |
| `{exportados/evtx,notas,entrega}` | Separa el EVTX, las salidas técnicas y la bitácora. |
| `sudo mkdir -p /mnt/...` | Crea los dos puntos de montaje. `sudo` se usa porque `/mnt` es una ruta administrada por el sistema. |
| `cd` | Cambia la terminal a la carpeta de trabajo. |
| `pwd` | Muestra la ubicación actual; debe terminar en `/forense/LAB-07`. |
| `findmnt -T` | Muestra el sistema de archivos que contiene la ruta. En `LAB07_ENTREGA` debe aparecer `ro`; en `LAB07_SALIDA`, `rw`. |
| `test -r` y `test -w` | Comprueban lectura de la E01 y escritura en la carpeta destinada a recuperar la entrega. |
| `cp -n` | Copia la plantilla sin reemplazar una bitácora que ya exista. |

No continúes si `LAB07_ENTREGA` aparece como `rw`, la E01 no es accesible o `LAB07_SALIDA` no admite escritura. Si la carpeta local contiene resultados de otra sesión, informa al docente; no mezcles dos ejecuciones.

## 2. Verificar la copia y leer los parámetros

```bash
(cd "$COMPARTIDA/evidencia" && sha256sum -c "$COMPARTIDA/notas/WIN10-CASO01-contenedores.sha256") | tee "$LAB/notas/verificacion-sha256.txt"
sed -n '1,20p' "$COMPARTIDA/notas/WIN10-CASO01-caso.env"
. "$COMPARTIDA/notas/WIN10-CASO01-caso.env"
echo "sector=$SECTOR_INICIAL_WINDOWS  bytes_por_sector=$TAMANO_SECTOR  zona=$ZONA_CASO"
```

| Parte                                         | Explicación                                                                                                                                                                                                                             |
| --------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `(cd "$COMPARTIDA/evidencia" && ...)`         | Ejecuta la verificación en la carpeta compartida y luego regresa automáticamente a `LAB-07`.                                                                                                                                            |
| `sha256sum -c`                                | Compara cada segmento de la E01 con la huella registrada en el manifiesto. No modifica la evidencia.                                                                                                                                    |
| `\| tee "$LAB/notas/verificacion-sha256.txt"` | Muestra el resultado y guarda una copia en el archivo indicado.                                                                                                                                                                         |
| `sed -n '1,20p'`                              | Muestra las primeras veinte líneas del archivo de parámetros sin editarlo.                                                                                                                                                              |
| `. archivo.env`                               | Carga el archivo de parámetros y crea tres variables en la terminal actual: `SECTOR_INICIAL_WINDOWS`, `TAMANO_SECTOR` y `ZONA_CASO`. El punto inicial es el comando: aplica el contenido a esta terminal en lugar de ejecutarlo aparte. |
| `echo "sector=..."`                           | Muestra las tres variables cargadas. Sirve para comprobar que el archivo se leyó: si alguna aparece vacía, los pasos siguientes fallarán.                                                                                               |

Todos los segmentos deben indicar `OK`. La línea del `echo` debe mostrar un número de sector, un tamaño de sector y una zona horaria; ningún valor puede quedar vacío ni contener `NUMERO_START_CONFIRMADO`. Si ocurre cualquiera de esas situaciones, detente e informa al docente.

## 3. Montar Windows en solo lectura

Ejecuta los comandos en este orden:

```bash
sudo ewfmount "$COMPARTIDA/evidencia/WIN10-CASO01.E01" /mnt/win10-caso01-ewf
sudo mmls /mnt/win10-caso01-ewf/ewf1 | tee "$LAB/notas/mmls.txt"
OFFSET_BYTES=$((SECTOR_INICIAL_WINDOWS * TAMANO_SECTOR))
printf 'Offset: %s bytes\n' "$OFFSET_BYTES" | tee "$LAB/notas/offset-windows.txt"
sudo mount -t ntfs-3g -o ro,loop,offset="$OFFSET_BYTES" /mnt/win10-caso01-ewf/ewf1 /mnt/win10-caso01-windows
findmnt /mnt/win10-caso01-windows | tee "$LAB/notas/montaje-windows.txt"
```

| Comando                 | Explicación                                                                                                      |
| ----------------------- | ---------------------------------------------------------------------------------------------------------------- |
| `ewfmount`              | Expone el contenido de la E01 como `/mnt/win10-caso01-ewf/ewf1`. Todavía no monta la partición NTFS.             |
| `mmls`                  | Muestra la tabla de particiones. La columna `Start` de Windows debe coincidir con `SECTOR_INICIAL_WINDOWS`.      |
| `OFFSET_BYTES=$((...))` | Multiplica el sector inicial por el tamaño de sector para obtener el desplazamiento en bytes.                    |
| `printf ... \| tee`     | Muestra el offset y lo guarda en la bitácora.                                                                    |
| `mount -t ntfs-3g`      | Monta la partición como NTFS.                                                                                    |
| `-o ro,loop,offset=...` | Activa solo lectura (`ro`), usa el archivo expuesto como dispositivo (`loop`) y comienza en el offset calculado. |
| `findmnt`               | Muestra cómo quedó montada la partición. En las opciones debe aparecer `ro`.                                     |

No continúes si `mmls` no coincide con el parámetro entregado o si `findmnt` no muestra `ro`. No uses `ntfsfix` ni retires la opción de solo lectura.

## 4. Exportar `Security.evtx`

```bash
EVTX_ORIGEN=/mnt/win10-caso01-windows/Windows/System32/winevt/Logs/Security.evtx
sudo stat "$EVTX_ORIGEN" | tee notas/stat-security-evtx.txt
sudo cp --preserve=timestamps "$EVTX_ORIGEN" exportados/evtx/
sudo chown "$(id -u):$(id -g)" exportados/evtx/Security.evtx
SEC="$LAB/exportados/evtx/Security.evtx"
sha256sum "$SEC" | tee notas/security-evtx.sha256
ls -lh "$SEC"
```

| Parte                      | Explicación                                                                                                                                           |
| -------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| `EVTX_ORIGEN=...`          | Guarda la ruta interna del registro para no escribirla varias veces.                                                                                  |
| `stat`                     | Registra tamaño, permisos y marcas temporales visibles del archivo original.                                                                          |
| `cp --preserve=timestamps` | Copia el EVTX al área de análisis y conserva sus marcas temporales disponibles.                                                                       |
| `chown`                    | Asigna la copia exportada al usuario actual para poder analizarla sin `sudo`. `$(id -u):$(id -g)` obtiene los identificadores del usuario y su grupo. |
| `SEC=...`                  | Guarda la ruta de la copia que consultará Chainsaw.                                                                                                   |
| `sha256sum`                | Calcula una huella que identifica la copia exportada. No reemplaza la verificación de la E01.                                                         |
| `ls -lh`                   | Confirma que el archivo existe y muestra su tamaño.                                                                                                   |

## 5. Guardar las búsquedas con Chainsaw

```bash
CAMPO_TIEMPO='Event.System.TimeCreated_attributes.SystemTime'
DESDE='2026-08-21T04:45:00'
HASTA='2026-08-21T05:15:00'

for id in 4624 4634 4648 4688 4698 4720 4732; do
  chainsaw search -t "Event.System.EventID: =$id" --timestamp "$CAMPO_TIEMPO" --from "$DESDE" --to "$HASTA" "$SEC" \
    > "notas/evento-$id.txt"
  printf '%s -> %s eventos\n' "$id" "$(grep -c 'EventRecordID' "notas/evento-$id.txt")"
done
```

Ningún identificador debe informar cero eventos. Si alguno lo hace, revisa la ventana temporal antes de continuar.

| Parte | Explicación |
| --- | --- |
| `CAMPO_TIEMPO` | Indica a Chainsaw dónde está la fecha del evento. Chainsaw convierte los atributos XML añadiendo el sufijo `_attributes`, por eso la ruta termina en `TimeCreated_attributes.SystemTime`. |
| `DESDE` y `HASTA` | Limitan la búsqueda a la ventana UTC del caso. |
| `for id in ...; do` | Repite la misma búsqueda para cada Event ID de la actividad. |
| `chainsaw search` | Busca eventos dentro de la copia `Security.evtx`. |
| `-t "Event.System.EventID: =$id"` | Filtra por el ID que toma el bucle en cada repetición. |
| `--from` y `--to` | Aplican los límites temporales definidos arriba. |
| `>` | Guarda cada resultado en un archivo separado. Si ya existe, reemplaza solo esa salida de trabajo. |
| `printf ... grep -c` | Cuenta los eventos guardados en cada archivo y muestra el total por identificador. |
| `done` | Cierra el bucle. |

El banner y el resumen de Chainsaw salen por la salida de error, de modo que los archivos contienen únicamente los eventos. No canalices Chainsaw hacia `head`: al cerrarse la tubería antes de tiempo, el programa termina con un error de Rust que no indica ningún problema con la evidencia. Guarda siempre con `>` y revisa con `less`.

Revisa primero los eventos de referencia:

```bash
less notas/evento-4698.txt
less notas/evento-4720.txt
less notas/evento-4732.txt
```

`less` permite desplazarte sin modificar el archivo. Escribe `/campo` para buscar, `n` para ir a la coincidencia siguiente y `q` para salir. Después revisa 4624, 4634 y 4648 usando los campos indicados en la actividad.

Para reducir los procesos 4688 a los dos tipos obligatorios:

```bash
grep -niE -B 22 -A 10 'NewProcessName:.*(runas|net)\.exe$' notas/evento-4688.txt > notas/evento-4688-candidatos.txt
less notas/evento-4688-candidatos.txt
```

| Parte | Explicación |
| --- | --- |
| `grep` | Busca texto dentro del resultado 4688. |
| `-n` | Muestra el número de línea. |
| `-iE` | Ignora mayúsculas y permite buscar `runas` o `net` con una sola expresión. |
| `-B 22 -A 10` | Conserva líneas anteriores y posteriores para no perder los demás campos del evento. |
| `> notas/evento-4688-candidatos.txt` | Guarda el subconjunto para revisarlo con calma. |

## 6. Normalizar una hora

Copia un `SystemTime` del evento y reemplaza la fecha del ejemplo:

```bash
TZ="$ZONA_CASO" date --date='2026-08-21T04:45:00Z' '+%Y-%m-%dT%H:%M:%S.%N%:z'
```

| Parte | Explicación |
| --- | --- |
| `TZ="$ZONA_CASO"` | Aplica la zona horaria confirmada para el caso sin cambiar la configuración de Kali. |
| `date --date='...'` | Interpreta la hora original. La `Z` indica UTC. |
| `'+%Y-...%:z'` | Muestra fecha, hora, fracciones de segundo y desplazamiento UTC. |

Conserva en la tabla tanto el `SystemTime` original como el resultado normalizado. No uses la hora local de Kali como sustituto.

## 7. Comprobar el archivo de la tarea

Usa el `TaskName` obtenido en el evento 4698 para localizar el archivo correspondiente:

```bash
sudo find /mnt/win10-caso01-windows/Windows/System32/Tasks -type f -printf '%f\t%p\n' | sort | less -S
```

| Parte | Explicación |
| --- | --- |
| `find RUTA` | Recorre la carpeta de tareas del Windows montado. |
| `-type f` | Muestra solamente archivos. |
| `-printf '%f\t%p\n'` | Presenta el nombre y la ruta completa de cada archivo. |
| `\| sort` | Ordena los nombres. |
| `\| less -S` | Abre un visor sin partir las líneas largas. Se sale con `q`. |

Dentro de `less`, escribe `/` seguido del nombre observado en 4698. Registra `presente` y la ruta encontrada, o `no localizado`. No encontrar el archivo no invalida por sí solo el evento.

## 8. Ampliación guiada

Realiza solo una ampliación y únicamente después de completar el producto obligatorio.

### Revisar el inicio fallido 4625

```bash
chainsaw search -t 'Event.System.EventID: =4625' \
  --timestamp "$CAMPO_TIEMPO" --from "$DESDE" --to "$HASTA" "$SEC" \
  > "$LAB/notas/evento-4625.txt"
less "$LAB/notas/evento-4625.txt"
```

En el caso validado, `Status 0xc000006d` señala un fallo de autenticación y `SubStatus 0xc000006a` precisa contraseña incorrecta. Chainsaw puede mostrar `FailureReason: %%2313`: es un identificador de mensaje de Windows sin traducir, no otro código de fallo. Sustenta la interpretación específica en `Status` y `SubStatus`. Un único 4625 no demuestra por sí mismo un ataque.

### Relacionar `schtasks.exe` con 4698

```bash
grep -niE -B 22 -A 10 'NewProcessName:.*schtasks\.exe$' "$LAB/notas/evento-4688.txt" \
  > "$LAB/notas/evento-4688-schtasks.txt"
less "$LAB/notas/evento-4688-schtasks.txt"
```

### Consultar PowerShell 4104

```bash
PS_ORIGEN='/mnt/win10-caso01-windows/Windows/System32/winevt/Logs/Microsoft-Windows-PowerShell%4Operational.evtx'
sudo cp --preserve=timestamps "$PS_ORIGEN" "$LAB/exportados/evtx/"
sudo chown "$(id -u):$(id -g)" "$LAB/exportados/evtx/Microsoft-Windows-PowerShell%4Operational.evtx"
POW="$LAB/exportados/evtx/Microsoft-Windows-PowerShell%4Operational.evtx"
chainsaw search -t 'Event.System.EventID: =4104' \
  --timestamp "$CAMPO_TIEMPO" --from "$DESDE" --to "$HASTA" "$POW" \
  > "$LAB/notas/evento-4104.txt"
less "$LAB/notas/evento-4104.txt"
```

### Localizar Prefetch de PowerShell

```bash
sudo find /mnt/win10-caso01-windows/Windows/Prefetch \
  -maxdepth 1 -type f -iname 'POWERSHELL.EXE-*.pf' -printf '%f\t%p\n'
```

Estas ampliaciones no cambian las ocho filas obligatorias. Los comandos solo recuperan candidatos; debes aplicar los mismos controles de cuenta, campo de unión, tiempo y limitaciones usados en la actividad.

## 9. Desmontar y recuperar la entrega

```bash
cd "$LAB"
sudo umount /mnt/win10-caso01-windows
sudo umount /mnt/win10-caso01-ewf
findmnt /mnt/win10-caso01-windows
findmnt /mnt/win10-caso01-ewf
cp "$LAB/entrega/bitacora-equipo.md" "$SALIDA/"
cmp -s "$LAB/entrega/bitacora-equipo.md" "$SALIDA/bitacora-equipo.md" \
  && echo 'OK: bitácora recuperada en LAB07_SALIDA' \
  || echo 'ERROR: las copias de la bitácora no coinciden'
```

| Parte | Explicación |
| --- | --- |
| `cd "$LAB"` | Evita que la terminal permanezca dentro de una ruta que se desmontará. |
| Primer `umount` | Desmonta la partición Windows. |
| Segundo `umount` | Desmonta después la capa EWF. El orden es importante. |
| `findmnt RUTA` | Después del desmontaje no debe mostrar ninguna línea para esas rutas. |
| `cp ... "$SALIDA/"` | Copia únicamente la bitácora a la carpeta recuperable del anfitrión. |
| `cmp -s` | Compara ambas copias byte a byte. El mensaje `OK` confirma que coinciden. |

Si aparece `target is busy`, cierra otras terminales o visores ubicados dentro de `/mnt/win10-caso01-windows` y vuelve a intentarlo. No uses desmontaje forzado. No apagues ni reinicies hasta ver el mensaje `OK` de la bitácora.

## Qué llevar a la bitácora

| Evidencia mínima | Qué registrar |
| --- | --- |
| `verificacion-sha256.txt` | Segmentos coincidentes o discrepancias. |
| `mmls.txt` y `montaje-windows.txt` | Sector confirmado y opción `ro`. |
| `stat-security-evtx.txt` y `security-evtx.sha256` | Ruta de origen, metadatos y huella de la copia exportada. |
| `evento-ID.txt` | Los campos de las ocho evidencias seleccionadas; no copies la salida completa. |
| Búsqueda en `Tasks` | Presencia o ausencia del archivo asociado con 4698. |
| `LAB07_SALIDA/bitacora-equipo.md` | Copia final que debe sobrevivir al reinicio del equipo. |

## Si aparece un error

| Situación | Qué revisar |
| --- | --- |
| No existe `/media/sf_LAB07_ENTREGA` | Apaga Kali y revisa en VirtualBox el nombre y el montaje automático de la carpeta. |
| `LAB07_ENTREGA` aparece como `rw` | Detente y cambia la carpeta compartida a solo lectura desde VirtualBox. |
| `LAB07_SALIDA` no admite escritura | Revisa que sea una carpeta compartida distinta y que no tenga activada la opción de solo lectura. |
| `No such file or directory` | Ejecuta `pwd` y comprueba el nombre de la ruta. |
| Un comando usa una ruta vacía, o `offset=0` | Se perdieron las variables, casi siempre por abrir otra terminal. Repite el bloque «Si abres otra terminal» al comienzo de esta guía. |
| Un segmento muestra `FAILED` | Detente; no vuelvas a calcular ni reemplaces el manifiesto. |
| `ewf1` no aparece | Confirma que usaste el primer segmento `.E01` y consulta al docente. |
| `ls` sobre `/mnt/win10-caso01-ewf` responde `Permission denied` | Es normal: `ewfmount` se ejecutó como administrador y la capa pertenece a `root`. Comprueba con `sudo ls` si necesitas verla. |
| `mmls` no coincide | No montes la partición hasta que el docente confirme el sector. |
| `findmnt` no muestra `ro` | Desmonta inmediatamente y revisa el comando con el docente. |
| Chainsaw no devuelve resultados | Revisa el archivo `Security.evtx`, el Event ID y la ventana temporal. |
| Chainsaw termina con `panicked ... Broken pipe` | Canalizaste la salida hacia `head`. No es un problema de la evidencia: guarda con `>` y revisa con `less`. |
| Falta un campo | Registra `no registrado`; no lo completes por inferencia. |
