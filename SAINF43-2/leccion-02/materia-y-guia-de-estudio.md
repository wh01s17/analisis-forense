---
title: "Materia y guía de estudio - Proceso de investigación forense"
tags:
  - nota
  - course
  - curso
  - materia
  - guia-de-estudio
  - proceso-forense
  - materia-y-guia-de-estudio
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "02"
author: Jordy
start: 2026-08-17
end: 2026-08-19
created_at: "2026-08-06 17:35"
aliases:
  - "Materia y guía de estudio - Proceso de investigación forense"
---

# Materia y guía de estudio - Proceso de investigación forense

```toc
```

## Propósito de esta guía

Esta guía contiene todo el contenido conceptual necesario para la lección 02. Su objetivo es aprender a diseñar un proceso forense autorizado, ordenado, verificable y orientado a preguntas. Sirve por igual para la actividad A (`actividad.md`) y la actividad B (`actividad-02.md`); cada sección resolverá solo una.

Las fuentes externas del final son opcionales. Para preparar la clase basta estudiar esta guía y resolver la versión asignada.

### Alcance de esta lección

En esta sesión se decide **qué debe ocurrir, en qué orden lógico y qué producto deja cada fase**. Todavía no se seleccionan herramientas, no se prepara una estación forense, no se crea una imagen forense, no se ejecutan comandos hash y no se redacta un informe completo. Esos procedimientos se desarrollan en las lecciones 03, 04 y 12.

## Ruta mínima de preparación

Estudia en este orden:

1. Propósito y mapa general del proceso.
2. Objetivo, alcance, preguntas y planificación.
3. Identificación y propósito conceptual de adquisición/preservación.
4. Diferencia entre examen, análisis y conclusión.
5. Documentación transversal: hash, custodia y bitácora.
6. Caso de la versión asignada.
7. Autoevaluación.

### Criterio de suficiencia

Estás preparado cuando puedes:

- Ordenar las fases y explicar por qué una precede o alimenta a otra.
- Indicar la entrada, acción y producto de cada fase.
- Mantener las acciones dentro del alcance autorizado.
- Explicar por qué identificar, adquirir y preservar no son sinónimos.
- Distinguir examen, análisis, hallazgo y conclusión.
- Reconocer que la documentación ocurre durante todo el proceso.

## Objetivos de estudio

Al finalizar deberías poder:

- Formular un objetivo, alcance y preguntas investigativas.
- Identificar fuentes y explicar cuáles requieren atención temprana, sin seleccionar todavía una técnica de adquisición.
- Diferenciar identificación, adquisición, preservación, examen, análisis y presentación.
- Asociar cada fase con un producto verificable y un riesgo de ejecución.
- Explicar las funciones complementarias de hashes, cadena de custodia y bitácora.
- Diseñar un proceso reproducible que admita iteraciones justificadas.

## 1. ¿Por qué se necesita un proceso?

Una investigación sin proceso puede alterar información, exceder la autorización, perder contexto o transformar una sospecha en conclusión antes de revisar la evidencia.

El proceso permite que las decisiones sean:

- **Autorizadas:** cada acción tiene fundamento y responsable.
- **Pertinentes:** responde a una pregunta del caso.
- **Controladas:** protege fuentes y registra cambios.
- **Trazables:** permite reconstruir el tratamiento de la evidencia.
- **Revisables:** otra persona puede comprobar el trabajo.
- **Comunicables:** el resultado responde al objetivo y declara límites.

Seguir un proceso no garantiza que toda pregunta pueda responderse. Su valor está en reducir errores y explicar de manera transparente qué se hizo, qué se encontró y qué no fue posible determinar.

## 2. Mapa general del proceso

NIST SP 800-86 agrupa el trabajo en **recolección, examen, análisis y reporte**. El programa utiliza una secuencia más detallada. No existe contradicción: identificación, adquisición y preservación forman parte del tratamiento inicial de las fuentes.

Para el curso utilizaremos este mapa:

> **Solicitud y autorización → planificación → identificación → adquisición y preservación → examen → análisis → presentación y cierre**

La documentación acompaña todas las fases.

