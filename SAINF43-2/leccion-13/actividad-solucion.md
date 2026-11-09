---
title: "Solución - Práctica guiada 13 - Análisis estático y recuperación autorizada"
tags:
  - nota
  - course
  - curso
  - docente
  - reservado
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "13"
author: Jordy
start: 2026-11-03
end: 2026-11-09
created_at: 2026-10-29
aliases:
  - "Solución - Práctica guiada 13 - Análisis estático y recuperación autorizada"
---

# Solución - Actividad Lección 13

**Uso reservado para el equipo docente.**
**Actividad:** [[actividad|Práctica guiada 13]].
**Pauta:** [[pauta-docente|Pauta docente]].
**Material maestro:** [contingencia/salidas-referencia.txt](contingencia/salidas-referencia.txt), generado por `construir_entrega_l13.sh`.

Las respuestas pueden variar en su redacción. Deben conservar estas decisiones: no ejecutar la muestra, determinar el tipo por firma, clasificar cada indicador con un falso positivo, separar capacidad de comportamiento, recuperar solo con la lista autorizada y verificar el texto por hash.

## Datos de control

Valores estables del caso `AR-CASO06` (la muestra y el texto recuperado son deterministas; el SHA-256 del contenedor cambia en cada reconstrucción por el cifrado ZipCrypto y siempre coincide con el manifiesto empaquetado).

| Elemento | Valor |
| --- | --- |
| `muestra/actualizacion-seguridad.pdf` SHA-256 | `8ee46b6503782a6714cd1bf7959d979f6004f6bd216be8fe50debf85f768a352` |
| Tamaño de la muestra | 794 bytes |
| Magic bytes | `4d 5a` (`MZ`) |
| Tipo real (`file`) | `PE32 executable for Unknown subsystem 0, Intel i386, 3 sections` |
| Contraseña correcta del contenedor | `andes-2026` (segundo candidato de la lista) |
| `texto-control.txt` recuperado SHA-256 | `647cc5107f8950537c5eeda5b7c2359d6586690a9211f17045736c23ec42e477` |

Publicar al alumnado, para la verificación de la recuperación, el SHA-256 del texto de control: `647cc510...e477`.

## 1. Preparar el caso y verificar la integridad

- `findmnt` muestra `ro` entre las opciones de `LAB13_ENTREGA`.
- `00_control/verificacion-recibidos.txt` contiene **dos** líneas `OK`:

```text
actualizacion-seguridad.pdf: OK
AR-CASO06-contenedor.zip: OK
```

- Sin `--ignore-missing`, cada comando agrega una línea `FAILED open or read` por el archivo que no está en esa carpeta. No es una falla del material; ver la pauta.
- `00_control/copia.txt` muestra el mismo SHA-256 para la muestra recibida y para la copia.
- Existe `00_control/ficha-l13.md`, que es la entrega.

```text
¿Qué demuestra y qué no?: la copia tiene exactamente los mismos bytes que la muestra recibida; no demuestra completitud, origen auténtico ni intención.
¿Por qué se analiza la copia?: para no alterar la muestra recibida y poder repetir el examen.
```

## 2. Caracterizar la muestra sin ejecutarla

### 2.1 Tipo real frente a la extensión

| Nombre recibido | Tipo real (por firma) | Magic bytes | Hallazgo observable |
| --- | --- | --- | --- |
| `actualizacion-seguridad.pdf` | PE32 executable (Intel i386) | `4d 5a` (`MZ`) | La extensión dice PDF, pero la firma es de un ejecutable de Windows: discrepancia de tipo |

Clave: la discrepancia se registra como hecho, no como prueba de malware. Sube la prioridad del análisis.

### 2.2 Tamaño y metadatos

