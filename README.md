# INF43 · Análisis Forense

Repositorio académico de **INF43 - Análisis Forense**, asignatura de la carrera Técnico de Nivel Superior en Informática mención Ciberseguridad del CFT San Antonio. Aquí se publican materiales de clase, actividades, casos controlados y recursos de apoyo para las secciones diurna y vespertina del segundo semestre de 2026.

La asignatura aborda la investigación de incidentes mediante un proceso autorizado, ordenado, documentado y reproducible. Su foco no está solo en utilizar herramientas: también exige preservar la integridad de la evidencia, distinguir hechos de interpretaciones, reconocer las limitaciones del análisis y comunicar conclusiones técnicamente fundamentadas.

## Información general

| Antecedente | Detalle |
| --- | --- |
| Sigla | INF43 |
| Carrera | TNS en Informática mención Ciberseguridad |
| Requisitos | No considera |
| Carga total | 72 horas: 24 teóricas y 48 prácticas |
| Dedicación semanal | 4 horas |
| Créditos | 8 |
| Periodo académico | Segundo semestre de 2026 |
| Secciones | SAINF43-1, jornada diurna; SAINF43-2, jornada vespertina |

## Propósito formativo

Al finalizar el curso, cada estudiante deberá ser capaz de aplicar una metodología de análisis forense digital para adquirir, preservar, examinar y analizar evidencia procedente de distintas fuentes. El proceso debe mantener la trazabilidad de las acciones y producir resultados claros, verificables y adecuados para su destinatario.

En particular, se espera que pueda:

- definir el propósito, el alcance, la autorización y las preguntas investigativas de un caso;
- preparar un ambiente de trabajo aislado, controlado y reproducible;
- adquirir y duplicar datos sin alterar innecesariamente la fuente original;
- utilizar hashes, cadena de custodia y bitácoras para demostrar integridad y trazabilidad;
- examinar evidencia de sistemas operativos, redes, bases de datos, servicios cloud y dispositivos móviles;
- aplicar técnicas básicas de análisis estático y dinámico de malware en laboratorios aislados;
- investigar hechos relacionados con aplicaciones web y correo electrónico;
- correlacionar fuentes, construir cronologías y separar hallazgos, inferencias y conclusiones;
- elaborar y defender un informe forense que documente procedimientos, resultados y limitaciones.

## Unidades de aprendizaje

| Unidad | Horas | Contenidos principales | Evaluación |
| --- | ---: | --- | ---: |
| UA1 · Las bases del análisis forense | 20 | Fundamentos, ética, proceso de investigación, herramientas, preparación del laboratorio, adquisición, duplicación e integridad. | 20% |
| UA2 · La evidencia digital | 24 | Evaluación y preservación de evidencia; análisis de sistemas operativos, redes, bases de datos, ambientes cloud y dispositivos móviles. | 25% |
| UA3 · Metodología forense | 28 | Metodología integrada, malware, ataques web y de correo electrónico, documentación, presentación y reporte. | 25% |
| Evaluación final integradora | - | Resolución de un caso completo mediante procedimientos practicados durante el semestre. | 30% |

Las ponderaciones suman el 100% de la calificación.

## Calendario de evaluaciones 2026/2

| Evaluación | Ponderación | SAINF43-1 · Diurna | SAINF43-2 · Vespertina |
| --- | ---: | --- | --- |
| Evaluación UA1 | 20% | 07-09-2026 | 08 y 09-09-2026 |
| Evaluación UA2 | 25% | 26-10-2026 | 20 y 21-10-2026 |
| Evaluación UA3 | 25% | 07-12-2026 | 01 y 02-12-2026 |
| Evaluación final | 30% | 14-12-2026 | 09 y 15-12-2026 |

En SAINF43-2 cada evaluación se distribuye en dos bloques: la primera fecha corresponde al inicio y la segunda, al cierre. La evaluación final de esa sección utiliza el bloque del 15 de diciembre para recuperar la clase suspendida por el feriado del 8 de diciembre. Cualquier reprogramación se comunicará mediante los canales institucionales de la asignatura.

## Ruta del semestre

La secuencia común comprende 18 lecciones:

1. **Lecciones 01–05 - Fundamentos:** evidencia, ética, proceso forense, laboratorio, adquisición e integridad. La lección 05 integra la UA1.
2. **Lecciones 06–11 - Evidencia digital:** registro técnico y análisis de sistemas operativos, red, bases de datos, cloud y móviles. La lección 11 integra la UA2.
3. **Lecciones 12–17 - Investigación aplicada:** metodología e informe, análisis estático y dinámico, ataques web, correo electrónico y resolución de casos. La lección 17 integra la UA3.
4. **Lección 18 - Evaluación final:** investigación forense completa y presentación de resultados.

SAINF43-1 desarrolla normalmente cada lección en una clase de cuatro horas. SAINF43-2 distribuye el mismo núcleo formativo en dos bloques de dos horas, de acuerdo con su calendario institucional.

