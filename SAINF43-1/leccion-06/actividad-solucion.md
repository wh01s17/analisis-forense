---
title: "Solución Actividad Lección 06 - Mesa de evaluación y preservación de evidencia"
tags:
  - nota
  - course
  - curso
  - actividad
  - solucion
  - docente
  - reservado
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "06"
author: Jordy
start: 2026-09-14
end: 2026-09-16
created_at: "2026-08-07 11:56"
aliases:
  - "Solución Actividad Lección 06 - Mesa de evaluación y preservación de evidencia"
---
# Actividad Lección 06 - Mesa de evaluación y preservación de evidencia — Solución

- **Modalidad:** equipos de tres, con roles estables.
- **Caso:** `CSA-2026-044` - COSTA AZUL SPA (ficticia).
- **Carácter:** formativo, sin calificación.
- **Producto:** una ficha técnica compacta.

## Pregunta del caso

> ¿Qué fuentes deben preservarse primero y bajo qué condiciones pueden aceptarse para un análisis posterior?

La actividad evalúa decisiones de preservación. No busca reconstruir el incidente ni identificar a una persona.

## Lo que debes demostrar

1. Interpretar una verificación de integridad sin inventar su causa.
2. Evaluar seis fuentes por origen, volatilidad, amenaza y valor potencial.
3. Distinguir propietario, custodio, fuente original y objeto recibido.
4. Proponer preservación con responsable, autorización, integridad y condición de detención.
5. Estimar almacenamiento, considerar la sensibilidad, controlar el acceso y comunicar límites.

## Material y roles

Usa solamente:

- `material/acta-recepcion.md`;
- `material/inventario-fuentes.csv`;
- `material/manifest_entrega.sha256`;
- los cuatro archivos de `material/exportados/`.

| Rol | Responsabilidad |
| --- | --- |
| Operador | Ejecuta los comandos mínimos autorizados. |
| Registrador | Completa la ficha y conserva la salida relevante. |
| Revisor | Comprueba alcance, responsables y límites de las conclusiones. |

Mantengan los roles durante toda la actividad.

## Reglas

```text
Trabaja sobre una copia local del material.
No accedas a sistemas, cuentas ni dispositivos reales.
No modifiques el manifiesto ni los archivos recibidos.
Ante una discrepancia: registra, aísla y escala.
Una prioridad de preservación no demuestra culpabilidad ni intención.
```

## Fase 1 - Alcance e integridad

1. Lean `material/acta-recepcion.md`.
2. Registren integrantes, roles, una acción permitida y una prohibida.
3. Desde `analisis-forense/leccion-06/`, creen la copia y ejecuten solo estos controles:

```bash
mkdir -p ~/forense/LAB-06
cp -a material/. ~/forense/LAB-06/
cd ~/forense/LAB-06
sha256sum -c manifest_entrega.sha256
sed -n '1,/^$/p' exportados/correo-alerta.eml
```

Si un comando falla, consulten la guía técnica o informen al docente. No regeneren el manifiesto.

**Registro de ejemplo:** integrante A, operador; integrante B, registrador; integrante C, revisor. Acción permitida: verificar los archivos recibidos contra el manifiesto. Acción prohibida: reemplazar o regenerar el manifiesto.

### E1 - Registro mínimo

```text
Archivo con discrepancia: exportados/auditoria-saas.csv.
Archivos que coinciden con el manifiesto: correo-alerta.eml, pcap-indice.csv y respaldo-bd-inventario.csv.
La discrepancia demuestra que: el hash actual del archivo no coincide con la referencia del manifiesto.
La discrepancia no permite afirmar que: hubo manipulación intencional ni cuál fue la causa de la diferencia.
Cabecera ausente y limitación del EML: no contiene Received; no permite reconstruir la ruta completa de transporte.
```

## Fase 2 - Evaluación de las seis fuentes

Lean `material/inventario-fuentes.csv`. No copien nuevamente propietario, custodio, tamaño ni retención: consúltenlos allí y completen solo las decisiones.

| ID       | Volatilidad y amenaza principal                                                                 | Valor potencial y sensibilidad                                                 | Estado o acción inmediata y responsable                                                                                       |
| -------- | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------- |
| EV-06-01 | Alta: la RAM cambia durante el uso y se pierde al apagar o reiniciar.                           | Alto: puede contener procesos, conexiones y secretos; sensibilidad alta.       | Pendiente de adquisición: infraestructura mantiene el equipo encendido y un especialista autorizado adquiere la RAM.          |
| EV-06-02 | Alta: el búfer circular se sobrescribe a las 08:55.                                             | Alto: puede registrar actividad de red e incluye comunicaciones de terceros.   | Preservación inmediata: redes exporta la PCAP del intervalo autorizado antes de la sobrescritura.                             |
| EV-06-03 | Media: depende de un tercero y tiene siete días de retención; además presenta una discrepancia. | Alto: registra actividad de cuentas e IP; contiene identificadores.            | Aislada por discrepancia: el administrador SaaS y el proveedor confirman el hash en origen y generan una entrega documentada. |
| EV-06-04 | Baja en la copia recibida; la amenaza es la pérdida de cabeceras y contexto.                    | Medio: sirve como alerta y contiene una comunicación privada.                  | Aceptada con condición: correo preserva el mensaje nativo con cabeceras completas.                                            |
| EV-06-05 | Baja relativa: el respaldo permanece 30 días; existe riesgo de vencimiento o acceso excesivo.   | Medio como estado previo; contiene datos personales y tiene sensibilidad alta. | Pendiente: el administrador de base de datos protege el respaldo y prepara una copia minimizada cuando exista autorización.   |
| EV-06-06 | Media-alta: cambia con la actividad y el volumen cifrado puede bloquearse al reiniciar.         | Alto: puede contener artefactos del sistema; sensibilidad alta.                | Pendiente: infraestructura mantiene el estado y un especialista autorizado adquiere el disco después de la RAM.               |

