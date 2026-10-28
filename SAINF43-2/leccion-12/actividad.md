---
title: "Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "12"
author: Jordy
start: 2026-10-27
end: 2026-11-02
created_at: 2026-10-24
aliases:
  - "Práctica guiada 12 - Plan forense, cadena de custodia y hallazgo reproducible"
---

# Actividad Lección 12 - Plan forense, cadena de custodia y hallazgo reproducible

- **Modalidad:** equipos de tres personas.
- **Duración:** 40 minutos de activación, 80 minutos de taller y 40 minutos de cierre.
- **Caso:** `NS-CASO05` - NORTE-SUR SPA (sociedad por acciones; ficticia).
- **Entorno:** estación Kali con el paquete del caso montado en solo lectura y una carpeta de salida con escritura; no se requiere red.
- **Carácter:** formativo, sin calificación.
- **Producto:** `bitacora-equipo.md`, salidas de análisis y manifiestos de hashes. No se crea una cadena de custodia paralela para la dinámica del aula.

Consulten [[guia-comandos-leccion-12|la guía práctica de comandos]] cuando necesiten comprender un comando, su resultado esperado o qué hacer ante un error.

## El caso

NORTE-SUR SPA participa en la licitación `PUERTO-2027`. La mañana del **23-10-2026** la Gerencia de Operaciones escribe al área de Seguridad de la Información:

> Nos llegó el rumor de que la competencia ya tendría nuestra oferta para la licitación PUERTO-2027. Sospecho de Rodrigo Ibarra, de Compras, porque anoche estuvo conectado tarde.
>
> Necesito que revisen todo: su notebook, su correo, su teléfono, su WhatsApp, la nube y lo que haga falta. Revisen también si alguien más de Compras está metido en esto.
>
> Pídanle a TI que apague el notebook de inmediato para que no alcance a borrar nada.
>
> Tienen 24 horas. Necesito saber quién lo hizo para tomar medidas.

Dos horas después, Gerencia General y Asesoría Legal emiten una autorización formal. TI entrega un inventario preliminar, una cadena de custodia con omisiones y tres exportaciones sintéticas:

| Archivo | Contenido | Tiempo |
| --- | --- | --- |
| `caso/solicitud-gerencia.txt` | Solicitud original de la Gerencia de Operaciones | - |
| `caso/autorizacion.txt` | Objetivo, recursos, intervalo, exclusiones y escalamiento | - |
| `caso/inventario-preliminar.csv` | Inventario levantado por TI; contiene errores deliberados | - |
| `caso/cadena-custodia-recibida.csv` | Registro llevado antes de la recepción del caso; contiene omisiones | Hora local `-03:00` |
| `evidencia/vpn-2026-10-22.csv` | Exportación de conexiones VPN | Hora local sin desfase escrito |
| `evidencia/cloud-audit.jsonl` | Exportación de auditoría cloud | UTC (`Z`) |
| `evidencia/correo-metadatos.csv` | Exportación de metadatos del buzón corporativo | Hora local con desfase `-03:00` |
| `notas/ficha-tecnica.txt` | Origen, filtro y zona horaria de cada exportación | - |
| `notas/NS-CASO05.sha256` | Manifiesto SHA-256 de las tres exportaciones | - |

Los archivos de `evidencia/` son **exportaciones recibidas**, no los dispositivos ni servicios originales. El manifiesto permite comprobar que los archivos no cambiaron desde que se preparó el paquete; no demuestra por sí solo que la exportación sea completa o verdadera.

> ¿Cómo se transforma una solicitud ambigua en un plan forense ejecutable, con trazabilidad y un hallazgo que otra persona pueda reproducir?

## Ruta de trabajo

| Tramo | Tiempo | Producto esperado |
| --- | ---: | --- |
| Activación | 40 min | Problemas de la solicitud, alcance y dos preguntas forenses |
| Taller | 80 min | Copias verificadas, auditoría de inventario y custodia, matriz y cronología |
| Cierre | 40 min | Un hallazgo reproducible, revisión entre pares y respaldo verificado |

Roles sugeridos: **analista** (ejecuta), **registrador** (documenta decisiones y resultados) y **revisor** (comprueba alcance y trazabilidad). Cambien de rol al pasar del taller al cierre.

## 1. Convertir la solicitud en un encargo forense

Lean `caso/solicitud-gerencia.txt`. Identifiquen cuatro problemas que deban resolverse antes de actuar.

| Frase o supuesto | Problema | Pregunta de aclaración |
| --- | --- | --- |
| [Completar] | [Alcance, autoridad, método, sesgo, plazo o privacidad] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |

Lean `caso/autorizacion.txt` y `notas/ficha-tecnica.txt`. La autorización define el límite; lo que queda fuera se excluye o se escala.

