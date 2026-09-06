---
title: "Materia y guía de lectura - Fundamentos del análisis forense digital"
tags:
  - nota
  - course
  - curso
  - materia-y-guia-de-lectura
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "01"
author: Jordy
start: 2026-08-10
end: 2026-08-12
created_at: "2026-08-06 17:21"
aliases:
  - "Materia y guía de lectura - Fundamentos del análisis forense digital"
---

# Materia y guía de lectura - Fundamentos del análisis forense digital

```toc
```

## Propósito de esta guía

Esta guía contiene todo el contenido conceptual necesario para la primera lección. No exige experiencia previa ni el uso de herramientas forenses. Su objetivo es aprender a observar con cuidado, proteger la información y separar lo que **sabemos** de lo que solamente **suponemos**.

Las fuentes externas incluidas al final son respaldo y profundización opcional. No es necesario consultarlas para preparar esta sesión. Esta guía tampoco reemplaza asesoría jurídica ni los procedimientos autorizados de una organización.

## Ruta mínima de preparación

Estudia en este orden:

1. Qué es y para qué sirve el análisis forense digital.
2. Alcance, autorización y preguntas investigativas.
3. Fuente, dato, indicio, evidencia potencial, hallazgo y conclusión.
4. Principios fundamentales.
5. Original, copia de trabajo y hash.
6. Caso NORTE SUR SPA.
7. Autoevaluación.

### Criterio de suficiencia

Estás preparado cuando puedes:

- Explicar para qué sirve el análisis forense sin reducirlo a “recuperar archivos”.
- Distinguir un dato observable de una interpretación.
- Mencionar al menos cuatro principios y asociarlos con una acción concreta.
- Proponer dos primeras acciones seguras para el caso.
- Explicar qué ayuda a verificar un hash y qué no demuestra.

## Objetivos de estudio

Al finalizar deberías poder:

- Explicar el propósito y los límites del análisis forense digital.
- Definir un alcance mediante preguntas investigativas concretas.
- Distinguir los niveles que van desde una fuente hasta una conclusión.
- Relacionar autorización, preservación, integridad, trazabilidad, reproducibilidad e imparcialidad.
- Reconocer los límites éticos y técnicos del rol del analista.
- Evitar conclusiones de autoría que no estén respaldadas por evidencia suficiente.

## 1. ¿Qué es el análisis forense digital?

El análisis forense digital es el tratamiento **sistemático, autorizado y documentado** de información electrónica para responder preguntas sobre un hecho. Incluye identificar posibles fuentes, preservar datos, obtenerlos de manera controlada, examinarlos, interpretarlos y comunicar resultados.

La disciplina no consiste solamente en abrir archivos, ejecutar una herramienta o encontrar algo llamativo. Un trabajo forense debe permitir explicar:

- Qué pregunta se intentó responder.
- Qué información se utilizó y de dónde provino.
- Qué acciones se realizaron y quién las realizó.
- Qué se observó realmente.
- Cómo se interpretaron las observaciones.
- Qué no fue posible determinar.

Un resultado útil debe ser:

| Cualidad | Explicación sencilla |
| --- | --- |
| Pertinente | Responde una pregunta incluida en el alcance. |
| Íntegro | No presenta cambios no explicados respecto de lo adquirido. |
| Trazable | Permite reconstruir quién hizo qué, cuándo y sobre qué elemento. |
| Verificable | Otra persona competente puede revisar el procedimiento y los resultados. |
| Comprensible | Separa hechos, interpretación, conclusiones y limitaciones. |

### ¿Para qué se utiliza?

En una organización puede ayudar a:

- Investigar un acceso no autorizado.
- Reconstruir una secuencia de eventos.
- Determinar qué cuentas, equipos o datos estuvieron involucrados.
- Investigar fraude, fuga de información o incumplimientos internos.
- Apoyar decisiones de contención, recuperación y mejora.
- Comunicar hallazgos a responsables técnicos, directivos, auditores o autoridades.

El propósito depende del encargo. Un registro puede bastar para confirmar que ocurrió una conexión, pero no necesariamente para determinar quién controlaba la cuenta.

