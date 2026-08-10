---
title: "Solución - ¿Qué puede convertirse en evidencia?"
tags:
  - nota
  - course
  - curso
  - docente
  - reservado
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "01"
author: Jordy
start: 2026-08-10
end: 2026-08-12
created_at: "2026-08-07 11:58"
aliases:
  - "Solución - ¿Qué puede convertirse en evidencia?"
---
# Actividad - ¿Qué puede convertirse en evidencia?

**Duración:** 50 minutos. 
**Modalidad:** trabajo guiado en parejas. 
**Carácter:** formativo, sin calificación.

> **Recordatorio:** esta es la primera actividad del curso. No necesitas experiencia previa ni conocer herramientas forenses. Lo importante es observar con cuidado, no inventar datos y explicar por qué protegerías la información.

## Caso: acceso remoto en NORTE SUR SPA

A las 08:15, soporte recibe un aviso de Camila, encargada de remuneraciones: al iniciar su equipo encuentra abierta una aplicación de acceso remoto que no recuerda haber instalado. El registro de VPN muestra una conexión nocturna desde una dirección IP no habitual. En Descargas aparece un archivo llamado `actualizacion.zip`. Un supervisor tomó una fotografía de la pantalla y desconectó el cable de red. Nadie ha autorizado todavía revisar archivos personales ni acceder a la cuenta de correo de Camila.

## Objetivo de la actividad

Reconocer información que podría ayudar en una investigación y proponer una primera acción segura, sin manipular archivos ni acusar a una persona.

## Recordatorio de conceptos

| Concepto | Explicación simple | Ejemplo |
| --- | --- | --- |
| Fuente | Lugar o elemento del que obtenemos información. | El registro de VPN. |
| Dato | Algo concreto que podemos leer u observar. | Una conexión registrada a las 02:15. |
| Indicio | Dato que orienta la investigación, pero todavía necesita confirmación. | La conexión provino de una IP no habitual. |
| Evidencia potencial | Información que podría utilizarse si se conserva y documenta correctamente. | Una copia verificada del registro. |
| Opinión o hipótesis | Explicación que todavía no está demostrada. | “Seguramente fue Camila”. |

Un mismo elemento puede cumplir más de una función según el contexto. Se evaluará la explicación, no memorizar una etiqueta.

## Elementos para revisar

1. Aviso verbal de Camila.
2. Exportación del registro de VPN.
3. Fotografía de la pantalla.
4. Archivo `actualizacion.zip` en el equipo.
5. Hash SHA-256 calculado sobre una copia del archivo.
6. Comentario del supervisor: “seguramente fue Camila”.

## Trabajo paso a paso

1. **Lean el caso y los conceptos**
2. **Ejemplo guiado con el docente**. Clasifiquen juntos el aviso verbal de Camila.
3. **Trabajo en parejas**. Completen los elementos 2 al 6.
4. **Puesta en común**. Comparen dos respuestas y corrijan afirmaciones que excedan los datos.
5. **Ticket de salida individual**.

| Elemento                         | Clasificación propuesta                         | ¿Qué sabemos realmente?                                                                                                         | Primera acción segura                                                                                        |
| -------------------------------- | ----------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| Aviso de Camila - ejemplo guiado | Fuente testimonial / dato inicial               | Camila informó una situación; todavía no conocemos la causa.                                                                    | Registrar sus palabras, fecha y contexto sin alterar el equipo.                                              |
| Registro VPN                     | Fuente digital / indicio / evidencia potencial. | Registra una conexión nocturna desde una IP no habitual; no demuestra quién utilizó la cuenta ni que la conexión sea maliciosa. | Conservar una exportación autorizada y documentar su origen, fecha, hora y zona horaria.                     |
| Fotografía                       | Dato visual / evidencia potencial.              | Muestra lo que aparecía en la pantalla al tomarla; no demuestra quién abrió la aplicación ni qué ocurrió antes.                 | Conservar el archivo original y registrar quién tomó la fotografía, cuándo y en qué contexto.                |
| Archivo ZIP                      | Evidencia potencial.                            | Existe un archivo llamado `actualizacion.zip` en Descargas; todavía no conocemos su origen ni contenido.                        | No abrirlo en el equipo original; documentarlo y obtener, con autorización, una copia de trabajo verificada. |
| Hash SHA-256                     | Dato de verificación.                           | El valor corresponde a una copia concreta; no identifica al autor ni indica si el archivo es seguro.                            | Registrar el algoritmo y el valor junto con la copia para verificarla posteriormente.                        |
| Comentario del supervisor        | Opinión o hipótesis.                            | El supervisor sospecha de Camila, pero no entrega datos que demuestren su responsabilidad.                                      | Registrar el comentario como una opinión y buscar información que lo confirme o descarte.                    |

## Tres preguntas para conversar

1. ¿La fotografía o el registro demuestran quién controlaba la cuenta? ¿Por qué?
   **Respuesta:** No. Muestran una pantalla y una conexión registrada, pero no identifican por sí solos a la persona que realizó las acciones.
2. ¿Qué podría cambiar si abrimos el ZIP directamente en el equipo original?
   **Respuesta:** Podrían modificarse fechas, archivos o registros, e incluso ejecutarse contenido no deseado.
3. ¿Qué acción requeriría autorización adicional?
   **Respuesta:** Revisar archivos personales o acceder a la cuenta de correo de Camila.

Complete tres frases breves:

- Una evidencia digital es información que puede ayudar a responder preguntas de una investigación si se conserva y documenta correctamente.
- Antes de analizar, debemos confirmar la autorización, preservar la información y registrar nuestras acciones.
- Sobre el equipo original evitaría abrir el archivo ZIP porque podría alterar información o ejecutar contenido no deseado.
