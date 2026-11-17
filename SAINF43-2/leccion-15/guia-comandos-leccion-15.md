---
title: "Guía práctica de comandos Lección 15 - Correo y web"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "15"
author: Jordy
start: 2026-11-17
end: 2026-11-23
created_at: 2026-11-15
aliases:
  - "Guía práctica de comandos Lección 15 - Correo y web"
---

# Guía práctica de comandos Lección 15 - Correo y web

Esta guía acompaña la actividad de la Lección 15. Explica, en el orden de la actividad, qué hace cada comando y cada parámetro, qué resultado esperar y qué hacer si algo falla. Todo se hace en Kali, **sin red** y sin privilegios de administrador.

El paquete del caso está en `/media/sf_LAB15_ENTREGA/` (solo lectura). Se trabaja en `~/forense/LAB-15/AR-CASO06/` y se respalda en `/media/sf_LAB15_SALIDA/`.

> [!warning] Seguridad
> Los correos y registros contienen dominios, enlaces e IP del caso. **No los visiten, no los copien en un navegador y no los consulten en servicios en línea** (*whois*, VirusTotal, buscadores). El adjunto se extrae con ripMIME y **no se abre**.

## Herramientas

| Comando | Uso en la práctica |
| --- | --- |
| `findmnt`, `test -w` | Comprobar que el paquete está en solo lectura y la salida admite escritura |
| `sha256sum` | Calcular hashes y verificar contra un manifiesto (`-c`) |
| `cp`, `chmod` | Crear las copias de trabajo y quitarles el permiso de escritura |
| `sed` | Mostrar una parte de un archivo; aquí, las cabeceras del correo |
| `cat -n` | Numerar las líneas de lo que recibe |
| `grep` | Buscar líneas o fragmentos; con `-n` muestra el número de línea |
| `ripmime` | Extraer los adjuntos de un correo MIME sin abrirlos |
| `mv` | Añadir `.no-abrir` al nombre del adjunto extraído |
| `file` | Identificar el tipo real de un archivo por su contenido |
| `date` | Convertir una hora a UTC |
| `awk` | Separar una línea en columnas y filtrar o sumar por columna |
| `sort`, `uniq -c` | Ordenar y contar repeticiones |
| `wc -l` | Contar líneas |
| `column` | Mostrar un CSV con columnas alineadas |
| `tee` | Mostrar la salida en pantalla y guardarla en un archivo |

Comprueben que estén disponibles:

```bash
command -v findmnt sha256sum sed grep ripmime file date awk sort uniq wc column tee
```

El docente deja ripMIME instalado antes de la clase. Si `command -v ripmime` no muestra una ruta, avisen; no necesitan instalar nada ni conectar la estación a Internet durante la práctica.