## 2. Alcance y autorización

Antes de examinar información se debe establecer:

- Quién solicita el trabajo y qué autoridad posee.
- Qué preguntas deben responderse.
- Qué sistemas, cuentas, fechas y datos están incluidos.
- Qué acciones están permitidas.
- Qué acciones requieren autorización adicional.
- A quién deben comunicarse los hallazgos urgentes.

**Tener acceso técnico no equivale a tener autorización.** Poder abrir un correo personal, copiar todo un disco o revisar una cuenta no significa que esté permitido hacerlo.

### De una pregunta vaga a una pregunta investigable

“¿Qué pasó en el computador?” es demasiado amplio. Preguntas más útiles serían:

- ¿Existió un acceso remoto no autorizado entre las 22:00 y las 06:00?
- ¿Qué cuenta aparece asociada con la sesión registrada?
- ¿Qué archivos se crearon o modificaron durante ese período?
- ¿Qué fuentes apoyan o contradicen la hipótesis inicial?

Una buena pregunta delimita el período, el sistema o la cuenta y el hecho que se desea comprobar. El alcance protege a la organización, a las personas cuyos datos podrían revisarse y al propio analista.

## 3. De la fuente a la conclusión

Estos conceptos describen niveles distintos. No deben tratarse como sinónimos.

| Concepto                    | Definición                                                                         | Ejemplo                                                                           |
| --------------------------- | ---------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| Fuente                      | Lugar o elemento desde el cual pueden obtenerse datos.                             | Registro de VPN, disco, teléfono o servicio cloud.                                |
| Dato                        | Representación observable que todavía necesita contexto.                           | “Inicio de sesión a las 02:14 UTC”.                                               |
| Indicio                     | Dato o relación de datos que orienta una hipótesis.                                | El acceso ocurrió desde una IP no habitual.                                       |
| Evidencia digital potencial | Información preservada y documentada que podría sostener o refutar una afirmación. | Exportación verificada del registro de VPN.                                       |
| Artefacto                   | Rastro generado por un sistema o aplicación.                                       | Evento, historial, archivo temporal o metadato.                                   |
| Hallazgo                    | Afirmación técnica respaldada por evidencia examinada y contextualizada.           | El registro contiene una autenticación exitosa de la cuenta X a las 02:14 UTC.    |
| Hipótesis                   | Explicación provisional que debe contrastarse.                                     | Las credenciales de la cuenta podrían haber sido comprometidas.                   |
| Conclusión                  | Respuesta razonada que integra hallazgos y declara limitaciones.                   | No fue posible determinar quién controlaba la cuenta con las fuentes disponibles. |

### Ejemplo de razonamiento correcto

1. **Dato:** el registro muestra una conexión desde una IP no habitual.
2. **Indicio:** la conexión podría estar relacionada con una actividad no autorizada.
3. **Pregunta:** ¿la actividad coincide con eventos del equipo y de la cuenta?
4. **Corroboración necesaria:** registros del endpoint, identidad, VPN y otros datos dentro del alcance.
5. **Límite:** la IP por sí sola no identifica a la persona que realizó la acción.

El error habitual es saltar directamente del dato a una acusación. El analista debe explicar cada paso intermedio y considerar información que contradiga la hipótesis inicial.

## 4. Principios fundamentales

| Principio | Qué significa | Acción concreta |
| --- | --- | --- |
| Autorización | Actuar dentro de un propósito y alcance aprobados. | Detenerse y consultar antes de revisar información no autorizada. |
| Preservación | Evitar pérdida o modificación innecesaria. | Documentar el estado y proteger el original antes de analizar. |
| Integridad | Poder detectar y explicar cambios en los datos. | Usar controles de acceso, hashes y procedimientos documentados. |
| Trazabilidad | Poder reconstruir las acciones realizadas. | Registrar responsable, fecha, hora, zona, acción, herramienta y resultado. |
| Verificabilidad | Permitir que otra persona revise el trabajo. | Describir el procedimiento y conservar referencias precisas. |
| Imparcialidad | Buscar evidencia favorable y contraria a la hipótesis. | Separar observación, interpretación y conclusión. |
| Minimización y privacidad | Acceder solo a lo necesario para el objetivo. | Excluir datos personales que no sean pertinentes. |
| Competencia | Conocer el alcance y las limitaciones del método utilizado. | No ejecutar una técnica que no se sabe aplicar o validar. |

