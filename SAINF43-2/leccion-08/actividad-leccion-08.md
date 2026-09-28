---
title: "Actividad Lección 08 - Reconstrucción de sesiones en una captura de red"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "08"
author: Jordy
start: 2026-09-28
end: 2026-09-30
created_at: 2026-09-26
aliases:
  - "Actividad Lección 08 - Reconstrucción de sesiones en una captura de red"
---

# Actividad Lección 08 - Reconstrucción de sesiones en una captura de red

**Duración total:** 80 minutos.
**Modalidad:** parejas.
**Caso único:** `RED-CASO02` - MUELLE SUR LTDA (ficticia).
**Fuente:** captura `RED-CASO02.pcap` entregada por el docente y compartida con Kali en modo de solo lectura.
**Carácter:** formativo.

## Pregunta del caso

El área de TI de MUELLE SUR informó que el equipo `PC-CONTAB-07` descargó un archivo desde un dominio que no figura en el catálogo de proveedores. Se solicitó un intervalo de diez minutos al sensor del puerto SPAN del switch de la oficina; la ventana entre el primer y el último paquete observado puede ser menor.

> ¿Qué comunicaciones de `PC-CONTAB-07` (`10.8.20.45`) pueden reconstruirse desde la captura, qué objeto se transfirió y qué patrón de conexiones aparece después?

No se busca demostrar que el equipo fue comprometido ni atribuir las acciones a una persona. Se busca reconstruir hechos observables en la red, citarlos por número de paquete y formular inferencias limitadas.

## Resultado esperado

Cada pareja entrega una bitácora con:

1. control de la evidencia y del entorno;
2. panorama de la captura;
3. cuatro filtros reproducibles con su resultado;
4. tabla de ocho hitos DNS, TCP y HTTP de la descarga;
5. objeto extraído con su SHA-256;
6. serie de conexiones TLS posteriores con sus intervalos;
7. tres indicadores contextualizados;
8. conclusión breve con nivel de confianza y limitaciones.

La captura contiene tráfico de otros equipos y más detalles de los que se piden. Se evalúa la calidad de la selección y del razonamiento, no la cantidad de filas.

## Fuente de trabajo

Usa únicamente la captura entregada para esta lección:

```text
/media/sf_LAB08_ENTREGA/evidencia/RED-CASO02.pcap
```

La práctica retoma la estación Kali de la Lección 03 y el esquema de carpetas compartidas de la Lección 07: `LAB08_ENTREGA` en solo lectura y `LAB08_SALIDA` con escritura. No copies la captura dentro de Kali ni la abras desde el pendrive.

La **guía práctica de comandos de la Lección 08** contiene la secuencia exacta de cada fase. Todos los comandos usan `-n`, que desactiva la resolución de nombres: la captura se analiza sin generar tráfico nuevo y los nombres que aparezcan provienen de los paquetes, no de una consulta hecha hoy.

## Herramientas y roles

- `capinfos` y `tshark`: verificar, resumir, filtrar y exportar.
- Wireshark (interfaz gráfica): opcional, para ver lo mismo que `tshark`. Si lo usas, desactiva la resolución de nombres y registra el filtro exacto que aplicaste.
- Integrante 1: operador.
- Integrante 2: registrador y verificador.

Cambien los roles al terminar la Fase 3.

## Fase 1 - Verificar y preparar

1. Comprueba con la prueba de escritura de la guía que `LAB08_ENTREGA` rechaza la escritura y que `LAB08_SALIDA` la admite. Kali puede mostrar ambas carpetas como `rw` aunque VirtualBox tenga marcada la opción Solo lectura; lo que vale es el resultado de la prueba.
2. Comprueba la captura con el manifiesto `RED-CASO02.sha256`.
3. Registra la versión de `tshark`.
4. Lee `RED-CASO02-caso.env` y `RED-CASO02-captura.txt`. Registra la IP investigada, la zona horaria del caso, el punto de captura y dos limitaciones declaradas por TI.

