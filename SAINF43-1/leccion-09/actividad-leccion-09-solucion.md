---
title: "Práctica guiada 09 - ¿Qué cambió en la base de datos?"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "09"
author: Jordy
start: 2026-10-05
end: 2026-10-07
created_at: 2026-10-04
aliases:
  - "Práctica guiada 09 - ¿Qué cambió en la base de datos?"
---

# Práctica guiada 09 - ¿Qué cambió en la base de datos?

**En parejas · 80 minutos · Sin nota ni entrega para calificación.** Completen este archivo como bitácora.

## El caso y los datos necesarios

MUELLE SUR LTDA recibió el siguiente reclamo: el pago `9921` del proveedor `184` fue enviado a otra cuenta. Reconstruyan el cambio y expliquen qué permiten afirmar las fuentes.

- `app-erp.log`: solicitudes de la aplicación; hora local `-03:00`, escrita al terminar cada solicitud.
- `postgresql.log`: sentencias del motor, en UTC. PID identifica el proceso de la conexión; `xid`, la transacción. La conexión puede reutilizarse para distintos usuarios.
- `BD-CASO03.sqlite`: extracción de TI del 16-09-2026, con horas UTC. La aplicación usa la cuenta técnica `erp_app`; declara el usuario y la solicitud con `SET LOCAL`, y un trigger los guarda en la auditoría.

**Solicitud** (`req`/`request_id`) = una petición; **transacción** (`xid`/`txid`) = operaciones de la base agrupadas; **auditoría** = registro del cambio. `ROLLBACK` revierte; una orden `COMMIT` se corrobora con datos persistidos. No hay registros de VPN, AD ni del equipo del usuario. Los relojes tienen una desviación informada menor a 50 ms: diferencias pequeñas entre servidores no prueban el orden exacto ni quién actuó.

## 1. Preparación

Copiar `LAB09_ENTREGA` al anfitrión y configura en VirtualBox dos carpetas: `LAB09_ENTREGA` en **solo lectura** y `LAB09_SALIDA` con escritura, ambas con montaje automático. Confirma acceso y permisos antes de avanzar.

En una terminal de Kali, ejecuten el bloque. Abran después `~/forense/LAB-09/entrega/bitacora-equipo.md` en el editor, junto a la terminal. Los comandos no escriben en la bitácora: copien los datos de la terminal a las tablas y guarden antes de respaldar en el paso 4. Si abren otra terminal, repitan el bloque para recuperar las variables; `cp -n` conserva la bitácora existente.

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

**Antes de seguir:** confirmen tres archivos `OK` en el manifiesto. Ante errores o `FAILED`, deténganse y avisen; no recalculen el manifiesto. Solo ejecuten consultas de lectura sobre la evidencia.

## 2. Reconstruir cinco hitos

Ejecuten y lean estas salidas. `grep -n` muestra el número de línea; en SQLite citen la tabla y su identificador de fila.

```bash
grep -n 'R-5102\|R-5103' "$EV/app-erp.log"
grep -n ' 13:03:\| 13:04:' "$EV/postgresql.log"
sql "SELECT * FROM auditoria_proveedores WHERE proveedor_id=184 ORDER BY ts_utc;"
sql "SELECT id,banco,cuenta FROM proveedores WHERE id=184;"
sql "SELECT * FROM pagos WHERE id=9921;"
```

Modelo de conversión: `10:03:15.433-03:00` equivale a `13:03:15.433 UTC`. Para convertir la respuesta de `R-5103`, copien su hora completa:

```bash
date -u -d '2026-09-15T10:04:02.231-03:00' '+%F %T.%3N UTC'
```

La primera fila es un modelo. Completen las otras cuatro; todos los hitos son del 15-09-2026. Comparen el `COMMIT` con la auditoría y la cuenta conservada en `proveedores`.

