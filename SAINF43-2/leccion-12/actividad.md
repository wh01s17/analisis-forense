---
title: "Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "12"
author: Jordy
start: 2026-10-27
end: 2026-11-02
created_at: 2026-10-24
aliases:
  - "Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
---

# Actividad Lección 12 - Plan forense, cadena de custodia y hallazgo reproducible

- **Modalidad:** equipos de tres personas.
- **Duración:** 40 minutos de activación, 80 minutos de taller y 40 minutos de cierre.
- **Caso:** `NS-CASO05` - NORTE-SUR SPA (sociedad por acciones; ficticia).
- **Entorno:** estación Kali con el paquete del caso montado en solo lectura y carpeta de salida con escritura; no se requiere conexión de red.
- **Carácter:** formativo, sin calificación ni entrega evaluada.
- **Producto:** este archivo completado como `bitacora-equipo.md`, más la cadena de custodia `custodia-NS-CASO05.csv` y las salidas que respaldan el hallazgo.

## El caso

NORTE-SUR SPA participa en la licitación `PUERTO-2027`. La mañana del **23-10-2026** la Gerencia de Operaciones escribe al área de Seguridad de la Información:

> Nos llegó el rumor de que la competencia ya tendría nuestra oferta para la licitación PUERTO-2027. Sospecho de Rodrigo Ibarra, de Compras, porque anoche estuvo conectado tarde.
>
> Necesito que revisen todo: su notebook, su correo, su teléfono, su WhatsApp, la nube y lo que haga falta. Revisen también si alguien más de Compras está metido en esto.
>
> Pídanle a TI que apague el notebook de inmediato para que no alcance a borrar nada.
>
> Tienen 24 horas. Necesito saber quién lo hizo para tomar medidas.

Dos horas después, Gerencia General y Asesoría Legal emiten una autorización formal. TI (tecnologías de la información) entrega un inventario preliminar, un registro de cadena de custodia y tres exportaciones sintéticas:

| Archivo | Contenido | Tiempo |
| --- | --- | --- |
| `caso/solicitud-gerencia.txt` | Correo original de la Gerencia de Operaciones | - |
| `caso/autorizacion.txt` | Objetivo, recursos, intervalo, exclusiones y escalamiento autorizados | - |
| `caso/inventario-preliminar.csv` | Inventario levantado por TI; contiene errores | - |
| `caso/cadena-custodia-recibida.csv` | Registro de custodia llevado hasta ahora; contiene omisiones | Hora local `-03:00` |
| `evidencia/vpn-2026-10-22.csv` | Conexiones VPN (red privada virtual) del 22-10-2026 | Hora local, **sin desfase escrito** |
| `evidencia/cloud-audit.jsonl` | Auditoría del almacenamiento cloud corporativo, en JSONL (un objeto JSON por línea) | UTC (`Z`) |
| `evidencia/correo-metadatos.csv` | Metadatos de mensajes del buzón corporativo; sin cuerpos ni adjuntos | Hora local con desfase `-03:00` |
| `notas/ficha-tecnica.txt` | Quién exportó cada fuente, con qué filtro y en qué zona horaria | - |
| `notas/NS-CASO05.sha256` | Manifiesto SHA-256 de las tres exportaciones | - |

UTC (tiempo universal coordinado) se marca con `Z`. Chile continental usa `-03:00` en estas fechas. Todas las cuentas, dominios, direcciones y entidades son ficticios o reservados para documentación.

> ¿Cómo se transforma una solicitud ambigua en un plan forense ejecutable, con custodia sin saltos y al menos un hallazgo que otra persona pueda reproducir?

## Ruta de trabajo y tiempos

| Tramo | Tiempo | Pasos | Producto esperado |
| --- | ---: | --- | --- |
| Activación | 40 min | 1 | Ambigüedades, preguntas de aclaración y preguntas forenses. |
| Taller | 80 min | 2 a 6 | Estructura del caso, alcance, inventario, custodia, matriz y cronología. |
| Cierre | 40 min | 7 y 8 | Hallazgo reproducible, índice del informe, revisión entre pares y respaldo. |

La **matriz del paso 6**, el **hallazgo del paso 7** y la **cadena de custodia** son la evidencia principal de esta lección. Si el taller se retrasa, completen al menos tres filas de la matriz y cuatro hitos de la cronología, y continúen con el hallazgo; no omitan la verificación de integridad.

Roles sugeridos dentro del equipo, rotando en cada paso: **analista** (ejecuta comandos), **registrador** (completa la bitácora y la custodia) y **revisor** (verifica que cada afirmación tenga fuente).

