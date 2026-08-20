---
title: "Solución docente - Proceso de investigación, versión B"
tags: [nota, course, curso, docente, reservado]
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "02"
author: Jordy
start: 2026-08-17
end: 2026-08-19
created_at: 2026-08-12
aliases:
  - "Solución docente - Proceso de investigación, versión B"
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

| Decisión | Evaluación | Fase                      | Justificación o corrección                                                                                |
| -------: | ---------- | ------------------------- | --------------------------------------------------------------------------------------------------------- |
|        1 | Adecuada   | Alcance y autorización    | Define autoridad, preguntas, período y límites antes de intervenir.                                       |
|        2 | Inadecuada | Identificación            | La planilla debe registrarse antes de evaluar su relevancia; no se excluye una fuente por una suposición. |
|        3 | Adecuada   | Identificación y custodia | Documenta qué se recibió y en qué estado.                                                                 |
|        4 | Inadecuada | Examen/preservación       | No abrir la planilla en el original; documentarla y utilizar una copia verificada.                        |
|        5 | Adecuada   | Adquisición/preservación  | Mantiene una referencia verificable y separa el trabajo del original.                                     |
|        6 | Adecuada   | Examen                    | Extrae y organiza información pertinente con referencias de fecha, hora y fuente.                         |
|        7 | Inadecuada | Análisis y conclusión     | La cuenta no demuestra quién realizó o aprobó la acción; se deben contrastar fuentes y alternativas.      |
|        8 | Adecuada   | Presentación              | Comunica el fundamento, los datos faltantes y los límites junto con los hallazgos.                        |

## Parte B - Construir el mapa corregido (40 minutos)

Construyan un proceso que otra persona pueda revisar. Algunas acciones pueden repetirse o ejecutarse en paralelo, pero deben explicar cualquier cambio de orden.

| Orden | Fase                             | Objetivo                                        | Entrada                                  | Acción                                                                     | Responsable           | Producto verificable                         | Riesgo si se ejecuta mal                             |
| :---: | -------------------------------- | ----------------------------------------------- | ---------------------------------------- | -------------------------------------------------------------------------- | --------------------- | -------------------------------------------- | ---------------------------------------------------- |
|   1   | Alcance y autorización           | Definir qué se investigará y con qué autoridad. | Solicitud y antecedentes                 | Confirmar preguntas, período, fuentes y exclusiones.                       | Coordinación          | Plan autorizado                              | Acceso indebido o preguntas imposibles de responder. |
|   2   | Identificación                   | Localizar y priorizar fuentes pertinentes.      | Plan y antecedentes                      | Inventariar equipo, accesos, planilla y eventos; registrar su estado.      | Coordinación/custodia | Inventario priorizado                        | Omitir una fuente o perder contexto.                 |
|   3   | Adquisición/preservación         | Obtener y proteger datos verificables.          | Fuentes identificadas                    | Registrar, preservar, verificar y separar original y copia.                | Adquisición/custodia  | Fuentes protegidas y copias verificadas      | Alteración o pérdida de trazabilidad.                |
|   4   | Examen                           | Localizar y organizar información pertinente.   | Copias verificadas                       | Filtrar y referenciar los datos dentro del alcance.                        | Analista              | Resultados organizados                       | Alterar el original o perder contexto.               |
|   5   | Análisis                         | Responder las preguntas mediante razonamiento.  | Resultados examinados                    | Correlacionar fuentes y contrastar explicaciones.                          | Analista/revisor      | Hallazgos sustentados                        | Confundir asociación con causalidad o autoría.       |
|   6   | Presentación                     | Comunicar resultados proporcionales.            | Hallazgos y bitácora                     | Exponer métodos, hallazgos, límites y conclusión.                          | Coordinación/revisor  | Comunicación verificable                     | Ocultar límites o exagerar conclusiones.             |
|   —   | Transversal: bitácora y custodia | Mantener trazabilidad durante todo el proceso.  | Fuentes, copias y registros de cada fase | Registrar responsables, fechas, acciones, transferencias y verificaciones. | Todo el equipo        | Bitácora y registro de custodia actualizados | Perder trazabilidad o no poder revisar el proceso.   |

La bitácora y el registro de custodia acompañan el proceso: no deben aparecer únicamente al final.

## Parte C - Resolver una contingencia (15 minutos)

Al verificar la copia de la planilla, su hash no coincide con el valor registrado al recibirla. Redacten cuatro acciones e indiquen:

1. qué acción se detiene o limita;
   **Respuesta:** Se detiene el examen de la copia discrepante para no basar resultados en una fuente cuya integridad no está confirmada.
2. qué se registra;
   **Respuesta:** Se registran la fuente, ambos valores hash, el algoritmo, la fecha, el responsable y el resultado, sin modificar silenciosamente el registro anterior.
3. a quién se comunica y quién decide;
   **Respuesta:** Se comunica a custodia y coordinación; ellas deciden si corresponde una nueva obtención o una verificación controlada.
4. qué ocurre con la copia discrepante mientras se resuelve la incidencia.
   **Respuesta:** Se aísla y conserva identificada; no se borra ni se utiliza hasta resolver y documentar la incidencia.

## Parte D - Cierre individual (5 minutos)

Cada integrante responde en dos o tres frases:

1. ¿Por qué el examen no equivale al análisis?
   **Respuesta:** El examen localiza, extrae, filtra y organiza datos pertinentes. El análisis correlaciona e interpreta esos resultados para responder las preguntas de la investigación.
2. ¿Por qué un registro asociado a una cuenta no demuestra por sí solo quién realizó una acción?
   **Respuesta:** El registro vincula el evento con una cuenta o sesión, pero no identifica necesariamente a la persona que la controlaba. La atribución requiere evidencia adicional y límites explícitos.

## Entrega y criterios comunes

Entreguen la tabla de revisión, el mapa, la respuesta a la contingencia y los cierres individuales.

| Criterio | Logro esperado |
| --- | --- |
| Orden y alcance | Corrige las decisiones y mantiene todas las acciones dentro de la autorización. |
| Proceso y productos | Completa las seis fases con entradas, acciones, responsables y productos coherentes. |
| Integridad y trazabilidad | Protege el original y contempla verificación, custodia y documentación transversal. |
| Razonamiento | Distingue examen, análisis, hallazgo y conclusión, y reconoce límites de atribución. |
