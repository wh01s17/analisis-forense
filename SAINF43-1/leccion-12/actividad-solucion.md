---
title: "Solución - Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
tags:
  - nota
  - course
  - curso
  - docente
  - reservado
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "12"
author: Jordy
start: 2026-10-27
end: 2026-11-02
created_at: 2026-10-24
aliases:
  - "Solución - Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
---

# Solución - Actividad Lección 12

**Uso reservado para el equipo docente.**  
**Actividad:** Práctica guiada 12.  
**Pauta:** Pauta docente.

Las respuestas pueden variar en su redacción. Deben conservar estas decisiones: respetar la autorización, no examinar lo excluido, no inventar datos de custodia, citar archivo y línea, normalizar las horas y separar observación de interpretación.

## Datos de control

### Hashes de las exportaciones recibidas

| Archivo | SHA-256 |
| --- | --- |
| `cloud-audit.jsonl` | `d1557921017d307faf5897695e1d8399856978a2f263b5c167c0175520616c53` |
| `correo-metadatos.csv` | `3849f75953bb082087464fdf5fbb55865efae8e4bc95cafebad1514a6fb36142` |
| `vpn-2026-10-22.csv` | `79e0fabfb420efff7029fde36e2475692688d937dc3af0e5943ca0309c2646a0` |

### Mapa de identificadores

| ID del caso | Archivo o fuente |
| --- | --- |
| `NS05-EV01` | Notebook corporativo `NS-NB-017`, todavía sin adquisición |
| `NS05-EV02` | `vpn-2026-10-22.csv` |
| `NS05-EV03` | `correo-metadatos.csv` |
| `NS05-EV04` | `cloud-audit.jsonl` |
| sufijo `-T1` | Primera copia de trabajo verificada de una exportación |

## 1. Convertir la solicitud en un encargo forense

### Problemas de la solicitud inicial

Se solicitan cuatro; esta tabla ofrece seis respuestas válidas.

| Frase o supuesto | Problema | Pregunta de aclaración |
| --- | --- | --- |
| “Revisen todo” | Alcance ilimitado y desproporcionado | ¿Qué recursos, datos e intervalo están formalmente autorizados? |
| “Sospecho de Rodrigo” | Introduce sesgo de confirmación | ¿Qué hecho se investiga y qué evidencia podría confirmarlo o refutarlo? |
| “Su teléfono y su WhatsApp” | Incluye recursos personales y datos privados | ¿Existe autorización específica para adquirirlos? |
| “Alguien más de Compras” | Extiende el examen a terceros | ¿Qué otras cuentas o trabajadores están autorizados? |
| “Apaguen el notebook” | Puede destruir datos volátiles | ¿Quién decidirá entre adquisición en vivo, aislamiento y apagado controlado? |
| “Necesito saber quién lo hizo” | Pide atribución personal a fuentes que identifican cuentas y sesiones | ¿Qué grado de atribución se requiere y qué fuentes podrían respaldarlo? |

### Alcance autorizado

| Elemento | Definición para `NS-CASO05` | Fuente y apartado |
| --- | --- | --- |
| Objetivo | Determinar si información de `PUERTO-2027` salió mediante recursos corporativos | `caso/autorizacion.txt`, “Objetivo” |
| Recursos incluidos | Notebook `NS-NB-017`, VPN, auditoría cloud y metadatos de mensajes externos del buzón corporativo | `caso/autorizacion.txt`, “Recursos autorizados” |
| Exclusiones | Dispositivos, cuentas y mensajería personales; mensajes personales; otros trabajadores | `caso/autorizacion.txt`, “Exclusiones” |
| Intervalo local | 22-10-2026, desde 20:00:00 hasta 23:00:00, hora de Chile `-03:00` | `caso/autorizacion.txt`, “Intervalo autorizado” |
| Intervalo UTC | 22-10-2026 23:00:00Z hasta 23-10-2026 02:00:00Z | Conversión del intervalo autorizado |
| Responsable | Jefatura de Seguridad de la Información | Encabezado de `caso/autorizacion.txt` |
| Escalamiento | Asesoría Legal ante datos personales o ampliación de intervalo o recursos | `caso/autorizacion.txt`, “Escalamiento” |
| Criterio de término | Responder P1 y P2 o documentar por qué no pueden responderse; detenerse ante fallo de integridad o necesidad de ampliar alcance | Decisión del equipo |

### Preguntas forenses modelo

```text
P1: ¿Qué acciones registran las fuentes corporativas autorizadas respecto de información asociada con PUERTO-2027 durante el intervalo?

P2: ¿Qué relación temporal y por identificadores puede establecerse entre los eventos VPN, cloud y correo autorizados, y cuáles son sus límites de atribución?
```

