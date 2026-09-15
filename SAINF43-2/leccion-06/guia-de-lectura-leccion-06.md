---
title: "Evaluación y preservación de evidencia digital"
tags:
  - nota
  - course
  - curso
  - materia
  - guia-de-lectura
  - evidencia-digital
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "06"
author: Jordy
start: 2026-09-14
end: 2026-09-16
created_at: 2026-09-13
aliases:
  - "Evaluación y preservación de evidencia digital"
  - "Materia y guía de lectura - Evaluación y preservación de evidencia digital"
---
# Evaluación y preservación de evidencia digital

```toc
```

## Evaluación y preservación

La evaluación de evidencia digital se realiza después de delimitar el encargo, preparar el laboratorio, adquirir una fuente y comprobar su integridad. Su propósito es determinar si lo recibido es útil, qué puede perderse primero, qué amenaza su calidad, quién tiene autoridad sobre la fuente y bajo qué condiciones debe preservarse y almacenarse.

La pregunta central no es «¿qué archivo parece más interesante?», sino:

> ¿Qué decisión sobre una fuente puede justificarse con los datos disponibles, el alcance autorizado y un registro revisable por terceros?

Evaluar evidencia significa examinar cinco aspectos de cada fuente: **origen**, **integridad**, **volatilidad**, **amenazas** y **almacenamiento**. Preservar significa actuar para que esa fuente conserve su valor hasta que pueda analizarse.

## 1. De la fuente a la conclusión

La **evidencia digital** es información almacenada o transmitida en forma digital que puede ayudar a responder una pregunta investigativa. No toda información digital es evidencia: lo es cuando se relaciona con una pregunta autorizada y se obtuvo y conservó de forma que otra persona pueda revisarla.

Una investigación contiene varios niveles que no deben mezclarse:

| Nivel | Pregunta |
| --- | --- |
| Fuente potencial | ¿Dónde podría existir información relacionada con el incidente? |
| Objeto recibido o adquirido | ¿Qué archivo, dispositivo, exportación o imagen se preservó? |
| Dato observado | ¿Qué valor, registro, cabecera o metadato muestra el objeto? |
| Inferencia | ¿Qué explicación es compatible con uno o más datos? |
| Conclusión | ¿Qué respuesta limitada está suficientemente sustentada para el encargo? |

Un evento de inicio de sesión es un dato observado; no identifica por sí solo a la persona que utilizó la cuenta. Una prioridad de preservación es una decisión operativa, no una conclusión sobre el incidente.

Un error frecuente es evaluar el archivo que **describe** una fuente como si fuera la fuente. Un índice de una captura de red no contiene paquetes; el inventario de un respaldo no contiene el respaldo. Ambos sirven para decidir, pero no reemplazan a la fuente que describen.

## 2. Características de la evidencia digital

Las características de la evidencia digital explican por qué no se maneja igual que un objeto físico. Cada una genera una exigencia concreta para el analista.

| Característica | Qué significa | Exigencia para el analista |
| --- | --- | --- |
| Intangible | No se ve directamente; necesita hardware y software para leerse | Registrar con qué herramienta y versión se leyó |
| Frágil | Cambia con acciones cotidianas: iniciar sesión, abrir un archivo, sincronizar, reiniciar | Minimizar cambios y registrar los inevitables |
| Volátil | Puede desaparecer rápido | Decidir temprano qué preservar |
| Replicable | Los bytes se copian sin pérdida, pero la copia no trae el contexto (quién exportó, filtros, zona horaria) | Verificar con hash y preservar el contexto por separado |
| Alterable sin rastro visible | Un cambio puede no dejar huella evidente | Fijar una referencia de integridad lo antes posible |
| Dependiente del entorno | Requiere formato, clave, proveedor o sistema para interpretarse | Preservar el formato y las condiciones de lectura |
| Dispersa | Está repartida en varios sistemas y responsables | Evaluar cada fuente y declarar lo que falta |

### Volatilidad y orden de volatilidad

La volatilidad expresa con qué facilidad o rapidez puede desaparecer o cambiar la información. La memoria RAM depende de la energía; una captura circular se sobrescribe; una plataforma puede conservar auditoría durante pocos días.

