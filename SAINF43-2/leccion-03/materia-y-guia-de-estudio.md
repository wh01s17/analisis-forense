---
title: "Materia y guía de estudio — Sistemas, herramientas y laboratorio forense"
tags:
  - nota
  - course
  - curso
  - materia-y-guia-de-estudio
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "03"
author: Jordy
start: 2026-08-24
end: 2026-08-26
created_at: "2026-08-06 17:41"
aliases:
  - "Materia y guía de estudio — Sistemas, herramientas y laboratorio forense"
---

# Materia y guía de estudio — Sistemas, herramientas y laboratorio forense

```toc
```

## Propósito de esta guía

Esta guía contiene lo necesario para construir y verificar el laboratorio de la Lección 03 sin depender de instrucciones externas. El objetivo no es aprender Kali o Windows en profundidad, sino comprender cómo preparar un entorno controlado antes de manipular evidencia digital.

El laboratorio se ejecutará en VirtualBox y tendrá dos máquinas virtuales:

- **Kali Linux:** estación principal del analista.
- **Windows:** sistema objetivo de laboratorio, importado desde un OVA preparado por el docente.

En esta lección Windows es un sistema autorizado para practicar. **No es una evidencia forense adquirida.** El conjunto `LAB-03` contiene archivos benignos para practicar SHA-256, organización y registro; copiar esos archivos tampoco constituye una adquisición forense completa.

### Progresión de la asignatura

| Momento | Propósito |
| --- | --- |
| Lección 03 | Preparar, aislar y verificar el laboratorio Kali + Windows. |
| Lección 04 | Realizar adquisición de datos, obtener imágenes o copias forenses y verificar hashes. |
| Lección 05 | Evaluación de la UA1. |
| Lecciones posteriores | Analizar artefactos, red, memoria y otros tipos de evidencia. |

En esta guía no se enseñan `dd`, FTK Imager, Guymager, E01, RAW/DD ni procedimientos completos de adquisición. Tampoco se examinan Event Logs, Registry, Prefetch, memoria, malware o tráfico de red. Esos contenidos tendrán su momento después de preparar correctamente el entorno.

## Ruta de estudio

Estudia en este orden:

1. Comprende qué es un laboratorio forense y los roles de sus componentes.
2. Construye la arquitectura Kali + Windows a partir del OVA docente.
3. Comprende qué aporta la virtualización y qué riesgos no elimina.
4. Configura red, integraciones y controles de aislamiento.
5. Prepara snapshots, tiempo, almacenamiento y organización del caso.
6. Distingue proceso, sistema y herramienta; luego inventaría las herramientas disponibles.
7. Comprende validación, verificación y hashes.
8. Ejecuta el procedimiento de `LAB-03`, completa el checklist y la autoevaluación.

### Criterio de suficiencia

Estás preparado cuando puedes:

- Dibujar la arquitectura e identificar host, hipervisor, estación analista y sistema objetivo.
- Explicar por qué una VM no está aislada automáticamente.
- Elegir entre Internal Network, host-only, NAT, bridged o sin adaptador y justificar el riesgo.
- Verificar que Kali y Windows se comunican solo cuando corresponde y que no acceden a redes no autorizadas.
- Registrar recursos, integraciones, tiempo, almacenamiento, herramientas y snapshots.
- Separar `original/`, `trabajo/`, `exportados/` y `notas/`.
- Verificar con SHA-256 que los archivos benignos recibidos y sus copias coinciden.
- Explicar por qué esa copia benigna no es una adquisición forense.
- Entregar una bitácora que permita reproducir la preparación.

## Objetivos de aprendizaje

Al finalizar la lección deberías poder:

- Preparar un laboratorio virtual controlado y recuperable.
- Diferenciar la estación del analista del sistema objetivo.
- Reducir canales de comunicación no necesarios entre host, VMs e Internet.
- Seleccionar un modo de red adecuado y comprobar su configuración.
- Registrar y comparar hora y zona horaria sin alterar artificialmente los sistemas.
- Inventariar herramientas y relacionarlas con una función y una limitación.
- Aplicar controles básicos de integridad sobre un conjunto benigno conocido.
- Documentar decisiones, resultados, excepciones y desviaciones durante la sesión.

## 1. ¿Qué es un laboratorio forense?

Un laboratorio forense es un entorno controlado donde personas, procedimientos, equipos y software se preparan para tratar datos digitales de forma segura, íntegra y reproducible.

No basta con “tener programas forenses”. El laboratorio debe permitir responder preguntas como:

- ¿Quién realizó cada acción y con qué autorización?
- ¿Qué sistema desempeñó cada función?
- ¿Qué conexiones e integraciones estaban habilitadas?
- ¿Qué versión de herramienta se utilizó?
- ¿Qué hora mostraba cada sistema?
- ¿Cómo se comprobó que un archivo no cambió?
- ¿Cómo puede otra persona reconstruir o repetir la preparación?

Tres principios orientan la lección:

| Principio | Aplicación en el laboratorio |
| --- | --- |
| Integridad | Evitar cambios accidentales y comprobar archivos con SHA-256. |
| Reproducibilidad | Registrar configuración, versiones, comandos, resultados y snapshots. |
| Seguridad | Limitar red, USB, credenciales e integraciones VM-host. |

La preparación no elimina todo riesgo, pero lo hace visible, justificable y controlable.

## 2. Host, hipervisor, estación analista y sistema objetivo

Estos conceptos suelen confundirse:

| Componente | Qué es | Función en la lección |
| --- | --- | --- |
| Host | Equipo físico y sistema operativo que ejecutan VirtualBox. | Aporta CPU, RAM, almacenamiento, red y dispositivos físicos. |
| Hipervisor | Software que crea y administra máquinas virtuales. | VirtualBox controla recursos, adaptadores, integraciones y snapshots. |
| Estación analista | Sistema desde el cual el analista organiza, verifica y posteriormente examina datos. | Kali Linux. |
| Sistema objetivo | Sistema autorizado sobre el que se practicarán procedimientos posteriores. | Windows importado desde el OVA docente. |

