---
title: "Actividad Lección 04 - Adquisición del pendrive EV-01"
tags:
  - nota
  - course
  - curso
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "04"
author: Jordy
start: 2026-08-31
end: 2026-09-02
created_at: 2026-08-29
aliases:
  - "Actividad Lección 04 - Adquisición del pendrive EV-01"
---

# Actividad Lección 04 - Adquisición del pendrive EV-01

**Duración práctica:** 80 minutos.  
**Modalidad:** parejas con cambio de roles.  
**Caso:** `PCL-2026-018` - PUERTO CLARO SPA.  
**Evidencia:** `EV-01`, pendrive USB rotulado «RESPALDO ADMIN».  
**Rol que asumes:** perito de adquisición del laboratorio.

## El caso

En PUERTO CLARO SPA, entre las 01:00 y las 05:00 hubo accesos remotos al servidor administrativo y quedó la sospecha de una planilla extraída.

Un pendrive USB estuvo conectado a un equipo del sector en ese mismo período. La jefatura lo selló y lo derivó al laboratorio el 26 de agosto, con un rótulo manuscrito -«RESPALDO ADMIN»- y sin documentación técnica: se desconoce su capacidad, formato y contenido. Nadie lo ha conectado desde entonces.

La gerencia autorizó **adquirir y preservar**. No autorizó examinar el contenido: eso es una etapa posterior, con otro encargo y probablemente otro perito.

## Tu encargo de hoy

Actúas como perito de adquisición. Al cerrar el bloque debes entregar:

1. una imagen forense del pendrive completo, creada por ustedes;
2. la prueba de que el original no se modificó;
3. una copia de trabajo verificable para quien analice;
4. una bitácora que permita a un tercero repetir el procedimiento.

Criterio de suficiencia: otro perito que no estuvo en la sala debe poder establecer, leyendo tu bitácora, qué dispositivo adquiriste, en qué estado estaba y por qué la copia es fiel.

```text
identificar -> verificar fuente -> proteger -> adquirir
-> verificar E01 -> comparar -> duplicar -> documentar
```

Cada flecha es un punto donde la evidencia podría alterarse sin que nadie lo note. El procedimiento existe para que, si ocurre, quede registrado.

| Producto                                                                 | Evidencia |
| ------------------------------------------------------------------------ | --------- |
| Identificación y control de entrega                                      | E1        |
| Prueba de que el dispositivo estaba en solo lectura y adquisición propia | E2        |
| Verificación EWF y comparación RAW/`ewf1` en los tres algoritmos         | E3        |
| Copia de trabajo idéntica a los contenedores preservados                 | E4        |
| Matriz de interpretación, conclusión y cierre                            | E5        |

## Lo que hoy no corresponde hacer

| No harás | Por qué |
| --- | --- |
| Abrir, leer o listar los archivos del pendrive | La autorización cubre adquisición y preservación, no examen de contenido. |
| Montar la fuente en modo de escritura | Una escritura sobre el original no es reversible y destruye su valor probatorio. |
| Buscar «la planilla» de la Lección 02 | Es la pregunta de la etapa siguiente; adelantarla sesga lo que se busca. |
| Adquirir la evidencia Windows de la Lección 07 | Es otro caso y otra etapa del curso. |

Vas a tener la evidencia en las manos y no vas a poder mirarla. Quien adquiere no siempre es quien analiza, y esa separación es lo que sostiene la cadena de custodia.

## Por qué no basta con copiar los archivos

Copiar con el explorador toma solo lo que el sistema de archivos muestra: deja fuera el espacio no asignado, los archivos borrados aún recuperables y los metadatos de bajo nivel. Además actualiza tiempos de acceso, es decir, modifica la evidencia mientras la copia.

Una imagen forense toma el dispositivo completo, sector por sector, con controles que permiten demostrar después que nada cambió. Esa diferencia entre copiar y adquirir es el contenido central de la lección.

## Cómo está montado esto en el laboratorio