| Fase | Pregunta principal | Producto esperado |
| --- | --- | --- |
| Solicitud y autorización | ¿Quién solicita qué y con qué autoridad? | Encargo y alcance autorizados. |
| Planificación | ¿Cómo responderemos sin exceder el alcance? | Plan, roles, prioridades y contingencias. |
| Identificación | ¿Dónde podrían existir datos pertinentes? | Inventario priorizado de fuentes. |
| Adquisición y preservación | ¿Cómo obtendremos y protegeremos los datos? | Adquisiciones identificadas y verificables. |
| Examen | ¿Qué datos relevantes pueden localizarse y organizarse? | Artefactos, tablas y resultados referenciados. |
| Análisis | ¿Qué significan los resultados en relación con las preguntas? | Hallazgos, hipótesis y limitaciones. |
| Presentación y cierre | ¿Qué puede concluirse y cómo se conservará lo trabajado? | Informe, anexos y disposición de evidencia. |

Las flechas no representan una receta rígida. Un hallazgo puede revelar otra fuente y obligar a volver a identificación. La iteración debe estar justificada, documentada y, si cambia el alcance, autorizada.

## 3. Solicitud, objetivo, alcance y preguntas

### Solicitud y autoridad

Antes de intervenir una fuente se debe conocer:

- Quién solicita el análisis.
- Qué autoridad posee.
- Qué resultado necesita.
- Qué sistemas, cuentas, fechas y acciones están permitidos.
- A quién deben comunicarse hallazgos urgentes.

Una autorización ambigua debe aclararse. El acceso técnico nunca amplía por sí solo el permiso otorgado.

### Objetivo

Describe el resultado general que se busca.

Ejemplo:

> Determinar si la cuenta de remuneraciones fue utilizada para un acceso remoto no autorizado durante la madrugada y establecer qué activos estuvieron involucrados.

### Alcance

Declara expresamente qué está incluido y excluido.

| Incluido | Excluido sin autorización adicional |
| --- | --- |
| Equipo corporativo asignado. | Correo personal. |
| Cuenta corporativa. | Dispositivos particulares. |
| Registros VPN e identidad del período. | Períodos ajenos al incidente. |
| Archivo sospechoso identificado. | Contactar al supuesto atacante. |

### Preguntas investigativas

Transforman el objetivo en tareas verificables:

- ¿Qué autenticaciones se registraron durante el período?
- ¿Qué procesos o archivos se relacionan temporalmente con la sesión?
- ¿Existe evidencia disponible de transferencia de datos?
- ¿Qué fuentes apoyan o contradicen la hipótesis inicial?
- ¿Qué limitaciones impiden responder con mayor confianza?

Una acción útil debe poder vincularse con una pregunta. “Revisar todo el disco por si aparece algo” no constituye un objetivo suficiente.

## 4. Planificar antes de ejecutar

El plan traduce preguntas en acciones, responsables y productos.

| Componente | Pregunta que debe responder |
| --- | --- |
| Fuentes | ¿Dónde podrían existir datos pertinentes? |
| Prioridad | ¿Qué puede perderse primero o posee mayor valor? |
| Método | ¿Cómo se obtendrá y protegerá cada fuente? |
| Responsables | ¿Quién autoriza, custodia, adquiere, analiza y revisa? |
| Herramientas | ¿Qué función cumplen y qué limitaciones poseen? |
| Productos | ¿Qué inventarios, logs, tablas e informes se generarán? |
| Riesgos | ¿Qué acción puede alterar evidencia o afectar la operación? |
| Contingencias | ¿Qué hacer ante cifrado, falla, falta de espacio o fuente inaccesible? |
| Cierre | ¿Cuándo las preguntas se consideran respondidas o no determinables? |

La prioridad no depende solo de la volatilidad. También considera relevancia, rotación, impacto de intervenir, disponibilidad de copias, autorización y capacidad técnica.

## 5. Identificación de fuentes

**Identificar** significa localizar y caracterizar fuentes plausibles. No significa recolectar todo.

Fuentes posibles para un acceso remoto incluyen:

- Equipo y sistema de archivos.
- Memoria y estado activo, si el equipo está encendido.
- Registros de VPN e identidad.
- DNS, DHCP, proxy, firewall y otros registros de red.
- Correo o colaboración dentro del alcance.
- Aplicaciones, cloud y proveedores.
- Tickets, inventarios y entrevistas.

Para cada fuente conviene registrar:

| Campo | Ejemplo |
| --- | --- |
| Identificador | `SRC-VPN-01`. |
| Descripción | Registros VPN corporativos. |
| Propietario | Operaciones de red. |
| Período | 10-08 20:00 a 11-08 08:00. |
| Valor esperado | Cuenta, origen, inicio, término y resultado. |
| Volatilidad | Rotación cada siete días. |
| Método de acceso | Exportación autorizada. |
| Riesgo | Zona horaria o pérdida por rotación. |

Preguntas críticas:

- ¿La fuente existía durante el período investigado?
- ¿Registra realmente el evento buscado?
- ¿Quién puede modificarla?
- ¿Durante cuánto tiempo se conserva?
- ¿Qué zona horaria utiliza?
- ¿Cómo se exporta sin perder campos o contexto?

## 6. Adquisición y preservación

### Adquisición

Es la obtención controlada de datos desde una fuente. Para esta lección basta explicar **qué datos se necesitan, por qué son pertinentes, quién autoriza obtenerlos y qué producto verificable se espera**. La comparación entre métodos de adquisición y su ejecución técnica se realizará en la lección 04.

### Preservación

Es el conjunto de medidas destinadas a mantener la utilidad, contexto e integridad de los datos:

- Documentar el estado inicial.
- Limitar acceso.
- Proteger el original.
- Crear y utilizar copias de trabajo.
- Registrar hashes cuando corresponda.
- Conservar metadatos, filtros, zona horaria y formato.
- Evitar rotación o eliminación de registros.
- Registrar errores y transferencias.

### Diferencia esencial

| Concepto | Acción |
| --- | --- |
| Identificación | Localiza y caracteriza la fuente. |
| Adquisición | Obtiene datos mediante un método controlado. |
| Preservación | Mantiene su utilidad, contexto e integridad durante el caso. |

La preservación no es un único paso ubicado después de adquirir: comienza cuando se identifica una fuente y continúa hasta el cierre.

## 7. Examen, análisis, hallazgo y conclusión

### Examen

Consiste en localizar, extraer, filtrar y organizar información relevante. Puede producir tablas, artefactos, archivos recuperados o cronologías preliminares.

Ejemplo:

> Se extrajeron 24 eventos de autenticación exitosa para la cuenta `camila` entre las 00:00 y las 06:00 UTC.

El examen todavía no establece quién controlaba la cuenta ni si los eventos fueron maliciosos.

### Análisis

Correlaciona e interpreta resultados para responder las preguntas:

1. Revisa el objetivo.
2. Relaciona resultados de distintas fuentes.
3. Construye una secuencia temporal.
4. Formula y contrasta hipótesis.
5. Busca datos favorables y contradictorios.
6. Declara nivel de confianza y limitaciones.

### Comparación rápida

| Examen | Análisis |
| --- | --- |
| Extrae eventos VPN. | Evalúa si forman parte de una sesión relevante. |
| Lista procesos. | Relaciona proceso, usuario, archivo y tiempo. |
| Filtra tráfico. | Interpreta la secuencia observada. |
| Recupera un archivo. | Evalúa su relación con el caso. |

### Hallazgo y conclusión

- **Hallazgo:** afirmación técnica respaldada por evidencia y una referencia verificable.
- **Conclusión:** respuesta razonada que integra hallazgos y reconoce límites.

Una coincidencia temporal puede justificar una asociación, pero no demuestra automáticamente causalidad o autoría.

## 8. Documentación transversal

La documentación no es una fase final. Se registra durante todo el trabajo.

### Hash, cadena de custodia y bitácora

