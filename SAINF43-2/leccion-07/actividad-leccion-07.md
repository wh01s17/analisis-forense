---
title: "Actividad Lección 07 - Correlación de eventos en Windows 10"
tags:
  - nota
  - course
  - curso
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "07"
author: Jordy
start: 2026-09-21
end: 2026-09-23
created_at: 2026-08-21
aliases:
  - "Actividad Lección 07 - Correlación de eventos en Windows 10"
---

# Actividad Lección 07 - Correlación de eventos en Windows 10

**Duración total:** 80 minutos.
**Modalidad:** parejas.
**Caso único:** `WIN10-CASO01`.
**Fuente:** copia de análisis E01 entregada por el docente y compartida con Kali en modo de solo lectura.

## Pregunta del caso

> ¿Qué secuencia de accesos, creación de cuenta, cambio de privilegios, ejecución de procesos y persistencia mediante tarea programada puede reconstruirse desde la imagen?

No se busca demostrar intención ni atribuir las acciones a una persona. Se busca reconstruir hechos observables y formular inferencias limitadas.

## Resultado esperado

Cada pareja entrega:

1. comprobación de la copia de análisis entregada;
2. tabla cronológica de ocho evidencias seleccionadas;
3. dos correlaciones justificadas;
4. una comprobación con un artefacto del sistema;
5. conclusión técnica breve con nivel de confianza y limitaciones.

El caso completo contiene más eventos y permite correlaciones adicionales. En esta clase se evalúa la calidad de la selección y del razonamiento, no la cantidad de filas.

## Fuente de trabajo

Usa únicamente la copia de análisis entregada para esta lección:

```text
/media/sf_LAB07_ENTREGA/evidencia/WIN10-CASO01.E01
```

Esta práctica retoma el laboratorio Kali–Windows preparado en la Lección 03. Verifica con el docente que la estación de análisis parte del estado `LAB-03-configurado`; la VM Windows permanece apagada y la evidencia se examina únicamente desde Kali.

Antes de la práctica se copia `LAB07_ENTREGA` desde el pendrive al PC anfitrión y se comparte con Kali como solo lectura. No trabajes desde el pendrive. No inicies la OVA, no arranques Windows y no modifiques los segmentos. La adquisición completa ya se practicó en la Lección 04 con `PENDRIVE_ADMIN_01`; aquí se transfiere esa disciplina al análisis de una evidencia mayor.

La guía práctica de comandos de la Lección 07 contiene la secuencia exacta para montar la E01 en modo de solo lectura y analizarla desde Kali.

## Herramientas y roles

- `ewfmount` y Sleuth Kit: exponer la E01, confirmar particiones y montar Windows en solo lectura.
- Chainsaw: buscar los Event ID y mostrar sus campos.
- Integrante 1: operador.
- Integrante 2: registrador y verificador.

Cambien los roles después de extraer el EVTX obligatorio.

## Fase 1 - Verificar y montar el caso

1. Comprueba la copia de análisis con el manifiesto SHA-256 incluido en `LAB07_ENTREGA`. El docente ejecuta `ewfverify` antes de la clase; no repitas esa verificación completa durante la actividad.
2. Comprueba que `LAB07_ENTREGA` está compartida con Kali en modo `ro` y que `LAB07_SALIDA` admite escritura.
3. Registra fecha, hora, zona horaria del laboratorio y zona horaria conocida del Windows analizado.
4. Lee `WIN10-CASO01-caso.env` y registra el sector inicial, tamaño de sector, zona horaria y procedencia pedagógica del caso.
5. Expón la copia E01 mediante `ewfmount`.
6. Confirma con `mmls` la tabla de particiones.
7. Monta la partición Windows en modo de solo lectura.
8. Comprueba que existen `Windows` y `Users`.

| Control | Resultado |
| --- | --- |
| SHA-256 de todos los segmentos | [Todos OK / discrepancia] |
| Carpeta `LAB07_ENTREGA` compartida como `ro` | [Sí / No] |
| Procedencia pedagógica del caso | [Mismo caso de L03 / imagen docente equivalente] |
| Zona horaria original del caso | [Completar] |
| Zona usada para normalizar | [Completar] |
| Archivo `ewf1` creado | [Sí / No] |
| Sector inicial confirmado con `mmls` | [Completar] |
| Montaje de Windows muestra `ro` | [Sí / No] |

