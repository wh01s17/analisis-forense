---
title: "Guía práctica de comandos Lección 12 - Plan forense y cadena de custodia"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "12"
author: Jordy
start: 2026-10-27
end: 2026-11-02
created_at: 2026-10-24
aliases:
  - "Guía práctica de comandos Lección 12 - Plan forense y cadena de custodia"
---

# Guía práctica de comandos Lección 12 - Plan forense y cadena de custodia

## Recorrido seguro

```text
autorización y alcance -> carpeta de solo lectura -> manifiesto OK -> recepción en custodia
-> copias de trabajo OK -> filtros con intervalo y exclusiones -> salidas guardadas con hash
-> hallazgo reproducible -> manifiesto OK -> respaldo verificado
```

La evidencia original permanece en `/media/sf_LAB12_ENTREGA/evidencia/`. El trabajo se hace en `~/forense/LAB-12/NS-CASO05/`. Ningún comando requiere red ni privilegios de administrador.

## Herramientas

| Comando | Uso en la práctica |
| --- | --- |
| `sha256sum -c` | Compara archivos con el manifiesto recibido. |
| `sha256sum` | Calcula el hash de un archivo para custodia o anexos. |
| `mkdir -p` | Crea la estructura de carpetas del caso. |
| `cp -n` | Copia sin sobrescribir un archivo que ya existe. |
| `chmod a-w` | Quita el permiso de escritura a las copias de trabajo. |
| `column -s, -t` | Muestra un CSV como tabla legible. |
| `awk -F,` | Filtra filas de un CSV por columna y por intervalo. |
| `grep -n` / `grep -nF` | Busca texto y muestra el número de línea. |
| `date -u -d` | Convierte una hora con desfase a UTC. |
| `tee` | Muestra la salida y la guarda en un archivo al mismo tiempo. |
| `cmp -s` | Comprueba que un respaldo sea idéntico al original. |

Comprueben disponibilidad:

```bash
command -v sha256sum awk grep column date tee cmp
```

## Variables y estructura del caso

```bash
LAB=~/forense/LAB-12
COMPARTIDA=/media/sf_LAB12_ENTREGA
RECUPERACION=/media/sf_LAB12_SALIDA
EV="$COMPARTIDA/evidencia"
CASO="$LAB/NS-CASO05"
CUSTODIA="$CASO/00_control/custodia-NS-CASO05.csv"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_informe,04_anexos}
```

Las llaves `{...}` crean varias carpetas con un solo comando. Las variables se pierden al cerrar la terminal: al abrir otra o al reanudar, vuelvan a ejecutar este bloque y a definir la función `registrar`; no repitan las recepciones ya registradas.

| Carpeta | Contenido |
| --- | --- |
| `00_control` | Bitácora, verificaciones y cadena de custodia. |
| `01_copias_trabajo` | Copias verificadas y sin escritura sobre las que se analiza. |
| `02_salidas` | Resultados de cada comando, tal como salieron. |
| `03_informe` | Borradores del informe. |
| `04_anexos` | Hashes de salidas y material que respalda los hallazgos. |

## Verificar y copiar

Lean antes la autorización (paso 2). Tras verificar el manifiesto, registren la recepción en custodia (paso 3) y recién entonces creen las copias.

```bash
(cd "$EV" && sha256sum -c ../notas/NS-CASO05.sha256)
cp -n "$EV"/* "$CASO/01_copias_trabajo/"
(cd "$CASO/01_copias_trabajo" && sha256sum -c "$COMPARTIDA/notas/NS-CASO05.sha256")
chmod a-w "$CASO/01_copias_trabajo"/*
```

- Los paréntesis ejecutan el `cd` en una subcapa: al terminar, la terminal sigue en la carpeta anterior.
- El manifiesto contiene nombres sin ruta; por eso se ejecuta desde la carpeta que contiene los archivos.
- `OK` significa igualdad con la referencia. No demuestra que la fuente sea completa ni verdadera.
- Ante `FAILED`, deténganse; no creen un manifiesto nuevo.

## Leer los documentos del caso

```bash
cat "$COMPARTIDA/caso/autorizacion.txt"
sed -n '1,40p' "$COMPARTIDA/notas/ficha-tecnica.txt"
column -s, -t < "$COMPARTIDA/caso/inventario-preliminar.csv"
column -s, -t < "$COMPARTIDA/caso/cadena-custodia-recibida.csv"
```

`column` no modifica el archivo; solo alinea columnas en pantalla. Para citar una fila, cuenten el encabezado como línea 1, o usen `cat -n archivo.csv`.

## Registrar custodia

El encabezado se crea solo si el archivo no existe o está vacío; así, volver a ejecutar el bloque al reanudar no borra filas. Luego se define la función:

```bash
if [ ! -s "$CUSTODIA" ]; then
  printf 'fecha_hora_utc,evidencia,sha256,entrega,recibe,proposito,estado,archivo\n' > "$CUSTODIA"
fi
registrar() {
  local hash
  hash=$(sha256sum "$2") || return
  printf '%s,%s,%s,%s,%s,%s,%s,%s\n' "$(date -u +%FT%TZ)" "$1" "${hash%% *}" "$3" "$4" "$5" "$6" "$(basename "$2")" >> "$CUSTODIA"
}
```

Uso:

```bash
registrar ID RUTA "Entrega" "Recibe" "Propósito" "Estado"
```

| Argumento | Ejemplo |
| --- | --- |
| `ID` | `NS05-EV04` |
| `RUTA` | `"$EV/cloud-audit.jsonl"` |
| `Entrega` | `"Docente (custodio del paquete)"` |
| `Recibe` | `"Equipo Norte"` |
| `Propósito` | `"Recepción"` |
| `Estado` | `"Original en solo lectura; manifiesto OK"` |