## 1. Leer la solicitud antes de actuar

Una solicitud de gerencia expresa una preocupación, no un encargo forense. Antes de tocar cualquier fuente hay que separar lo que se pide, lo que se puede hacer y lo que nadie ha autorizado todavía.

Lean el correo del caso **sin abrir todavía el paquete**. Identifiquen al menos seis problemas de la solicitud y la pregunta que harían para resolver cada uno.

| # | Frase o supuesto de la solicitud | Problema (alcance, autoridad, método, sesgo, plazo, privacidad) | Pregunta de aclaración |
| ---: | --- | --- | --- |
| 1 | [Completar] | [Completar] | [Completar] |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |
| 4 | [Completar] | [Completar] | [Completar] |
| 5 | [Completar] | [Completar] | [Completar] |
| 6 | [Completar] | [Completar] | [Completar] |

Reformulen la solicitud en tres preguntas forenses. Una pregunta forense se responde con evidencia, tiene límites de tiempo y objeto, y no presupone culpables.

```text
P1:
P2:
P3:
¿Por qué "saber quién lo hizo" no es una pregunta forense adecuada tal como está escrita?:
```

## 2. Preparar el caso y contrastar la solicitud con la autorización

Copien `LAB12_ENTREGA` al anfitrión y configuren en VirtualBox `LAB12_ENTREGA` en **solo lectura** y `LAB12_SALIDA` con escritura, ambas con montaje automático.

En Kali ejecuten. `LAB` (laboratorio), `EV` (evidencia) y `CASO` (carpeta del caso) son variables con las rutas de trabajo. `CUSTODIA` es el archivo donde el equipo registrará sus propias transferencias:

```bash
LAB=~/forense/LAB-12
COMPARTIDA=/media/sf_LAB12_ENTREGA
RECUPERACION=/media/sf_LAB12_SALIDA
EV="$COMPARTIDA/evidencia"
CASO="$LAB/NS-CASO05"
CUSTODIA="$CASO/00_control/custodia-NS-CASO05.csv"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_informe,04_anexos}
cd "$CASO"
cp -n "$COMPARTIDA/documentos/actividad-leccion-12.md" 00_control/bitacora-equipo.md
chmod u+w 00_control/bitacora-equipo.md
```

Este bloque no borra nada que ya exista: al abrir otra terminal o al reanudar la clase, ejecútenlo de nuevo para recuperar las variables y volver a la carpeta del caso.

Comprueben los permisos reales de las carpetas:

```bash
if touch "$COMPARTIDA/.prueba" 2>/dev/null; then rm -f "$COMPARTIDA/.prueba"; echo 'ERROR: la evidencia admite escritura'; else echo 'OK: evidencia en solo lectura'; fi
if touch "$RECUPERACION/.prueba" 2>/dev/null; then rm -f "$RECUPERACION/.prueba"; echo 'OK: salida con escritura'; else echo 'ERROR: la salida no admite escritura'; fi
```

Antes de tocar la evidencia, lean la autorización: es el límite del trabajo. Lo que la solicitud pide y la autorización no cubre se **escala**, no se ejecuta.

```bash
cat "$COMPARTIDA/caso/autorizacion.txt"
sed -n '1,40p' "$COMPARTIDA/notas/ficha-tecnica.txt"
```

| Elemento | Definición para NS-CASO05 | Fuente (documento y sección) |
| --- | --- | --- |
| Objetivo | [Completar] | [Completar] |
| Recursos incluidos | [Completar] | [Completar] |
| Exclusiones | [Completar] | [Completar] |
| Intervalo en hora local y en UTC | [Completar] | [Completar] |
| Responsable y canal de escalamiento | [Completar] | [Completar] |
| Criterio de término 1 | [Completar] | - |
| Criterio de término 2 | [Completar] | - |

Comparen la autorización con su tabla del paso 1:

```text
Pedido de la gerencia que queda fuera de alcance y por qué:
Fuente mencionada por TI que la autorización no cubre:
Conflicto dentro de la propia autorización (pista: "dominios externos" y "mensajes personales"):
Decisión pendiente sobre el notebook encendido y a quién se escala:
```

## 3. Verificar, registrar la recepción y crear las copias de trabajo

Con el alcance claro, verifiquen el paquete recibido contra el manifiesto:

```bash
(cd "$EV" && sha256sum -c ../notas/NS-CASO05.sha256) | tee 00_control/verificacion-inicial.txt
```

Ante un `FAILED` (fallo de verificación), deténganse y avisen. No recalculen el manifiesto.