Si la verificación falla, detente e informa. No continúes con otra imagen.

## Fase 2 - Extraer la fuente de eventos

En la partición montada localiza la fuente obligatoria:

```text
C:\Windows\System32\winevt\Logs\Security.evtx
```

Extráela en:

```text
~/forense/LAB-07/exportados/evtx/
```

Registra el archivo:

| Archivo | Ruta dentro de la imagen | Tamaño | Fecha de modificación | Destino exportado |
| --- | --- | ---: | --- | --- |
| `Security.evtx` | [Completar] | [ ] | [ ] | [Completar] |

Registra los metadatos con `stat`, copia `Security.evtx` a `~/forense/LAB-07/exportados/evtx/` y calcula SHA-256 sobre la copia. La ruta interna identifica la fuente; la carpeta de exportación contiene el archivo que analizará Chainsaw. El registro de PowerShell se extrae únicamente si el equipo realiza esa ampliación.

## Fase 3 - Seleccionar las evidencias del caso

Busca para el recorrido obligatorio:

```text
Security.evtx: 4624, 4634, 4648, 4688, 4698, 4720, 4732
```

Realiza la primera búsqueda entre `2026-08-21T04:45:00Z` y `2026-08-21T05:15:00Z`. Esta ventana reduce eventos del sistema ajenos al escenario sin indicar cuál entrada debes seleccionar.

No debes revisar todos los procesos 4688. Usa primero 4698, 4720 y 4732 como eventos de referencia; después busca los procesos relacionados por cuenta, nombre de objeto y tiempo. Completa estas ocho selecciones. El 4688 aparece dos veces porque documenta procesos distintos.

| Fila | Selección                  | Hora original | Hora normalizada | Fuente          |   ID | Cuenta o SID | Logon ID | Proceso, tarea u objeto | Hecho observado | Inferencia | Confianza         |
| ---: | -------------------------- | ------------- | ---------------- | --------------- | ---: | ------------ | -------- | ----------------------- | --------------- | ---------- | ----------------- |
|    1 | Tarea creada               | [ ]           | [ ]              | `Security.evtx` | 4698 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [Alta/Media/Baja] |
|    2 | Cuenta creada              | [ ]           | [ ]              | `Security.evtx` | 4720 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    3 | Incorporación a grupo      | [ ]           | [ ]              | `Security.evtx` | 4732 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    4 | Proceso que crea la cuenta | [ ]           | [ ]              | `Security.evtx` | 4688 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    5 | Uso de `runas`             | [ ]           | [ ]              | `Security.evtx` | 4688 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    6 | Credenciales explícitas    | [ ]           | [ ]              | `Security.evtx` | 4648 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    7 | Inicio correcto            | [ ]           | [ ]              | `Security.evtx` | 4624 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |
|    8 | Cierre de sesión           | [ ]           | [ ]              | `Security.evtx` | 4634 | [ ]          | [ ]      | [ ]                     | [ ]             | [ ]        | [ ]               |

### Campos mínimos

|   ID | Campos que debes revisar                                                                                                             |
| ---: | ------------------------------------------------------------------------------------------------------------------------------------ |
| 4624 | `SubjectLogonId`, `TargetUserName`, `TargetUserSid`, `LogonType`, `TargetLogonId`, `TargetLinkedLogonId` y origen si está disponible |
| 4634 | `TargetUserName`, `TargetLogonId`, `LogonType`                                                                                       |
| 4648 | cuenta solicitante, `SubjectLogonId`, cuenta objetivo, servidor objetivo, proceso                                                    |
| 4688 | cuenta, `SubjectLogonId`, `NewProcessName`, `ParentProcessName` y `CommandLine` si existe                                            |
| 4698 | cuenta creadora, nombre de tarea, XML, identidad de ejecución y acción                                                               |
| 4720 | cuenta creadora, cuenta nueva y SID nuevo                                                                                            |
| 4732 | cuenta modificadora, grupo local y miembro agregado                                                                                  |