| Control | Resultado |
| --- | --- |
| `LAB08_ENTREGA` rechaza la escritura | [Sí / No] |
| SHA-256 de `RED-CASO02.pcap` | [OK / discrepancia] |
| Versión de `tshark` | [Completar] |
| IP y equipo investigados | [Completar] |
| Zona horaria del caso | [Completar] |
| Punto de captura | [Completar] |

Si la verificación falla, detente e informa. No continúes con otra captura.

## Fase 2 - Panorama de la captura

Antes de buscar lo interesante, describe lo que contiene la captura.

1. Con `capinfos`, registra la cantidad de paquetes, el primer y el último paquete en UTC y la duración.
2. Con la jerarquía de protocolos, identifica qué protocolos aparecen.
3. Con las conversaciones IPv4, identifica todas las conversaciones de `10.8.20.45` y la conversación externa con más paquetes.

| Dato                                  | Registro |
| ------------------------------------- | -------- |
| Paquetes                              | [ ]      |
| Primer paquete (UTC y hora local)     | [ ]      |
| Último paquete (UTC y hora local)     | [ ]      |
| Protocolos presentes                  | [ ]      |
| Conversaciones de `10.8.20.45`        | [ ]      |
| Conversación externa con más paquetes | [ ]      |

## Fase 3 - Filtros reproducibles

Aplica estos cuatro filtros de visualización y registra el filtro exacto y la cantidad de paquetes que muestra:

|   # | Pregunta                                                       | Filtro usado | Paquetes |
| --: | -------------------------------------------------------------- | ------------ | -------: |
|   1 | ¿Qué respuestas DNS recibió el equipo investigado?             | [ ]          |      [ ] |
|   2 | ¿Qué solicitudes HTTP aparecen en la captura?                  | [ ]          |      [ ] |
|   3 | ¿Qué respuestas HTTP indican un error?                         | [ ]          |      [ ] |
|   4 | ¿Qué mensajes `Client Hello` (inicio del handshake TLS) envió el equipo investigado? | [ ]          |      [ ] |

Un filtro es reproducible cuando otra pareja puede escribirlo tal cual, sobre la misma captura, y obtener la misma cantidad de paquetes.

## Fase 4 - Reconstruir la sesión HTTP

Con los filtros 1 y 2 ubica la descarga del equipo investigado. Identifica su `tcp.stream` y selecciona ocho hitos de la secuencia: resolución del nombre, apertura de la conexión, solicitudes y respuestas. La tabla resume hitos; no representa todos los paquetes del stream TCP.

Completa las ocho filas en orden temporal. La hora local se obtiene convirtiendo la hora UTC con la zona del caso.

| Fila | Paso | Paquete | Hora UTC | Hora local | Origen:puerto | Destino:puerto | Protocolo | Dato observado |
| ---: | --- | ---: | --- | --- | --- | --- | --- | --- |
| 1 | Consulta DNS | [ ] | [ ] | [ ] | [ ] | [ ] | DNS | [Nombre consultado] |
| 2 | Respuesta DNS | [ ] | [ ] | [ ] | [ ] | [ ] | DNS | [Dirección obtenida] |
| 3 | Apertura TCP (`SYN`) | [ ] | [ ] | [ ] | [ ] | [ ] | TCP | [ ] |
| 4 | Aceptación (`SYN, ACK`) | [ ] | [ ] | [ ] | [ ] | [ ] | TCP | [ ] |
| 5 | Primera solicitud | [ ] | [ ] | [ ] | [ ] | [ ] | HTTP | [Método, recurso y `User-Agent`] |
| 6 | Primera respuesta | [ ] | [ ] | [ ] | [ ] | [ ] | HTTP | [Código, `Content-Type` y `Content-Length`] |
| 7 | Segunda solicitud | [ ] | [ ] | [ ] | [ ] | [ ] | HTTP | [Método y recurso] |
| 8 | Segunda respuesta | [ ] | [ ] | [ ] | [ ] | [ ] | HTTP | [Código] |

Responde además:

```text
tcp.stream de la sesión:
¿Qué indica el User-Agent y por qué no basta para afirmar qué programa lo envió?
¿Qué nombre de archivo declara el servidor y qué tipo de contenido declara?
```

## Fase 5 - Extraer el objeto