Registren su propia recepción del paquete. Primero creen el registro de custodia y la función `registrar`, que agrega una fila con hora UTC y SHA-256 calculado en el momento. El encabezado solo se escribe si el archivo no existe o está vacío, y la función no agrega la fila si el hash no se pudo calcular:

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

Luego registren las tres recepciones, **una sola vez**. Escriban los argumentos **sin comas**:

```bash
registrar NS05-EV02 "$EV/vpn-2026-10-22.csv" "Docente (custodio del paquete)" "Equipo [nombre]" "Recepción" "Original en solo lectura; manifiesto OK"
registrar NS05-EV03 "$EV/correo-metadatos.csv" "Docente (custodio del paquete)" "Equipo [nombre]" "Recepción" "Original en solo lectura; manifiesto OK"
registrar NS05-EV04 "$EV/cloud-audit.jsonl" "Docente (custodio del paquete)" "Equipo [nombre]" "Recepción" "Original en solo lectura; manifiesto OK"
column -s, -t < "$CUSTODIA"
```

Al reanudar en otra terminal, ejecuten el bloque de variables del paso 2 y el bloque de la función `registrar`, pero **no** repitan las recepciones: ya quedaron registradas.

Creen las copias de trabajo, verifíquenlas contra el manifiesto recibido y protéjanlas contra escritura:

```bash
cp -n "$EV"/* 01_copias_trabajo/
(cd 01_copias_trabajo && sha256sum -c "$COMPARTIDA/notas/NS-CASO05.sha256") | tee 00_control/verificacion-copias.txt
chmod a-w 01_copias_trabajo/*
ls -l 01_copias_trabajo/
```

| Control | Resultado |
| --- | --- |
| Tres archivos originales terminan en `OK` | [Sí / No] |
| La carpeta de evidencia rechaza escritura | [Sí / No] |
| Las tres recepciones tienen hora UTC, hash completo, entrega, recibe, propósito y estado | [Sí / No] |
| Los hashes registrados coinciden con el manifiesto | [Sí / No] |
| Tres copias de trabajo terminan en `OK` | [Sí / No] |
| Las copias de trabajo quedaron sin permiso de escritura | [Sí / No] |

```text
¿Para qué sirve cada carpeta de NS-CASO05? (una línea por carpeta):
¿Por qué se analizan las copias y no los archivos de la carpeta compartida?:
```

## 4. Corregir el inventario y ordenar la adquisición

Un inventario forense asigna un identificador único a cada fuente y registra su estado, custodio, integridad y urgencia. El orden de adquisición considera **volatilidad** (qué se pierde primero), **retención** (qué expira) y **dependencia** (qué fuente necesita a otra).

```bash
column -s, -t < "$COMPARTIDA/caso/inventario-preliminar.csv"
grep -n 'retención\|conserva\|Pendiente' -i "$COMPARTIDA/notas/ficha-tecnica.txt"
```

Detecten al menos cuatro problemas del inventario:

| Fila | Problema | Corrección propuesta |
| ---: | --- | --- |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |

Propongan el orden de adquisición o preservación de las fuentes **autorizadas**. Usen identificadores con el prefijo del caso, por ejemplo `NS05-EV01`.

| Orden | Identificador y fuente | Motivo del orden (volatilidad, retención, dependencia) | Acción y responsable |
| ---: | --- | --- | --- |
| 1 | [Completar] | [Completar] | [Completar] |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |
| 4 | [Completar] | [Completar] | [Completar] |

```text
¿Qué se perdería si TI apaga el notebook sin un plan?:
¿Qué fuente expira primero por retención y cuándo?:
```

## 5. Reparar la cadena de custodia recibida

La cadena de custodia demuestra quién tuvo cada evidencia, cuándo, para qué y en qué estado. Un campo vacío no se rellena con suposiciones: se marca **por confirmar** y se informa.

```bash
column -s, -t < "$COMPARTIDA/caso/cadena-custodia-recibida.csv"
sha256sum "$EV"/vpn-2026-10-22.csv
```

Revisen las cinco filas recibidas. Para cada una, indiquen qué falta o qué es ambiguo y qué riesgo introduce.

| Fila | Omisión o ambigüedad | Riesgo para la trazabilidad | Registro corregido (sin inventar datos) |
| ---: | --- | --- | --- |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |
| 4 | [Completar] | [Completar] | [Completar] |
| 5 | [Completar] | [Completar] | [Completar] |
| 6 | [Completar] | [Completar] | [Completar] |