En un laboratorio real usarías un **bloqueador de escritura por hardware** -*Tableau Forensic USB 3.0 Bridge* (T8u), *CRU WiebeTech USB WriteBlocker*, *Digital Intelligence UltraBlock*-, validado según la [especificación NIST](https://www.nist.gov/document/cftt-hwb-hardware-write-block-specs-version-20) y sus [informes de prueba por modelo](https://www.nist.gov/itl/ssd/software-quality-group/computer-forensics-tool-testing-program-cftt/cftt-technical/hardware). 

La sala no tiene uno: la fuente se expone como disco de solo lectura con `qemu-nbd`, y para Guymager `/dev/nbd0` equivale al pendrive conectado.

Cambia de dónde viene la garantía. Con un bloqueador la impone el hardware; aquí depende de que `--read-only` esté puesto y de que tú lo compruebes. Por eso `blockdev --getro` devolviendo `1` no es un trámite: es la evidencia que sostiene tu afirmación de no haber escrito sobre el original.

El resto del procedimiento es idéntico al de un dispositivo real.

## Material entregado

```text
LAB04_PENDRIVE/
├── fuente/
│   └── PENDRIVE_ADMIN_01.raw
├── adquisicion/
├── trabajo/
├── notas/
│   ├── PENDRIVE_ADMIN_01-ficha-fuente.txt
│   ├── PENDRIVE_ADMIN_01-fuente.md5
│   ├── PENDRIVE_ADMIN_01-fuente.sha1
│   └── PENDRIVE_ADMIN_01-fuente.sha256
└── registros/
```

Trabaja desde una copia local en:

```text
~/forense/LAB-04/
```

El archivo `guia-comandos-leccion-04.md` contiene la secuencia operativa exacta.

## Roles

Una adquisición no se hace en solitario: alguien opera y alguien atestigua. Cambien los roles al terminar la adquisición.

| Rol | Responsabilidad |
| --- | --- |
| Operador | Ejecuta los comandos y maneja Guymager. Anuncia en voz alta cada acción antes de realizarla. |
| Registrador | Verifica dispositivo, tamaño, modo de solo lectura y rutas de destino. Escribe la bitácora y detiene el procedimiento ante cualquier discrepancia. |

El registrador puede detener al operador. Si dice «espera», se espera.

## Reglas de seguridad

```text
Selecciona solo el dispositivo comprobado; nunca un disco físico del equipo.
No montes la fuente en modo de escritura.
Verbaliza dispositivo, tamaño, modo RO y destino antes de presionar Start.
Ante una discrepancia: detente, conserva la salida y avisa al docente.
Nunca sobrescribas una adquisición existente.
```

## Fase 1 - Identificar y verificar la fuente

1. Registra fecha, hora, zona horaria y nombres de los integrantes.
2. Confirma que estás en `~/forense/LAB-04/`.
3. Lee la ficha del caso.
4. Registra nombre, tamaño y tipo de la fuente.
5. Comprueba con qué permisos llegó la fuente. No los modifiques.
6. Ejecuta los tres manifiestos entregados: MD5, SHA-1 y SHA-256.

Completa:

| Campo                            | Valor                             |
| -------------------------------- | --------------------------------- |
| Caso                             | PCL-2026-018                      |
| Evidencia                        | [ ]                               |
| Fuente                           | [ ]                               |
| Tamaño en bytes                  | [ ]                               |
| Sistema de archivos declarado    | [ ]                               |
| Permisos con que llegó la fuente | [solo lectura / admite escritura] |
| Resultado del manifiesto MD5     | [OK / discrepancia]               |
| Resultado del manifiesto SHA-1   | [OK / discrepancia]               |
| Resultado del manifiesto SHA-256 | [OK / discrepancia]               |
| Zona horaria del laboratorio     | [ ]                               |

Registra también los tres valores calculados sobre el mismo archivo y responde en una frase:

| Algoritmo | Valor obtenido | Largo en caracteres |
| --- | --- | ---: |
| MD5 | [ ] | [ ] |
| SHA-1 | [ ] | [ ] |
| SHA-256 | [ ] | [ ] |

> Los tres valores son distintos entre sí y ninguno se parece a los otros. ¿Qué tienen en común y qué los diferencia?

### Evidencia E1 - Identificación y control de entrega

Incluye ambas tablas y la salida de `md5sum -c`, `sha1sum -c` y `sha256sum -c`. Los tres hashes identifican el mismo archivo RAW entregado; ninguno identifica todavía una adquisición E01.

## Fase 2 - Proteger y adquirir

1. Expón `PENDRIVE_ADMIN_01.raw` mediante NBD como dispositivo de solo lectura y guarda su nombre exacto.
2. Comprueba que es `/dev/nbd0` -previamente libre-, que mide `536870912 bytes`, que `blockdev --getro` devuelve `1` y que no está montado.
3. Abre Guymager y selecciona únicamente ese dispositivo.
4. Adquiere el dispositivo completo en formato E01 hacia `adquisicion/`.
5. Marca `Calculate MD5`, `Calculate SHA-1`, `Calculate SHA-256` y `Verify image after acquisition`. Deja `Re-read source after acquisition` sin marcar: esa comparación la harás tú en la fase 3.

Usa:

| Campo | Valor |
| --- | --- |
| Case number | `PCL-2026-018` |
| Evidence number | `EV-01` |
| Examiner | Identificador de la pareja |
| Description | `Pendrive EV-01 rotulado RESPALDO ADMIN` |
| Notes | `Fuente expuesta mediante qemu-nbd en modo de solo lectura` |
| Image directory | Ruta absoluta de `~/forense/LAB-04/adquisicion/` |
| Image filename | `PENDRIVE_ADMIN_01` (con guion bajo, sin extensión) |
| Info filename | `PENDRIVE_ADMIN_01` |

Guymager solo admite letras, dígitos y guion bajo: si escribes un guion normal lo elimina sin avisar. Comprueba que el campo *Image file* de la ventana principal termine en `PENDRIVE_ADMIN_01.Exx`.

Antes de iniciar, ambos integrantes verbalizan:

```text
Dispositivo seleccionado:
Tamaño:
Modo de solo lectura:
Destino:
```

### Evidencia E2 - Protección y adquisición

Registra el nombre del dispositivo, salida de `lsblk`, resultado de `blockdev --getro`, parámetros de Guymager, hora inicial/final y estado de verificación mostrado por la herramienta.

## Fase 3 - Verificar la E01

1. Cierra Guymager y desconecta el dispositivo NBD.
2. Vuelve a verificar el hash de la fuente.
3. Lee los metadatos con `ewfinfo` y los hashes del archivo `.info`.
4. Ejecuta `ewfverify` desde el primer segmento `.E01`.
5. Expón la E01 con `ewfmount` y calcula MD5, SHA-1 y SHA-256 sobre `fuente/PENDRIVE_ADMIN_01.raw` y sobre `ewf1`.
6. Compara los tres pares y desmonta la capa EWF.

Completa:

| Control | Resultado |
| --- | --- |
| Fuente después de la adquisición | [OK / discrepancia] |
| Metadatos de caso y evidencia | [Correctos / corregir] |
| `ewfverify` | [SUCCESS / error] |
| Algoritmos que aparecen en `ewfinfo` | [ ] |
| Algoritmos que aparecen en el archivo `.info` | [ ] |
| Tamaño del archivo `.E01` frente a los 512 MiB de la fuente | [ ] |

`ewfinfo` no mostrará los tres algoritmos aunque los marcaras en Guymager. Anota cuáles aparecen y cuáles no: explicarlo es parte de E3.

| Algoritmo | Valor del RAW | Valor de `ewf1` | Comparación |
| --- | --- | --- | --- |
| MD5 | [ ] | [ ] | [Coincide / no coincide] |
| SHA-1 | [ ] | [ ] | [Coincide / no coincide] |
| SHA-256 | [ ] | [ ] | [Coincide / no coincide] |

### Evidencia E3 - Integridad del contenido

Responde en una frase cada una:

1. Si los tres algoritmos coinciden, ¿son tres pruebas independientes o tres formas de comprobar el mismo hecho?
2. ¿Por qué el hash del contenedor `.E01` no se compara con el del RAW? Usa la diferencia de tamaño entre ambos archivos como argumento.
3. ¿Por qué `ewfinfo` muestra menos algoritmos de los que marcaste, y dónde quedó el que falta?
4. ¿Qué algoritmo elegirías como control externo del caso? MD5 y SHA-1 no resisten a un adversario que construya colisiones; aquí solo compruebas una copia contra su origen.

## Fase 4 - Crear y comprobar la copia de trabajo

1. Genera un manifiesto SHA-256 de todos los segmentos presentes en `adquisicion/`.
2. Copia los segmentos a `trabajo/` conservando sus timestamps.
3. Verifica desde `trabajo/` el manifiesto creado sobre `adquisicion/`.
4. Ejecuta `ewfverify` sobre la copia de trabajo.
5. No abras la adquisición preservada para las operaciones posteriores.

Completa:

| Objeto | Ubicación | Control | Resultado |
| --- | --- | --- | --- |
| Adquisición preservada | `adquisicion/` | Manifiesto de contenedores | [ ] |
| Copia de trabajo | `trabajo/` | `sha256sum -c` | [ ] |
| Contenido EWF de trabajo | `trabajo/` | `ewfverify` | [ ] |

### Evidencia E4 - Duplicación

Incluye el manifiesto, la salida de su comprobación y una frase que distinga la adquisición preservada de la copia de trabajo.

## Fase 5 - Interpretar y cerrar

Completa esta matriz:

| Hash o control | Objeto cubierto | Qué permite afirmar | Qué no permite afirmar |
| --- | --- | --- | --- |
| MD5, SHA-1 y SHA-256 entregados | Archivo RAW | [ ] | [ ] |
| Verificación EWF | Contenido adquirido | [ ] | [ ] |
| MD5, SHA-1 y SHA-256 de RAW frente a `ewf1` | Mismo flujo de bytes | [ ] | [ ] |
| Manifiesto de `.E01`/segmentos | Archivos contenedores | [ ] | [ ] |

Redacta la conclusión que cerraría tu informe de adquisición: entre 100 y 150 palabras, dirigida al perito que recibirá el caso y no estuvo en la sala. Debe indicar:

1. qué evidencia se adquirió y de qué caso;
2. cómo se protegió el original;
3. qué controles resultaron satisfactorios;
4. cuál es la copia sobre la que debe trabajar quien analice;
5. qué limitación tiene este pendrive simulado frente a una evidencia real.

No conjetures sobre el contenido del pendrive ni sobre el caso: no lo abriste, y afirmarlo excedería lo que tu procedimiento sostiene.

Desmonta cualquier capa EWF y confirma que `/dev/nbd0` quedó desconectado.

### Evidencia E5 - Interpretación y cierre seguro

Entrega la matriz, la conclusión y las salidas finales de desmontaje.

## Si aparece una discrepancia

```text
1. Detener la operación.
2. No sobrescribir ni repetir sobre los mismos archivos.
3. Registrar objeto, comando, hora y salida completa.
4. Mantener separados fuente, adquisición y copia de trabajo.
5. Informar al docente.
6. Continuar solo con una instrucción documentada.
```

Una discrepancia bien documentada es mejor que un `OK` fabricado.

## Entrega

Entrega una bitácora única en PDF o Markdown con E1 a E5. No entregues el RAW, la E01 ni su copia salvo instrucción docente.

## Autoevaluación

- [ ] Identifiqué el objeto antes de calcular o interpretar su hash.
- [ ] Comprobé los tres manifiestos entregados y la fuente antes y después de adquirir.
- [ ] Verifiqué `/dev/nbd0` con `RO 1` antes de abrir Guymager.
- [ ] Adquirí el dispositivo completo con los identificadores correctos.
- [ ] `ewfverify` terminó satisfactoriamente o documenté el error.
- [ ] Comparé RAW con `ewf1` en los tres algoritmos, no RAW con el contenedor `.E01`.
- [ ] Puedo justificar qué algoritmo usaría como control externo y por qué.
- [ ] La copia de trabajo coincide con el manifiesto de contenedores.
- [ ] Dejé fuente, adquisición y copia de trabajo separadas, sin montajes pendientes.