- `stat`, con el formato `%n %s %y`, imprime el nombre, el tamaño (794 bytes) y la fecha de modificación. Gracias a `--preserve=timestamps`, la fecha de modificación es la del original, no la de la copia.
- Respuesta esperada en una línea: `stat` muestra nombre, tamaño y fecha de modificación del sistema de archivos; autor, firma digital y fecha de compilación **no se examinaron** con estas herramientas.
- No aceptar «no tiene firma digital» ni «no tiene autor»: `stat` no lee esos campos. Que un «parche de seguridad» no tenga firma sería una señal de alerta, pero aquí no se comprobó.

### 2.3 Cadenas e indicadores (clasificación esperada)

| Cadena o nombre | Qué sugiere | Clasificación | Falso positivo posible |
| --- | --- | --- | --- |
| `HKCU\Software\Microsoft\Windows\CurrentVersion\Run` | Ubicación real de arranque automático: persistencia | **Fuerte** | Software legítimo también se registra ahí para iniciar con la sesión |
| `c2-telemetria.andes-soft.invalid` | Dominio externo de telemetría/control | **Fuerte** | Podría ser un señuelo nunca contactado, o telemetría legítima del proveedor |
| `Global\ASUpd_7731` | *Mutex* que evita doble ejecución | **Fuerte / contextual** | Muchos instaladores usan *mutex* |
| `%APPDATA%\andes\cache.dat` | Ruta de escritura en perfil de usuario | **Contextual** | Rutas de caché son comunes |
| `WinHttpOpen`, `WinHttpConnect` | Nombres de funciones de red | **Contextual** | Muchos programas consultan servidores; además, el texto no demuestra que se importen ni se usen |
| `RegSetValueExW` | Nombre de función de escritura en el registro | **Contextual** | Configuración legítima también escribe el registro |
| `CreateFileW`, `CreateProcessW` | Nombres de funciones de archivos y procesos | **Contextual** | APIs ubicuas |
| `kernel32.dll` | Biblioteca base de Windows | **Débil** | Presente en casi todo PE |
| `Microsoft Corporation` | Cadena de marca | **Débil / señuelo** | Puede incrustarse para parecer legítimo |
| `Update complete`, `FARO SECURITY UPDATE PACKAGE 3.2` | Mensajes de apariencia benigna | **Débil / señuelo** | Texto decorativo |

Se exigen cuatro clasificaciones con falso positivo, al menos una de cada clase; esta tabla ofrece el conjunto completo. El sufijo `.invalid` es una convención del laboratorio: no se acepta como argumento que «no existe, así que es señuelo».

### 2.4 Estructura y segunda herramienta

- `strings -n 4 ... | grep -oE '\.(text|data|rsrc)'` muestra `.text`, `.data` y `.rsrc`. Con la versión anterior (`grep -E '^\.…'`) solo aparecía `.text`, porque `strings` pega un byte vecino a las otras dos (`` `.data``, `` `.rsrc``).
- `od -An -c` muestra la firma `MZ` al inicio.
- Respuesta esperada: es un PE con tres secciones (código, datos y recursos). `file` resume el tipo, `strings` extrae texto y `od` muestra bytes crudos; coinciden en que es un PE, con distinto nivel de detalle.

### 2.5 Mapeo y límites

La fila de la clave de autoarranque viene resuelta como ejemplo. Se aceptan dos filas, siempre que la técnica se elija desde `notas/referencia-attack.txt` y se justifique con un indicador observado:

| Indicador | Técnica ATT&CK compatible | Redacción acotada |
| --- | --- | --- |
| Dominio externo + nombres `WinHttp*` | Command and Control (TA0011), T1071.001 Web Protocols | "Contiene un dominio externo y nombres de funciones de red; su importación, su uso y el contacto efectivo no se verificaron." |
| Extensión `.pdf` en un PE | Defense Evasion, T1036.008 Masquerade File Type | "El nombre presenta como PDF un archivo con firma de ejecutable, compatible con enmascaramiento del tipo." |