La guía RFC 3227 propone preservar primero lo que desaparece antes. De mayor a menor volatilidad:

1. registros y caché del procesador;
2. tabla de rutas, caché ARP, tabla de procesos y memoria RAM;
3. sistemas de archivos temporales;
4. disco;
5. registros remotos y datos de monitoreo;
6. configuración física y topología de red;
7. medios de archivo o respaldo.

Volátil no significa automáticamente «más valioso», y el orden de volatilidad no es una regla automática. Una fuente muy volátil puede quedar después si no hay autorización o personal competente para tocarla, o si intervenir pone en riesgo un servicio. En esos casos la decisión correcta es mantener el estado y escalar.

## 3. Criterios para evaluar una fuente

Estos criterios se aplican en conjunto. Un hash correcto no compensa una fuente irrelevante, y una fuente relevante no debe presentarse como íntegra si su transferencia no puede verificarse.

### Marco normativo

- **ISO/IEC 27037** trata la identificación, recolección, adquisición y preservación. Propone que la evidencia sea **relevante** (ayuda a responder la pregunta), **confiable** (lo que muestra corresponde a la fuente) y **suficiente** (alcanza para sostener la afirmación).
- **ISO/IEC 27042** trata el análisis y la interpretación. Exige métodos válidos para su propósito y un trabajo **repetible** (la misma persona obtiene el mismo resultado), **reproducible** (otra persona obtiene el mismo resultado) y revisable por un tercero.

### Origen y autenticidad

El examen del origen establece quién produjo el objeto, desde qué sistema, mediante qué función y en qué momento. La autenticidad no se sostiene con el nombre del archivo. Requiere procedencia, identificadores, metadatos y documentación compatibles.

- ¿Qué sistema generó el dato y quién lo exportó?
- ¿Se entregó en formato nativo o se transformó (copiar y pegar, captura de pantalla, reenvío)?
- ¿Qué filtros, período y cuentas se incluyeron?

### Integridad

El examen de integridad determina si el objeto actual conserva los bytes que se pretendía entregar y si los cambios ocurridos están explicados. Tiene dos partes:

1. **Integridad técnica:** el hash actual coincide con una referencia. Si el hash se calculó al recibir y no en el origen, demuestra que el archivo no cambió desde la recepción, no desde su creación.
2. **Integridad de procedimiento:** existe registro de quién tuvo el objeto, cuándo y qué hizo con él.

### Riesgo de alteración

Es la probabilidad de que la fuente cambie antes de preservarse o durante su manejo. Depende de la volatilidad, de si el sistema sigue operando y de cuántas personas tienen acceso. Una fuente estable en manos de un custodio confiable tiene bajo riesgo; un registro local en un equipo comprometido tiene riesgo alto.

### Relevancia y suficiencia

Una fuente es **relevante** cuando puede ayudar a responder una pregunta autorizada. Que contenga muchos datos no la vuelve pertinente.

La **suficiencia** se evalúa respecto de una afirmación. Un correo sin la cadena completa de transporte puede servir como indicio, pero ser insuficiente para validar su ruta. Una fuente relevante puede ser todavía insuficiente.

### Trazabilidad

Cada afirmación debe conducir al objeto, la operación y el registro que la sustentan:

```text
conclusión -> inferencia -> dato -> archivo o fuente -> método -> registro
```

Si se rompe esa ruta, un tercero no puede revisar el razonamiento.

### Sensibilidad

Indica qué daño produciría exponer la fuente: datos personales, comunicaciones privadas, credenciales o información de terceros ajenos al caso. No reduce el valor de la fuente, pero exige más control de acceso, cifrado y minimización.

## 4. Valor de la evidencia

El valor de una fuente no es una propiedad fija. Resulta de combinar los criterios anteriores frente a una pregunta concreta: una fuente relevante, íntegra y con procedencia clara vale más que otra con más datos pero sin origen verificable.

En esta etapa se estima el **valor potencial**: lo que la fuente podría aportar si se preserva y analiza. Afirmar que una fuente «prueba» algo antes de analizarla es un error.