| Elemento | Definición para `NS-CASO05` | Fuente y apartado |
| --- | --- | --- |
| Objetivo | [Completar] | [Completar] |
| Recursos incluidos | [Completar] | [Completar] |
| Exclusiones | [Completar] | [Completar] |
| Intervalo local y UTC | [Completar] | [Completar] |
| Responsable y canal de escalamiento | [Completar] | [Completar] |
| Criterio de término | [Completar] | [Definido por el equipo] |

Formulen dos preguntas forenses que puedan responderse con las fuentes autorizadas, sin presuponer quién actuó.

```text
P1:
P2:
¿Por qué “saber quién lo hizo” excede lo que estas fuentes pueden demostrar?:
Pedido de gerencia que queda fuera del alcance autorizado:
Conflicto que debe escalarse antes de examinar mensajes personales:
```

## 2. Preparar el caso y verificar las exportaciones

Definan las rutas y creen cuatro carpetas con funciones distintas:

```bash
LAB=~/forense/LAB-12
COMPARTIDA=/media/sf_LAB12_ENTREGA
RECUPERACION=/media/sf_LAB12_SALIDA
EV="$COMPARTIDA/evidencia"
CASO="$LAB/NS-CASO05"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_anexos}
cd "$CASO"
cp -n "$COMPARTIDA/documentos/actividad-leccion-12.md" 00_control/bitacora-equipo.md
chmod u+w 00_control/bitacora-equipo.md
```

Comprueben la configuración sin intentar escribir en el paquete:

```bash
findmnt -no TARGET,OPTIONS -T "$COMPARTIDA"
test -w "$RECUPERACION" && echo 'OK: salida con escritura' || echo 'ERROR: salida sin escritura'
```

La primera salida debe incluir la opción `ro`. Si no aparece, deténganse y avisen al docente.

Verifiquen las exportaciones recibidas contra el manifiesto:

```bash
(cd "$EV" && sha256sum -c ../notas/NS-CASO05.sha256) | tee 00_control/verificacion-recibidas.txt
```

Solo continúen si los tres archivos terminan en `OK`. Luego creen copias de trabajo, compruébenlas contra el mismo manifiesto y quiten el permiso de escritura:

```bash
cp -n "$EV"/* 01_copias_trabajo/
(cd 01_copias_trabajo && sha256sum -c "$COMPARTIDA/notas/NS-CASO05.sha256") | tee 00_control/verificacion-copias.txt
chmod a-w 01_copias_trabajo/*
ls -l 01_copias_trabajo/
```

Documenten la derivación. Una copia de trabajo no implica una nueva transferencia de custodia; se registra como un artefacto derivado de una exportación identificada.

| Exportación recibida | Copia de trabajo | Archivo | ¿Hash coincide? |
| --- | --- | --- | --- |
| `NS05-EV02` | `NS05-EV02-T1` | `vpn-2026-10-22.csv` | [Sí / No] |
| `NS05-EV03` | `NS05-EV03-T1` | `correo-metadatos.csv` | [Sí / No] |
| `NS05-EV04` | `NS05-EV04-T1` | `cloud-audit.jsonl` | [Sí / No] |

```text
¿Qué demuestra la coincidencia de hashes?:
¿Qué no demuestra?:
¿Por qué el análisis se realiza sobre las copias?:
```

## 3. Auditar el inventario y la cadena de custodia recibida

Revisen `caso/inventario-preliminar.csv`. Identifiquen cuatro problemas y propongan una corrección sin modificar el archivo recibido.

| Fuente o fila | Problema | Riesgo | Corrección propuesta |
| --- | --- | --- | --- |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |

Ordenen las tres primeras acciones de preservación o adquisición de fuentes autorizadas. Consideren volatilidad, retención y dependencia.

| Orden | Fuente | Motivo | Acción y responsable |
| ---: | --- | --- | --- |
| 1 | [Completar] | [Completar] | [Completar] |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |

Revisen ahora `caso/cadena-custodia-recibida.csv`. Para cada fila defectuosa, registren el problema y la acción correcta. Un dato ausente se marca **por confirmar**; no se completa por intuición.

| Fila | Omisión o ambigüedad | Riesgo para la trazabilidad | Acción correctiva sin inventar datos |
| ---: | --- | --- | --- |
| 2 | [Completar] | [Completar] | [Completar] |
| 3 | [Completar] | [Completar] | [Completar] |
| 4 | [Completar] | [Completar] | [Completar] |
| 5 | [Completar] | [Completar] | [Completar] |
| 6 | [Completar] | [Completar] | [Completar] |

```text
¿Por qué un prefijo de hash no basta para demostrar igualdad?:
¿Qué dato permite resolver a cuál de los dos EV-04 se refiere la fila 4?:
¿Qué diferencia hay entre una transferencia de custodia, una copia derivada y un acceso de análisis?:
```

## 4. Ejecutar el plan y construir la cronología