### Preservar no siempre significa apagar

Apagar un equipo puede eliminar memoria, conexiones y procesos activos. Mantenerlo encendido también puede permitir que continúe el daño o que cambien datos. Por eso no existe una instrucción universal como “siempre apagar” o “nunca apagar”.

La decisión debe considerar:

- Riesgo para las personas y la operación.
- Posible pérdida de datos volátiles.
- Necesidad de contener el incidente.
- Autoridad y procedimiento aplicable.
- Competencia técnica disponible.

En esta primera clase no se exige decidir una adquisición técnica. La respuesta esperada es más básica: **no improvisar, documentar el estado, evitar acciones innecesarias y escalar la decisión**.

## 5. Roles y conducta ética

| Rol | Responsabilidad principal |
| --- | --- |
| Solicitante o autoridad | Define propósito, autorización, alcance y destinatarios. |
| Responsable del incidente | Coordina prioridad, contención y comunicación. |
| Custodio de evidencia | Registra recepción, almacenamiento, acceso y transferencias. |
| Analista o perito | Aplica métodos, documenta, interpreta y declara limitaciones. |
| Propietario del sistema o dato | Aporta contexto, criticidad y restricciones. |
| Revisor | Verifica consistencia, trazabilidad y claridad. |

En una organización pequeña una persona puede cumplir más de un rol, pero las responsabilidades deben seguir siendo explícitas.

El analista debe:

- Actuar únicamente con autorización.
- Mantener confidencialidad.
- Evitar curiosidad o acceso innecesario a datos personales.
- No atribuir responsabilidad sin evidencia suficiente.
- Informar errores, desviaciones y limitaciones.
- Reconocer cuándo necesita ayuda especializada.

## 6. Original, copia de trabajo y hash

### Original y copia de trabajo

- **Original:** fuente recibida o identificada que debe conservarse con el menor cambio posible.
- **Adquisición o imagen:** representación obtenida mediante un procedimiento documentado.
- **Copia de trabajo:** duplicado utilizado para examinar y analizar sin intervenir innecesariamente el original.

Una copia común de archivos no siempre equivale a una imagen forense: puede omitir metadatos, estructuras del sistema de archivos, espacio no asignado o archivos eliminados. Esta diferencia se estudiará con mayor profundidad en las lecciones de adquisición.

### Función hash

Una función hash calcula un valor a partir de un conjunto de datos. En este curso se utiliza principalmente para registrar y comparar una versión concreta de un archivo o imagen.

Un hash puede ayudar a:

- Comparar una adquisición con su copia.
- Detectar cambios accidentales o no explicados.
- Identificar con precisión la versión analizada.

Un hash **no**:

- Cifra el contenido.
- Identifica al autor.
- Demuestra intención, culpabilidad o inocencia.
- Determina si el archivo es benigno o malicioso.
- Reemplaza la bitácora, la procedencia ni la cadena de custodia.

## 7. Primeras acciones ante un posible incidente

Antes de usar herramientas, conviene seguir una secuencia prudente:

1. **Proteger a las personas y la operación.** La seguridad y la contención urgente pueden tener prioridad.
2. **Confirmar autoridad y alcance.** Determinar quién puede autorizar las acciones.
3. **Observar sin interpretar de más.** Registrar lo visible y distinguirlo de las opiniones.
4. **Documentar el estado.** Anotar fecha, hora, zona, personas presentes y acciones ya realizadas.
5. **Evitar manipulación innecesaria.** No abrir archivos sospechosos ni conectar dispositivos por curiosidad.
6. **Proteger fuentes que podrían perderse.** Escalar la conservación de registros sujetos a rotación.
7. **Trabajar sobre copias cuando corresponda.** Proteger el original y documentar el procedimiento.
8. **Comunicar límites y decisiones.** Registrar lo que se hizo, lo que no se hizo y por qué.