| Aumenta el valor | Reduce el valor |
| --- | --- |
| Responde directamente a una pregunta autorizada | No se relaciona con la pregunta |
| Procedencia documentada y formato nativo | Origen desconocido o transformado |
| Integridad verificable | Hash ausente o discrepante sin explicación |
| Es única: no hay otra copia equivalente | Duplica información disponible en otra fuente |
| Cubre el período del incidente | Es anterior o posterior al período de interés |

| Nivel | Criterio orientativo |
| --- | --- |
| Alto | Relevante para la pregunta principal, cubre el período y su integridad es verificable o puede asegurarse. |
| Medio | Relevante, pero incompleta, indirecta o con una condición pendiente. |
| Bajo | Relación débil con la pregunta, fuera del período o sin forma de verificar su origen. |

El valor también cambia con el tiempo: un respaldo creado antes del incidente sirve como **estado previo** para comparar, no como registro de lo ocurrido después. Y una fuente valiosa que no se preserva a tiempo se pierde.

## 5. Propiedad, custodia y responsabilidad

### Roles

| Rol | Responsabilidad típica |
| --- | --- |
| Propietario | Tiene autoridad sobre el sistema o los datos y autoriza su tratamiento. |
| Custodio | Mantiene o administra la fuente: infraestructura, redes, SaaS, correo o base de datos. |
| Proveedor | Controla infraestructura, formatos de exportación y retención en servicios externos. |
| Analista | Ejecuta únicamente el método autorizado, registra acciones y comunica límites. |
| Titular de los datos | Persona a quien se refieren los datos personales contenidos en la fuente. |

El custodio no siempre es el propietario. Un proveedor SaaS administra la plataforma, mientras la organización es responsable de sus datos corporativos. El administrador de base de datos custodia el respaldo, pero no decide por sí solo entregarlo.

### Tipos de propiedad

| Situación | Quién suele autorizar | Consideración |
| --- | --- | --- |
| Equipo y datos de la organización | La organización, por medio de quien tiene autoridad | Verificación de que la solicitud provenga de quien puede autorizar |
| Servicio contratado a un tercero | La organización, dentro de lo que permite el contrato | El proveedor controla formato, retención y acceso |
| Dispositivo personal usado para trabajar | La persona dueña o una autoridad competente | La organización no puede adquirirlo solo porque contiene datos laborales |
| Datos de terceros (clientes, proveedores) | El responsable de esos datos | Obtención limitada a lo necesario para la pregunta |

### Consecuencias prácticas

- La preservación requiere identificar **quién puede autorizar** y **quién puede ejecutar**. No siempre son la misma persona.
- Si la propiedad es dudosa o compartida, la decisión correcta es detenerse y escalar, no adquirir «por si acaso».
- La presencia de datos personales exige mínimo acceso y minimización.
- En Chile, el tratamiento de datos personales está regulado por la Ley 19.628, que será reemplazada en lo sustancial por la Ley 21.719 al entrar en vigencia en diciembre de 2026. El analista no interpreta la ley por su cuenta: reconoce la restricción y la consulta con el área responsable.

## 6. Amenazas a la evidencia

Una amenaza es cualquier situación que puede hacer que la evidencia se pierda, cambie o deje de ser confiable. Según su origen pueden ser:

- **técnicas:** apagado, rotación de registros, vencimiento de la retención, falla del medio;
- **humanas accidentales:** abrir el original, reiniciar, reenviar un correo, exportar con filtros equivocados;
- **humanas intencionales:** borrar o modificar registros para dificultar la investigación;
- **de procedimiento:** falta de registro, transferencia sin hash, acceso de personas no autorizadas.

| Amenaza | Efecto posible | Control inicial |
| --- | --- | --- |
| Apagado o reinicio | Pérdida de RAM o bloqueo de volumen cifrado | Documentar estado; no improvisar; escalar. |
| Rotación o retención corta | Sobrescritura de capturas o registros | Preservación temprana y registro de la ventana. |
| Exportación con filtros | Omisión de campos, usuarios o período | Conservar parámetros y alcance de la exportación. |
| Reenvío o captura de pantalla | Pérdida de cabeceras o metadatos | Solicitar formato nativo y conservar procedencia. |
| Transferencia sin control | Imposibilidad de demostrar igualdad | Hash en origen y en recepción cuando sea viable. |
| Hash discrepante | Objeto distinto, error o cambio no explicado | Detener, aislar, repetir la verificación e informar. |
| Relojes y zonas distintas | Orden de hechos incorrecto | Conservar el tiempo original y registrar su zona. |
| Acceso excesivo | Exposición de datos ajenos al caso | Mínimo privilegio y registro de accesos. |
| Actor interesado con acceso | Borrado o modificación deliberada | Preservar antes de alertar y restringir su acceso. |