El host no desaparece cuando se usan VMs. Sigue siendo una superficie de contacto: almacena discos virtuales y snapshots, controla interfaces de red y puede compartir portapapeles, archivos y USB.

El término **sistema objetivo** no significa automáticamente **evidencia**. En la Lección 03 la VM Windows es una plataforma de práctica conocida. En la Lección 04 se enseñará cómo obtener una copia o imagen forense mediante un procedimiento documentado, sin trabajar directamente sobre el origen.

## 3. Arquitectura Kali + Windows

La arquitectura mínima es:

```text
Equipo físico / Host
└── VirtualBox
    ├── Kali Linux
    │   └── Estación analista
    │
    └── Windows
        └── Sistema objetivo de laboratorio
```

Cuando la actividad necesite comunicación, ambas VMs compartirán una red interna de VirtualBox. Esa red no debe incluir el host, Internet ni la LAN institucional.

```text
                    Red interna: LAB-03-INT
                 sin gateway y sin DNS externo

        Kali Linux  <---------------------->  Windows
     estación analista                   sistema objetivo
       192.168.77.10                       192.168.77.20
             │                                  │
             └──── sin salida autorizada ───────┘
```

Las direcciones son un ejemplo. Se deben usar las que entregue el docente.

### Preparación docente

El docente debe entregar:

- Un OVA de Windows previamente configurado, autorizado y reutilizable.
- Credenciales exclusivas de laboratorio.
- Requisitos o valores esperados de CPU, RAM y almacenamiento.
- Nombre y direccionamiento de la red interna.
- `LAB-03` con archivos benignos y hashes SHA-256 conocidos.
- Un medio controlado para entregar esos archivos sin habilitar integraciones improvisadas.
- Una alternativa aceptada si el host no soporta snapshots.

El OVA evita convertir la sesión en una instalación de Windows desde cero. Debe conservarse sin modificaciones para poder reimportarlo en prácticas posteriores.

### Importar el OVA de Windows

La ubicación exacta de los menús puede variar levemente según la versión de VirtualBox, pero el flujo es el mismo:

1. En VirtualBox Manager, elige la opción **Importar** o **Import Appliance**.
2. Selecciona el OVA entregado por el docente.
3. Revisa la información del dispositivo virtual antes de confirmar.
4. Importa la VM y espera a que VirtualBox copie sus discos.
5. Con la VM apagada, revisa CPU, RAM, controladores, almacenamiento, red, carpetas compartidas, USB y demás integraciones.
6. No inicies la VM con NAT o bridged solo porque el OVA los incluya.
7. En el primer inicio, identifica la versión de Windows, hora, zona horaria y cuenta de laboratorio.
8. Registra que su función es **sistema objetivo de práctica**.

Asignar más CPU o RAM no siempre mejora el laboratorio. Una VM que consume casi todos los recursos del host puede volver inestable a la otra. Se deben respetar los valores docentes o documentar cualquier ajuste.

### Preparar Kali como estación analista

Kali debe estar disponible antes de la actividad. En ella se registrará:

- Sistema operativo, versión y kernel.
- Usuario exclusivo de laboratorio.
- CPU y RAM asignadas.
- Capacidad y espacio libre.
- Fecha, hora, zona horaria y sincronización.
- Herramientas instaladas y sus versiones.
- Configuración de red e integraciones.
- Estructura del caso.
- Snapshot limpio o alternativa de reversión.

No uses cuentas personales, correo institucional, sincronización cloud ni credenciales reales en Kali o Windows.

## 4. Ventajas y limitaciones de la virtualización

### Ventajas

- Permite ejecutar Kali y Windows en un mismo host.
- Facilita repetir una configuración en distintos equipos.
- Permite volver a un estado conocido mediante snapshots.
- Separa lógicamente los sistemas del laboratorio.
- Hace posible importar el OVA docente y reutilizarlo.
- Reduce el costo de disponer de equipos físicos separados.

### Limitaciones

- Una VM puede seguir conectada a Internet o a la LAN mediante NAT o bridged.
- El host puede comunicarse con la VM mediante host-only, carpetas, portapapeles, drag and drop o USB.
- Un error o falta de espacio en el host puede afectar ambas VMs.
- Los snapshots dependen de archivos almacenados en el host y pueden crecer.
- El reloj de una VM puede sincronizarse o desviarse respecto del host.
- Restaurar un snapshot puede eliminar trabajo posterior.
- La virtualización no reproduce necesariamente todo el comportamiento de hardware físico.

Por eso, **una máquina virtual no equivale automáticamente a aislamiento**. El aislamiento es una configuración que debe diseñarse, probarse y registrarse.

## 5. Modos de red de VirtualBox

El modo de red determina con quién puede comunicarse una VM. Debe elegirse por necesidad, no por comodidad.

| Modo | Comunicación esperada | Uso posible | Riesgo desde una perspectiva forense |
| --- | --- | --- | --- |
| Sin adaptador de red | La VM no dispone de un adaptador virtual activo. | Trabajo que no requiere ninguna comunicación. | Reduce exposición, pero impide comunicación Kali-Windows y puede ocultar una dependencia no documentada. |
| Internal Network | Se comunican las VMs conectadas a la misma red interna con el mismo nombre. El host y el exterior no participan por defecto. | Opción recomendada cuando Kali y Windows deben comunicarse exclusivamente entre sí. | Un nombre distinto impide comunicación; una segunda tarjeta NAT o bridged puede romper el aislamiento. |
| Host-only | Se comunican las VMs y el host mediante una interfaz virtual; no hay salida física por ese modo de forma predeterminada. | Administración o transferencia controlada que requiera al host. | Expone servicios y archivos del host; el host podría actuar como ruta o puente por otra configuración. |
| NAT | La VM obtiene salida usando la conectividad del host. Suele ser el modo predeterminado. | Actualización excepcional y autorizada. | Puede generar tráfico externo, telemetría, sincronización o transferencia no prevista. Debe permanecer deshabilitado durante el trabajo forense. |
| Bridged | La VM se conecta a la interfaz física del host y aparece como otro equipo en la LAN. | Escenarios de red expresamente diseñados y autorizados. | Expone la VM a la LAN institucional y la LAN a la VM. No debe usarse en esta actividad. |