| Control | Qué permite demostrar | Qué no reemplaza |
| --- | --- | --- |
| Hash | Comparación del contenido de una versión concreta. | Procedencia, autorización o interpretación. |
| Cadena de custodia | Posesión, control, ubicación y transferencias. | Registro detallado de acciones técnicas. |
| Bitácora técnica | Quién hizo qué, cuándo, cómo y con qué resultado. | Registro de custodia física o lógica. |

Una bitácora mínima incluye:

- Fecha, hora y zona.
- Responsable.
- Evidencia o copia utilizada.
- Acción y propósito.
- Herramienta, versión y configuración.
- Resultado y ubicación de la salida.
- Error, decisión o desviación.

Los tres controles se complementan. Un hash aislado no explica de dónde provino un archivo ni cómo fue tratado.

## 9. Presentación y cierre

En esta lección, **presentar** significa comunicar una respuesta proporcional y verificable, no redactar todavía el informe forense completo de la lección 12. La comunicación breve debe incluir:

1. objetivo y alcance;
2. fuentes consideradas y proceso seguido;
3. hallazgos que responden las preguntas;
4. datos faltantes, limitaciones y conclusión.

El cierre también deja indicada la disposición de la evidencia según las instrucciones recibidas. No es necesario definir todavía anexos, formato institucional ni una plantilla completa de informe.

## 10. Iteración controlada

El proceso puede volver a fases anteriores:

- Un log revela otra cuenta y requiere identificar una fuente adicional.
- Un desfase horario obliga a revisar la cronología.
- Aparece información fuera de alcance y se solicita ampliación.
- Una herramienta falla y se utiliza otro método documentado.
- Una fuente nueva obliga a revisar si está dentro del alcance autorizado.

Iterar no significa improvisar. Se debe registrar qué cambió, por qué, quién lo autorizó y qué efecto tiene.

## 11. Casos aplicados

Las dos actividades utilizan organizaciones y fuentes diferentes, pero exigen el mismo aprendizaje: revisar ocho decisiones, construir las seis fases, responder una contingencia y distinguir examen de análisis.

### Versión A: NORTE SUR SPA

NORTE SUR SPA autoriza investigar el acceso remoto presentado en la lección anterior. El objetivo es establecer si existió acceso no autorizado, qué activos pudieron verse afectados y qué información permite reconstruir los hechos.

Se dispone de:

- Equipo corporativo apagado.
- Registros de VPN.
- Copia del archivo sospechoso.
- Exportación de eventos.

No está autorizado acceder a archivos personales ajenos al incidente ni contactar al supuesto atacante.

### Ejemplo de inicio del plan

| Etapa | Acción | Producto | Riesgo que debe controlarse |
| --- | --- | --- | --- |
| Alcance | Confirmar preguntas, período, fuentes y exclusiones. | Plan autorizado. | Acceder a información no pertinente. |
| Identificación | Registrar equipo, VPN, archivo y eventos disponibles. | Inventario priorizado. | Omitir una fuente o perder un log por rotación. |

Completa las etapas restantes mediante la actividad. Para cada una pregunta:

1. ¿Qué objetivo cumple?
2. ¿Cuál es su entrada?
3. ¿Qué acción se realiza?
4. ¿Quién es responsable?
5. ¿Qué producto verificable genera?
6. ¿Qué puede salir mal?

### Versión B: PUERTO CLARO SPA

La actividad B (`actividad-02.md`) presenta actividad inusual en un servidor administrativo y una posible planilla extraída. Cambian el nombre, el período y la contingencia, pero no cambian las fases, los productos ni los criterios de logro. No se deben inventar procedimientos técnicos que el caso no entrega.

### Posibles limitaciones

- Registros rotados o incompletos.
- Desfase horario.
- Cuenta compartida o comprometida.
- Falta de telemetría del endpoint.
- Imposibilidad de asociar una IP con una persona.

## 12. Errores frecuentes

- Comenzar por una herramienta en vez de una pregunta.
- Recolectar todo sin criterio ni autorización.
- Tratar el proceso como una secuencia rígida que nunca itera.
- Analizar directamente el original.
- Confundir copia común con adquisición controlada.
- Confundir extracción con interpretación.
- Suponer que un hash demuestra toda la trazabilidad.
- Omitir resultados negativos o hipótesis alternativas.
- Ocultar errores o cambios del plan.
- Redactar el informe al final sin haber mantenido bitácora.

