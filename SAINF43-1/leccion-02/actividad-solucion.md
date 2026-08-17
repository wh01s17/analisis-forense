---
title: "Solución docente - Proceso de investigación, versión A"
tags: [nota, course, curso, docente, reservado]
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "02"
author: Jordy
start: 2026-08-17
end: 2026-08-19
created_at: "2026-08-07 11:58"
aliases:
  - "Solución docente - Proceso de investigación, versión A"
---
# Actividad - Diseñar el proceso de investigación

**Duración:** 80 minutos.
**Modalidad:** parejas.
**Carácter:** formativo, sin calificación.
**Versión:** A.
**Aplicación prevista:** SAINF43-1, jornada diurna.

## Caso: acceso nocturno y posible extracción

NORTE SUR SPA autoriza investigar el acceso remoto de la lección anterior. El objetivo es establecer si existió acceso no autorizado, qué activos pudieron verse afectados y qué información permite reconstruir los hechos. Se dispone del equipo apagado, registros de VPN, una copia del archivo sospechoso y una exportación de eventos. No está autorizado acceder a archivos personales ajenos al incidente ni contactar al supuesto atacante.

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

## Parte A - Revisar decisiones

El equipo anterior dejó estas ocho decisiones. Para cada una indiquen si es **adecuada**, **inadecuada** o **depende de una condición**. Identifiquen la fase y justifiquen; si es inadecuada, propongan una corrección.

1. Confirmar quién autoriza, las preguntas, el horario investigado y las exclusiones.
2. Abrir el archivo sospechoso directamente en el equipo original para “ganar tiempo”.
3. Identificar el equipo, los registros VPN, la copia del archivo y la exportación de eventos; registrar su recepción y estado.
4. Verificar la integridad de la copia recibida y mantenerla separada del original antes de examinarla.
5. Revisar también el correo personal del usuario, aunque está fuera del alcance, para buscar contexto.
6. Afirmar que el titular de la cuenta realizó la extracción solo porque su cuenta aparece en el registro VPN.
7. Examinar las fuentes autorizadas, correlacionar sus resultados y considerar una explicación alternativa.
8. Comunicar una conclusión definitiva sin explicar fuentes ausentes ni limitaciones.

| Decisión | Evaluación | Fase                      | Justificación o corrección                                                                                         |
| -------: | ---------- | ------------------------- | ------------------------------------------------------------------------------------------------------------------ |
|        1 | Adecuada   | Alcance y autorizacion    | Debe ocurrir antes de intervenir las fuentes y definir límites verificables.                                       |
|        2 | Inadecuada | Examen/preservación       | No abrir el archivo en el original; documentar su estado y examinar una copia verificada.                          |
|        3 | Adecuada   | Identificación y custodia | Inventariar todas las fuentes y registrar su recepción y estado antes de tratarlas.                                |
|        4 | Adecuada   | Adquisición/preservación  | Verificar la copia y separarla del original reduce alteraciones no controladas.                                    |
|        5 | Inadecuada | Alcance y autorización    | No acceder al correo excluido; registrar su posible relevancia y solicitar una ampliación autorizada.              |
|        6 | Inadecuada | Análisis y conclusión     | Una cuenta registrada no demuestra identidad ni autoría; se deben correlacionar fuentes y considerar alternativas. |
|        7 | Adecuada   | Examen y análisis         | Examina datos autorizados y luego los correlaciona sin descartar otras explicaciones.                              |
|        8 | Inadecuada | Presentación              | Comunicar fuentes, datos faltantes, limitaciones y una conclusión proporcional a los hallazgos.                    |

## Parte B - Construir el mapa corregido

Construyan un proceso que otra persona pueda revisar. Algunas acciones pueden repetirse o ejecutarse en paralelo, pero deben explicar cualquier cambio de orden.

| Orden | Fase                     | Objetivo                                        | Entrada                  | Acción                                                         | Responsable           | Producto verificable                    | Riesgo si se ejecuta mal                             |
| :---: | ------------------------ | ----------------------------------------------- | ------------------------ | -------------------------------------------------------------- | --------------------- | --------------------------------------- | ---------------------------------------------------- |
|   1   | Alcance y autorización   | Definir qué se investigará y con qué autoridad. | Solicitud y antecedentes | Confirmar preguntas, período, fuentes y exclusiones.           | Coordinación          | Plan autorizado                         | Acceso indebido o preguntas imposibles de responder. |
|   2   | Identificación           | Localizar y priorizar fuentes pertinentes.      | Plan y antecedentes      | Inventariar equipo, VPN, copia y eventos; registrar su estado. | Coordinación/custodia | Inventario priorizado                   | Omitir una fuente o perder contexto.                 |
|   3   | Adquisición/preservación | Obtener y proteger datos verificables.          | Fuentes identificadas    | Registrar, preservar, verificar y separar original y copia.    | Adquisición/custodia  | Fuentes protegidas y copias verificadas | Alteración o pérdida de trazabilidad.                |
|   4   | Examen                   | Localizar y organizar información pertinente.   | Copias verificadas       | Filtrar y referenciar los datos dentro del alcance.            | Analista              | Resultados organizados                  | Alterar el original o perder contexto.               |
|   5   | Análisis                 | Responder las preguntas mediante razonamiento.  | Resultados examinados    | Correlacionar fuentes y contrastar explicaciones.              | Analista/revisor      | Hallazgos sustentados                   | Confundir asociación con causalidad o autoría.       |
|   6   | Presentación             | Comunicar resultados proporcionales.            | Hallazgos y bitácora     | Exponer métodos, hallazgos, límites y conclusión.              | Coordinación/revisor  | Comunicación verificable                | Ocultar límites o exagerar conclusiones.             |

La bitácora y el registro de custodia acompañan el proceso: no deben aparecer únicamente al final.


## Parte C - Resolver una contingencia

Durante el examen aparece una referencia a correos personales que podrían aportar contexto, pero estos están excluidos del alcance. Redacten cuatro acciones e indiquen:

1. qué acción se detiene o limita;
   **Respuesta:** Se detiene el acceso al contenido de los correos personales y se continúa solo con fuentes autorizadas que no dependan de él.
2. qué se registra;
   **Respuesta:** Se registra la referencia encontrada, cuándo y en qué fuente apareció y por qué podría ser pertinente, sin copiar el contenido personal.
3. a quién se comunica y quién decide;
   **Respuesta:** Se comunica a la coordinación y a quien autorizó el caso; esa autoridad decide si amplía el alcance.
4. cómo continúa el trabajo mientras no exista una ampliación autorizada.
   **Respuesta:** Se mantiene protegida la fuente, se examinan las demás fuentes autorizadas y se declara la limitación si no se amplía el alcance.

## Parte D - Cierre individual

Cada integrante responde en dos o tres frases:

1. ¿Por qué el examen no equivale al análisis?
   **Respuesta:** El examen localiza, extrae, filtra y organiza datos pertinentes. El análisis correlaciona e interpreta esos resultados para responder las preguntas de la investigación.
2. ¿Por qué un registro asociado a una cuenta no demuestra por sí solo quién realizó una acción?
   **Respuesta:** El registro vincula el evento con una cuenta o sesión, pero no identifica necesariamente a la persona que la controlaba. La atribución requiere evidencia adicional y límites explícitos.