### Configuración recomendada

Cuando Kali y Windows necesiten comunicarse:

1. Apaga ambas VMs.
2. Abre la configuración de red de cada VM.
3. Habilita solo el adaptador necesario.
4. Selecciona **Internal Network**.
5. Escribe exactamente el mismo nombre, por ejemplo `LAB-03-INT`.
6. Deshabilita los demás adaptadores, especialmente NAT y bridged.
7. Usa direcciones del mismo segmento, sin puerta de enlace y sin DNS externo.

Ejemplo:

| VM | Dirección | Máscara o prefijo | Puerta de enlace | DNS |
| --- | --- | --- | --- | --- |
| Kali | `192.168.77.10` | `/24` o `255.255.255.0` | Vacía | Vacío |
| Windows | `192.168.77.20` | `/24` o `255.255.255.0` | Vacía | Vacío |

### Configurar las direcciones dentro de las VMs

En Kali con entorno gráfico:

1. Abre la configuración de red y selecciona la conexión cableada correspondiente al adaptador interno.
2. En IPv4, elige el método manual.
3. Ingresa dirección y prefijo o máscara entregados por el docente.
4. Deja vacías la puerta de enlace y las direcciones DNS; desactiva DNS automático si la interfaz lo solicita.
5. Aplica los cambios y reconecta la interfaz.

En Windows:

1. Abre **Configuración > Red e Internet > Ethernet**.
2. En la asignación de IP, selecciona **Editar > Manual** y activa IPv4.
3. Ingresa la dirección y longitud de prefijo entregadas por el docente.
4. Deja vacías la puerta de enlace y las direcciones DNS.
5. Guarda los cambios.

Si la edición de Windows presenta menús diferentes, abre las propiedades IPv4 del adaptador Ethernet desde el panel de conexiones de red y aplica los mismos valores. No improvises otra puerta de enlace o DNS para “hacer funcionar” la conexión.

Si no se necesita comunicación, deshabilitar el adaptador es más simple y reduce superficie de exposición.

NAT solo se habilita con autorización expresa. Se debe registrar:

- Quién autorizó la excepción.
- Motivo y recurso que se necesita.
- VM afectada.
- Hora de habilitación y deshabilitación.
- Tráfico o cambio esperado.
- Repetición de las pruebas de aislamiento al cerrar la excepción.

## 6. Aislamiento y su verificación

Aislar no significa marcar una casilla. Requiere combinar configuración y pruebas.

### Capas de control

1. **Red:** Internal Network o adaptador deshabilitado; sin NAT ni bridged.
2. **Rutas:** sin puerta de enlace predeterminada hacia el exterior.
3. **Integraciones:** portapapeles, drag and drop y carpetas compartidas deshabilitados.
4. **Dispositivos:** USB y otros periféricos revisados.
5. **Identidad:** sin credenciales personales o institucionales.
6. **Registro:** toda excepción posee propósito, responsable y duración.

### Comprobar la red interna

En Kali:

```bash
ip -brief address
ip route
ping -c 4 192.168.77.20
```

En Windows:

```powershell
ipconfig /all
route print
ping 192.168.77.10
```

Sustituye las direcciones por las asignadas. La comunicación en ambos sentidos demuestra que existe un camino entre las VMs, siempre que el firewall permita ICMP. Si un `ping` falla, revisa dirección, prefijo, nombre de red interna y firewall. Un fallo no demuestra por sí mismo que el laboratorio esté aislado.

### Comprobar ausencia de salida no autorizada

La verificación debe combinar:

- Revisión visual de todos los adaptadores en VirtualBox.
- Confirmación de que no existe una ruta predeterminada en Kali ni Windows.
- Prueba negativa hacia un destino externo definido por el docente.
- Confirmación de que no existen adaptadores adicionales activos.

No hagas barridos, escaneos ni pruebas sobre la LAN institucional. Un `ping` externo fallido puede deberse a filtrado y no demuestra por sí solo ausencia de conectividad. La evidencia principal es la configuración, complementada con rutas y pruebas autorizadas.

La bitácora debe registrar fecha y hora, VM, comando o pantalla revisada, resultado esperado, resultado observado y cualquier desviación.

## 7. Integraciones VM-host y sus riesgos

Las integraciones hacen más cómodo usar una VM, pero crean canales entre el laboratorio y el host.

| Integración | Riesgo | Control para la lección |
| --- | --- | --- |
| Portapapeles compartido | Puede transferir texto, comandos, contraseñas o fragmentos de datos sin un archivo visible. | Deshabilitado en ambas direcciones. |
| Drag and drop | Permite mover archivos entre host y VM fuera del procedimiento registrado. | Deshabilitado. |
| Carpetas compartidas | Da a la VM acceso directo a rutas del host; una escritura, borrado o programa ejecutado puede afectar ambos entornos. | Sin carpetas compartidas, salvo excepción justificada. |
| USB | Un dispositivo puede conectarse a la VM equivocada, desmontarse del host o transportar datos no autorizados. | Revisar controladores y filtros; no usar dispositivos personales o institucionales. |
| Adaptadores de red | Una segunda tarjeta NAT o bridged puede mantener salida aunque la primera sea interna. | Revisar todos los adaptadores, no solo el primero. |
| Credenciales | Una cuenta real puede activar servicios, sincronización o exposición de datos personales. | Usar solo cuentas de laboratorio. |