## 7. Cómo priorizar preservación

No existe un orden universal. La prioridad se justifica comparando al menos cinco factores:

1. **Ventana de pérdida:** ¿cuándo se apaga, rota, sobrescribe o elimina?
2. **Unicidad:** ¿existe otra copia equivalente y verificable?
3. **Relevancia:** ¿qué pregunta autorizada podría responder?
4. **Dependencia:** ¿se requiere un proveedor, una sesión activa o una clave?
5. **Restricciones:** ¿existen autorización, competencia y capacidad para actuar?

Una decisión útil puede ser «solicitar preservación inmediata» aunque el analista no esté autorizado para adquirir. Detenerse no es omitir el deber: es reconocer el límite y escalar antes de causar una alteración irreversible.

La prioridad no siempre es una lista de uno en uno. Si dos fuentes urgentes dependen de responsables distintos, ambas acciones pueden iniciarse a la vez.

| Fuente | Riesgo | Decisión defendible |
| --- | --- | --- |
| RAM de un servidor activo | Pérdida al apagar; cambios continuos | Mantener estado y solicitar adquisición urgente por personal competente. |
| Captura circular de red | Sobrescritura en minutos | Congelar o exportar el intervalo autorizado antes de la rotación. |
| Auditoría de un servicio externo | Retención de días y dependencia de tercero | Solicitar preservación temprana y exportación documentada. |
| Respaldo estable | Retención conocida y repositorio controlado | Proteger acceso y programar copia verificada después de las fuentes urgentes. |

## 8. Preservación como plan verificable

Preservar no siempre significa copiar. Puede significar **mantener un estado** (no apagar un equipo), **impedir la eliminación** (pedir que no se borren datos aunque venza su retención, lo que en inglés se llama *legal hold*) u **obtener una copia verificada**.

Para cada fuente, el plan debería responder:

| Elemento | Pregunta |
| --- | --- |
| Identificador | ¿Cómo se distingue de todas las demás? |
| Alcance | ¿Qué se autoriza obtener y qué queda fuera? |
| Responsable | ¿Quién ejecuta y quién autoriza? |
| Método y formato | ¿Cómo se preservará sin perder estructura ni contexto? |
| Integridad | ¿Cuál será la referencia y cuándo se calculará? |
| Acceso y almacenamiento | ¿Quién puede ver o copiar, y dónde se conserva? |
| Retención | ¿Qué política o instrucción autorizada define el plazo? |
| Condición de detención | ¿Qué situación obliga a suspender y escalar? |

La retención no la inventa el analista: proviene del encargo, de una política o de una decisión autorizada. Cifrar una evidencia protege su confidencialidad, pero no reemplaza inventario, hash ni trazabilidad.

La **condición de detención** distingue un plan responsable de una lista de tareas. Ejemplos: «detener si el método puede bloquear el volumen cifrado», «detener si la exportación incluye cuentas fuera del alcance», «detener si no hay personal competente disponible».

## 9. Almacenamiento de la evidencia

### Capacidad necesaria

Antes de preservar hay que saber si existe espacio suficiente y protegido. Una estimación mínima considera:

1. el tamaño de cada fuente que se va a preservar, no el del archivo que la describe;
2. una copia preservada y una copia de trabajo;
3. un margen para registros y verificaciones.

Ejemplo con un notebook ficticio de 512 GiB de disco y 8 GiB de RAM:

```text
Fuentes: 512 GiB + 8 GiB = 520 GiB
x 2 (copia preservada + copia de trabajo) = 1040 GiB
+ 20 % de margen = 1248 GiB -> se necesita un medio de al menos 1,5 TB
```