1. Exporta los objetos HTTP a `~/forense/LAB-08/exportados/http/`, nunca a la carpeta de evidencia.
2. Identifica cuál de los archivos exportados corresponde a la primera respuesta de la Fase 4. Los demás pertenecen a otras respuestas: explica de cuál proviene cada uno.
3. Calcula el SHA-256 del objeto y compara su tamaño con el `Content-Length` registrado.

| Dato | Registro |
| --- | --- |
| Nombre del objeto | [ ] |
| Paquete de la respuesta que lo contiene | [ ] |
| Tamaño en bytes y `Content-Length` | [ ] |
| SHA-256 | [ ] |
| Otros objetos exportados y su origen | [ ] |

**No abras el archivo exportado con una aplicación asociada ni lo ejecutes.** Puedes leer su contenido tal como aparece en el stream TCP de la captura (Follow TCP Stream), que muestra los bytes transmitidos como texto. Registra en una línea qué describe el contenido.

## Fase 6 - Serie de conexiones TLS

Después de la descarga, el equipo investigado abre varias conexiones cifradas. El contenido no puede leerse, pero el inicio del handshake TLS, el mensaje `Client Hello`, viaja en claro.

1. Con el filtro 4, lista los `Client Hello` del equipo investigado con su hora, `tcp.stream` y nombre de servidor (SNI).
2. Calcula el intervalo entre `Client Hello` consecutivos.

| Paquete | Hora UTC | `tcp.stream` | Destino | SNI | Intervalo con el anterior (s) |
| ---: | --- | ---: | --- | --- | ---: |
| [ ] | [ ] | [ ] | [ ] | [ ] | - |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |

Construye una correlación entre la descarga y la serie TLS:

```text
Evidencia A:
Evidencia B:
Campo o dato de unión:
Relación observada:
Inferencia:
Confianza:
Explicación alternativa o dato faltante:
```

## Fase 7 - Indicadores y conclusión

Selecciona tres indicadores observados en la captura. Cada uno debe incluir su fuente, la ventana en que aparece, por qué interesa y un posible uso legítimo o falso positivo.

| Indicador | Tipo | Paquetes | Ventana UTC | Por qué interesa | Posible uso legítimo o falso positivo |
| --- | --- | --- | --- | --- | --- |
| [ ] | [Dominio / IP / URL / hash / User-Agent] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |

Escribe los dominios y URL en forma neutralizada (*defang*), por ejemplo `ejemplo[.]invalid` y `hxxp://`, para que nadie los abra por accidente al leer la bitácora.

Termina con una conclusión de 80 a 100 palabras. Debe quedar claro:

- qué secuencia está mejor sustentada y con qué paquetes;
- qué objeto se transfirió y cómo lo identificaste;
- qué relación propones entre la descarga y la serie TLS, con qué confianza;
- qué no puede afirmarse solo con la captura y qué fuente te faltaría.

## Ampliación

Solo si el recorrido obligatorio está cerrado, elige **una**:

- revisar la respuesta DNS de `wpad` y explicar por qué no es un indicador del caso;
- identificar el fabricante asociado a la MAC del equipo investigado y explicar qué aporta y qué no demuestra;
- localizar el paquete marcado por `tcp.analysis.flags` y explicar qué significa;
- comparar la conexión TLS de `PC-VENTAS-02` con la serie de `PC-CONTAB-07` y explicar por qué una conexión TLS a un dominio desconocido no es sospechosa por sí sola.

## Entrega

Entrega una bitácora única en PDF o Markdown, a partir de la plantilla de la lección. Debe incluir capturas de pantalla o salidas breves que respalden cada tabla. No copies salidas completas de `tshark`: cita los paquetes.

Al terminar, copia la bitácora a `/media/sf_LAB08_SALIDA/` y comprueba que ambas copias coincidan. Los PC del laboratorio se restablecen al reiniciarse: lo que quede en el disco del anfitrión, incluida la carpeta `LAB08_SALIDA`, se pierde al apagarlo. Por eso, antes de apagar, entrega o respalda `bitacora-equipo.md` desde Windows por el medio que indique el docente (aula virtual, correo o pendrive). No entregues la captura ni los objetos exportados.