## Metodología de trabajo

El curso combina exposiciones breves, demostraciones, laboratorios guiados, estudio de casos, trabajo cooperativo y actividades basadas en problemas. Las prácticas utilizan datos sintéticos o material expresamente autorizado y enfatizan la reproducibilidad del procedimiento por sobre la obtención apresurada de una respuesta.

En cada caso se recomienda seguir este flujo:

1. Leer el escenario, confirmar la autorización y delimitar el alcance.
2. Registrar la evidencia recibida y verificar su integridad.
3. Conservar el original y trabajar sobre una copia controlada.
4. Documentar fecha, hora, herramienta, versión, comando, entrada y resultado.
5. Examinar los artefactos y correlacionar las fuentes disponibles.
6. Separar observaciones, interpretaciones, limitaciones y conclusiones.
7. Entregar los productos solicitados con sus anexos y referencias.

## Organización del repositorio

```text
.
├── SAINF43-1/
│   └── leccion-XX/       # materiales de la sección diurna
├── SAINF43-2/
│   └── leccion-XX/       # materiales de la sección vespertina
└── cheatsheets/          # ayudas de Linux, Git y Markdown
```

Dentro de cada lección pueden existir guías, actividades, archivos de evidencia y documentos de apoyo. Se debe trabajar en la carpeta de la sección correspondiente y respetar las instrucciones particulares de cada entrega.

## Ética, seguridad e integridad académica

- Analiza únicamente sistemas, cuentas, archivos y redes para los cuales exista autorización expresa.
- No ejecutes muestras ni artefactos sospechosos fuera de un ambiente aislado, desechable y supervisado.
- No cargues evidencia real, credenciales, datos personales ni información institucional sensible en servicios públicos.
- Preserva las fuentes originales y registra cualquier acción que pueda modificar la evidencia.
- Declara herramientas, fuentes, colaboración y asistencia automatizada cuando las reglas de la actividad lo exijan.
- Fundamenta cada conclusión en evidencia verificable; una ausencia de evidencia no demuestra por sí sola que un hecho no ocurrió.

## Bibliografía

### Bibliografía del programa

- Dirección Académica. (2022). *Programa de asignatura INF43: Análisis Forense* [Programa de asignatura].
- Lázaro Domínguez, F. (2016). *Investigación forense de dispositivos móviles Android*. RA-MA. ISBN 978-84-9964-497-4.
- Santo Orcero, D. (2018). *Kali Linux*. RA-MA. ISBN 978-84-9964-709-8.
- Térmens Graells, M. (2014). *Preservación digital*. Editorial UOC. ISBN 978-84-9064-082-1.

### Referencias técnicas complementarias

- Kent, K., Chevalier, S., Grance, T., & Dang, H. (2006). [*Guide to Integrating Forensic Techniques into Incident Response* (NIST SP 800-86)](https://csrc.nist.gov/pubs/sp/800/86/final). National Institute of Standards and Technology.
- National Institute of Standards and Technology. (2025). [*Incident Response Recommendations and Considerations for Cybersecurity Risk Management* (NIST SP 800-61 Rev. 3)](https://csrc.nist.gov/pubs/sp/800/61/r3/final).
- National Institute of Standards and Technology. (s. f.). [*Computer Forensic Reference Data Sets (CFReDS)*](https://cfreds.nist.gov/).
- National Institute of Standards and Technology. (s. f.). [*Computer Forensics Tool Testing Program (CFTT)*](https://www.nist.gov/itl/csd/secure-systems-and-applications/computer-forensics-tool-testing-program-cftt).
- Scientific Working Group on Digital Evidence. (s. f.). [*Best Practices for Computer Forensic Acquisition*, versión 2.1](https://www.swgde.org/documents/published-complete-listing/17-f-002-2-1/).
- Scientific Working Group on Digital Evidence. (s. f.). [*Requirements for Report Writing in Digital and Multimedia Forensics*](https://www.swgde.org/documents/published-complete-listing/18-q-002-swgde-requirements-for-report-writing-in-digital-and-multimedia-forensics/).
- The Sleuth Kit. (s. f.). [*Autopsy User Documentation*](https://www.sleuthkit.org/autopsy/docs/user-docs/).
- Volatility Foundation. (s. f.). [*Volatility 3 Documentation*](https://volatility3.readthedocs.io/en/latest/).
- Wireshark Foundation. (s. f.). [*Wireshark User's Guide*](https://www.wireshark.org/docs/wsug_html/).
- MITRE. (s. f.). [*MITRE ATT&CK*](https://attack.mitre.org/).
- OWASP Foundation. (s. f.). [*Web Security Testing Guide*](https://owasp.org/www-project-web-security-testing-guide/).

Las publicaciones oficiales antiguas incluidas aquí continúan siendo útiles para fundamentos cuando no existe una revisión final posterior; deben complementarse con la documentación vigente de las herramientas utilizadas en cada laboratorio.
