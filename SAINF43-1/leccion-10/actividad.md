---
title: "Práctica guiada 10 - Correlación de evidencia cloud y móvil"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "10"
author: Jordy
start: 2026-10-13
end: 2026-10-19
created_at: 2026-10-11
aliases:
  - "Práctica guiada 10 - Correlación de evidencia cloud y móvil"
---

# Actividad Lección 10 - Correlación de evidencia cloud y móvil

- **Modalidad:** trabajo en parejas.
- **Duración:** 80 minutos de laboratorio y 40 minutos de cierre.
- **Caso:** `CLOUD-MOVIL-CASO04` - MUELLE SUR LTDA (sociedad de responsabilidad limitada; ficticia).
- **Entorno:** estación Kali con evidencia sintética montada en solo lectura y carpeta de salida con escritura; no se requiere conexión de red.
- **Carácter:** formativo, sin calificación ni entrega evaluada.
- **Producto:** este archivo completado como `bitacora-equipo.md`, con cronología, correlaciones y plan mínimo de adquisición.

## El caso

MUELLE SUR LTDA investiga si la cuenta corporativa `m.vera@muellesur.invalid` compartió el archivo `costos-2027.xlsx` fuera de la organización entre el **10-10-2026 08:00** y el **12-10-2026 18:00**, hora de Chile (`-03:00`).

TI (tecnologías de la información) y el proveedor entregaron seis archivos sintéticos. Las horas UTC (tiempo universal coordinado) usan `Z` para indicar esa referencia horaria:

| Fuente | Contenido | Tiempo |
| --- | --- | --- |
| `saas-audit.jsonl` | Auditoría SaaS (software como servicio), en JSONL (JSON por líneas); JSON (notación de objetos de JavaScript) representa cada evento | UTC (`Z`) |
| `identity.csv` | Inicio de sesión, MFA (autenticación multifactor), sesión, dispositivo e IP (protocolo de Internet) pública; CSV (valores separados por comas) | UTC (`Z`) |
| `mdm.csv` | MDM (gestión de dispositivos móviles): asignación e inventario del dispositivo corporativo | UTC (`Z`) |
| `proxy.log` | Conexiones de red del entorno corporativo | Hora local `-03:00` |
| `MOVIL-CASO04.sqlite` | Adquisición lógica seleccionada de la aplicación corporativa | UTC (`Z`) |
| `costos-2027.xlsx` | Objeto exportado por el proveedor | El contenido se valida por SHA-256 (algoritmo hash seguro de 256 bits) |

La adquisición móvil **no es una imagen física**: contiene metadatos, eventos y caché de la aplicación corporativa seleccionada. No incluye mensajes personales, memoria completa ni datos borrados. Todas las direcciones, cuentas y entidades son ficticias o reservadas para documentación.

> ¿Qué secuencia puede reconstruirse entre la cuenta, la sesión, el dispositivo, el objeto y la compartición, y qué no permiten atribuir estas fuentes?

## Ruta de trabajo y tiempos

| Tramo | Tiempo | Producto esperado |
| --- | ---: | --- |
| Laboratorio | 80 min | Completar los pasos 1 a 5: preservación, identificación, objeto, cronología y correlación. |
| Cierre guiado | 40 min | Completar los pasos 6 y 7: plan mínimo de adquisición, conclusión, revisión y respaldo. |

El **plan de adquisición del paso 6** es la evidencia principal de esta lección. Si el laboratorio se retrasa, documenten al menos tres relaciones —identidad/SaaS, SaaS/móvil y compartición/acceso— y continúen con el plan; no omitan la verificación de integridad.

## 1. Preparar y verificar

Verificar significa comprobar que los archivos no cambiaron. Un resultado `OK` (correcto) indica que el hash calculado coincide con el manifiesto; no demuestra quién creó el archivo.

Copien `LAB10_ENTREGA` al anfitrión y configuren en VirtualBox `LAB10_ENTREGA` en **solo lectura** y `LAB10_SALIDA` con escritura, ambas con montaje automático.

En Kali ejecuten. `LAB` (laboratorio), `EV` (evidencia) y `BD` (base de datos) son variables con las rutas de trabajo. La función `sql` ejecuta consultas SQL (lenguaje de consulta estructurado) sin modificar la base de datos:

```bash
LAB=~/forense/LAB-10
COMPARTIDA=/media/sf_LAB10_ENTREGA
RECUPERACION=/media/sf_LAB10_SALIDA
EV="$COMPARTIDA/evidencia"
BD="file:$EV/MOVIL-CASO04.sqlite?immutable=1"
sql() { [ -n "$BD" ] || { echo "ERROR: falta definir BD" >&2; return 1; }; sqlite3 -readonly -header -column "$BD" "$@"; }
mkdir -p "$LAB"/{notas,entrega}
cd "$LAB"
(cd "$EV" && sha256sum -c ../notas/CLOUD-MOVIL-CASO04.sha256) | tee notas/verificacion-inicial.txt
cp -n "$COMPARTIDA/documentos/actividad-leccion-10.md" entrega/bitacora-equipo.md
chmod u+w entrega/bitacora-equipo.md
```

Comprueben los permisos reales de las carpetas:

```bash
if touch "$COMPARTIDA/.prueba" 2>/dev/null; then rm -f "$COMPARTIDA/.prueba"; echo 'ERROR: la evidencia admite escritura'; else echo 'OK: evidencia en solo lectura'; fi
if touch "$RECUPERACION/.prueba" 2>/dev/null; then rm -f "$RECUPERACION/.prueba"; echo 'OK: salida con escritura'; else echo 'ERROR: la salida no admite escritura'; fi
```

| Control | Resultado |
| --- | --- |
| Seis archivos terminan en `OK` | [Sí / No] |
| Versión de `sqlite3` | [Completar] |
| La carpeta de evidencia rechaza escritura | [Sí / No] |
| La carpeta de salida admite escritura | [Sí / No] |

Ante un `FAILED` (fallo de verificación), deténganse y avisen. No recalculen el manifiesto. No abran el XLSX (formato de libro de Excel) con una aplicación de oficina; solo se usará su hash.

Lean el comprobante:

```bash
sed -n '1,20p' "$COMPARTIDA/notas/preservacion-proveedor.txt"
```

```text
Identificador de preservación:
Cuenta, objeto e intervalo preservados:
¿El comprobante acredita preservación, entrega o ambas?:
```

## 2. Identificar cuenta, sesión y dispositivo

Busquen identificadores que se repiten entre fuentes, como cuenta, sesión, dispositivo o IP. La coincidencia crea un vínculo técnico, pero todavía no identifica a la persona que operaba el equipo.

Ejecuten las búsquedas y citen archivo y L (línea), por ejemplo `identity.csv` L3. En las tablas de la base de datos, `id` (identificador de fila) permite citar el registro.

```bash
grep -n 'S-A913\|DEV-MOB-042' "$EV/identity.csv"
grep -n 'DEV-MOB-042' "$EV/mdm.csv"
grep -n '10.8.40.23' "$EV/proxy.log"
sql "SELECT * FROM device_info;"
```

| Dato | Fuente y línea/fila | Valor |
| --- | --- | --- |
| Cuenta y resultado de autenticación | [Completar] | [Completar] |
| Método MFA y sesión | [Completar] | [Completar] |
| Dispositivo y serie | [Completar] | [Completar] |
| IP local e IP pública | [Completar] | [Completar] |
| Tipo, hora y herramienta de adquisición móvil | [Completar] | [Completar] |

Conviertan la primera conexión relevante del proxy a UTC:

```bash
date -u -d '2026-10-12T13:47:05.120-03:00' '+%F %T.%3N UTC'
```

```text
Hora UTC obtenida:
¿Qué une el proxy con el dispositivo?
¿Qué no demuestra una conexión CONNECT (método del proxy para abrir un túnel de conexión) cifrada?
```

## 3. Examinar el objeto sin abrirlo

El nombre y el tamaño orientan; el SHA-256 permite una comparación más fuerte. Un mismo hash respalda igualdad de bytes, no autoría ni procedencia.

```bash
sha256sum "$EV/costos-2027.xlsx"
file "$EV/costos-2027.xlsx"
unzip -t "$EV/costos-2027.xlsx"
sql "SELECT object_id,name,size_bytes,sha256,last_open_utc FROM files;"
```

| Dato | Exportación cloud | Adquisición móvil | ¿Coincide? |
| --- | --- | --- | :---: |
| Nombre | [Completar] | [Completar] | [ ] |
| Tamaño | [Completar] | [Completar] | [ ] |
| SHA-256 | [Completar] | [Completar] | [ ] |

```text
¿Qué permite sostener la coincidencia del hash?
¿Qué no permite sostener?
```

## 4. Reconstruir la secuencia

Ordenen los eventos por hora UTC. Cada fila debe representar un hecho observable y citar la fuente que lo registra.

Extraigan solo los eventos relacionados con `OBJ-7742`, `SH-204` o `S-A913`.

```bash
grep -n 'OBJ-7742\|SH-204\|S-A913' "$EV/saas-audit.jsonl"
sql "SELECT id,ts_utc,app_account,session_id,action,object_id,share_id,result FROM sync_events ORDER BY ts_utc;"
sql "SELECT * FROM share_cache;"
```