Las Guest Additions pueden habilitar funciones de integración. No es necesario desinstalarlas para esta actividad, pero las funciones que no se necesitan deben permanecer deshabilitadas y registradas.

Una excepción no debe quedar descrita como “necesaria” sin más. Debe indicar propósito, alcance, responsable, inicio, término y control compensatorio.

## 8. Snapshots y restauración

Un snapshot conserva un estado de la VM para volver a él más adelante. Según el momento y la configuración, incluye los parámetros de la VM, el estado de sus discos virtuales y, si se toma en ejecución, puede incluir memoria.

Sirve para:

- Mantener una base reproducible.
- Repetir una práctica desde condiciones conocidas.
- Recuperarse de una configuración incorrecta.
- Separar etapas de laboratorio.

No sirve para:

- Respaldar evidencia.
- Crear una imagen forense.
- Sustituir una adquisición.
- Reemplazar la bitácora.
- Garantizar recuperación si se pierde el almacenamiento del host.

Un snapshot utiliza discos diferenciales y consume espacio a medida que la VM cambia. Restaurarlo revierte el estado de la VM y puede eliminar archivos creados después. La bitácora debe guardarse donde el docente indique si necesita sobrevivir a una restauración.

### Estado base recomendado

Antes de ejercicios posteriores deben existir puntos limpios y descritos, por ejemplo:

- `KALI-L03-BASE-LIMPIA`.
- `WIN-L03-BASE-LIMPIA`.

La descripción debe indicar:

- Fecha y responsable.
- Sistema y versión.
- Modo y nombre de red.
- Integraciones deshabilitadas.
- Estado de herramientas y directorios.
- Si la VM estaba apagada o encendida.

En Windows, conserva además el OVA docente original. El OVA permite reimportar la VM; el snapshot permite volver a un estado local específico. No son equivalentes.

Si no se pueden crear snapshots, documenta una alternativa aprobada: clon completo, imagen base docente o reimportación controlada del OVA más instrucciones de configuración.

## 9. Gestión del tiempo y la zona horaria

El tiempo permite ordenar acciones y relacionar registros entre sistemas. En un laboratorio virtual pueden existir diferencias porque el host, VirtualBox, Kali y Windows utilizan zonas o mecanismos de sincronización distintos.

Se debe registrar:

- Hora del host.
- Hora de Kali.
- Hora de Windows.
- Zona horaria de cada sistema.
- Diferencia observada.
- Mecanismo de sincronización.

No modifiques artificialmente un reloj para que “se vea bien”. La diferencia se documenta. Cambiarla sin registro elimina contexto y dificulta reproducir resultados.

En un host Linux o en Kali:

```bash
date --iso-8601=seconds
timedatectl status
```

En un host Windows o en la VM Windows:

```powershell
Get-Date -Format o
Get-TimeZone
w32tm /query /status
```

Si un comando informa que el servicio de sincronización no está activo, registra el resultado; no lo corrijas sin autorización.

Plantilla:

| Sistema | Fecha y hora | Zona | Sincronización | Diferencia observada |
| --- | --- | --- | --- | --- |
| Host |  |  |  | Referencia |
| Kali |  |  |  |  |
| Windows |  |  |  |  |

## 10. Gestión del almacenamiento y permisos

El host debe disponer de espacio para:

- El OVA original.
- Los discos virtuales importados.
- Los snapshots, que pueden crecer.
- El conjunto benigno `LAB-03`.
- Bitácora, salidas y capturas.

Dentro de Kali se debe comprobar capacidad y espacio libre:

```bash
lsblk
df -h
```

En Windows puede registrarse el almacenamiento sin examinar artefactos:

```powershell
Get-Volume
```

No confundas:

- **Tamaño virtual asignado:** capacidad máxima presentada a la VM.
- **Espacio libre dentro de la VM:** capacidad disponible para archivos.
- **Espacio real del host:** capacidad que sostiene discos virtuales y snapshots.

Comprueba también:

- Ruta donde VirtualBox guarda las VMs.
- Permisos sobre la carpeta del caso.
- Que `original/` pueda protegerse contra escritura.
- Que bitácora y exportaciones tengan un destino identificado.

La protección lógica contra escritura reduce errores, pero un usuario con permisos suficientes puede revertirla. En esta lección se usa como control de preparación, no como sustituto de un bloqueador de escritura ni de una adquisición forense.

## 11. Organización del caso

La estructura común será:

```text
caso/
├── original/
├── trabajo/
├── exportados/
└── notas/
```

| Directorio | Propósito | Regla |
| --- | --- | --- |
| `original/` | Conservar los archivos benignos recibidos y verificados. | Proteger contra escritura después de verificar. |
| `trabajo/` | Contener copias sobre las que se permiten operaciones controladas. | No confundir con el original. |
| `exportados/` | Guardar resultados o salidas generadas. | Mantener relación con herramienta, versión y entrada. |
| `notas/` | Guardar bitácora, manifiestos, inventario y explicación de capturas. | Registrar durante la sesión. |

Crear la estructura:

```bash
mkdir -p ~/caso/{original,trabajo,exportados,notas}
find ~/caso -maxdepth 1 -type d -print
```

Proteger y revisar los archivos de `original/`:

```bash
chmod -R a-w ~/caso/original
find ~/caso/original -maxdepth 1 -type f -printf '%M %p\n'
```

Los permisos se aplican después de copiar y verificar `LAB-03`. Si es necesario corregir el contenido, no cambies silenciosamente los permisos: registra qué ocurrió y consulta al docente.

