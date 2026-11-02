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

Esta guía acompaña la actividad de la Lección 12. Cada bloque explica la decisión forense, el resultado esperado y el límite del comando. No es una secuencia para ejecutar completa sin haber definido antes el alcance.

## Recorrido de trabajo

```text
autorización y alcance -> exportaciones recibidas verificadas -> copias de trabajo verificadas
-> inventario y custodia auditados -> filtros autorizados -> salidas con hash
-> hallazgo reproducible -> respaldo verificado
```

Los archivos de `/media/sf_LAB12_ENTREGA/evidencia/` son exportaciones preparadas por TI para el caso. Los comandos se ejecutan sobre copias locales en `~/forense/LAB-12/NS-CASO05/`. Ningún comando requiere red ni privilegios de administrador.

## Tres registros que no deben confundirse

| Registro | Cuándo corresponde | Ejemplo en esta práctica |
| --- | --- | --- |
| Cadena de custodia | Una persona entrega el control y otra asume responsabilidad sobre una evidencia | Auditar las transferencias ya consignadas en `cadena-custodia-recibida.csv` |
| Derivación | Se crea una copia o salida a partir de una fuente identificada | `NS05-EV04` produce `NS05-EV04-T1` y luego una salida de análisis |
| Bitácora de análisis | Se documentan accesos, decisiones, comandos y resultados | La persona analista filtra una copia y la persona revisora comprueba el hallazgo |

Abrir un archivo, crear una copia o permitir que otra persona revise una salida no constituye automáticamente una transferencia de custodia. Por eso esta práctica no crea una cadena paralela entre el docente y los equipos.

## Preparar las rutas

```bash
LAB=~/forense/LAB-12
COMPARTIDA=/media/sf_LAB12_ENTREGA
RECUPERACION=/media/sf_LAB12_SALIDA
EV="$COMPARTIDA/evidencia"
CASO="$LAB/NS-CASO05"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_anexos}
cd "$CASO"
```

| Carpeta | Propósito |
| --- | --- |
| `00_control` | Bitácora y resultados de controles de integridad |
| `01_copias_trabajo` | Copias verificadas sobre las que se examinan los datos |
| `02_salidas` | Resultados guardados de los comandos de análisis |
| `03_anexos` | Manifiestos de hashes que respaldan el hallazgo y la entrega |

Las variables existen solo en la terminal actual. Si abren otra terminal, vuelvan a ejecutar el bloque.

## Comprobar el entorno sin modificar el paquete

**Decisión:** confirmar que el paquete está montado en solo lectura y que la carpeta de recuperación admite escritura.

```bash
findmnt -no TARGET,OPTIONS -T "$COMPARTIDA"
test -w "$RECUPERACION" && echo 'OK: salida con escritura' || echo 'ERROR: salida sin escritura'
```

**Resultado esperado:** la primera salida contiene `ro` entre las opciones y la segunda muestra `OK`.

**Si falla:** deténganse y avisen al docente. No prueben la protección creando archivos dentro del paquete.

## Leer los documentos de control

**Decisión:** conocer la autorización, el origen de las exportaciones y los defectos que deben auditarse antes de analizar.

```bash
cat "$COMPARTIDA/caso/autorizacion.txt"
sed -n '1,40p' "$COMPARTIDA/notas/ficha-tecnica.txt"
column -s, -t < "$COMPARTIDA/caso/inventario-preliminar.csv"
column -s, -t < "$COMPARTIDA/caso/cadena-custodia-recibida.csv"
```

`column` solo mejora la presentación en pantalla. No valida un CSV ni modifica el archivo. Los CSV del laboratorio no contienen comas dentro de sus campos; en archivos reales se debe usar un lector de CSV adecuado.

Para citar una fila con su número:

```bash
cat -n "$COMPARTIDA/caso/cadena-custodia-recibida.csv"
```

## Verificar las exportaciones recibidas

**Pregunta:** ¿los tres archivos recibidos coinciden, byte por byte, con el manifiesto que acompaña al paquete?

```bash
(cd "$EV" && sha256sum -c ../notas/NS-CASO05.sha256) | tee 00_control/verificacion-recibidas.txt
```

**Resultado esperado:** tres líneas terminadas en `OK`.

**Qué demuestra:** cada archivo tiene el mismo contenido que la referencia del manifiesto.

**Qué no demuestra:** que TI exportó todos los registros, que la fuente era auténtica o que la información sea verdadera.

**Si aparece `FAILED`:** deténganse. No sustituyan el manifiesto ni calculen uno nuevo para hacer desaparecer la diferencia.

## Crear y verificar copias de trabajo

**Decisión:** examinar copias identificadas, manteniendo sin cambios las exportaciones recibidas.

```bash
cp -n "$EV"/* 01_copias_trabajo/
(cd 01_copias_trabajo && sha256sum -c "$COMPARTIDA/notas/NS-CASO05.sha256") | tee 00_control/verificacion-copias.txt
chmod a-w 01_copias_trabajo/*
ls -l 01_copias_trabajo/
```