```text
El hash parcial de la fila 3, ¿coincide con el manifiesto? ¿Basta un prefijo?:
La fila 4 dice EV-04, pero el inventario tiene dos EV-04. ¿Qué dato resuelve cuál es?:
```

## 6. Plan de análisis y cronología del intervalo

Cada pregunta forense debe conectarse con una fuente, una técnica, un resultado esperado y una limitación. Esa matriz evita "revisar todo" y deja claro qué evidencia respondería cada pregunta.

| Pregunta | Fuente (ID) | Técnica o comando | Resultado esperado | Limitación |
| --- | --- | --- | --- | --- |
| P1 | [Completar] | [Completar] | [Completar] | [Completar] |
| P2 | [Completar] | [Completar] | [Completar] | [Completar] |
| P3 | [Completar] | [Completar] | [Completar] | [Completar] |

Ejecuten ahora solo las técnicas de la matriz, sobre las copias de trabajo y **dentro del intervalo autorizado**. Cada salida se guarda en `02_salidas/` porque será anexo del informe.

VPN, en hora local sin desfase escrito (la ficha técnica indica `-03:00`):

```bash
awk -F, 'NR==1 || ($2=="r.ibarra" && $1>="2026-10-22 20:00:00" && $1<="2026-10-22 23:00:00") {print NR": "$0}' 01_copias_trabajo/vpn-2026-10-22.csv | tee 02_salidas/vpn-intervalo.txt
```

Auditoría cloud, en UTC. El intervalo 20:00-23:00 `-03:00` equivale a 23:00Z del 22-10 a 02:00Z del 23-10:

```bash
grep -nF -e 'r.ibarra' -e 'SH-881' 01_copias_trabajo/cloud-audit.jsonl | awk -F'"' '$4>="2026-10-22T23:00:00Z" && $4<="2026-10-23T02:00:00Z"' | tee 02_salidas/cloud-intervalo.txt
```

Metadatos de correo. Apliquen las exclusiones del paso 2: solo mensajes enviados por la cuenta, dentro del intervalo, desde la carpeta Enviados y a destinatarios externos. En este caso sintético, `correo-personal.invalid` identifica mensajes personales, por lo que el filtro los deja fuera; esa regla no sirve para clasificar dominios en otras investigaciones:

```bash
awk -F, 'NR==1 || ($3=="r.ibarra@nortesur.invalid" && $2>="2026-10-22T20:00:00-03:00" && $2<="2026-10-22T23:00:00-03:00" && $8=="Enviados" && $4 !~ /@(nortesur\.invalid|correo-personal\.invalid)$/) {print NR": "$0}' 01_copias_trabajo/correo-metadatos.csv | tee 02_salidas/correo-intervalo.txt
```

Conviertan a UTC las horas locales. Para la VPN agreguen el desfase que indica la ficha:

```bash
date -u -d '2026-10-22 20:52:10 -03:00' '+%F %T UTC'
date -u -d '2026-10-22T21:24:40-03:00' '+%F %T UTC'
```

Completen la cronología en UTC. Citen archivo y L (línea).

| # | Hito | Fuente y línea | Hora original | Hora UTC | Identificadores |
| ---: | --- | --- | --- | --- | --- |
| 1 | Conexión VPN de la cuenta | [Completar] | [Completar] | [Completar] | [Completar] |
| 2 | Inicio de sesión cloud | [Completar] | [Completar] | [Completar] | [Completar] |
| 3 | Carga del objeto | [Completar] | [Completar] | [Completar] | [Completar] |
| 4 | Creación del enlace de compartición | [Completar] | [Completar] | [Completar] | [Completar] |
| 5 | Mensaje enviado con el enlace | [Completar] | [Completar] | [Completar] | [Completar] |
| 6 | Acceso al enlace | [Completar] | [Completar] | [Completar] | [Completar] |
| 7 | Cierre de sesión cloud | [Completar] | [Completar] | [Completar] | [Completar] |
| 8 | Desconexión VPN | [Completar] | [Completar] | [Completar] | [Completar] |

Revisen qué quedó fuera del filtro y decidan qué hacer, sin analizarlo:

```bash
grep -n 'SH-881' 01_copias_trabajo/cloud-audit.jsonl | cut -c1-90
```

| Registro | ¿Por qué queda fuera? | Decisión (excluir, escalar o pedir ampliación) |
| --- | --- | --- |
| Mensaje a `correo-personal.invalid` dentro del intervalo, excluido por el filtro | [Completar] | [Completar] |
| `cloud-audit.jsonl` L8 | [Completar] | [Completar] |
| Sesiones VPN de `j.soto` | [Completar] | [Completar] |

