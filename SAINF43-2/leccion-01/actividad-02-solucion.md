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

## Misión

Preparen una respuesta inicial que respete el alcance autorizado, proteja la información y separe lo observable de aquello que todavía debe comprobarse.

## 1 - Delimitar el encargo

Completen la ficha usando solamente la información del caso.

| Pregunta de alcance                          | Respuesta                                                                                                                                                    |
| -------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ¿Quién solicita o autoriza el trabajo?       | La jefatura.                                                                                                                                                 |
| ¿Qué equipo y registros están incluidos?     | El computador compartido de recepción, la fotografía de la pantalla, el registro de VPN y los datos relacionados con el caso que se encuentran en el equipo. |
| ¿Qué acciones están expresamente permitidas? | Documentar el estado actual, proteger el equipo y conservar los registros relacionados.                                                                      |
| ¿Qué acciones están fuera del alcance?       | Abrir correos, revisar archivos personales y ejecutar `turnos.zip`.                                                                                          |
| ¿Qué dato importante todavía no se conoce?   | Quién utilizaba físicamente el computador a las 17:55.                                                                                                       |

## 2 - Decidir sin improvisar

Para cada acción, marquen una opción:

- **Realizar:** está autorizada y ayuda a preservar o documentar.
- **Detener y consultar:** requiere confirmar el alcance o una decisión técnica.
- **Evitar:** puede alterar información o contradice los límites indicados.

| Acción propuesta                                                           | Realizar | Detener y consultar | Evitar | Justificación                                                                                                        |
| -------------------------------------------------------------------------- | :------: | :-----------------: | :----: | -------------------------------------------------------------------------------------------------------------------- |
| Registrar la hora, el estado del equipo y quiénes están presentes.         |    X     |                     |        | Está autorizado y documenta el contexto sin intervenir innecesariamente el equipo.                                   |
| Abrir `turnos.zip` directamente en el computador original.                 |          |                     |   X    | No está autorizado y podría alterar información del original o ejecutar contenido no deseado.                        |
| Ingresar al correo de las personas que usaron recepción.                   |          |                     |   X    | El correo está expresamente fuera del alcance autorizado.                                                            |
| Apagar inmediatamente el equipo sin documentar ni consultar.               |          |                     |   X    | Apagarlo podría destruir datos volátiles; primero se debe documentar y escalar la decisión técnica.                  |
| Proteger la fotografía y el registro de VPN para evitar su pérdida.        |    X     |                     |        | Son fuentes relacionadas con el caso y la autorización permite conservarlas.                                         |
| Trabajar sobre una copia cuando llegue el momento de examinar información. |    X     |                     |        | Cuando el examen esté autorizado, una copia de trabajo permite analizar sin intervenir innecesariamente el original. |

> Algunas decisiones pueden admitir más de una respuesta si la justificación respeta la autorización, la preservación y los límites técnicos.

## 3 - Auditar afirmaciones

Clasifiquen cada afirmación como **hecho observable**, **indicio** o **hipótesis no confirmada**. Después expliquen qué parte del caso respalda su decisión.

| Afirmación | Clasificación | ¿Qué la respalda o qué falta comprobar? |
| ---------- | ------------- | ---------------------------------------- |
| La fotografía muestra una aplicación de acceso remoto abierta a las 18:22. | Hecho observable. | La fotografía registra lo que aparecía en la pantalla a esa hora; no demuestra quién abrió la aplicación ni lo ocurrido antes. |
| El registro contiene una conexión VPN exitosa a las 17:55. | Hecho observable. | La exportación del registro muestra esa conexión; todavía se debe confirmar su contexto y quién utilizó la cuenta o el equipo. |
| La IP no habitual identifica a la persona que realizó la conexión. | Hipótesis no confirmada. | La IP orienta la investigación, pero no identifica físicamente a una persona por sí sola. |
| `turnos.zip` contiene información sustraída. | Hipótesis no confirmada. | Solo sabemos que existe un archivo con ese nombre; su contenido y origen no han sido examinados. |
| La persona del turno de la tarde fue responsable. | Hipótesis no confirmada. | Es un comentario sin datos que demuestren autoría; tampoco se sabe quién utilizaba el computador a las 17:55. |
| La conexión y el archivo justifican continuar la investigación. | Indicio. | Ambos datos orientan la investigación, pero necesitan confirmación y corroboración antes de formular una conclusión. |

## Cierre

Respondan en una oración cada pregunta:

1. ¿Por qué tener acceso técnico no equivale a tener autorización?  
   **Respuesta:** Porque la autorización define quién puede actuar, sobre qué sistemas y datos, y con qué límites; la capacidad técnica de acceder no concede ese permiso.
2. ¿Por qué una dirección IP no demuestra por sí sola quién realizó una acción?  
   **Respuesta:** Porque registra un origen de conexión, pero no identifica físicamente a la persona que utilizó el equipo o la cuenta.
3. ¿Por qué preservar no significa necesariamente apagar el equipo?  
   **Respuesta:** Porque apagarlo puede destruir datos volátiles; primero se debe documentar el estado y escalar la decisión técnica según el contexto.