Esta secuencia no sustituye el procedimiento de respuesta a incidentes. Su propósito es evitar que una reacción apresurada destruya información, amplíe el daño o exceda la autorización.

## 8. Caso aplicado: NORTE SUR SPA

A las 08:15, soporte recibe un aviso de Camila, encargada de remuneraciones: al iniciar su equipo encuentra abierta una aplicación de acceso remoto que no recuerda haber instalado. El registro de VPN muestra una conexión nocturna desde una dirección IP no habitual. En Descargas aparece `actualizacion.zip`. Un supervisor tomó una fotografía de la pantalla y desconectó el cable de red. Nadie ha autorizado todavía revisar archivos personales ni acceder al correo de Camila.

### Hechos observables

- Camila informó que no recuerda haber instalado la aplicación.
- Existe un registro de conexión VPN nocturna desde una IP no habitual.
- En Descargas aparece un archivo con un nombre determinado.
- El supervisor fotografió la pantalla y desconectó la red.
- El acceso al correo personal no está autorizado.

### Afirmaciones que todavía no pueden hacerse

- Que Camila instaló la aplicación.
- Que la IP identifica físicamente al atacante.
- Que el archivo ZIP contiene malware.
- Que existió extracción de información.
- Que desconectar la red fue necesariamente la mejor decisión.

### Preguntas para razonar el caso

1. ¿Qué elementos son fuentes y qué datos concretos aporta cada uno?
2. ¿Qué afirmaciones son observaciones y cuáles son hipótesis?
3. ¿Qué podría cambiar si se abre el ZIP sobre el equipo original?
4. ¿Qué información debería registrarse sobre las acciones ya realizadas?
5. ¿Qué acceso requeriría autorización adicional?
6. ¿Qué registros podrían perderse si no se preservan oportunamente?

### Primeras acciones razonables

- Confirmar autoridad, objetivo y alcance.
- Registrar quién observó qué, cuándo y en qué condiciones.
- Documentar el estado del equipo y las acciones ya realizadas.
- Evitar abrir el ZIP o conectar dispositivos al equipo original.
- Solicitar la preservación de registros que puedan rotar.
- Escalar cualquier acceso que no esté expresamente autorizado.

La actividad de la clase desarrolla este caso con seis elementos. La meta no es encontrar una etiqueta única, sino justificar qué permite saber cada elemento y qué acción evita alterarlo.

## 9. Errores frecuentes

### “Si está en el computador, ya es evidencia concluyente”

No. Un elemento puede ser una fuente o evidencia potencial, pero su valor depende de procedencia, contexto, integridad, relevancia y documentación.

### “La dirección IP identifica al atacante”

No por sí sola. Puede corresponder a un dispositivo compartido, VPN, proxy, traducción de direcciones, infraestructura comprometida o registro incompleto.

### “El hash demuestra que el archivo es original”

El hash ayuda a comparar una versión concreta. Para explicar origen y autenticidad también se necesita procedencia, contexto y documentación.

### “La fotografía reemplaza la adquisición”

La fotografía conserva una vista limitada de la pantalla. No contiene necesariamente los datos subyacentes, metadatos, estado completo ni contexto técnico.

### “El analista debe confirmar la sospecha inicial”

El analista debe examinar la hipótesis y buscar también información que pueda refutarla. Su obligación es explicar lo que la evidencia permite sostener.

### “Preservar siempre significa no tocar nada”

Toda interacción puede producir cambios, pero algunas acciones controladas son necesarias. Lo correcto es minimizar, justificar y documentar los cambios dentro de la autorización.

## 10. Qué se estudiará después

Para mantener esta primera lección enfocada, los siguientes temas solo se introducen y se desarrollarán más adelante:

| Tema | Lección posterior |
| --- | --- |
| Fases completas del proceso forense | Lección 02. |
| Laboratorio, herramientas y validación | Lección 03. |
| Adquisición, imágenes y verificación | Lección 04. |
| Cadena de custodia detallada y registro técnico | Lecciones 04 y 06. |
| Artefactos de sistemas operativos y cronologías | Lección 07. |
| Evidencia de red, bases de datos, cloud y móviles | Lecciones 08 a 10. |