- **Por qué “saber quién lo hizo” excede las fuentes:** los registros vinculan cuentas, sesiones, equipo e IP, pero no prueban qué persona operaba la cuenta.
- **Pedidos fuera del alcance:** teléfono, WhatsApp, respaldo de la base de datos de Compras y examen de otros trabajadores.
- **Conflicto con mensajes personales:** preservar la exportación, aplicar un filtro que evite examinar lo excluido y consultar a Asesoría Legal si la clasificación no puede resolverse con metadatos.

## 2. Preparar el caso y verificar las exportaciones

### Resultados esperados

- `findmnt` muestra `ro` entre las opciones de `LAB12_ENTREGA`.
- La prueba de escritura muestra `OK: salida con escritura`.
- `sha256sum -c` termina en `OK` para las tres exportaciones y para sus copias.
- `ls -l` no muestra permiso `w` en las copias de trabajo.

Si una condición falla, el equipo se detiene y avisa al docente.

### Registro de derivaciones

| Exportación recibida | Copia de trabajo | Archivo | ¿Hash coincide? |
| --- | --- | --- | --- |
| `NS05-EV02` | `NS05-EV02-T1` | `vpn-2026-10-22.csv` | Sí |
| `NS05-EV03` | `NS05-EV03-T1` | `correo-metadatos.csv` | Sí |
| `NS05-EV04` | `NS05-EV04-T1` | `cloud-audit.jsonl` | Sí |

- **Qué demuestra la coincidencia:** los archivos comparados contienen exactamente los mismos bytes.
- **Qué no demuestra:** completitud, autenticidad del origen, exactitud de los eventos ni identidad del usuario.
- **Por qué se analizan copias:** reduce el riesgo de alterar las exportaciones recibidas y permite recrear el examen.

Crear una copia es una **derivación**. No constituye una transferencia de custodia porque no cambia la persona responsable del artefacto recibido.

## 3. Auditar el inventario y la cadena de custodia

### Problemas del inventario preliminar

| Fuente o fila | Problema | Riesgo | Corrección propuesta |
| --- | --- | --- | --- |
| Inventario L2, `EV-01` | Notebook encendido y conectado, con instrucción de apagarlo | Pérdida de datos volátiles y posible actividad remota | Aislar, documentar el estado y escalar la estrategia de adquisición |
| Inventario L5-L6 | `EV-04` identifica dos fuentes | Confusión entre auditoría cloud y respaldo de BD | Mantener el recibido y crear un registro corregido con IDs únicos |
| Inventario L6 | El respaldo de BD no está autorizado | Examen fuera de alcance | No analizarlo sin ampliación formal |
| Inventario L7 | El teléfono es personal y está excluido | Vulneración de privacidad | Mantenerlo fuera del examen y escalar cualquier solicitud |
| Ficha técnica, cloud | Auditoría con siete días de retención y objeto sin exportar | Pérdida de registros y del objeto | Solicitar preservación inmediata de auditoría, objeto y enlace |

No se debe renombrar el respaldo de BD como `EV-05`, porque ese ID ya aparece asignado al teléfono. Los identificadores nuevos se asignarían en un registro corregido; el inventario recibido no se modifica.

### Orden de preservación o adquisición

| Orden | Fuente | Motivo | Acción y responsable |
| ---: | --- | --- | --- |
| 1 | Notebook `NS-NB-017` | Alta volatilidad; está encendido y conectado | Aislar y decidir adquisición en vivo o apagado controlado; Jefatura y analista competente |
| 2 | Auditoría cloud, `OBJ-3310` y `SH-881` | Retención breve y posibilidad de borrado | Solicitud de preservación a TI Cloud/proveedor |
| 3 | Buzón corporativo | Retención de eliminados de treinta días; exportación demasiado amplia | Preservar y pedir reexportación acotada a TI Correo |

Se acepta invertir los dos primeros si el notebook ya está aislado y se justifica la urgencia de la retención cloud.

### Auditoría de la cadena recibida