## 13. Qué se estudiará después

| Tema | Lección posterior |
| --- | --- |
| Selección y validación de herramientas | Lección 03. |
| Preparación de la estación forense | Lección 03. |
| Adquisición e imágenes forenses | Lección 04. |
| Registro técnico y calidad de evidencia | Lección 06. |
| Examen de sistemas, red, bases de datos, cloud y móviles | Lecciones 07 a 10. |
| Informe forense completo | Lección 12. |

Esta sesión estudia el mapa y las decisiones del proceso; no exige ejecutar todas sus técnicas.

## 14. Guía de estudio

### Paso 1 - Reconstruir el mapa

Dibuja las fases sin mirar. Bajo cada una escribe:

- Pregunta principal.
- Entrada.
- Acción.
- Producto.
- Riesgo de ejecución.

### Paso 2 - Practicar diferencias

Explica con un ejemplo propio:

- Identificación frente a adquisición.
- Adquisición frente a preservación.
- Examen frente a análisis.
- Hallazgo frente a conclusión.
- Iteración frente a improvisación.

### Paso 3 - Aplicar

1. Completa solo la versión asignada: actividad A (`actividad.md`) o actividad B (`actividad-02.md`).
2. Revisa que cada acción responda una pregunta.
3. Marca cualquier acción fuera de alcance.
4. Verifica que original y copia estén separados.
5. Comprueba que tus dos respuestas individuales distinguen examen/análisis y reconocen los límites de atribución.

## Autoevaluación

### Preguntas

1. ¿Por qué el proceso debe comenzar con objetivo y autorización?
2. ¿Cómo se relaciona el modelo de NIST con la secuencia del curso?
3. ¿Qué diferencia existe entre identificación, adquisición y preservación?
4. ¿Qué factores determinan la prioridad de una fuente?
5. ¿Qué funciones diferentes cumplen hash, cadena de custodia y bitácora?
6. ¿Cuál es la diferencia entre examen y análisis?
7. ¿Por qué el proceso puede iterar?
8. ¿Qué producto genera la fase de presentación?
9. ¿Qué debe hacerse si aparece una fuente pertinente fuera del alcance autorizado?
10. Corrige: “Encontré una IP extranjera; por lo tanto identifiqué al atacante”.

### Respuestas orientativas

1. Para asegurar autoridad, pertinencia, límites y acciones vinculadas con preguntas concretas.
2. Identificación, adquisición y preservación detallan actividades incluidas en la recolección; examen, análisis y reporte mantienen propósitos diferenciados.
3. Identificar localiza y caracteriza; adquirir obtiene; preservar mantiene utilidad, contexto e integridad.
4. Relevancia, volatilidad, riesgo de pérdida, impacto de intervenir, copias disponibles, autorización y capacidad técnica.
5. El hash compara contenido; la custodia registra control y transferencias; la bitácora documenta acciones técnicas.
6. El examen extrae y organiza datos; el análisis los correlaciona e interpreta para responder preguntas.
7. Porque los resultados pueden revelar fuentes, errores, desfases o necesidades de alcance adicionales.
8. Un informe con objetivo, métodos, hallazgos, limitaciones, conclusiones y anexos verificables.
9. Detener el acceso fuera de alcance, registrar la situación y solicitar una decisión a quien tenga autoridad; mientras tanto, continuar solo con acciones autorizadas.
10. “El registro relaciona una conexión con una IP; se necesitan fuentes adicionales para determinar quién controlaba la cuenta o conexión”.

## Fuentes base y profundización opcional

- [NIST SP 800-86](https://csrc.nist.gov/pubs/sp/800/86/final): proceso y fuentes forenses desde una perspectiva de TI.
- [NIST Digital Evidence](https://www.nist.gov/digital-evidence): fundamentos y validación.
- [NIST SP 800-61 Rev. 3](https://csrc.nist.gov/pubs/sp/800/61/r3/final): relación con respuesta y gestión del riesgo.

Fecha de consulta de recursos vivos: 06-08-2026.
