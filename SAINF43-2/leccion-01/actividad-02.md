---
title: Actividad 02 - Decidir antes de intervenir
tags:
  - nota
  - course
  - curso
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "01"
author: Jordy
start: 2026-08-12
end: 2026-08-12
created_at: 2026-08-12
aliases:
  - Actividad 02 - Decidir antes de intervenir
---
# Actividad 02 - Decidir antes de intervenir

**Duración:** 50 minutos.  
**Modalidad:** trabajo guiado en parejas.  
**Carácter:** formativo, sin calificación.

## Caso: equipo compartido en recepción

A las 18:20, el encargado de seguridad informa que el computador de recepción muestra abierta una aplicación de acceso remoto. El equipo es utilizado por personas de distintos turnos y permanece encendido.

Soporte entrega los siguientes antecedentes:

- Una fotografía de la pantalla tomada a las 18:22.
- Una exportación del registro de VPN que muestra una conexión exitosa a las 17:55 desde una dirección IP no habitual.
- La presencia de un archivo llamado `turnos.zip` en la carpeta Descargas.
- El comentario de un trabajador: “la persona del turno de la tarde copió información”.

La jefatura autoriza documentar el estado actual, proteger el equipo y conservar los registros relacionados. La autorización no incluye abrir correos, revisar archivos personales ni ejecutar el archivo ZIP. Todavía no se ha determinado quién utilizaba físicamente el computador a las 17:55.

## Objetivo de la actividad

Preparen una respuesta inicial que respete el alcance autorizado, proteja la información y separe lo observable de aquello que todavía debe comprobarse.

## 1 - Delimitar el encargo

Completen la ficha usando solamente la información del caso.

| Pregunta de alcance                          | Respuesta |
| -------------------------------------------- | --------- |
| ¿Quién solicita o autoriza el trabajo?       |           |
| ¿Qué equipo y registros están incluidos?     |           |
| ¿Qué acciones están expresamente permitidas? |           |
| ¿Qué acciones están fuera del alcance?       |           |
| ¿Qué dato importante todavía no se conoce?   |           |

## 2 - Decidir sin improvisar

Para cada acción, marquen una opción:

- **Realizar:** está autorizada y ayuda a preservar o documentar.
- **Detener y consultar:** requiere confirmar el alcance o una decisión técnica.
- **Evitar:** puede alterar información o contradice los límites indicados.

| Acción propuesta                                                           | Realizar | Detener y consultar | Evitar |
| -------------------------------------------------------------------------- | :------: | :-----------------: | ------ |
| Registrar la hora, el estado del equipo y quiénes están presentes.         |          |                     |        |
| Abrir `turnos.zip` directamente en el computador original.                 |          |                     |        |
| Ingresar al correo de las personas que usaron recepción.                   |          |                     |        |
| Apagar inmediatamente el equipo sin documentar ni consultar.               |          |                     |        |
| Proteger la fotografía y el registro de VPN para evitar su pérdida.        |          |                     |        |
| Trabajar sobre una copia cuando llegue el momento de examinar información. |          |                     |        |

> Algunas decisiones pueden admitir más de una respuesta si la justificación respeta la autorización, la preservación y los límites técnicos.

## 3 - Auditar afirmaciones

Clasifiquen cada afirmación como **hecho observable**, **indicio** o **hipótesis no confirmada**. Después expliquen qué parte del caso respalda su decisión.

| Afirmación                                                                 | Clasificación | ¿Qué la respalda o qué falta comprobar? |
| -------------------------------------------------------------------------- | ------------- | --------------------------------------- |
| La fotografía muestra una aplicación de acceso remoto abierta a las 18:22. |               |                                         |
| El registro contiene una conexión VPN exitosa a las 17:55.                 |               |                                         |
| La IP no habitual identifica a la persona que realizó la conexión.         |               |                                         |
| `turnos.zip` contiene información sustraída.                               |               |                                         |
| La persona del turno de la tarde fue responsable.                          |               |                                         |
| La conexión y el archivo justifican continuar la investigación.            |               |                                         |

## Cierre

Respondan en una oración cada pregunta:

1. ¿Por qué tener acceso técnico no equivale a tener autorización?
2. ¿Por qué una dirección IP no demuestra por sí sola quién realizó una acción?
3. ¿Por qué preservar no significa necesariamente apagar el equipo?