Una imagen física ocupa el tamaño completo del disco, no solo el espacio usado, y un volumen cifrado casi no se comprime. Por ello, la planificación considera el tamaño completo.

### Condiciones de resguardo

| Condición | Práctica |
| --- | --- |
| Separación | Copia preservada y copia de trabajo en ubicaciones distintas |
| Confidencialidad | Cifrado cuando hay datos sensibles; la clave se gestiona aparte |
| Control de acceso | Solo las personas asignadas al caso, con cuentas nominativas |
| Registro de accesos | Quién accedió, cuándo y para qué |
| Identificación | Etiqueta con caso, identificador de evidencia, fecha y hash |
| Verificación periódica | Recalcular hashes para detectar degradación del medio |

Cuando vence el plazo autorizado, la evidencia se elimina según la política y se registra quién lo autorizó. Mientras no exista esa decisión, se conserva.

## 10. Aceptar, condicionar o aislar

Al recibir o evaluar una fuente se le asigna un estado operativo:

- **Aceptada para el propósito declarado:** identificada, dentro de alcance, con procedencia suficiente y controles coherentes.
- **Aceptada con condición:** conserva valor, pero falta una confirmación obtenible (cabeceras completas, autorización, hash de origen, parámetros de exportación). La condición queda explícita antes de usarla para una afirmación.
- **Aislada por discrepancia:** el examen se detiene por un fallo no explicado, por ejemplo un hash distinto. Se conserva el objeto, el manifiesto y la salida del control, y se solicita una decisión.
- **Pendiente de adquisición:** la fuente fue evaluada, pero todavía no existe un objeto preservado.

Aislar no significa borrar ni ocultar. El término «rechazada» no equivale a «eliminada»: una fuente problemática también forma parte de la historia del procedimiento.

## 11. Qué demuestra cada control técnico

| Control | Permite afirmar | No permite afirmar |
| --- | --- | --- |
| `file` | Tipo que la herramienta reconoce por contenido o estructura | Que el contenido sea verdadero o auténtico |
| `stat` | Tamaño y marcas del sistema de archivos donde está la copia | Fecha real del hecho investigado |
| `sha256sum -c` | Coincidencia o discrepancia con el manifiesto indicado | Quién creó el archivo o por qué discrepó |
| Cabeceras de un EML | Campos presentes en el archivo recibido | Que no existan cabeceras omitidas en el origen |
| Conteo de filas | Volumen aparente del extracto | Cobertura completa del sistema fuente |

Ante un `FAILED`, las causas posibles incluyen: el archivo cambió, el manifiesto tiene un error, se entregó otra versión, la transferencia se corrompió o se verificó desde la carpeta equivocada. El resultado solo demuestra que **los valores difieren**.

## 12. Registro técnico breve

Un registro técnico breve separa las acciones del razonamiento mediante la siguiente estructura:

1. **Alcance:** pregunta, autorización y exclusiones.
2. **Evidencia evaluada:** identificadores, fuente, custodio y objetos recibidos.
3. **Método:** controles realizados, herramientas, versión, fecha y zona.
4. **Resultados:** salidas verificables, incluidas discrepancias.
5. **Decisión de preservación:** prioridad, estado y controles propuestos.
6. **Limitaciones:** metadatos, autorización, competencia o contexto ausente.
7. **Conclusión:** respuesta acotada a la pregunta de esta etapa.

Ejemplo de redacción:

> El archivo X no coincide con el SHA-256 consignado en el manifiesto recibido. Se aisló sin modificar y no se utilizó para inferir actividad del usuario. La causa de la discrepancia no fue determinada; se recomienda confirmar el hash en origen y obtener una nueva exportación documentada.

Ese texto informa un resultado, una acción y un límite. No inventa una causa.

## 13. Ejemplo resuelto - caso LAGUNA SPA (ficticio)

A las 10:05 de un martes, el área de finanzas de LAGUNA SPA informa que una planilla de pagos fue modificada. Se autoriza evaluar fuentes y proponer preservación.

