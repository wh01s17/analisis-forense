---
title: "Actividad 02 - Auditar un proceso forense"
tags: [nota, course, curso]
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "02"
author: Jordy
start: 2026-08-17
end: 2026-08-19
created_at: 2026-08-12
aliases:
  - "Actividad 02 - Auditar un proceso forense"
---
# Actividad 02 - Auditar un proceso forense

**Duración:** 80 minutos. 
**Modalidad:** parejas. 
**Carácter:** formativo, sin calificación. 
**Versión:** B. 
**Aplicación prevista:** SAINF43-2, jornada vespertina.

## Propósito

Detectar errores de orden, alcance y documentación en un proceso de investigación forense y proponer una versión corregida.

## Caso: actividad inusual en servidor administrativo

PUERTO CLARO SPA autoriza investigar accesos a un servidor administrativo ocurridos entre las 01:00 y las 05:00. Se dispone de un equipo apagado, registros de acceso remoto, una copia de una planilla posiblemente extraída y una exportación de eventos de identidad. Se excluyen mensajes personales y períodos ajenos al incidente.

## Recordatorio para resolver la actividad

| Concepto o fase | Pregunta guía | Producto o idea clave |
| --- | --- | --- |
| Alcance y autorización | ¿Quién autoriza, qué preguntas se responderán y qué queda excluido? | Plan y límites autorizados antes de intervenir. |
| Identificación | ¿Qué fuentes existen y en qué estado se reciben? | Inventario de fuentes y registro de recepción. |
| Adquisición/preservación | ¿Cómo se obtienen y protegen datos sin alterar el original? | Fuentes protegidas, copias verificadas y custodia. |
| Examen | ¿Qué datos pertinentes se localizan, extraen, filtran u organizan? | Resultados organizados y referenciados. |
| Análisis | ¿Qué significan los resultados al correlacionarlos y contrastar alternativas? | Hallazgos sustentados, sin confundir asociación con autoría. |
| Presentación | ¿Cómo se explican métodos, hallazgos, datos faltantes y límites? | Comunicación verificable y conclusión proporcional. |
| Bitácora y custodia | ¿Quién hizo qué, cuándo, sobre qué fuente y bajo qué responsabilidad? | Registros que acompañan todas las fases, no solo el cierre. |
| Hash | ¿La copia conserva el mismo valor de verificación registrado? | Permite comprobar integridad; no demuestra autoría ni que un archivo sea seguro. |

Para la Parte A, usen estas etiquetas:

| Evaluación | Cuándo utilizarla |
| --- | --- |
| Adecuada | La decisión respeta el alcance, protege las fuentes y deja un resultado verificable. |
| Inadecuada | La decisión excede la autorización, altera o arriesga una fuente, o sostiene más de lo que permiten los datos. |
| Depende de una condición | Solo sería válida si se cumple una condición que deben escribir, por ejemplo una autorización o verificación previa. |

## Parte A - Revisar decisiones (20 minutos)

El analista registró estas ocho decisiones. Para cada una indiquen si es **adecuada**, **inadecuada** o **depende de una condición**. Identifiquen la fase y justifiquen; si es inadecuada, propongan una corrección.

1. Confirmar quién autoriza, las preguntas, el horario investigado y las exclusiones.
2. Descartar la planilla del inventario porque “probablemente no aporta información”.
3. Registrar la recepción y el estado de todas las fuentes disponibles.
4. Abrir la planilla directamente en el equipo original para comprobar su contenido.
5. Verificar la integridad de la copia y trabajar sobre una copia separada del original.
6. Examinar los eventos autorizados y organizar los resultados por fecha, hora y fuente.
7. Concluir que el titular de la cuenta aprobó el acceso sin contrastar otras fuentes ni explicaciones.
8. Informar los hallazgos, las fuentes utilizadas, los datos faltantes y los límites de la conclusión.

| Decisión | Evaluación | Fase | Justificación o corrección |
| ---: | --- | --- | --- |
| 1 |  |  |  |
| 2 |  |  |  |
| 3 |  |  |  |
| 4 |  |  |  |
| 5 |  |  |  |
| 6 |  |  |  |
| 7 |  |  |  |
| 8 |  |  |  |

## Parte B - Construir el mapa corregido (40 minutos)

Construyan un proceso que otra persona pueda revisar. Algunas acciones pueden repetirse o ejecutarse en paralelo, pero deben explicar cualquier cambio de orden.

| Orden | Fase                     | Objetivo | Entrada | Acción | Responsable | Producto verificable | Riesgo si se ejecuta mal |
| :---: | ------------------------ | -------- | ------- | ------ | ----------- | -------------------- | ------------------------ |
|   1   | Alcance y autorización   |          |         |        |             |                      |                          |
|   2   | Identificación           |          |         |        |             |                      |                          |
|   3   | Adquisición/preservación |          |         |        |             |                      |                          |
|   4   | Examen                   |          |         |        |             |                      |                          |
|   5   | Análisis                 |          |         |        |             |                      |                          |
|   6   | Presentación             |          |         |        |             |                      |                          |
|       |                          |          |         |        |             |                      |                          |

La bitácora y el registro de custodia acompañan el proceso: no deben aparecer únicamente al final.

## Parte C - Resolver una contingencia (15 minutos)

Al verificar la copia de la planilla, su hash no coincide con el valor registrado al recibirla. Redacten cuatro acciones e indiquen:

1. qué acción se detiene o limita;
2. qué se registra;
3. a quién se comunica y quién decide;
4. qué ocurre con la copia discrepante mientras se resuelve la incidencia.

## Parte D - Cierre individual (5 minutos)

Cada integrante responde en dos o tres frases:

1. ¿Por qué el examen no equivale al análisis?
2. ¿Por qué un registro asociado a una cuenta no demuestra por sí solo quién realizó una acción?

## Entrega y criterios comunes

Entreguen la tabla de revisión, el mapa, la respuesta a la contingencia y los cierres individuales.

| Criterio | Logro esperado |
| --- | --- |
| Orden y alcance | Corrige las decisiones y mantiene todas las acciones dentro de la autorización. |
| Proceso y productos | Completa las seis fases con entradas, acciones, responsables y productos coherentes. |
| Integridad y trazabilidad | Protege el original y contempla verificación, custodia y documentación transversal. |
| Razonamiento | Distingue examen, análisis, hallazgo y conclusión, y reconoce límites de atribución. |