| Hito | Fuente y línea o fila | Hora UTC | Identificador o dato observado |
| --- | --- | --- | --- |
| Intento rechazado (modelo) | PG L5 a L8; app L6 | PG `13:03:15.401` a `.404`; app `13:03:15.433` | `R-5102`, error `cuenta_formato`, `ROLLBACK` |
| Nuevo intento de cambio | PG L10 y L11 | `13:04:02.190` | `R-5103`; cuenta `9988-7766-55` (Banco Austral) |
| Orden `COMMIT` y cambio persistido | PG L12; auditoría filas `id` 4 y 5; proveedores `id` 184 | PG `13:04:02.198`; auditoría `13:04:02.190` | xid/txid `88232`; cuenta conservada `9988-7766-55` |
| Respuesta de la aplicación al cambio | app L7 | `13:04:02.231` | `R-5103`, código `200` |
| Pago 9921 | pagos `id` 9921 | `14:20:31.390` | Cuenta `9988-7766-55` (Banco Austral); aprobador `jsoto` |

Cambien roles: quien operaba la terminal ahora registra. Si continúan el miércoles, reserven los últimos cinco minutos de este paso para el respaldo indicado al final.

## 3. Unir las fuentes

**Directa:** mismo identificador de solicitud/transacción. **Por valor:** misma cuenta bancaria u otro valor reutilizable. **Solo temporal:** cercanía horaria sin identificador que enlace los eventos.

En C1, `R-5102` aparece en app L6 y PG L4; el PID `4410` permite seguir la secuencia hasta el error y el `ROLLBACK`. Por eso el enlace es directo. El PID por sí solo no identifica a una persona.

| Relación | Dato que las une | Tipo y confianza | Qué falta o qué limita la conclusión |
| --- | --- | --- | --- |
| C1: app R-5102 ↔ error de PG (modelo) | `R-5102`, app L6 y PG L4 a L8 | Directa; alta para enlazar los registros | No demuestra quién usó la cuenta |
| C2: `COMMIT` ↔ auditoría del cambio | xid `88232` en PG L12 = txid `88232` en auditoría filas `id` 4 y 5; `R-5103` en PG L10 y en la auditoría | Directa; alta | Falta confirmar el código del trigger; no demuestra quién usó la cuenta |
| C3: cuenta nueva en auditoría ↔ pago 9921 | Cuenta `9988-7766-55` en auditoría fila `id` 5 y en pagos `id` 9921; PG L21 copia la cuenta vigente al aprobar | Por valor; alta-media | Son transacciones distintas (`88232` y `88301`); falta la tabla `lote_pendiente` |

## 4. Explicar y guardar

Escriban **tres frases**, con sus fuentes: qué cambio quedó persistido y qué pasó con el pago; qué puede atribuirse a una cuenta; qué falta para identificar a una persona.

```text
Cambio y consecuencia: la solicitud R-5103 cambió la cuenta del proveedor 184 a 9988-7766-55 y el cambio quedó persistido (PG L11-L12, auditoría filas 4-5, proveedores id 184); el pago 9921 se envió después a esa cuenta (pagos id 9921).
Atribución a una cuenta: el cambio corresponde a la sesión S-7Q2 de la cuenta mrojas (app L7, auditoría app_usuario); el pago lo aprobó la cuenta jsoto (pagos id 9921).
Limitación para atribuir a una persona: sin registros de VPN, AD ni del equipo PC-CONTAB-07, las fuentes no identifican a la persona que usó la cuenta mrojas ni su intención.
```

Guarden la bitácora en el editor y ejecuten. El respaldo copia solo lo que esté guardado:

```bash
(cd "$EV" && sha256sum -c ../notas/BD-CASO03.sha256) | tee notas/verificacion-final.txt
cp "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/"
cmp -s "$LAB/entrega/bitacora-equipo.md" "$RECUPERACION/bitacora-equipo.md" && echo 'OK: respaldo' || echo 'ERROR: revisar copia con el docente'
```

**Antes de apagar:** desde Windows respalden la bitácora de `LAB09_SALIDA` en pendrive, correo o aula virtual. El PC se restablece y borra su copia local. No entreguen la evidencia. Para retomar otra clase, monten las carpetas, ejecuten el paso 1 y recuperen la bitácora respaldada.