Completen ocho hitos en UTC. Citen línea de archivo o tabla e `id`.

| # | Hito | Fuente y referencia | Hora UTC | Identificadores observados |
| ---: | --- | --- | --- | --- |
| 1 | Autenticación de la cuenta | [Completar] | [Completar] | [Completar] |
| 2 | Inicio de sesión SaaS/móvil | [Completar] | [Completar] | [Completar] |
| 3 | Apertura del objeto en SaaS | [Completar] | [Completar] | [Completar] |
| 4 | Descarga/sincronización | [Completar] | [Completar] | [Completar] |
| 5 | Apertura local registrada | [Completar] | [Completar] | [Completar] |
| 6 | Preparación de compartición móvil | [Completar] | [Completar] | [Completar] |
| 7 | Creación de compartición SaaS | [Completar] | [Completar] | [Completar] |
| 8 | Acceso desde la cuenta externa | [Completar] | [Completar] | [Completar] |

## 5. Correlacionar sin sobreafirmar

Una unión directa comparte un identificador; una unión por valor comparte un dato como hash o IP; una unión temporal solo ocurre cerca en el tiempo y suele tener menor confianza.

Clasifiquen cada unión como **directa** (mismo identificador), **por valor** (mismo hash, IP u otro dato) o **temporal** (solo cercanía horaria).

| Relación | Dato de unión | Tipo y confianza | Qué permite sostener | Limitación |
| --- | --- | --- | --- | --- |
| Identidad ↔ SaaS | [Completar] | [Completar] | [Completar] | [Completar] |
| MDM ↔ proxy | [Completar] | [Completar] | [Completar] | [Completar] |
| SaaS ↔ móvil | [Completar] | [Completar] | [Completar] | [Completar] |
| Objeto cloud ↔ metadato móvil | [Completar] | [Completar] | [Completar] | [Completar] |
| Compartición ↔ acceso externo | [Completar] | [Completar] | [Completar] | [Completar] |

Respondan:

```text
Hecho mejor sustentado:
Inferencia razonable:
¿Puede atribuirse la acción a una persona física?:
Fuente adicional que ayudaría:
```

## 6. Cerrar con un plan de adquisición

Para completar cada fila, determinen qué obtener primero, cómo y quién lo obtiene, cómo se protege su integridad y qué límite o falla podría presentarse.

Vuelvan al momento inicial del caso, antes de recibir las exportaciones. Propongan un plan mínimo que permita obtener las fuentes usadas en el laboratorio sin ampliar el alcance.

| Prioridad y fuente | Método y responsable | Control de integridad/custodia | Limitación o contingencia |
| --- | --- | --- | --- |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |

El plan debe incluir al menos auditoría/objeto SaaS, dispositivo móvil y una fuente corporativa. Preservar no equivale a divulgar; una adquisición lógica no promete datos borrados; y las decisiones de alcance o jurisdicción se escalan.

## 7. Concluir y respaldar

Organicen la conclusión en este orden: evidencia observada → interpretación prudente → limitaciones → acción de preservación prioritaria.

Escriban una conclusión de 90 a 120 palabras. Debe incluir:

- la secuencia mejor sustentada con al menos tres referencias;
- la coincidencia del objeto;
- la diferencia entre cuenta, sesión, dispositivo y persona;
- dos limitaciones de las fuentes;
- una acción de preservación que habría sido prioritaria antes de la adquisición.

> [Completar]

Verifiquen de nuevo y respalden la bitácora guardada:

```bash
(cd "$EV" && sha256sum -c ../notas/CLOUD-MOVIL-CASO04.sha256) | tee notas/verificacion-final.txt
cp "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/"
cmp -s "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/bitacora-equipo.md" && echo 'OK: respaldo' || echo 'ERROR: revisar copia con el docente'
```

Antes de apagar, respalden `bitacora-equipo.md` desde Windows por el medio indicado. No entreguen ni copien la evidencia.

## Comprobación final

- [ ] Se verificó la evidencia al inicio y al final.
- [ ] Todas las horas de la cronología están en UTC.
- [ ] Cada afirmación cita archivo/línea o tabla/fila.
- [ ] El hash se interpreta como igualdad de bytes, no como prueba de autoría.
- [ ] El proxy no se usa para afirmar contenido cifrado.
- [ ] La adquisición lógica no se presenta como imagen física completa.
- [ ] La conclusión distingue cuenta, dispositivo y persona.
- [ ] El plan final incluye método, responsable, integridad y limitación.