Referencia de la herramienta: [manual de ripMIME](https://manpages.debian.org/trixie/ripmime/ripmime.1.en.html).

### Piezas que se repiten

- **`| tee archivo`**: la tubería `|` entrega la salida de un comando al siguiente. `tee` la muestra y la guarda. `tee -a` agrega al final en lugar de reemplazar. El archivo guardado es el que se cita en la actividad DOCX.
- **`( cd carpeta && comando )`**: el `cd` ocurre solo dentro de los paréntesis; la terminal sigue donde estaba.
- **Comillas simples `'...'`**: protegen el texto para que la terminal no interprete `"`, `$` ni `|` dentro de un patrón.
- **`../02_salidas/`**: `..` sube una carpeta. Desde `01_copias_trabajo/` llega a la carpeta del caso.

## Paso 1. Preparar y verificar

```bash
COMPARTIDA=/media/sf_LAB15_ENTREGA
SALIDA=/media/sf_LAB15_SALIDA
CASO=~/forense/LAB-15/AR-CASO06
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_anexos}
cd "$CASO"
```

| Parte | Qué hace |
| --- | --- |
| `COMPARTIDA=...` | Guarda una ruta en una variable; después se usa como `"$COMPARTIDA"` |
| `mkdir -p` | Crea las carpetas (y las intermedias) sin fallar si ya existen |
| `{00_control,...}` | Expansión de llaves: una ruta por cada nombre |

| Carpeta | Propósito |
| --- | --- |
| `00_control` | Resultados de las verificaciones de integridad |
| `01_copias_trabajo` | Copias verificadas de las cinco exportaciones; aquí se analiza |
| `02_salidas` | Resultados guardados de cada comando, y el adjunto extraído |
| `03_anexos` | Manifiestos de hashes de las salidas y de la entrega |

Las variables existen solo en la terminal donde se definieron. **Si abren otra terminal**, repitan estas tres líneas antes de seguir:

```bash
COMPARTIDA=/media/sf_LAB15_ENTREGA
SALIDA=/media/sf_LAB15_SALIDA
CASO=~/forense/LAB-15/AR-CASO06
```

### Comprobar el entorno

```bash
findmnt -no TARGET,OPTIONS -T "$COMPARTIDA"
test -w "$SALIDA" && echo 'OK: salida con escritura' || echo 'ERROR: salida sin escritura'
```

**Resultado esperado:** la primera línea incluye `ro`; la segunda dice `OK`. **Si falla:** avisen al docente; no copien el paquete a otra parte para «arreglarlo».

### Verificar y copiar

```bash
(cd "$COMPARTIDA/evidencia" && sha256sum -c ../notas/AR-CASO06-L15.sha256) | tee 00_control/verificacion-recibidas.txt
cp -n "$COMPARTIDA"/evidencia/* 01_copias_trabajo/
(cd 01_copias_trabajo && sha256sum -c "$COMPARTIDA/notas/AR-CASO06-L15.sha256") | tee 00_control/verificacion-copias.txt
chmod a-w 01_copias_trabajo/*
cd 01_copias_trabajo
```

| Parte | Qué hace |
| --- | --- |
| `sha256sum -c manifiesto` | Recalcula el hash de cada archivo listado y lo compara: `OK` o `FAILED` |
| `cp -n` | Copia sin reemplazar archivos que ya existan |
| `chmod a-w` | Quita el permiso de escritura a todos (`a`): un error de comando no puede modificar la copia |
| `cd 01_copias_trabajo` | Los pasos 2 a 5 se ejecutan desde aquí |

**Resultado esperado:** cinco líneas `OK` en cada verificación. **Si aparece `FAILED`:** deténganse, no recalculen el manifiesto y pidan una copia nueva.

## Paso 2. Comparar los correos

### Ver las cabeceras con número de línea

```bash
sed '/^$/q' correo-02.eml | cat -n | tee ../02_salidas/cabeceras-correo-02.txt
```

| Parte | Qué hace |
| --- | --- |
| `sed '/^$/q'` | Imprime el archivo hasta la primera línea vacía (`^$`) y se detiene (`q`): son las cabeceras |
| `cat -n` | Numera cada línea; como las cabeceras empiezan en la línea 1, el número es la línea del archivo |

**Cómo leerlo:** una línea que empieza con espacio o tabulación continúa la cabecera anterior. Las `Received` se leen **de abajo hacia arriba**.

Para mirar una cabecera concreta con sus líneas de continuación:

```bash
grep -n -A3 '^Authentication-Results' correo-02.eml
grep -n -A2 '^Received' correo-02.eml
```

`-A3` muestra además las tres líneas siguientes (*after*).

**Atención:** `correo-02.eml` trae **dos** `Authentication-Results`. La que vale empieza con `mx.andes-soft.invalid;`. La otra la escribió alguien antes de que el correo llegara a ANDES SOFT.

### Enlaces: destino real y texto visible

```bash
grep -n -o 'href="[^"]*">[^<]*' correo-01.eml correo-02.eml | tee ../02_salidas/enlaces.txt
```

| Parte | Qué hace |
| --- | --- |
| `-o` | Muestra solo el fragmento que coincide, no la línea completa |
| `href="[^"]*"` | El atributo `href` con su contenido: `[^"]*` es «cualquier cosa menos comillas» |
| `>[^<]*` | El texto visible del enlace, hasta el siguiente `<` |

**Resultado esperado:** una línea por correo, con la forma `archivo:línea:href="DESTINO">TEXTO`. Comparen DESTINO con TEXTO.

### Extraer el adjunto sin abrirlo

**ripMIME** es una herramienta especializada en separar las partes y extraer los adjuntos de correos MIME. El análisis se realiza sobre la copia de trabajo. Primero miren el nombre y el tipo declarados:

```bash
grep -n -A2 '^Content-Disposition: attachment' correo-02.eml
grep -n '^Content-Type: application/' correo-02.eml
```

Para este correo, el adjunto se llama `actualizacion-seguridad.pdf`. Extráiganlo una sola vez:

```bash
mkdir -p ../02_salidas/adjuntos
ripmime -i correo-02.eml -d ../02_salidas/adjuntos --no-nameless
mv ../02_salidas/adjuntos/actualizacion-seguridad.pdf ../02_salidas/adjuntos/actualizacion-seguridad.pdf.no-abrir
chmod 400 ../02_salidas/adjuntos/*.no-abrir
file ../02_salidas/adjuntos/*.no-abrir | tee ../02_salidas/adjunto-correo-02.txt
sha256sum ../02_salidas/adjuntos/*.no-abrir | tee -a ../02_salidas/adjunto-correo-02.txt
```

| Parte | Qué hace |
| --- | --- |
| `ripmime -i archivo -d carpeta` | Lee el correo y guarda las partes extraídas en la carpeta indicada |
| `--no-nameless` | Omite las partes sin nombre, como el texto y HTML de este correo |
| `mv origen destino` | Renombra el adjunto añadiendo `.no-abrir`; no cambia su contenido |
| `chmod 400` | Deja el archivo con permiso de lectura solo para su dueño, sin escritura ni ejecución |
| `file` | Identifica el tipo real por su contenido, no por la extensión |
| `sha256sum` | Calcula el hash del archivo extraído y lo agrega al registro de salida |

**Resultado esperado:** un archivo `.no-abrir`, su tipo real y su SHA-256 guardados en `adjunto-correo-02.txt`. Contrasten el resultado de `file` con el tipo declarado en el correo. Si necesitan repetir la extracción, usen una carpeta vacía para no mezclar resultados.

**No hagan doble clic** en el archivo extraído ni lo abran con un visor.

### Comparar con la muestra de la lección 13

```bash
cat "$COMPARTIDA/notas/antecedentes-l13-l14.txt"
```

| Parte | Qué hace |
| --- | --- |
| `cat archivo` | Muestra el archivo de antecedentes, incluido el SHA-256 de la muestra de L13 |

Comparen los **64 caracteres completos** del SHA-256 calculado con el de los antecedentes. Si coinciden, el adjunto es el mismo archivo analizado en L13. Esto no demuestra que se haya ejecutado en el equipo del caso.

### Corte entre sesiones: guardar y comprobar el avance

Guarden `actividad-leccion-15.docx` en el anfitrión. Este respaldo parcial conserva controles y salidas de texto, sin copiar el adjunto extraído:

```bash
cd "$CASO"
sha256sum 00_control/* 02_salidas/*.txt | tee 03_anexos/hashes-avance.txt
mkdir -p "$SALIDA/00_control" "$SALIDA/02_salidas" "$SALIDA/03_anexos"
cp 00_control/* "$SALIDA/00_control/"
cp 02_salidas/*.txt "$SALIDA/02_salidas/"
cp 03_anexos/hashes-avance.txt "$SALIDA/03_anexos/"
(cd "$SALIDA" && sha256sum -c 03_anexos/hashes-avance.txt)
cd "$CASO/01_copias_trabajo"
```

Al retomar, vuelvan a definir `COMPARTIDA`, `SALIDA` y `CASO` como en el paso 1. Si cambiaron de estación, repitan ese paso para recrear y verificar las copias; restauren `00_control/`, `02_salidas/` y `03_anexos/` desde su respaldo, y entren en `01_copias_trabajo/`. El docente reserva la holgura para este cambio. Al entregar, se genera el manifiesto final del paso 7.

## Paso 3. El proxy de navegación

```bash
grep -n 'nubefirma' proxy-navegacion.log | tee ../02_salidas/proxy-nubefirma.txt
grep -c 'c2-telemetria' proxy-navegacion.log
grep -n 'portal.andes-soft.invalid' proxy-navegacion.log | tee ../02_salidas/proxy-portal.txt
```

`grep -c` cuenta las líneas que coinciden; `0` significa ninguna.

Cada línea del proxy tiene nueve campos separados por espacios:

| Campo | Ejemplo | Significa |
| --- | --- | --- |
| fecha | `2026-11-02` | |
| hora | `08:46:12` | Hora **local** de Chile (`-03:00`), sin desfase escrito |
| ip_cliente | `10.20.0.45` | Equipo de la oficina |
| usuario | `mrojas` | Cuenta con que el equipo navega |
| metodo | `GET`, `POST`, `CONNECT` | `CONNECT` = conexión HTTPS: solo se ve el sitio y el puerto |
| url | `http://.../acceso.php` | Dirección pedida |
| estado | `200`, `302` | Código de respuesta |
| bytes | `310` | Tamaño de la respuesta |
| ip_destino | `198.51.100.23` | IP del sitio |

### Convertir a UTC

```bash
date -u -d '2026-11-02 08:47:31 -0300' '+%Y-%m-%dT%H:%M:%SZ'
```

| Parte | Qué hace |
| --- | --- |
| `-d '... -0300'` | Indica la fecha, la hora y **su** desfase (el que dice la ficha técnica) |
| `-u` | Muestra el resultado en UTC |
| `'+%Y-%m-%dT%H:%M:%SZ'` | Formato ISO con `Z` (UTC) |

**Resultado esperado:** `2026-11-02T11:47:31Z`. Para las horas de Apache y del registro de autenticación no hace falta convertir: ya están en UTC.

## Paso 4. El portal

### Eventos de una cuenta

```bash
grep -n '"usuario":"mrojas"' portal-auth.jsonl | tee ../02_salidas/auth-mrojas.txt
```

Cada línea es un evento JSON. Lean `ts` (hora UTC), `evento`, `ip_origen`, `resultado`, `mfa` y `sesion`.

### Qué IP aparecen y cuántas veces

```bash
awk '{print $1}' portal-access.log | sort | uniq -c | sort -rn | tee ../02_salidas/portal-ip.txt
```

| Parte | Qué hace |
| --- | --- |
| `awk '{print $1}'` | Imprime la primera columna de cada línea: la IP de origen |
| `sort` | Ordena para que las repeticiones queden juntas |
| `uniq -c` | Cuenta cada valor repetido |
| `sort -rn` | Ordena por número (`-n`), de mayor a menor (`-r`) |

### Aislar una IP

```bash
grep -n '^203.0.113.77 ' portal-access.log | tee ../02_salidas/portal-203.0.113.77.txt
```

`^` exige que la IP esté al **inicio** de la línea, y el espacio final evita confundir `203.0.113.77` con `203.0.113.7`.

### Columnas de una línea de Apache para `awk`

```text
36:203.0.113.77 - - [02/Nov/2026:13:30:58 +0000] "GET /login HTTP/1.1" 200 3410 "-" "Mozilla/5.0 ..."
 $1             $2 $3 $4                    $5     $6   $7     $8       $9  $10
```

`grep -n` agrega `36:` al inicio, pegado a la IP; las demás columnas no cambian. `$7` es la ruta, `$9` el código y `$10` el tamaño.

```bash
awk '{print $9}' ../02_salidas/portal-203.0.113.77.txt | sort | uniq -c
awk -F'"' '{print $6}' ../02_salidas/portal-203.0.113.77.txt | sort | uniq -c
```

| Comando | Qué cuenta |
| --- | --- |
| `awk '{print $9}'` | Los códigos de respuesta |
| `awk -F'"' '{print $6}'` | Con las comillas como separador (`-F'"'`), el sexto campo es el *User-Agent* |

### Documentos entregados y bytes

```bash
awk '$9==200 && $7 ~ /descargar/' ../02_salidas/portal-203.0.113.77.txt | grep -o 'id=[0-9]*' | sort -u | tee ../02_salidas/ids-descargados.txt | wc -l
awk '$9==200 && $7 ~ /descargar/ {s+=$10} END {print s}' ../02_salidas/portal-203.0.113.77.txt
```

| Parte | Qué hace |
| --- | --- |
| `$9==200 && $7 ~ /descargar/` | Solo líneas con código 200 **y** ruta que contiene `descargar` |
| `grep -o 'id=[0-9]*'` | Extrae el número de documento |
| `sort -u` | Ordena y deja cada número una sola vez |
| `{s+=$10} END {print s}` | Suma la columna 10 (bytes) y la imprime al final |

**Cuidado:** el primer comando cuenta documentos **distintos**; el segundo suma los bytes de **todas** las descargas con 200, incluida alguna repetida. Díganlo así en la actividad DOCX.

### Qué documentos eran

```bash
column -s, -t < "$COMPARTIDA/notas/documentos-portal.csv" | grep -E '^(id|10[456])'
```

`column -s, -t` alinea el CSV usando la coma como separador; `grep -E '^(id|10[456])'` deja el encabezado y los documentos que empiezan con 104, 105 o 106.

### Intentos de ataque en los parámetros

Las solicitudes con `..%2F` (es `../` codificado) o `%27` (es una comilla `'`) ya están en `portal-203.0.113.77.txt`. Para verlas solas:

```bash
grep -E '\.\.%2F|%27' ../02_salidas/portal-203.0.113.77.txt
```

Miren el código (`$9`): `400` es rechazo; `500` es un error del servidor, que no prueba ni éxito ni rechazo.

## Paso 6. Hashes de las salidas

```bash
cd "$CASO"
sha256sum 02_salidas/*.txt | tee 03_anexos/hashes-salidas.txt
```

Si después vuelven a ejecutar un comando con `tee`, la salida cambia de hash: recalculen este manifiesto antes del paso 7.

## Paso 7. Respaldo

```bash
cd "$CASO"
sha256sum 00_control/* 02_salidas/*.txt 03_anexos/hashes-salidas.txt | tee 03_anexos/hashes-entrega.txt
mkdir -p "$SALIDA/02_salidas"
cp -r 00_control 03_anexos "$SALIDA/"
cp 02_salidas/*.txt "$SALIDA/02_salidas/"
(cd "$SALIDA" && sha256sum -c 03_anexos/hashes-entrega.txt)
```

**Resultado esperado:** todas las líneas en `OK`. El adjunto extraído no se respalda.

## Problemas frecuentes

| Síntoma | Causa probable | Qué hacer |
| --- | --- | --- |
| `No such file or directory` con `$COMPARTIDA` vacío | Terminal nueva: la variable no existe | Repitan las tres líneas de variables |
| `Permission denied` al guardar con `tee` | Están dentro del paquete o de `01_copias_trabajo` sin `../` | Vuelvan a `01_copias_trabajo` y escriban en `../02_salidas/` |
| `sed '/^$/q'` muestra todo el correo | El archivo usa saltos de línea de Windows | Usen `sed '/^\r\?$/q'` y avisen al docente |
| `grep` no encuentra `href` | Comillas mal copiadas | Copien el comando desde el PDF o la guía, sin reescribirlo |
| `ripmime: command not found` | Falta la herramienta en la estación | Avisen al docente; debe estar instalada antes de la práctica |
| No aparece el adjunto tras usar ripMIME | Se usó el correo sin adjunto o una carpeta distinta | Comprueben `-i correo-02.eml` y la carpeta de `-d` |
| Aparecen adjuntos duplicados o con otro nombre | Se repitió la extracción en la misma carpeta | Usen una carpeta vacía; no mezclen resultados de ejecuciones |
| `awk` imprime columnas corridas | Se usó otra separación (`-F`) en el comando anterior | Revisen el comando: `-F'"'` solo se usa para el *User-Agent* |
| `wc -l` da 25 en lugar de un número menor | Se omitió `sort -u` | Sin `-u`, un documento descargado dos veces se cuenta dos veces |
| `sha256sum -c` en el respaldo marca `FAILED` | Se volvió a ejecutar un `tee` después del manifiesto | Recalculen `hashes-entrega.txt` y copien de nuevo |