### Tres comprobaciones obligatorias

Respondan con una oración cada una:

1. Elijan EV-06-03 o EV-06-05: ¿quién es propietario, quién es custodio y quién debe autorizar?

   **Respuesta:** en EV-06-05, COSTA AZUL SPA es propietaria, el administrador de base de datos es custodio y debe autorizar la persona o área competente de la organización responsable de los datos.

2. ¿Por qué `pcap-indice.csv` o `respaldo-bd-inventario.csv` no equivale a la fuente original?

   **Respuesta:** porque el índice solo describe la ventana de captura y el inventario solo describe el respaldo; no contienen los paquetes ni los datos respaldados.

3. Para una fuente, completen: «El dato disponible permite decidir ____, pero antes de afirmar ____ falta ____».

   **Respuesta:** el índice de EV-06-02 permite decidir que la PCAP debe preservarse antes de las 08:55, pero antes de afirmar qué tráfico contiene falta obtener y verificar la captura original.

## Fase 3 - Prioridad y preservación

Seleccionen las dos fuentes que requieren la primera acción. Pueden compartir prioridad si intervienen responsables distintos.

| Nivel | ID | Razón: pérdida + valor | Acción y responsable | Condición previa o de detención |
| ---: | --- | --- | --- | --- |
| 1A | EV-06-02 | El búfer se sobrescribe a las 08:55 y puede contener actividad de red relevante. | Redes exporta la PCAP nativa del intervalo autorizado. | Detener si la exportación excede el intervalo, altera el sensor o no hay personal autorizado. |
| 1B | EV-06-01 | La RAM se pierde al apagar y puede contener procesos, conexiones y secretos. | Infraestructura mantiene el servidor encendido y un especialista autorizado adquiere la RAM. | Detener si falta autorización, no se confirma el equipo o existe un riesgo operativo no controlado. |

Formulen un plan breve para esas dos fuentes. No ejecuten las acciones propuestas.

| ID | Método o formato | Control de integridad | Almacenamiento, acceso y conservación | Detener si... |
| --- | --- | --- | --- | --- |
| EV-06-02 | PCAP nativa del intervalo autorizado, junto con la configuración y hora del sensor. | Hash en origen y recepción; registro del intervalo y tamaño. | Copia preservada y copia de trabajo cifradas, acceso por caso y conservación según política. | La exportación excede el alcance, altera el sensor o no hay personal competente. |
| EV-06-01 | Adquisición en vivo con herramienta validada y registro de cambios inevitables. | Hash del archivo producido y bitácora de herramienta, versión y hora. | Dos copias cifradas con acceso restringido por la posible presencia de secretos. | Falta autorización, el equipo no coincide o el riesgo operativo no está controlado. |

### Estimación de almacenamiento

Usen `tamano_estimado` de las dos fuentes seleccionadas:

```text
Suma de las fuentes: 18 GiB (2 GiB de PCAP + 16 GiB de RAM)
x 2 (copia preservada + copia de trabajo): 36 GiB
+ 20 % de margen: 43,2 GiB
Medio, cifrado y control de acceso propuestos: unidad o repositorio de al menos 64 GiB, cifrado y restringido al equipo del caso.
```

No usen el tamaño del índice o inventario como si fuera el tamaño de la fuente.

## Fase 4 - Conclusión y salida

### Conclusión del equipo

Redacten brevemente e incluyan:

- alcance y método;
- resultado de integridad;
- dos decisiones de prioridad;
- una consideración de propiedad o almacenamiento;
- una limitación.

Cierren con esta idea, usando sus propias palabras:

> El plan decide cómo preservar fuentes potenciales; no demuestra quién realizó la actividad ni si fue maliciosa.

**Respuesta:**

> El equipo evaluó el paquete recibido sin acceder a sistemas activos. La verificación produjo tres coincidencias y una discrepancia en `auditoria-saas.csv`, cuya causa no puede determinarse con los datos disponibles. Se priorizó la exportación de EV-06-02 antes de su sobrescritura y la adquisición autorizada de EV-06-01 sin apagar el servidor. Ambas fuentes requieren 43,2 GiB para dos copias y margen, en almacenamiento cifrado con acceso restringido. El EML permanece condicionado por la ausencia de `Received`. Este plan establece cómo preservar fuentes potenciales, pero no identifica a una persona ni demuestra intención maliciosa.