## 12. Proceso, sistema, herramientas y criterios de selección

### Proceso forense

Define por qué se actúa, con qué autorización, sobre qué fuente, en qué orden y cómo se documentarán e interpretarán los resultados.

### Sistema forense

Es el conjunto de personas, procedimientos, equipos, software, almacenamiento y controles usados para tratar datos digitales.

En esta lección, el sistema incluye al estudiante, la guía, el host, VirtualBox, Kali, Windows, `LAB-03`, la estructura del caso, los controles de aislamiento y la bitácora.

### Herramienta forense

Es un programa o dispositivo que ejecuta una función específica. Puede calcular un hash, adquirir datos, buscar, extraer, comparar, analizar o reportar.

**Kali no es “la herramienta forense”.** Es el sistema operativo de la estación analista y contiene herramientas distintas. Cada una debe identificarse por nombre, versión, función, entrada, salida y limitaciones.

| Puede hacer una herramienta | No puede decidir por sí sola |
| --- | --- |
| Calcular un hash. | Si la acción está autorizada. |
| Extraer o mostrar datos. | Qué fuente es prioritaria. |
| Buscar patrones. | Qué significa el resultado para el caso. |
| Generar tablas o reportes. | Si la evidencia es suficiente. |
| Automatizar una tarea. | Qué limitaciones deben declararse. |

### Categorías de herramientas

| Categoría | Función general | Ejemplo | Límite en esta lección |
| --- | --- | --- | --- |
| Integridad | Calcular y comparar hashes. | `sha256sum`. | Se utiliza solo con archivos benignos conocidos. |
| Adquisición | Obtener una copia o imagen mediante un procedimiento controlado. | Herramientas de adquisición. | Solo se explica la categoría; se trabajará en Lección 04. |
| Protección contra escritura | Reducir escrituras sobre una fuente. | Bloqueador físico o control lógico. | No se implementa un procedimiento completo. |
| Disco y sistema de archivos | Examinar particiones, archivos y metadatos. | Autopsy. | No se analizan discos ni artefactos. |
| Artefactos de sistema | Procesar registros y datos del sistema operativo. | Herramientas especializadas. | No se estudian Event Logs, Registry ni Prefetch. |
| Memoria | Examinar una captura de RAM. | Volatility 3. | Solo se reconoce la función general. |
| Red | Examinar paquetes, flujos o registros. | Wireshark. | No se captura ni analiza tráfico. |
| Búsqueda y reducción | Indexar, filtrar o comparar conjuntos. | Indexadores y listas de hashes. | No se ejecuta análisis de evidencia. |
| Reporte | Organizar resultados y anexos. | Generadores de reportes. | La bitácora sigue siendo responsabilidad del analista. |

### Criterios de selección

La pregunta adecuada no es “¿cuál es la mejor herramienta?”, sino “¿es apta y verificable para esta tarea?”.

| Criterio | Pregunta |
| --- | --- |
| Función | ¿Realiza la tarea necesaria? |
| Fuente y formato | ¿Soporta los datos y la versión correspondientes? |
| Integridad | ¿Reduce cambios y conserva logs o hashes? |
| Validación | ¿Se ha probado con resultados conocidos? |
| Reproducibilidad | ¿Permite registrar configuración, consulta o módulo? |
| Transparencia | ¿Documenta su comportamiento y limitaciones? |
| Seguridad | ¿Introduce red, ejecución o dependencias innecesarias? |
| Compatibilidad | ¿Funciona en Kali y con los recursos disponibles? |
| Procedencia | ¿Se conoce su origen, paquete o proveedor? |
| Licencia y soporte | ¿Está permitido su uso y existe documentación? |

Una interfaz gráfica puede facilitar el aprendizaje y una línea de comandos puede favorecer la repetibilidad. El código abierto permite inspección, pero no garantiza calidad. La popularidad tampoco demuestra aptitud.

### Inventario básico

Registra al menos tres herramientas ya instaladas. No las instales o actualices durante la actividad salvo instrucción expresa.

| Nombre | Versión | Ruta o procedencia | Categoría | Observación |
| --- | --- | --- | --- | --- |
|  |  |  |  |  |

Comandos de apoyo:

```bash
command -v sha256sum
sha256sum --version
```

Para las demás herramientas usa su opción oficial de versión o la información del paquete. Si una herramienta no ofrece una opción de versión uniforme, registra el método utilizado. No abras una fuente ni ejecutes análisis solo para comprobar que el programa existe.

## 13. Validación y verificación

### Validación

Busca demostrar que una herramienta o método es apto para un propósito definido.

Ejemplo: ejecutar una función de hash sobre archivos benignos cuyos valores SHA-256 ya se conocen y comparar el resultado esperado con el obtenido.

### Verificación

Confirma una condición de una ejecución concreta.

Ejemplo: comprobar que el hash de la copia en `trabajo/` coincide con el hash del archivo ubicado en `original/`.

| Pregunta | Validación | Verificación |
| --- | --- | --- |
| ¿Qué se evalúa? | Aptitud de un método o herramienta. | Una ejecución o resultado particular. |
| ¿Con qué? | Datos de prueba y resultados conocidos. | Hashes, tamaños, logs u otras comprobaciones. |
| ¿Qué produce? | Una decisión sobre uso y límites. | Una coincidencia o desviación registrada. |

Para un resultado importante puede utilizarse otra herramienta, un método manual o una fuente independiente. Dos programas que comparten la misma biblioteca pueden repetir el mismo error; la coincidencia entre dos programas no siempre constituye validación independiente.

Se debe registrar herramienta, versión, sistema, configuración, datos de prueba, resultado esperado, resultado obtenido, diferencias, fecha y responsable.

## 14. SHA-256 como comprobación de integridad

