---
title: "Práctica guiada 15 - Investigación de correo y web con informe revisado"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "15"
author: Jordy
start: 2026-11-17
end: 2026-11-23
created_at: 2026-11-15
aliases:
  - "Práctica guiada 15 - Investigación de correo y web con informe revisado"
---

# Actividad Lección 15 - Investigación de correo y web

| Identificación | Completar |
| --- | --- |
| Integrantes y roles iniciales | [Completar] |
| Sección y fecha | [Completar] |

- **Modalidad:** parejas; actividad formativa.
- **Duración:** 100 minutos
- **Entrega:** este DOCX completado, revisado y corregido; un archivo por pareja.

## El caso

Investiguen el correo recibido por `mrojas` y el uso posterior de su cuenta en el caso `AR-CASO06` de ANDES SOFT SPA. La jefatura pide determinar si hubo suplantación, qué muestran los registros sobre el enlace, qué alcance tuvo el acceso al portal y qué indicadores permiten buscar o bloquear la actividad.

Lean `caso/solicitud-jefatura.txt`, `caso/autorizacion.txt` y `notas/ficha-tecnica.txt`. Trabajen en Kali sobre copias verificadas, sin visitar los enlaces ni ejecutar el adjunto. Si un hash falla, avisen al docente.

## 1. Preparar y verificar

Ejecuten el **paso 1 de la guía de comandos**: preparar carpetas, comprobar que `LAB15_ENTREGA` está en solo lectura y verificar las cinco exportaciones y sus copias. Guarden las salidas en `02_salidas/`. Los comandos de los pasos siguientes también están en esa guía.

| Control | Resultado |
| --- | --- |
| ¿Originales y copias muestran cinco `OK`? | [Sí / No] |
| Una acción que excluye la autorización | [Completar] |

## 2. Correo y adjunto

Con el **paso 2 de la guía de comandos**, comparen los dos EML y extraigan el adjunto sin abrirlo. Respondan brevemente y citen archivo y línea; para la extracción, citen la salida y su hash.

| Elemento | Resultado | Fuente |
| --- | --- | --- |
| `From` frente a `Return-Path` en ambos correos | [Completar] | [Completar] |
| Servidor e IP que entregaron `correo-02` al servidor propio | [Completar] | [Completar] |
| Autenticación: qué cabecera es confiable y por qué SPF puede aprobar mientras DMARC falla | [Completar] | [Completar] |
| Enlace: texto visible frente a destino real | [Completar] | [Completar] |
| Adjunto: tipo real y coincidencia del SHA-256 con L13 | [Completar] | [Completar] |

## 3. Navegación

Revisen el proxy con el **paso 3 de la guía de comandos**. Registren en la cronología del paso 5 la cuenta, el equipo, el clic y el `POST`, en UTC. En H-02 distingan lo que prueba ese registro de lo que declara la persona en la solicitud de jefatura.

Si aparece otra cuenta en el sitio del enlace, indiquen su identificador y la acción que exige la autorización:

| Cuenta y acción correspondiente | Fuente |
| --- | --- |
| [Completar] | [Completar] |

## 4. Portal

Intercambien roles. Usen el **paso 4 de la guía de comandos** y contrasten `portal-auth.jsonl`, `portal-access.log`, la ficha técnica y `documentos-portal.csv`.

| Aspecto | Resultado y fuente |
| --- | --- |
| Cuenta, sesión y MFA; IP de acceso frente a la salida pública de la oficina | [Completar] |
| Documentos distintos entregados y cuántos no estaban autorizados; áreas y clasificación | [Completar] |
| Intentos con `../` y `%27`: códigos y qué permiten concluir | [Completar] |
| Intentos de acceso después del restablecimiento de la clave | [Completar] |

## 5. Cronología e indicadores

Completen seis hitos en **UTC**, con archivo y línea. En clic y `POST`, incluyan cuenta y equipo. Una fila describe hechos, sin atribuir intenciones.

| UTC | Hecho | Fuente y línea |
| --- | --- | --- |
| [Completar] | Recepción del correo sospechoso | [Completar] |
| [Completar] | Clic en el enlace | [Completar] |
| [Completar] | Envío `POST` | [Completar] |
| [Completar] | Inicio de sesión en el portal | [Completar] |
| [Completar] | Descarga de documentos | [Completar] |
| [Completar] | Restablecimiento de clave y revocación | [Completar] |

Seleccionen **tres IoC** con contexto y acción concreta:

| Indicador | Fuente y contexto | ¿Buscar o bloquear? |
| --- | --- | --- |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |

## 6. Hallazgos y conclusión

Calculen los hashes de las salidas con el **paso 6 de la guía de comandos**. Redacten tres hallazgos: H-01, dictamen del correo; H-02, navegación y límite sobre el contenido enviado; H-03, acceso al portal y su alcance. Sustenten la relación entre eventos sin atribuir una persona a una IP.

| ID | Afirmación | Evidencia (archivo/línea o salida/hash) | Confianza | Limitación |
| --- | --- | --- | --- | --- |
| H-01 | [Completar] | [Completar] | [Alta / Media / Baja] | [Completar] |
| H-02 | [Completar] | [Completar] | [Completar] | [Completar] |
| H-03 | [Completar] | [Completar] | [Completar] | [Completar] |

Concluyan en **cuatro oraciones como máximo**: qué ocurrió, qué no puede afirmarse y las dos recomendaciones más urgentes.

| Conclusión |
| --- |
| [Completar] |

## 7. Revisión y defensa

Intercambien el documento con otra pareja. **Pareja revisora:** [Completar]. Puntúen cada criterio: 0 = ausente, 1 = parcial, 2 = logrado.

| Criterio | 0-2 | Observación breve |
| --- | --- | --- |
| Autorización e integridad | [Completar] | [Completar] |
| Correo y autenticación confiable | [Completar] | [Completar] |
| Portal: cuenta, permisos e intento frente a éxito | [Completar] | [Completar] |
| Cronología UTC con fuentes | [Completar] | [Completar] |
| Hallazgos sustentados, límites y conclusión | [Completar] | [Completar] |

Defiendan el informe durante **tres minutos**. La revisora elige dos preguntas: ¿por qué confían en esa cabecera?, ¿qué demuestra el `POST`?, ¿cómo prueban el acceso a documentos no autorizados? Respondan solo con lo escrito en el informe y corrijan una debilidad.

| Debilidad señalada | Corrección realizada |
| --- | --- |
| [Completar] | [Completar] |

Respalden las salidas y verifiquen el manifiesto con el **paso 7 de la guía de comandos**. Conserven ese respaldo y entreguen únicamente `actividad-leccion-15.docx`. Si la clase se divide en dos sesiones, guarden el DOCX al terminar el paso 2 y usen «Corte entre sesiones» de la guía para respaldar el avance.