- `cp -n` evita reemplazar una copia que ya exista.
- La segunda verificación demuestra que las copias coinciden con las exportaciones recibidas.
- `chmod a-w` reduce el riesgo de alterarlas durante el examen.
- En `ls -l`, las copias no deben mostrar `w` en sus permisos.

Documenten en la bitácora la relación entre cada exportación y su copia:

| Exportación | Copia derivada |
| --- | --- |
| `NS05-EV02` | `NS05-EV02-T1` |
| `NS05-EV03` | `NS05-EV03-T1` |
| `NS05-EV04` | `NS05-EV04-T1` |

Esto registra procedencia. No afirma que hubo una entrega entre personas.

## Auditar la cadena de custodia recibida

**Pregunta:** ¿cada transferencia permite saber qué se entregó, quién era responsable, cuándo ocurrió, para qué y en qué estado?

```bash
column -s, -t < "$COMPARTIDA/caso/cadena-custodia-recibida.csv"
sha256sum "$EV/vpn-2026-10-22.csv"
```

El segundo comando permite comparar el hash completo de la exportación VPN con el prefijo incompleto de la cadena recibida. Que el prefijo coincida orienta la revisión, pero no reemplaza un hash completo.

No corrijan el archivo recibido. Documenten en la bitácora:

- el dato ausente o ambiguo;
- el riesgo que introduce;
- quién podría confirmarlo;
- la expresión `por confirmar` cuando no exista evidencia suficiente.

## Filtrar dentro del alcance autorizado

Cada filtro debe responder una pregunta de la matriz de análisis. Los resultados incluyen el número de línea original para que el hallazgo pueda citarse y reproducirse.

### VPN: cuenta e intervalo autorizados

**Pregunta:** ¿qué eventos VPN de la cuenta autorizada ocurrieron entre las 20:00 y las 23:00, hora de Chile?

```bash
grep -nE '^(ts_local|2026-10-22 (20|21|22):[0-5][0-9]:[0-5][0-9],r\.ibarra,|2026-10-22 23:00:00,r\.ibarra,)' 01_copias_trabajo/vpn-2026-10-22.csv | tee 02_salidas/vpn-intervalo.txt
```

**Resultado esperado:** encabezado, conexión y desconexión de la cuenta dentro del intervalo.

**Lectura del comando:** `grep -nE` muestra el número de línea y acepta alternativas separadas por `|`. El patrón conserva el encabezado y las filas de `r.ibarra` entre las 20:00 y las 23:00. La alternativa final incluye exactamente las `23:00:00`, si existieran.

**Límite:** los bytes transferidos no identifican qué archivo salió.

### Cloud: primero la cuenta, después el enlace descubierto

**Pregunta inicial:** ¿qué acciones de la cuenta autorizada ocurrieron dentro del intervalo convertido a UTC?

```bash
grep -nF '"actor":"r.ibarra@nortesur.invalid"' 01_copias_trabajo/cloud-audit.jsonl | grep -E '"ts":"(2026-10-22T23:|2026-10-23T0[01]:|2026-10-23T02:00:00Z)' | tee 02_salidas/cloud-cuenta.txt
```

Revisen la salida e identifiquen el `share_id` creado por la cuenta. Solo después busquen eventos vinculados con ese identificador. En `NS-CASO05`, el valor descubierto es `SH-881`:

```bash
grep -nF '"share_id":"SH-881"' 01_copias_trabajo/cloud-audit.jsonl | grep -E '"ts":"(2026-10-22T23:|2026-10-23T0[01]:|2026-10-23T02:00:00Z)' | tee 02_salidas/cloud-enlace.txt
```

**Resultado esperado:** la primera salida contiene las acciones de la cuenta; la segunda relaciona la creación del enlace con su acceso dentro del intervalo.

**Lectura del intervalo:** el segundo `grep` conserva las 23:xx del 22 de octubre, las 00:xx y 01:xx del día 23, y exactamente las 02:00:00. Es la representación UTC del intervalo autorizado.

**Límite técnico:** buscar texto funciona en este archivo JSONL sintético porque cada evento ocupa una sola línea. En una investigación real se usa un analizador JSON, por ejemplo `jq`, y se documenta su versión.

**Límite interpretativo:** un acceso anónimo demuestra uso del enlace desde una IP, no la identidad de la persona ni que una empresa competidora recibió el objeto.

### Correo: mensajes autorizados y exclusiones

**Pregunta:** ¿la cuenta envió dentro del intervalo un mensaje desde `Enviados` hacia un dominio externo no excluido?

Después de descubrir `SH-881` en la auditoría cloud, búsquenlo en los metadatos de correo y conserven únicamente los mensajes de la carpeta `Enviados`:

```bash
grep -nF 'SH-881' 01_copias_trabajo/correo-metadatos.csv | grep -F ',Enviados' | tee 02_salidas/correo-intervalo.txt
```