No es necesario dominar esos contenidos para cumplir el objetivo de esta sesión.

## 11. Guía de estudio

### Paso 1 - Explicar con palabras propias

Completa sin copiar las definiciones:

- El análisis forense digital sirve para...
- Tener acceso no significa tener autorización porque...
- Un dato se convierte en indicio cuando...
- Preservación e integridad se diferencian en que...
- Un hash ayuda a verificar..., pero no permite afirmar...

### Paso 2 - Aplicar al caso

Sin mirar la pauta docente:

1. Separa hechos, hipótesis y acciones razonables del caso NORTE SUR SPA.
2. Completa [[analisis-forense/leccion-01/actividad]].
3. Explica por qué un registro de VPN y una IP no bastan para atribuir autoría.
4. Formula una pregunta investigativa concreta para el caso.

### Paso 3 - Ensayar la explicación

Explícale el caso a otra persona en tres minutos siguiendo esta estructura:

1. Qué sabemos.
2. Qué todavía no sabemos.
3. Qué debemos proteger.
4. Qué acción requiere autorización.

Si mezclas hechos con conclusiones, vuelve a la sección 3.

## Autoevaluación

### Preguntas

1. ¿Qué diferencia existe entre un dato y un hallazgo?
2. ¿Por qué debe definirse el alcance antes de examinar información?
3. ¿Qué diferencia existe entre preservación e integridad?
4. ¿Qué demuestra la coincidencia de dos hashes y qué no demuestra?
5. ¿Por qué conviene separar el original de la copia de trabajo?
6. ¿Qué significa actuar con imparcialidad?
7. ¿Por qué una fotografía de pantalla no reemplaza una adquisición?
8. Menciona dos riesgos de manipular directamente el equipo original.
9. Formula una primera acción segura para NORTE SUR SPA.
10. Corrige la frase: “La IP del registro demuestra quién fue el atacante”.

### Respuestas orientativas

1. Un dato es una representación observable; un hallazgo es una afirmación técnica sustentada después de examinar y contextualizar evidencia.
2. Para asegurar autorización, pertinencia, privacidad y límites claros de actuación.
3. Preservar busca evitar pérdida o modificación innecesaria; demostrar integridad permite detectar y explicar cambios en los datos.
4. Ayuda a verificar que las entradas comparadas producen el mismo valor con esa función; no demuestra procedencia, autoría, intención ni cadena de custodia.
5. Para reducir modificaciones sobre la fuente y permitir un análisis controlado y revisable.
6. Examinar evidencia favorable y contraria a la hipótesis, separando observación, interpretación y conclusión.
7. Porque solo muestra una vista, puede carecer de contexto y no conserva necesariamente metadatos ni datos subyacentes.
8. Alterar metadatos, ejecutar contenido, perder datos volátiles, contaminar el contexto o impedir una revisión posterior.
9. Por ejemplo: confirmar la autorización, documentar el estado o evitar abrir el ZIP hasta contar con un procedimiento autorizado.
10. “El registro asocia una conexión con una IP; se necesita evidencia adicional para determinar quién controlaba la conexión”.

## Fuentes base y profundización opcional

Estas fuentes respaldan el contenido. Su lectura no es obligatoria para esta lección.

- [NIST - Digital Evidence](https://www.nist.gov/digital-evidence): definición, herramientas y desafíos de la disciplina.
- [NIST SP 800-86 - Guide to Integrating Forensic Techniques into Incident Response](https://csrc.nist.gov/pubs/sp/800/86/final): proceso forense desde una perspectiva de TI y respuesta a incidentes.
- [NIST SP 800-61 Rev. 3 - Incident Response Recommendations](https://csrc.nist.gov/pubs/sp/800/61/r3/final): relación entre preparación, respuesta, recuperación y gestión del riesgo.

Fecha de consulta de recursos vivos: 06-08-2026.