Una función hash recibe datos y produce un valor de longitud fija. Una modificación en el contenido normalmente produce un hash diferente. En `LAB-03` se utiliza SHA-256 para:

1. Comprobar que los archivos recibidos coinciden con los valores entregados por el docente.
2. Comprobar que `original/` coincide con lo recibido.
3. Comprobar que la copia en `trabajo/` coincide con `original/`.

Una coincidencia demuestra que las entradas procesadas por el mismo algoritmo produjeron el mismo valor. No demuestra por sí sola autoría, legalidad, ausencia de malware ni que se realizó una adquisición forense completa.

SHA-256 no cifra los datos y no permite recuperar el archivo desde el hash.

### Formato esperado de `LAB-03`

```text
LAB-03/
├── archivo-01.txt
├── archivo-02.txt
└── SHA256SUMS
```

El manifiesto debe contener una línea por archivo:

```text
<hash-sha256>  archivo-01.txt
<hash-sha256>  archivo-02.txt
```

### Verificar lo recibido

```bash
cd /ruta/LAB-03
sha256sum -c SHA256SUMS
```

El resultado esperado es `OK` para cada archivo. Si un hash no coincide:

1. Detente.
2. Conserva el archivo y el valor observado.
3. Registra comando, ruta, hora, esperado y obtenido.
4. Revisa que estés en el directorio correcto.
5. Informa al docente antes de reemplazar o copiar archivos.

### Verificar original y copia de trabajo

```bash
cp -a /ruta/LAB-03/archivo-01.txt ~/caso/original/
cp -a /ruta/LAB-03/archivo-02.txt ~/caso/original/
cp -a /ruta/LAB-03/SHA256SUMS ~/caso/notas/SHA256SUMS-docente

cd ~/caso/original
sha256sum -c ../notas/SHA256SUMS-docente

cp -a ~/caso/original/. ~/caso/trabajo/
(cd ~/caso/original && sha256sum archivo-01.txt archivo-02.txt) > ~/caso/notas/hashes-original.sha256
(cd ~/caso/trabajo && sha256sum archivo-01.txt archivo-02.txt) > ~/caso/notas/hashes-trabajo.sha256
diff -u ~/caso/notas/hashes-original.sha256 ~/caso/notas/hashes-trabajo.sha256
```

La ausencia de salida de `diff` indica que los listados comparados no tienen diferencias. Conserva también las salidas SHA-256 y no registres únicamente “coincide”.

Este procedimiento es una práctica benigna de preparación e integridad. No obtiene sectores, datos eliminados, estructuras completas del disco ni una imagen del Windows objetivo.

## 15. Bitácora de preparación

La bitácora se completa mientras se trabaja. Reconstruirla al final aumenta errores y omisiones.

| Campo | Contenido |
| --- | --- |
| Fecha, hora y zona | Momento exacto de la acción. |
| Responsable | Integrante que ejecutó o verificó. |
| Componente | Host, VirtualBox, Kali, Windows o `LAB-03`. |
| Acción | Qué se revisó o cambió. |
| Propósito | Por qué era necesario. |
| Configuración | Valor anterior y final cuando corresponda. |
| Herramienta | Nombre, versión y comando. |
| Resultado esperado | Condición que se buscaba comprobar. |
| Resultado observado | Salida concreta, coincidencia o fallo. |
| Ubicación | Ruta del log, manifiesto, nota o captura. |
| Desviación | Error, excepción, corrección y autorización. |

Ejemplo:

| Hora | Componente | Acción y propósito | Resultado | Registro |
| --- | --- | --- | --- | --- |
| 09:20 CLT | Kali | Revisar rutas para confirmar ausencia de gateway. | Solo aparece `192.168.77.0/24`; sin ruta predeterminada. | `notas/red-kali.txt` |

### Capturas

Incluye aproximadamente tres o cuatro capturas. Cada una debe indicar:

- Qué componente muestra.
- Qué control demuestra.
- Qué elemento relevante debe observarse.
- Con qué entrada de la bitácora se relaciona.

Una captura sin explicación no es evidencia suficiente de preparación. Siempre que sea posible, conserva comandos y salidas como texto. No incluyas credenciales en las capturas.

## 16. Procedimiento completo de LAB-03

La actividad se distribuye en cinco fases y un total de 4 HPed.

| Fase | Trabajo | Tiempo sugerido | Resultado |
| --- | --- | ---: | --- |
| 1 | Importar y reconocer | 0,75 HPed | Dos VMs identificadas y recursos registrados. |
| 2 | Aislar y probar | 1,00 HPed | Red e integraciones verificadas. |
| 3 | Preparar Kali | 0,75 HPed | Estructura, espacio, herramientas y tiempo registrados. |
| 4 | Verificar integridad | 1,00 HPed | Recibido, original y trabajo coinciden con SHA-256. |
| 5 | Crear estado recuperable y cerrar | 0,50 HPed | Snapshots y bitácora completos. |
| **Total** |  | **4,00 HPed** |  |

### Fase 1 - Importación y reconocimiento

1. Abre la bitácora.
2. Importa el OVA de Windows; no instales Windows desde cero.
3. Con la VM apagada, revisa CPU, RAM, almacenamiento, red e integraciones.
4. Inicia Windows y registra versión, cuenta de laboratorio, hora y zona.
5. Revisa Kali y registra sistema, usuario, recursos y almacenamiento.
6. Identifica Kali como estación analista y Windows como sistema objetivo de práctica.

### Fase 2 - Aislamiento

1. Apaga ambas VMs antes de cambiar adaptadores.
2. Configura Internal Network con el mismo nombre cuando se requiera comunicación.
3. Deshabilita NAT, bridged y adaptadores adicionales.
4. Deshabilita portapapeles, drag and drop y carpetas compartidas.
5. Revisa USB y evita credenciales reales.
6. Inicia las VMs y comprueba direcciones, rutas y comunicación interna.
7. Verifica mediante configuración, rutas y pruebas autorizadas que no exista salida a Internet o a la LAN.
8. Registra toda excepción.