| ID | Fuente | Origen y custodio | Volatilidad | Valor potencial | Estado |
| --- | --- | --- | --- | --- | --- |
| F1 | Notebook de contabilidad encendido (512 GiB de disco, 8 GiB de RAM) | Empresa; TI | RAM alta; disco media | Alto: equipo donde se editó la planilla | Pendiente de adquisición |
| F2 | Registros del firewall | Empresa; redes | Alta: se pierden en 48 horas | Medio: muestra conexiones, no contenido | Pendiente de adquisición |
| F3 | Versiones anteriores de la planilla en el servidor de archivos | Empresa; TI | Media: se guardan 14 días | Alto: puede mostrar el contenido previo | Pendiente de adquisición |
| F4 | Exportación VPN entregada por TI | Empresa; TI | Baja: ya exportada | Medio: hash OK, pero sin filtros ni zona horaria declarados | Aceptada con condición |
| F5 | Teléfono personal del contador | **Propiedad personal** | Media | Posiblemente medio | Fuera de alcance: escalar |

**Prioridad:**

1. F2 y F1 en paralelo: redes preserva los registros antes de 48 horas; TI mantiene el notebook encendido y personal competente adquiere RAM y luego disco.
2. F3: solicitar que no se eliminen las versiones de la planilla.
3. F4: pedir los parámetros de exportación y la zona horaria antes de usarla.
4. F5: no se adquiere; requiere consentimiento de su dueño o una autoridad competente.

**Almacenamiento:** (8 + 512 + ~1 + ~1) GiB = ~522 GiB; x 2 = ~1044 GiB; + 20 % = ~1253 GiB. Se propone un medio de 2 TB cifrado, con acceso solo del equipo del caso.

**Conclusión de la etapa:**

> Se evaluaron cinco fuentes. F1 y F2 requieren preservación inmediata en paralelo por volatilidad; F3 tiene una ventana de 14 días; F4 queda condicionada a conocer filtros y zona horaria; F5 es un dispositivo personal fuera del alcance autorizado. Estas decisiones no identifican a quién modificó la planilla.

## 14. Glosario

| Término | Definición |
| --- | --- |
| Condición de detención | Situación que obliga a suspender una acción de preservación y escalar. |
| Custodio | Persona o área que mantiene y resguarda la fuente. |
| Fuente potencial | Lugar donde podría existir información útil para la pregunta. |
| Minimización | Obtener y exponer solo los datos necesarios para la pregunta. |
| Orden de volatilidad | Criterio para preservar primero lo que desaparece antes. |
| Propietario | Quien tiene autoridad sobre el sistema o los datos y puede autorizar su tratamiento. |
| Relevancia | Relación de una fuente con una pregunta autorizada. |
| Riesgo de alteración | Probabilidad de que la fuente cambie antes o durante su manejo. |
| Suficiencia | Respaldo bastante para sostener una afirmación concreta. |
| Valor potencial | Aporte que se estima que una fuente puede hacer antes de analizarla. |
| Volatilidad | Rapidez o facilidad con que la información puede desaparecer o cambiar. |

## Fuentes

- [ISO/IEC 27037:2012 - Guidelines for identification, collection, acquisition and preservation of digital evidence](https://www.iso.org/standard/44381.html). Principios de relevancia, confiabilidad y suficiencia.
- [ISO/IEC 27042:2015 - Guidelines for the analysis and interpretation of digital evidence](https://www.iso.org/standard/44406.html). Validez, repetibilidad, reproducibilidad y revisión independiente.
- [RFC 3227 - Guidelines for Evidence Collection and Archiving](https://www.rfc-editor.org/rfc/rfc3227). Orden de volatilidad.
- [NIST - Digital Evidence Preservation: Considerations for Evidence Handlers](https://www.nist.gov/itl/csd/secure-systems-and-applications/computer-forensics-tool-testing-program-cftt/digital). Problemas particulares de preservación de evidencia digital.
- [SWGDE 18-F-002-2.0 - Best Practices for Digital Evidence Collection](https://www.swgde.org/documents/published-complete-listing/18-f-002-2-0/). Integridad, estado de dispositivos, inventario y documentación.
- [Ley Chile - Biblioteca del Congreso Nacional](https://www.bcn.cl/leychile). Leyes 19.628 y 21.719: marco chileno de datos personales.
