---
title: "Guía práctica de comandos Lección 10 - Evidencia cloud y móvil"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "10"
author: Jordy
start: 2026-10-13
end: 2026-10-19
created_at: 2026-10-11
aliases:
  - "Guía práctica de comandos Lección 10 - Evidencia cloud y móvil"
---

# Guía práctica de comandos Lección 10 - Evidencia cloud y móvil

## Recorrido seguro

```text
carpeta de solo lectura -> manifiesto OK -> inventario -> búsquedas por identificador
-> consultas SQLite de solo lectura -> normalización UTC -> correlación -> manifiesto OK
```

La evidencia permanece en `/media/sf_LAB10_ENTREGA/evidencia/`. Las salidas y la bitácora se guardan en `~/forense/LAB-10/`. Ningún comando requiere red ni privilegios de administrador.

## Herramientas

| Comando | Uso en la práctica |
| --- | --- |
| `sha256sum -c` | Compara los archivos con el manifiesto recibido. |
| `grep -n` | Busca identificadores y muestra el número de línea. |
| `sqlite3 -readonly` | Consulta la adquisición lógica sin modificarla. |
| `date -u -d` | Convierte una hora con desfase a UTC. |
| `file` | Identifica el tipo aparente del objeto. |
| `unzip -t` | Comprueba la estructura ZIP interna del XLSX sin abrirlo. |
| `cmp -s` | Comprueba que el respaldo de la bitácora sea idéntico. |

Comprueben disponibilidad:

```bash
command -v sqlite3 sha256sum grep date file unzip
sqlite3 --version
```

## Variables y consulta protegida

```bash
LAB=~/forense/LAB-10
COMPARTIDA=/media/sf_LAB10_ENTREGA
RECUPERACION=/media/sf_LAB10_SALIDA
EV="$COMPARTIDA/evidencia"
BD="file:$EV/MOVIL-CASO04.sqlite?immutable=1"
sql() { [ -n "$BD" ] || { echo "ERROR: falta definir BD" >&2; return 1; }; sqlite3 -readonly -header -column "$BD" "$@"; }
```

- `-readonly` impide sentencias de escritura.
- `immutable=1` evita bloqueos y archivos auxiliares junto a la evidencia.
- `-header -column` hace legible la salida.
- `"$@"` pasa a SQLite la consulta escrita después de `sql`.

No abran la base con `sqlite3 ruta.sqlite` sin estas opciones.

## Verificar el manifiesto

```bash
(cd "$EV" && sha256sum -c ../notas/CLOUD-MOVIL-CASO04.sha256)
```

`OK` significa igualdad con la referencia del manifiesto. No demuestra que la fuente sea completa o verdadera. Ante `FAILED`, deténganse; no creen un manifiesto nuevo.

## Buscar identificadores

```bash
grep -n 'S-A913\|DEV-MOB-042' "$EV/identity.csv"
grep -n 'DEV-MOB-042' "$EV/mdm.csv"
grep -n '10.8.40.23' "$EV/proxy.log"
grep -n 'OBJ-7742\|SH-204\|S-A913' "$EV/saas-audit.jsonl"
```

Dentro del patrón de `grep`, `\|` significa «o». El número antes de cada resultado es la línea que se cita. JSONL guarda un objeto JSON completo por línea: no es necesario reformatearlo para esta práctica.

## Consultar la adquisición móvil

```bash
sql ".tables"
sql ".schema files"
sql "SELECT * FROM device_info;"
sql "SELECT object_id,name,size_bytes,sha256,last_open_utc FROM files;"
sql "SELECT id,ts_utc,app_account,session_id,action,object_id,share_id,result FROM sync_events ORDER BY ts_utc;"
sql "SELECT * FROM share_cache;"
```

Los comandos que empiezan con punto son propios de la consola SQLite. `SELECT` solo lee. Para citar una fila, usen tabla e `id`, por ejemplo `sync_events.id=4`.

## Convertir la hora del proxy

```bash
date -u -d '2026-10-12T13:47:05.120-03:00' '+%F %T.%3N UTC'
```

El sufijo `-03:00` ya contiene el desfase. No sumen tres horas manualmente y vuelvan a ejecutar `date`: eso duplicaría la conversión.

## Examinar el objeto sin ejecutarlo

```bash
sha256sum "$EV/costos-2027.xlsx"
file "$EV/costos-2027.xlsx"
unzip -t "$EV/costos-2027.xlsx"
```

Un XLSX es un contenedor ZIP. `unzip -t` prueba su estructura sin extraer ni abrir hojas. La coincidencia de SHA-256 demuestra igualdad de bytes respecto del valor comparado; no demuestra autoría, intención ni procedencia completa.

## Interpretar las fuentes

| Fuente | Afirmación válida | Sobreafirmación que debe evitarse |
| --- | --- | --- |
| Identidad | Una cuenta completó MFA y recibió una sesión. | La persona titular estaba frente al equipo. |
| MDM | Un dispositivo estaba asignado y reportó una IP. | El inventario contiene toda la actividad del dispositivo. |
| Proxy | Una IP estableció una conexión cifrada a un destino. | El proxy vio el archivo o la acción SaaS. |
| SaaS | El proveedor registró un evento con IDs concretos. | El registro es necesariamente completo. |
| Adquisición lógica | La aplicación conservó los artefactos seleccionados. | Se adquirió toda la memoria o los datos borrados. |

## Errores frecuentes

- Ejecutar consultas sin redefinir `BD` al abrir otra terminal.
- Comparar directamente hora local con UTC.
- Citar «el log» sin línea o tabla/fila.
- Interpretar `CONNECT` como descarga de un archivo específico.
- Confundir cuenta, sesión, dispositivo y persona.
- Reemplazar el manifiesto recibido por hashes nuevos.