### Fase 3 - Preparación de la estación analista

1. Crea `original/`, `trabajo/`, `exportados/` y `notas/`.
2. Comprueba capacidad y espacio libre de Kali y del host.
3. Registra hora, zona y sincronización de host, Kali y Windows.
4. Inventaría al menos tres herramientas instaladas con versión y procedencia.
5. No instales ni actualices herramientas sin autorización.

### Fase 4 - Verificación de integridad

1. Recibe `LAB-03` por el medio docente.
2. Ejecuta `sha256sum -c SHA256SUMS`.
3. Detente y registra si existe una discrepancia.
4. Copia los archivos verificados a `original/`.
5. Verifica `original/` contra el manifiesto.
6. Crea `trabajo/` y compara sus hashes con `original/`.
7. Protege `original/` contra escritura.
8. Registra comandos, salidas y desviaciones.

### Fase 5 - Estado recuperable

1. Deja ambas VMs en un estado conocido.
2. Crea o verifica snapshots limpios con nombre y descripción.
3. Si no es posible, documenta la alternativa autorizada.
4. Revisa que la bitácora cubra arquitectura, recursos, red, integraciones, tiempo, almacenamiento, herramientas, hashes y snapshots.
5. Guarda tres o cuatro capturas explicadas.

Completa la entrega indicada en [[analisis-forense/leccion-03/actividad]].

## 17. Errores frecuentes

| Error | Por qué es un problema | Corrección |
| --- | --- | --- |
| Suponer que una VM ya está aislada. | Puede conservar NAT, bridged o integraciones con el host. | Revisar y probar cada canal. |
| Importar el OVA e iniciarlo sin revisar red. | Puede generar tráfico no autorizado. | Revisar adaptadores con la VM apagada. |
| Usar Internal Network con nombres distintos. | Kali y Windows quedan en redes separadas. | Usar exactamente el mismo nombre. |
| Dejar un segundo adaptador NAT activo. | Mantiene una ruta de salida aunque el primero sea interno. | Revisar todos los adaptadores. |
| Interpretar un `ping` fallido como prueba total de aislamiento. | El firewall puede bloquear ICMP aunque exista conectividad. | Combinar configuración, rutas y pruebas autorizadas. |
| Habilitar carpetas o portapapeles para “facilitar” la tarea. | Crea transferencias no registradas con el host. | Usar el medio docente y registrar excepciones. |
| Usar credenciales reales. | Puede activar sincronización y exponer datos. | Usar cuentas de laboratorio. |
| Ajustar relojes para que coincidan. | Oculta una diferencia relevante. | Registrar hora, zona, fuente y diferencia. |
| Tratar Kali como una sola herramienta. | Oculta funciones, versiones y limitaciones distintas. | Inventariar herramientas por separado. |
| Analizar directamente `original/`. | Aumenta el riesgo de modificación y confusión. | Trabajar en `trabajo/`. |
| Guardar hashes sin nombre de archivo o comando. | No se puede reproducir la comprobación. | Relacionar archivo, algoritmo, ruta y salida. |
| Continuar después de una discrepancia. | Normaliza un estado no verificado. | Detenerse, registrar e informar. |
| Confundir snapshot con respaldo o imagen forense. | Un snapshot depende de la VM y no representa una adquisición. | Usarlo solo como estado recuperable. |
| Crear demasiadas capturas sin explicación. | No demuestra qué se comprobó. | Seleccionar tres o cuatro y explicarlas. |

## 18. Checklist de preparación

### Host y VirtualBox

- [ ] VirtualBox está disponible y su versión fue registrada.
- [ ] El host tiene capacidad para las VMs y snapshots.
- [ ] El OVA docente se conservó para reutilización.
- [ ] Kali y Windows aparecen como VMs separadas.

### Windows objetivo

- [ ] El OVA fue importado; no se instaló Windows desde cero.
- [ ] CPU, RAM y almacenamiento fueron revisados.
- [ ] Versión, hora, zona y cuenta de laboratorio fueron registradas.
- [ ] La VM está identificada como sistema objetivo de práctica, no como evidencia adquirida.

### Kali analista

- [ ] Sistema, versión, kernel y usuario fueron registrados.
- [ ] CPU, RAM, capacidad y espacio libre fueron comprobados.
- [ ] Se creó `original/trabajo/exportados/notas`.
- [ ] Se inventariaron al menos tres herramientas con versión y procedencia.

### Red e integraciones

- [ ] Se eligió Internal Network o se deshabilitó el adaptador con una justificación.
- [ ] Kali y Windows comparten el mismo nombre de red interna cuando deben comunicarse.
- [ ] NAT, bridged y adaptadores adicionales están deshabilitados.
- [ ] La comunicación interna requerida fue comprobada.
- [ ] No existe conectividad no autorizada hacia Internet o la LAN.
- [ ] Portapapeles y drag and drop están deshabilitados.
- [ ] No existen carpetas compartidas sin justificación.
- [ ] Dispositivos y filtros USB fueron revisados.
- [ ] No se usaron credenciales personales o institucionales.
- [ ] Toda excepción fue registrada y cerrada.

### Tiempo, integridad y almacenamiento

- [ ] Hora y zona de host, Kali y Windows fueron registradas.
- [ ] Diferencias y mecanismos de sincronización fueron documentados.
- [ ] Los relojes no se modificaron artificialmente.
- [ ] Los hashes recibidos de `LAB-03` fueron verificados.
- [ ] `original/` coincide con el manifiesto.
- [ ] `trabajo/` coincide con `original/`.
- [ ] `original/` está protegido contra escritura.
- [ ] Comandos, salidas y desviaciones están registrados.