Antes de filtrar, conecten cada pregunta con una fuente, una técnica y una limitación.

| Pregunta | Fuente e ID | Técnica | Resultado esperado | Limitación |
| --- | --- | --- | --- | --- |
| P1 | [Completar] | [Completar] | [Completar] | [Completar] |
| P2 | [Completar] | [Completar] | [Completar] | [Completar] |

Ejecuten únicamente las técnicas justificadas en la matriz. Usen los comandos explicados en [[guia-comandos-leccion-12#Filtrar dentro del alcance autorizado|la sección de filtrado de la guía]]. Guarden cada resultado en `02_salidas/`.

Completen una cronología breve en UTC:

| # | Hito | Fuente y línea | Hora original | Hora UTC | Identificadores relacionados |
| ---: | --- | --- | --- | --- | --- |
| 1 | Conexión VPN de la cuenta | [Completar] | [Completar] | [Completar] | [Completar] |
| 2 | Carga del objeto | [Completar] | [Completar] | [Completar] | [Completar] |
| 3 | Creación del enlace | [Completar] | [Completar] | [Completar] | [Completar] |
| 4 | Mensaje enviado con el enlace | [Completar] | [Completar] | [Completar] | [Completar] |
| 5 | Acceso anónimo al enlace | [Completar] | [Completar] | [Completar] | [Completar] |
| 6 | Desconexión VPN | [Completar] | [Completar] | [Completar] | [Completar] |

Sin examinar el contenido excluido, documenten tres límites del análisis:

| Registro o fuente | Motivo de exclusión | Decisión |
| --- | --- | --- |
| Mensaje personal dentro del intervalo | [Completar] | [Excluir o escalar] |
| Acceso al enlace fuera del intervalo | [Completar] | [Excluir o pedir ampliación] |
| Sesiones de otro trabajador | [Completar] | [Excluir o pedir nueva autorización] |

## 5. Redactar un hallazgo reproducible

Calculen hashes de las salidas. El manifiesto resultante permite identificar exactamente qué archivos respaldan el hallazgo:

```bash
sha256sum 02_salidas/*.txt | tee 03_anexos/hashes-salidas.txt
```

Redacten un hallazgo:

| Campo | Contenido |
| --- | --- |
| Identificador | `H-01` |
| Pregunta que responde | [Completar] |
| Evidencia y copias utilizadas | [ID, archivo y SHA-256] |
| Procedimiento | [Comandos ejecutados] |
| Salidas guardadas | [Archivo y SHA-256] |
| Resultado observado | [Solo lo que muestran los registros] |
| Interpretación y confianza | [Completar] |
| Explicación alternativa | [Completar] |
| Limitación | [Completar] |

Escriban una conclusión de tres oraciones:

1. qué muestran las fuentes;
2. qué no permiten atribuir a una persona;
3. qué decisión o preservación sigue pendiente.

## 6. Revisar y respaldar

Otro equipo revisa el hallazgo usando esta pauta:

| Control | Resultado y observación |
| --- | --- |
| Todas las acciones respetan recursos, intervalo y exclusiones | [Completar] |
| Cada afirmación cita archivo y línea | [Completar] |
| Los hashes permiten identificar evidencia, copias y salidas | [Completar] |
| La interpretación no excede el resultado observado | [Completar] |
| La explicación alternativa y la limitación son plausibles | [Completar] |

Equipo revisor: [Completar]

La revisión entre pares es un **acceso de revisión** documentado en la bitácora. Solo correspondería registrar una transferencia de custodia si otra persona asumiera efectivamente el control y la responsabilidad sobre un soporte o archivo.

Creen el manifiesto de entrega, respalden la bitácora, las verificaciones, las salidas y los anexos, y comprueben la copia:

```bash
sha256sum 00_control/* 02_salidas/* 03_anexos/hashes-salidas.txt | tee 03_anexos/hashes-entrega.txt
cp -r 00_control 02_salidas 03_anexos "$RECUPERACION/"
(cd "$RECUPERACION" && sha256sum -c 03_anexos/hashes-entrega.txt)
```

No respalden `01_copias_trabajo/`: son copias de las exportaciones recibidas y pueden recrearse a partir del paquete verificado.

## Comprobación final

- [ ] Las preguntas no presuponen culpabilidad.
- [ ] El alcance cita la autorización y distingue exclusión de escalamiento.
- [ ] Las tres exportaciones y sus copias terminan en `OK`.
- [ ] Las copias de trabajo tienen identificadores derivados y están sin permiso de escritura.
- [ ] La revisión de custodia no inventa responsables, horas ni hashes.
- [ ] La cronología está en UTC y cita archivo y línea.
- [ ] El hallazgo separa resultado, interpretación, alternativa y limitación.
- [ ] El respaldo contiene bitácora, verificaciones, salidas y anexos, y supera la comprobación de hashes.