No aceptar T1027 (ofuscación) ni T1105 (descarga de herramientas): están en la ficha como distractores y ningún indicador observado los respalda; las cadenas están en claro.

```text
Hipótesis principal: podría establecer persistencia mediante la clave de autoarranque y comunicarse con un dominio externo; ambas cosas son indicios, no capacidades verificadas.
Explicación benigna alternativa: un instalador legítimo mal nombrado usa las mismas APIs; el dominio podría ser telemetría legítima o un señuelo.
Afirmación no sostenible: que la persistencia se instaló, que hubo conexión o que se exfiltraron datos.
```

## 3. Recuperación autorizada del contenedor

- El contenedor termina en `OK` contra el manifiesto.
- La lista autorizada tiene cinco candidatos; el que recupera es **`andes-2026`** (segundo de la lista). El primer intento (`respaldo-andes`) responde `incorrect password`; el segundo (`andes-2026`) extrae el archivo y ahí se detiene la prueba. La ficha registra ambos candidatos en orden, y `recuperacion.txt`, la hora de inicio, la de término y el hash.
- `texto-control.txt` se extrae en `02_salidas/recuperado/` y su SHA-256 es `647cc510...e477`, que **coincide** con el valor publicado.

| Campo | Contenido esperado |
| --- | --- |
| Herramienta y versión | `unzip` (versión del sistema) |
| Lista utilizada | `candidatos-autorizados.txt`, sin ampliarla |
| Inicio y término | Horas registradas en `recuperacion.txt` (uno o dos minutos de diferencia) |
| Candidatos probados y candidato que recuperó | `respaldo-andes` (sin éxito), `andes-2026` (correcto) |
| SHA-256 del texto de control | `647cc5107f8950537c5eeda5b7c2359d6586690a9211f17045736c23ec42e477` |
| ¿Coincide con el publicado? | Sí |

```text
¿Qué demuestra y qué no?: acceso al contenido del contenedor; no autoría, intención ni licitud del contenido.
¿Por qué no probar otros contenedores?: la autorización cubre solo este contenedor y esta lista; salir de ahí es acceso no autorizado.
```

Si una pareja termina la lista sin acierto por un error de tipeo, se corrige y se reintenta **con la misma lista**; agotarla de verdad se reporta como resultado válido, no como permiso para ampliar.

## 4. Veredicto provisional y respaldo

| Campo | Contenido esperado |
| --- | --- |
| Resultado observado | PE32 con extensión `.pdf`, con indicadores de persistencia y red; contenedor recuperado con contenido verificado por hash. |
| Interpretación | La muestra contiene nombres de funciones asociados a red y registro, y el texto de una ubicación de autoarranque. No se verificó que importe esas funciones, que implemente esas capacidades ni que se ejecuten. |
| Confianza | Media: varios indicadores fuertes coinciden, pero nada se ejecutó. |
| Limitación | El análisis estático no demuestra ejecución, conexión ni exfiltración. |
| Prueba dinámica recomendada | VM desechable con *snapshot* y red simulada; monitorear procesos, archivos, registro y red; detener ante comportamiento destructivo, intento de propagación o tiempo límite; restaurar el *snapshot* tras exportar los registros. **No se ejecuta en esta clase.** |

La explicación alternativa ya quedó en el paso 2.5; no se repite en el veredicto.

El respaldo contiene `00_control` (con la ficha y las verificaciones), `02_salidas` y `03_anexos`, y la verificación de `hashes-salidas.txt` termina en `OK`.

## Señales de alerta para corregir

- Cualquier intento de ejecutar o abrir la muestra, o de subirla a un servicio en línea.
- Tratar la extensión o una cadena aislada como veredicto de malware.
- Afirmar comportamiento ("se conectó", "robó") a partir de capacidad.
- Ampliar la lista de candidatos o mencionar diccionarios externos.
- Entregar la ficha sin las salidas guardadas ni sus hashes.