### Recuperabilidad y entrega

- [ ] Existen snapshots limpios o una alternativa documentada.
- [ ] Cada snapshot tiene nombre, fecha y descripción.
- [ ] La bitácora permite reproducir la configuración.
- [ ] Se incluyeron aproximadamente tres o cuatro capturas explicadas.
- [ ] No se realizó una adquisición completa.
- [ ] No se analizaron artefactos de Windows, memoria, malware o red.

## 19. Autoevaluación

### Preguntas

1. ¿Qué diferencia existe entre host e hipervisor?
2. ¿Por qué Kali es un sistema forense y no una única herramienta?
3. ¿Qué función cumple Windows en la Lección 03?
4. ¿Por qué una VM no equivale automáticamente a aislamiento?
5. ¿Qué modo permite que Kali y Windows se comuniquen sin incluir al host o al exterior?
6. ¿Qué riesgo introducen NAT y bridged?
7. ¿Por qué se revisan portapapeles, drag and drop, carpetas compartidas y USB?
8. ¿Qué diferencia existe entre snapshot, respaldo e imagen forense?
9. ¿Por qué se registra la diferencia horaria en vez de modificar el reloj?
10. ¿Cuál es el propósito de cada directorio del caso?
11. ¿Qué diferencia existe entre validación y verificación?
12. ¿Qué demuestra y qué no demuestra una coincidencia SHA-256?
13. ¿Qué debes hacer si un hash no coincide?
14. ¿Por qué copiar `LAB-03` no es una adquisición forense?
15. ¿Qué información mínima debe permitir reconstruir la bitácora?

### Respuestas orientativas

1. El host es el equipo físico y su sistema; el hipervisor es el software que administra VMs y sus recursos.
2. Porque Kali contiene herramientas distintas y forma parte de un sistema mayor de personas, procedimientos y controles.
3. Es un sistema objetivo autorizado y reutilizable para prácticas; todavía no es evidencia adquirida.
4. Porque puede mantener red, integraciones, dispositivos, tiempo y almacenamiento compartidos con el host.
5. Internal Network, usando el mismo nombre de red en ambas VMs.
6. NAT permite salida mediante el host; bridged conecta la VM directamente a la LAN.
7. Porque son canales de transferencia o contacto entre VM, host y dispositivos físicos.
8. El snapshot restaura un estado local de VM; un respaldo recupera datos ante pérdida; una imagen forense representa una fuente mediante adquisición documentada.
9. Porque la diferencia es contexto que debe conservarse para interpretar y reproducir resultados.
10. `original/` conserva lo recibido; `trabajo/` contiene copias operativas; `exportados/` guarda resultados; `notas/` reúne bitácora y documentación.
11. La validación evalúa aptitud de un método o herramienta; la verificación confirma una ejecución o condición concreta.
12. Demuestra coincidencia de las entradas procesadas con SHA-256; no demuestra autoría, inocuidad, procedencia legal ni adquisición completa.
13. Detenerse, conservar valores, revisar ruta y comando, registrar e informar al docente.
14. Porque solo duplica archivos benignos seleccionados y no aplica un procedimiento completo de adquisición sobre una fuente.
15. Arquitectura, recursos, red, integraciones, tiempo, almacenamiento, herramientas, comandos, resultados, hashes, snapshots, responsables, decisiones y desviaciones.

## 20. Punto de partida de la Lección 04

Al terminar la Lección 03 debe existir:

```text
Laboratorio preparado
        ↓
Windows objetivo + Kali analista
        ↓
Aislamiento verificado
        ↓
Configuración documentada
        ↓
Integridad básica comprendida
        ↓
Snapshots/base limpia
```

La Lección 04 comenzará desde ese punto:

```text
Sistema origen
        ↓
Adquisición forense
        ↓
Imagen/copia forense
        ↓
Hashes
        ↓
Verificación
        ↓
Copia de trabajo
```

En Lección 04 se explicarán estos conceptos y procedimientos. La Lección 03 no adelanta sus comandos, formatos ni herramientas.

Después:

- Lección 05: evaluación de la UA1.
- Lecciones posteriores: análisis de artefactos de sistemas operativos, red, memoria y otros tipos de evidencia.

Preparar bien el laboratorio permite que las acciones posteriores comiencen desde un estado conocido, aislado y documentado.

## Fuentes base y profundización opcional

- [Oracle VirtualBox User Guide 7.2](https://docs.oracle.com/en/virtualization/virtualbox/7.2/user/index.html): documentación oficial del hipervisor.
- [Oracle VirtualBox — Virtual Networking](https://docs.oracle.com/en/virtualization/virtualbox/7.2/user/networkingdetails.html): modos de red y alcance de conectividad.
- [Oracle VirtualBox — Working with Virtual Machines](https://docs.oracle.com/en/virtualization/virtualbox/7.2/user/working-with-vms.html): importación de OVA y snapshots.
- [Oracle VirtualBox — Guest Additions](https://docs.oracle.com/en/virtualization/virtualbox/7.2/user/guestadditions.html): portapapeles, drag and drop y carpetas compartidas.
- [NIST CFTT](https://www.nist.gov/itl/csd/secure-systems-and-applications/computer-forensics-tool-testing-program-cftt): prueba y validación de herramientas forenses.
- [NIST Digital Evidence](https://www.nist.gov/digital-evidence): fundamentos y conjuntos de referencia.
- [Autopsy Documentation](https://www.sleuthkit.org/autopsy/docs/user-docs/): función general de una plataforma de análisis de discos.
- [Volatility 3 Documentation](https://volatility3.readthedocs.io/en/latest/): función general de una plataforma de análisis de memoria.

Fecha de consulta de recursos vivos: 20-08-2026.