| Fila | Omisión o ambigüedad | Riesgo | Acción correctiva sin inventar datos |
| ---: | --- | --- | --- |
| 2 | `EV-01` carece de serie o control físico preciso; “sí” no identifica el control | Confusión del equipo o control no demostrable | Confirmar serie, verificador y control; el hash queda no aplicable hasta adquirir una imagen |
| 3 | Faltan hora, zona, entrega, propósito, estado y control; hash parcial | Transferencia no reconstruible | Marcar por confirmar y agregar una anotación con SHA-256 completo |
| 4 | `EV-04` es ambiguo | Asociación con una fuente equivocada | Resolver mediante el hash completo de `cloud-audit.jsonl` y documentar `NS05-EV04` |
| 5 | Faltan zona, hash, receptor, estado y control | Tramo incompleto | Marcar por confirmar y no atribuir custodia a una persona desconocida |
| 6 | “Copia” sin ID derivado, método ni hash | Copia no identificable ni verificable | Asignar ID derivado y registrar método y hash antes de usarla |

- El prefijo `79e0fabf...` orienta hacia la VPN, pero no demuestra igualdad completa.
- El hash `d155...6c53` de la fila 4 coincide con `cloud-audit.jsonl` y resuelve el `EV-04` ambiguo.
- **Transferencia:** cambia quién controla el artefacto. **Derivación:** crea una copia o salida vinculada. **Acceso:** permite examinar sin asumir custodia.

## 4. Ejecutar el plan y construir la cronología

### Matriz de análisis

| Pregunta | Fuente e ID | Técnica | Resultado esperado | Limitación |
| --- | --- | --- | --- | --- |
| P1 | `NS05-EV04-T1` y `NS05-EV03-T1` | Filtrar cuenta e intervalo; descubrir y buscar `SH-881`; filtrar enviados externos | Carga, enlace, envío y acceso relacionados | Sin objeto ni cuerpo; acceso anónimo |
| P2 | `NS05-EV02-T1`, `NS05-EV04-T1` y `NS05-EV03-T1` | Normalizar a UTC y correlacionar cuenta, sesión, equipo, objeto y enlace | Secuencia temporal común | Cuenta, equipo e IP no atribuyen la acción a una persona |

### Salidas reproducibles

| Salida | Líneas incluidas | SHA-256 |
| --- | --- | --- |
| `vpn-intervalo.txt` | Encabezado, VPN L4 y L7 | `e53cff49c4cf9894c6a0857c1cfcc9c070a1161294b4ad559783605ffaf0a992` |
| `cloud-cuenta.txt` | Cloud L2, L3, L4 y L7 | `32706ff3913bc3ac582f77dc35228b08e761b1e76e80db80015c2e3b708ade74` |
| `cloud-enlace.txt` | Cloud L4 y L6 | `b3b208baa4c1dfaf829becaae913e4c8af8c2d8bea8483c11b7c68470ab250f4` |
| `correo-intervalo.txt` | Correo L4 | `744203bbb1eab000e7ce62c0e07efd46ba49f1321946af426bf9c76fab5f9b7d` |

### Resolución del filtro de correo

```bash
grep -nF 'SH-881' 01_copias_trabajo/correo-metadatos.csv | grep -F ',Enviados'
```

La tubería busca el identificador descubierto y exige la carpeta `Enviados`. Después se validan la cuenta, el intervalo y el destinatario directamente en la fila candidata:

| Línea | Remitente válido | Dentro del intervalo | Carpeta `Enviados` | Destinatario excluido | Resultado |
| ---: | --- | --- | --- | --- | --- |
| 2 | Sí | No | Sí | Sí | Se descarta |
| 3 | No | Sí | No | No | Se descarta |
| 4 | Sí | Sí | Sí | No | Se imprime |
| 5 | Sí | Sí | Sí | Sí | Se descarta |

La salida correcta es:

```text
4:<b88e@nortesur.invalid>,2026-10-22T21:24:40-03:00,r.ibarra@nortesur.invalid,contacto@licitacorp.invalid,Lo conversado,,https://almacen.nortesur-cloud.invalid/s/SH-881,Enviados
```

### Cronología en UTC

| # | Hito | Fuente y línea | Hora original | Hora UTC | Identificadores |
| ---: | --- | --- | --- | --- | --- |
| 1 | Conexión VPN | VPN L4 | 22-10-2026 20:52:10 `-03:00` | 22-10-2026 23:52:10Z | `r.ibarra`, `NS-NB-017`, `10.20.8.23` |
| 2 | Carga del objeto | Cloud L3 | 23-10-2026 00:18:44Z | 23-10-2026 00:18:44Z | `C-5531`, `OBJ-3310` |
| 3 | Creación del enlace | Cloud L4 | 23-10-2026 00:21:09Z | 23-10-2026 00:21:09Z | `OBJ-3310`, `SH-881` |
| 4 | Mensaje con el enlace | Correo L4 | 22-10-2026 21:24:40 `-03:00` | 23-10-2026 00:24:40Z | `<b88e...>`, `contacto@licitacorp.invalid`, `SH-881` |
| 5 | Acceso anónimo | Cloud L6 | 23-10-2026 00:40:12Z | 23-10-2026 00:40:12Z | `198.51.100.200`, `OBJ-3310`, `SH-881` |
| 6 | Desconexión VPN | VPN L7 | 22-10-2026 22:47:55 `-03:00` | 23-10-2026 01:47:55Z | `r.ibarra`, `NS-NB-017`, `10.20.8.23` |