- `date -u +%FT%TZ` escribe la hora actual en UTC, por ejemplo `2026-10-27T22:14:05Z`.
- `[ ! -s archivo ]` es verdadero si el archivo no existe o está vacío.
- La variable local `hash` recibe el resultado de `sha256sum`; `${hash%% *}` deja solo el hash, sin el nombre del archivo. Si el cálculo falla, `|| return` termina la función sin agregar una fila con el hash vacío.
- `>>` **agrega** una fila al final. Un solo `>` borraría el registro anterior: por eso el encabezado está protegido por la comprobación.
- No escriban comas dentro de los argumentos: el registro es CSV y cada coma crea una columna.

Si se equivocan en una fila, no la borren: agreguen otra que la corrija y explíquenlo en la bitácora. Un registro de custodia se corrige añadiendo, no reescribiendo.

## Filtrar por intervalo autorizado

### CSV con hora local

```bash
awk -F, 'NR==1 || ($2=="r.ibarra" && $1>="2026-10-22 20:00:00" && $1<="2026-10-22 23:00:00") {print NR": "$0}' 01_copias_trabajo/vpn-2026-10-22.csv
```

- `-F,` separa columnas por coma; `$1` es la primera columna y `$2` la segunda.
- `NR` es el número de línea; `NR==1` conserva el encabezado.
- Las fechas `AAAA-MM-DD hh:mm:ss` se pueden comparar como texto porque van de lo mayor a lo menor.
- `print NR": "$0` imprime el número de línea seguido de la fila completa, listo para citar.

### JSONL en UTC

```bash
grep -nF -e 'r.ibarra' -e 'SH-881' 01_copias_trabajo/cloud-audit.jsonl | awk -F'"' '$4>="2026-10-22T23:00:00Z" && $4<="2026-10-23T02:00:00Z"'
```

- `grep -F` busca el texto literal; `-e` permite varias búsquedas a la vez.
- Al separar por comillas (`-F'"'`), el cuarto campo es el valor de `ts`, la hora del evento.
- El número antes de `:` es la línea del archivo original.

### Correo con exclusiones

```bash
awk -F, 'NR==1 || ($3=="r.ibarra@nortesur.invalid" && $2>="2026-10-22T20:00:00-03:00" && $2<="2026-10-22T23:00:00-03:00" && $8=="Enviados" && $4 !~ /@(nortesur\.invalid|correo-personal\.invalid)$/) {print NR": "$0}' 01_copias_trabajo/correo-metadatos.csv
```

- `$8=="Enviados"` deja solo la carpeta de enviados.
- `$4 !~ /.../` excluye destinatarios internos (`nortesur.invalid`) y el dominio que en este caso sintético identifica mensajes personales (`correo-personal.invalid`). Esa regla vale solo para NS-CASO05.
- Estos CSV sintéticos no tienen comas dentro de los campos; por eso basta `awk -F,`.

### Guardar la salida

Agreguen `| tee 02_salidas/nombre.txt` al final del comando. La salida guardada es la que se cita en el hallazgo y se anexa al informe:

```bash
sha256sum 02_salidas/*.txt | tee 04_anexos/hashes-salidas.txt
```

## Convertir horas

```bash
date -u -d '2026-10-22 20:52:10 -03:00' '+%F %T UTC'
date -u -d '2026-10-22T21:24:40-03:00' '+%F %T UTC'
```

La VPN no escribe el desfase; la ficha técnica indica que es hora de Chile, por eso se agrega ` -03:00` al convertir. El correo ya incluye `-03:00`. No sumen tres horas a mano después de usar `date`: duplicarían la conversión.

| Hora local `-03:00` | Hora UTC |
| --- | --- |
| 22-10-2026 20:00 | 22-10-2026 23:00Z |
| 22-10-2026 23:00 | 23-10-2026 02:00Z |

## Respaldar

```bash
cp 00_control/bitacora-equipo.md "$CUSTODIA" "$RECUPERACION/"
cmp -s 00_control/bitacora-equipo.md "$RECUPERACION/bitacora-equipo.md" && echo 'OK: respaldo'
```

`cmp -s` no muestra nada si los archivos son idénticos; el `&&` imprime `OK` solo en ese caso.

## Interpretar las fuentes

| Fuente | Afirmación válida | Sobreafirmación que debe evitarse |
| --- | --- | --- |
| VPN | Una cuenta conectó un equipo en un intervalo y transfirió una cantidad de bytes. | La VPN muestra qué archivo salió. |
| Auditoría cloud | Una sesión de la cuenta cargó un objeto y creó un enlace. | La persona titular estaba frente al equipo. |
| Acceso anónimo al enlace | Alguien con el enlace accedió desde una IP. | La "competencia" recibió la oferta. |
| Metadatos de correo | La cuenta envió un mensaje con el enlace a un dominio externo. | Se conoce el contenido del mensaje o del objeto. |
| Hash informado por el proveedor | El proveedor registró ese valor para el objeto. | El equipo verificó el contenido del objeto. |

## Errores frecuentes

- Trabajar sobre la carpeta compartida en vez de las copias de trabajo.
- Usar `>` en lugar de `>>` y borrar la cadena de custodia.
- Escribir comas en los argumentos de `registrar`.
- Comparar hora local de la VPN directamente con UTC de la auditoría cloud.
- Citar "el log" sin archivo y línea.
- Analizar mensajes personales o registros fuera del intervalo "para ver si sirven".
- Redactar el hallazgo sin guardar la salida que lo respalda.