Lean la tubería de izquierda a derecha:

1. El primer `grep` busca literalmente el identificador descubierto y agrega el número de línea original.
2. El segundo conserva los resultados de la carpeta `Enviados`.
3. `tee` muestra el resultado y lo guarda en `correo-intervalo.txt`.

El comando encuentra un **candidato**. Antes de usarlo en el hallazgo, verifiquen en la misma fila que el remitente sea la cuenta autorizada, que la hora esté dentro del intervalo y que el destinatario no pertenezca a un dominio excluido.

Antes de ejecutar el comando, comprueben su decisión fila por fila:

| Línea | Remitente autorizado | Dentro del intervalo | Carpeta `Enviados` | Destinatario excluido | ¿Se imprime? |
| --- | --- | --- | --- | --- | --- |
| 1, encabezado | no se evalúa | no se evalúa | no se evalúa | no se evalúa | no |
| 2, `19:30` | sí | no | sí | sí | no |
| 3, `20:15` | no | sí | no | no | no |
| 4, `21:24` | sí | sí | sí | no | **sí** |
| 5, `22:05` | sí | sí | sí | sí | no |

La fila 4 es el único mensaje que contiene el identificador descubierto y satisface todas las condiciones al revisar sus campos. La exclusión de `correo-personal.invalid` es una regla explícita del caso sintético; no es un método general para clasificar mensajes personales.

```text
4:<b88e@nortesur.invalid>,2026-10-22T21:24:40-03:00,r.ibarra@nortesur.invalid,contacto@licitacorp.invalid,Lo conversado,,https://almacen.nortesur-cloud.invalid/s/SH-881,Enviados
```

**Límite:** los metadatos muestran destinatario, asunto, enlaces y adjuntos declarados, pero no el cuerpo del mensaje ni el contenido del objeto cloud.

## Convertir horas a UTC

La ficha técnica indica que la VPN usa hora de Chile sin escribir el desfase. Por eso el desfase se agrega al convertir:

```bash
date -u -d '2026-10-22 20:52:10 -03:00' '+%F %T UTC'
date -u -d '2026-10-22T21:24:40-03:00' '+%F %T UTC'
```

| Hora local | UTC |
| --- | --- |
| `2026-10-22 20:00:00 -03:00` | `2026-10-22 23:00:00Z` |
| `2026-10-22 23:00:00 -03:00` | `2026-10-23 02:00:00Z` |

`date -u -d` ya aplica la conversión. No sumen tres horas otra vez.

## Guardar e identificar las salidas

`tee` muestra el resultado y lo guarda al mismo tiempo. El archivo guardado es el que se cita en el hallazgo.

```bash
sha256sum 02_salidas/*.txt | tee 03_anexos/hashes-salidas.txt
```

El hash identifica la salida exacta usada para redactar el hallazgo. No demuestra que el comando fuera apropiado ni que la interpretación sea correcta; eso debe justificarse en la matriz y en el hallazgo.

## Interpretar sin exceder las fuentes

| Fuente | Afirmación respaldada | Afirmación que excede la fuente |
| --- | --- | --- |
| VPN | Una cuenta conectó un equipo y se registraron cantidades de bytes | La VPN muestra qué archivo salió |
| Auditoría cloud | Una sesión cargó un objeto y creó un enlace | La persona titular estaba frente al equipo |
| Acceso anónimo | El enlace fue utilizado desde una IP | La competencia recibió la oferta |
| Metadatos de correo | La cuenta envió un mensaje que contenía el enlace | Se conoce el contenido del mensaje o del objeto |
| Hash informado por el proveedor | El proveedor registró ese valor para el objeto | El equipo verificó el contenido del objeto no exportado |

## Respaldar los productos reproducibles

El respaldo debe incluir la bitácora, los controles, las salidas y sus hashes. Las copias de trabajo se pueden recrear desde el paquete verificado y no se respaldan.

```bash
sha256sum 00_control/* 02_salidas/* 03_anexos/hashes-salidas.txt | tee 03_anexos/hashes-entrega.txt
cp -r 00_control 02_salidas 03_anexos "$RECUPERACION/"
(cd "$RECUPERACION" && sha256sum -c 03_anexos/hashes-entrega.txt)
```

**Resultado esperado:** todos los archivos del manifiesto terminan en `OK` dentro de la carpeta de recuperación.

## Errores que obligan a detenerse o corregir el método

- Continuar después de un `FAILED` del manifiesto.
- Analizar directamente las exportaciones recibidas en vez de las copias verificadas.
- Llamar “transferencia de custodia” a cualquier acceso, copia o revisión.
- Completar responsables, horas o hashes ausentes mediante suposiciones.
- Comparar directamente horas locales con registros UTC.
- Examinar mensajes personales, otros trabajadores o eventos fuera del intervalo sin autorización.
- Citar una fuente sin archivo y línea.
- Guardar el hallazgo sin las salidas y los hashes que permiten reproducirlo.