En la ventana aparece más de un 4688 de `runas.exe` con la misma cuenta y el mismo `SubjectLogonId`. Ese campo no basta para elegir: selecciona el que sostiene la secuencia hacia el 4648 y el 4624, y justifica tu elección en la columna de inferencia.

Si un campo no está presente, escribe `no registrado`. Su ausencia es una limitación, no una invitación a completar por intuición.

Si una línea de comandos contiene una contraseña, reemplázala por `[REDACTADO]` en la entrega. Conserva el resto del comando y deja constancia de que el valor fue ocultado.

## Fase 4 - Correlacionar y corroborar

Construye estas dos relaciones obligatorias:

| Correlación | Claves mínimas |
| --- | --- |
| Uso de credenciales y sesión | 4688 de `runas`, 4648 y 4624 mediante el `SubjectLogonId` de la sesión solicitante; 4624 y 4634 mediante el `TargetLogonId` de la sesión creada; confirma además cuenta objetivo y secuencia temporal |
| Creación de cuenta y privilegios | 4688 de creación de cuenta, 4720 y 4732 mediante actor, cuenta/SID y tiempo |

Para cada relación redacta:

```text
Evidencia A:
Evidencia B:
Campo de unión:
Relación observada:
Inferencia:
Confianza:
Explicación alternativa o dato faltante:
```

Luego usa la partición montada en solo lectura para realizar una comprobación obligatoria:

1. Busca el archivo de la tarea bajo `C:\Windows\System32\Tasks\` usando el nombre obtenido en 4698.

Registra `presente` o `no localizado`. No localizar el artefacto no invalida automáticamente el evento; puede depender de configuración, limpieza o estado final de la imagen.

En este caso pueden aparecer dos inicios 4624 casi simultáneos para `soporte`, enlazados mediante `TargetLinkedLogonId`, y sus cierres 4634 correspondientes. Selecciona un par coherente: el `TargetLogonId` del 4624 elegido debe coincidir con el del 4634. No mezcles los identificadores de ambos pares.

### Ampliación

Si el equipo termina el recorrido obligatorio, puede añadir una de estas extensiones, sin reemplazar el producto obligatorio:

- relacionar el 4688 de PowerShell con el 4104;
- relacionar el 4688 de `schtasks.exe` con 4698;
- revisar el 4625 previo al uso de credenciales;
- localizar `POWERSHELL.EXE-*.pf` y explicar qué apoya y qué no demuestra.

Antes de terminar, desmonta primero la partición Windows y después la capa EWF, según la guía técnica.

## Fase 5 - Cerrar la línea temporal

Compara las ocho horas normalizadas y escribe la secuencia, del hecho más antiguo al más reciente, usando los identificadores de la columna `Fila`:

```text
Orden temporal: ___ , ___ , ___ , ___ , ___ , ___ , ___ , ___
```

Esa secuencia es la que usarás para llenar la línea temporal de la bitácora, donde cada evidencia se escribe en la posición que le corresponde por tiempo. El archivo de tarea se registra como comprobación de apoyo y no requiere una fila adicional.

No ordenes los hechos por el número del Event ID. Ordénalos por tiempo.

## Fase 6 - Conclusión

Escribe una conclusión breve con tus propias palabras.

Debe quedar claro:

- cuál es la secuencia mejor sustentada;
- cuál es la relación más fuerte y mediante qué campo la uniste;
- qué aporta el artefacto comprobado y qué no demuestra;
- qué no puedes afirmar y qué dato te faltaría para hacerlo;
- qué nivel de confianza global asignas y por qué.

## Entrega

Entrega una bitácora única en PDF o Markdown. Debe incluir capturas de pantalla y contener:

- control de integridad;
- identificación de `Security.evtx`;
- tabla cronológica de ocho eventos;
- dos correlaciones;
- una comprobación de artefacto;
- conclusión y limitaciones.

Antes de apagar o reiniciar, copia la bitácora terminada a `/media/sf_LAB07_SALIDA/` y comprueba que ambas copias coincidan. No entregues la E01, los EVTX exportados ni las salidas completas de Chainsaw, salvo instrucción docente.