### Límites y exclusiones

| Registro o fuente | Motivo | Decisión |
| --- | --- | --- |
| Correo L5 a `correo-personal.invalid` | Mensaje personal excluido | Excluir sin examinar el cuerpo; escalar una posible inclusión |
| Cloud L8, acceso a las 11:02:48Z | Fuera del intervalo | Preservar y solicitar ampliación antes de analizarlo |
| VPN L5-L6 y cloud L5, `j.soto` | Otro trabajador | Excluir; requerir nueva autorización |

El correo L2 también se descarta porque es personal y ocurre antes del intervalo.

## 5. Hallazgo reproducible modelo

| Campo | Contenido |
| --- | --- |
| Identificador | `H-01` |
| Pregunta | P1 y parcialmente P2 |
| Evidencia | `NS05-EV04-T1` y `NS05-EV03-T1`, verificadas contra las exportaciones recibidas |
| Procedimiento | Filtros con `grep` por cuenta e intervalo; descubrimiento de `SH-881`; búsqueda del enlace; normalización UTC |
| Salidas | `cloud-cuenta.txt`, `cloud-enlace.txt` y `correo-intervalo.txt`, con los hashes anteriores |
| Resultado observado | Cloud L3 carga `OBJ-3310`; cloud L4 crea `SH-881`; correo L4 incluye el enlace; cloud L6 registra acceso anónimo exitoso |
| Interpretación y confianza | El objeto quedó accesible mediante un enlace enviado externamente. Confianza media |
| Alternativa | Uso de credenciales por otra persona, contenido distinto del nombre o envío autorizado no documentado |
| Limitación | Sin cuerpo, contenido del objeto ni identidad del acceso; hay un evento posterior fuera del intervalo |

### Conclusión de tres oraciones

> Las fuentes registran una secuencia de carga, creación y envío de un enlace, seguida de un acceso externo exitoso relacionado mediante `OBJ-3310` y `SH-881`. Los registros identifican cuentas, sesiones, un equipo e IP, pero no demuestran quién operaba la cuenta ni cuál era el contenido real del objeto. Corresponde preservar `OBJ-3310`, `SH-881` y la auditoría asociada, y decidir formalmente si se amplía el intervalo para examinar el acceso posterior.

## 6. Revisar y respaldar

### Revisión modelo

| Control | Resultado y observación |
| --- | --- |
| Acciones dentro del alcance | Cumple; se excluyeron mensajes personales, otro trabajador y el evento posterior |
| Afirmaciones con archivo y línea | Cumple; se citan VPN L4/L7, cloud L3/L4/L6 y correo L4 |
| Hashes de evidencia, copias y salidas | Cumple; las copias coinciden y las cuatro salidas tienen SHA-256 |
| Interpretación limitada | Cumple; no se atribuye la operación a una persona ni se afirma conocer el contenido |
| Alternativa y limitación plausibles | Cumple; ambas pueden cambiar la interpretación |

La revisión entre pares es un **acceso de revisión**. Solo sería transferencia si el equipo revisor asumiera control y responsabilidad sobre un artefacto.

### Respaldo esperado

El manifiesto debe incluir la bitácora, las verificaciones de exportaciones y copias, las cuatro salidas, `hashes-salidas.txt` y `hashes-entrega.txt`. La comprobación desde `LAB12_SALIDA` debe terminar en `OK` para cada archivo. Las copias de trabajo no se respaldan porque pueden recrearse.

## Comprobación final resuelta

- [x] Las preguntas no presuponen culpabilidad.
- [x] El alcance cita la autorización y distingue exclusión de escalamiento.
- [x] Las tres exportaciones y sus copias terminan en `OK`.
- [x] Las copias tienen identificadores derivados y no permiten escritura.
- [x] La revisión de custodia no inventa responsables, horas ni hashes.
- [x] La cronología está en UTC y cita archivo y línea.
- [x] El hallazgo separa resultado, interpretación, alternativa y limitación.
- [x] El respaldo contiene bitácora, verificaciones, salidas y anexos.