## 7. Redactar un hallazgo reproducible y el índice del informe

Un hallazgo es reproducible cuando otra persona, con la misma evidencia y el mismo procedimiento, obtiene el mismo resultado. Por eso cita la evidencia por identificador y hash, el comando exacto, la salida guardada y separa lo observado de lo interpretado.

Calculen el hash de la salida que respalda el hallazgo:

```bash
sha256sum 02_salidas/*.txt | tee 04_anexos/hashes-salidas.txt
```

Redacten **un** hallazgo usando la cronología.

| Campo | Contenido |
| --- | --- |
| Identificador | H-01 |
| Pregunta que responde | [Completar] |
| Evidencia (ID y SHA-256) | [Completar] |
| Procedimiento (comando exacto) | [Completar] |
| Salida guardada (archivo y SHA-256) | [Completar] |
| Resultado observado | [Completar] |
| Interpretación | [Completar] |
| Confianza (alta, media o baja) y por qué | [Completar] |
| Explicación alternativa | [Completar] |
| Limitación | [Completar] |

Diseñen el índice del informe forense de NS-CASO05. Indiquen en una línea qué contiene cada sección y para qué lector es más importante.

| # | Sección | Contenido | Lector principal |
| ---: | --- | --- | --- |
| 1 | [Completar] | [Completar] | [Completar] |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |
| 4 | [Completar] | [Completar] | [Completar] |
| 5 | [Completar] | [Completar] | [Completar] |
| 6 | [Completar] | [Completar] | [Completar] |
| 7 | [Completar] | [Completar] | [Completar] |
| 8 | [Completar] | [Completar] | [Completar] |
| A | Anexos | [Completar] | [Completar] |

## 8. Revisión entre pares, resumen ejecutivo y respaldo

Intercambien la bitácora con otro equipo. El equipo revisor busca cuatro problemas y los anota en la bitácora ajena; si no encuentra uno, escribe "no detectado" y explica qué revisó.

| Busquen | Ubicación en la bitácora revisada | Observación |
| --- | --- | --- |
| Una acción fuera de alcance | [Completar] | [Completar] |
| Una fuente sin preservar o sin hash | [Completar] | [Completar] |
| Un hallazgo sin evidencia citada | [Completar] | [Completar] |
| Una conclusión que excede los datos | [Completar] | [Completar] |

Equipo revisor: [Completar]

Registren una transferencia interna: la persona analista entrega la copia de trabajo de la auditoría cloud a la persona revisora para la verificación del hallazgo.

```bash
registrar NS05-EV04-T1 01_copias_trabajo/cloud-audit.jsonl "[Analista]" "[Revisor]" "Verificación de H-01" "Copia de trabajo sin escritura; hash verificado"
column -s, -t < "$CUSTODIA"
```

Escriban un resumen ejecutivo de 60 a 80 palabras para la Gerencia General, sin tecnicismos innecesarios. Debe indicar qué se hizo, qué se observó, qué no puede afirmarse todavía y qué decisión necesita el equipo.

> [Completar]

Verifiquen de nuevo y respalden:

```bash
(cd "$EV" && sha256sum -c ../notas/NS-CASO05.sha256) | tee 00_control/verificacion-final.txt
cp 00_control/bitacora-equipo.md "$CUSTODIA" "$RECUPERACION/"
cmp -s 00_control/bitacora-equipo.md "$RECUPERACION/bitacora-equipo.md" && cmp -s "$CUSTODIA" "$RECUPERACION/custodia-NS-CASO05.csv" && echo 'OK: respaldo' || echo 'ERROR: revisar copia con el docente'
```

Antes de apagar, respalden `bitacora-equipo.md` y `custodia-NS-CASO05.csv` desde Windows por el medio indicado. No entreguen ni copien la evidencia.

## Comprobación final

- [ ] La solicitud se reformuló en preguntas que no presuponen culpables.
- [ ] El alcance cita la autorización y separa lo excluido de lo que se escala.
- [ ] El inventario tiene identificadores únicos y el orden considera volatilidad y retención.
- [ ] Ninguna fila de custodia corregida inventa responsables, horas ni hashes.
- [ ] La custodia propia tiene hora UTC y hash completo en cada fila.
- [ ] Se trabajó sobre copias verificadas y sin escritura.
- [ ] Todas las horas de la cronología están en UTC y citan archivo y línea.
- [ ] El hallazgo incluye comando, salida con hash, confianza, alternativa y limitación.
- [ ] La conclusión habla de cuentas, sesiones y equipos, no de culpabilidad de una persona.
