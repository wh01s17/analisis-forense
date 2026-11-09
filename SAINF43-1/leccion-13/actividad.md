---
title: "Práctica guiada 13 - Análisis estático y recuperación autorizada"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "13"
author: Jordy
start: 2026-11-03
end: 2026-11-09
created_at: 2026-10-29
aliases:
  - "Práctica guiada 13 - Análisis estático y recuperación autorizada"
---

# Actividad Lección 13 - Análisis estático y recuperación autorizada

- **Modalidad:** parejas.
- **Duración:** 40 minutos de activación, 80 minutos de práctica y 40 minutos de cierre.
- **Caso:** `AR-CASO06` - ANDES SOFT SPA (empresa ficticia).
- **Entorno:** estación Kali aislada con el paquete del caso montado en solo lectura y una carpeta de salida con escritura; **sin red**.
- **Carácter:** formativo, sin calificación.
- **Producto:** este documento completado como `00_control/ficha-l13.md`, las salidas de `02_salidas/` y el manifiesto `03_anexos/hashes-salidas.txt`.

Consulten [[guia-comandos-leccion-13|la guía práctica de comandos]] cuando necesiten comprender un comando, su resultado esperado o qué hacer ante un error. Respondan con frases breves: se valora separar lo observado de lo interpretado, no la extensión.

> [!warning] Seguridad del laboratorio
> La muestra es **material inerte de laboratorio**. No se ejecuta, no se abre con doble clic y no se sube a servicios en línea. La recuperación usa **solo** la lista de candidatos autorizada: no se generan diccionarios ni se aplica fuerza bruta abierta. Si un hash no coincide, el contenido es inesperado o la lista se agota sin resultado, detengan el procedimiento y avisen al docente.

## El caso

ANDES SOFT SPA recibió por correo un adjunto llamado `actualizacion-seguridad.pdf`, presentado como una actualización de seguridad de un proveedor. El equipo de soporte no la abrió y la derivó al laboratorio. Junto a la muestra llegó un contenedor protegido, `AR-CASO06-contenedor.zip`, que —según el remitente— guarda un documento de respaldo. La jefatura autoriza dos cosas y nada más:

1. **caracterizar la muestra sin ejecutarla**, para decidir si amerita un análisis dinámico posterior;
2. **recuperar el contenido del contenedor** probando **únicamente** una lista breve de contraseñas candidatas entregada para el caso.

| Archivo | Contenido |
| --- | --- |
| `caso/autorizacion.txt` | Objetivo, lo autorizado, lo excluido y el escalamiento |
| `caso/candidatos-autorizados.txt` | Lista acotada de contraseñas que pueden probarse |
| `muestra/actualizacion-seguridad.pdf` | Muestra inerte; su tipo real debe determinarse por firma |
| `recuperacion/AR-CASO06-contenedor.zip` | Contenedor protegido con un documento de control |
| `notas/ficha-tecnica.txt` | Origen y manejo de la muestra y del contenedor |
| `notas/AR-CASO06.sha256` | Manifiesto SHA-256 de la muestra y del contenedor |
| `notas/referencia-attack.txt` | Extracto local de MITRE ATT&CK con técnicas candidatas (la estación no tiene red) |

> ¿Qué puede afirmarse de un archivo mirándolo sin ejecutarlo, y dónde termina esa certeza?

Roles sugeridos: **analista** (ejecuta los comandos) y **registrador** (completa la ficha). Intercambien el rol al pasar de la muestra al contenedor.

## 1. Preparar el caso y verificar la integridad

Definan las rutas, creen la carpeta de trabajo y copien esta actividad como su ficha de entrega:

```bash
LAB=~/forense/LAB-13
COMPARTIDA=/media/sf_LAB13_ENTREGA
SALIDA=/media/sf_LAB13_SALIDA
CASO="$LAB/AR-CASO06"
mkdir -p "$CASO"/{00_control,01_copias_trabajo,02_salidas,03_anexos}
cd "$CASO"
cp -n "$COMPARTIDA/documentos/actividad-leccion-13.md" 00_control/ficha-l13.md
chmod u+w 00_control/ficha-l13.md
```

Completen las respuestas en `00_control/ficha-l13.md`.

Comprueben que el paquete está en solo lectura y que la salida admite escritura:

```bash
findmnt -no TARGET,OPTIONS -T "$COMPARTIDA"
test -w "$SALIDA" && echo 'OK: salida con escritura' || echo 'ERROR: salida sin escritura'
```

La primera salida debe incluir `ro`. Si no aparece, deténganse y avisen al docente.

Verifiquen la muestra y el contenedor contra el manifiesto **antes** de tocarlos. Cada carpeta contiene solo uno de los dos archivos del manifiesto; `--ignore-missing` evita que el otro aparezca como falla:

```bash
(cd "$COMPARTIDA/muestra" && sha256sum -c --ignore-missing ../notas/AR-CASO06.sha256) | tee 00_control/verificacion-recibidos.txt
(cd "$COMPARTIDA/recuperacion" && sha256sum -c --ignore-missing ../notas/AR-CASO06.sha256) | tee -a 00_control/verificacion-recibidos.txt
```

Deben aparecer **dos** líneas terminadas en `OK`: una para la muestra y otra para el contenedor. Luego creen una copia de trabajo de la muestra, comparen ambos hashes y quítenle el permiso de escritura:

```bash
cp -n --preserve=timestamps "$COMPARTIDA/muestra/actualizacion-seguridad.pdf" 01_copias_trabajo/
sha256sum "$COMPARTIDA/muestra/actualizacion-seguridad.pdf" 01_copias_trabajo/actualizacion-seguridad.pdf | tee 00_control/copia.txt
chmod a-w 01_copias_trabajo/*
```

```text
¿Qué demuestra que ambos SHA-256 coincidan, y qué NO demuestra (completitud, origen, intención)?:
¿Por qué se analiza la copia y no la muestra recibida?:
```

## 2. Caracterizar la muestra sin ejecutarla

Todo lo que sigue se hace **sobre la copia** y sin ejecutar el archivo. Guarden cada salida en `02_salidas/` con `tee`.

### 2.1 Tipo real frente a la extensión

```bash
file 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/tipo.txt
head -c 2 01_copias_trabajo/actualizacion-seguridad.pdf | od -An -tx1 | tee 02_salidas/magic.txt
```

| Nombre recibido | Tipo real (por firma) | Magic bytes | Hallazgo observable |
| --- | --- | --- | --- |
| `actualizacion-seguridad.pdf` | [Completar] | [Completar] | [Completar] |

La extensión dice `.pdf`; la firma dice otra cosa. Registren la **discrepancia** como hecho observable, no como prueba de intención.

### 2.2 Tamaño y metadatos disponibles

```bash
stat -c '%n  %s bytes  modificado %y' 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/metadatos.txt
```

`--preserve=timestamps` conservó la fecha de modificación del original; sin esa opción, `stat` mostraría la hora de la copia. `stat` solo informa propiedades del **sistema de archivos**; con el formato usado, imprime nombre, tamaño y fecha de modificación. No lee metadatos internos como autor, firma digital o fecha de compilación.

```text
Qué muestra `stat` y qué metadatos internos NO se examinaron con estas herramientas (recuerden: «no examinado» no es «ausente»):
```

### 2.3 Cadenas e indicadores

```bash
strings -n 6 01_copias_trabajo/actualizacion-seguridad.pdf | tee 02_salidas/cadenas.txt
```

Clasifiquen **cuatro** cadenas o nombres de función, al menos una de cada clase. Un indicador **fuerte** es específico de un comportamiento; uno **contextual** describe una capacidad posible; uno **débil** es común en software legítimo.

| Cadena o nombre | ¿Qué sugiere? | Clasificación (fuerte / contextual / débil) | Falso positivo posible |
| --- | --- | --- | --- |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] | [Completar] |

> [!note] Dominios del laboratorio
> El caso usa el sufijo reservado `.invalid`, que nunca resuelve, en lugar de un dominio real. Analícenlo **como si fuera un dominio real**: que no resuelva es una convención del laboratorio, no evidencia de que sea un señuelo.

### 2.4 Estructura y segunda herramienta

Busquen los nombres de sección dentro del archivo y comparen con una segunda vista de los primeros bytes:

```bash
strings -n 4 01_copias_trabajo/actualizacion-seguridad.pdf | grep -oE '\.(text|data|rsrc)' | tee 02_salidas/secciones.txt
od -An -c 01_copias_trabajo/actualizacion-seguridad.pdf | sed -n '1,4p'
```

Deben aparecer tres secciones: `.text`, `.data` y `.rsrc`.

```text
¿Qué revelan la cabecera y las secciones, y en qué se diferencian las vistas de `file`, `strings` y `od`?:
```

Las secciones y las cadenas son **indicios**. Encontrar el texto `WinHttpOpen` no demuestra que exista una entrada en la tabla de importación ni código que use esa función: verificarlo exige una herramienta que interprete la estructura PE, como Ghidra. Mucho menos demuestra comportamiento.

### 2.5 Hipótesis y mapeo inicial

Relacionen un segundo indicador con una técnica de MITRE ATT&CK usando el condicional. La estación no tiene red: usen `notas/referencia-attack.txt`, que incluye candidatas que pueden **no** corresponder a la muestra. La primera fila es un ejemplo.

| Indicador | Técnica ATT&CK compatible | Redacción acotada |
| --- | --- | --- |
| Clave de autoarranque `...\CurrentVersion\Run` | Persistence - Registry Run Keys (T1547.001) | "La muestra **contiene** el texto de una clave de arranque automático, **compatible con** persistencia; no se verificó que la escriba." |
| [Elijan: dominio externo + nombres de funciones de red, o la extensión que no coincide con el tipo] | [Completar] | [Completar] |

```text
Hipótesis principal (podría / es compatible con):
Explicación benigna alternativa:
Una afirmación que el análisis estático NO permite sostener:
```

## 3. Recuperación autorizada del contenedor

El contenedor ya se verificó en el paso 1. Revisen el alcance y registren la hora de inicio:

```bash
ZIP="$COMPARTIDA/recuperacion/AR-CASO06-contenedor.zip"
cat "$COMPARTIDA/caso/candidatos-autorizados.txt" | tee 02_salidas/alcance-recuperacion.txt
date '+inicio %F %T' | tee 02_salidas/recuperacion.txt
```

Prueben **solo** los candidatos de la lista, uno por vez y en orden. Empiecen por el primero y cambien la contraseña del comando en cada intento:

```bash
unzip -P 'respaldo-andes' "$ZIP" -d 02_salidas/recuperado
```

- `incorrect password`: el candidato no sirve. Anótenlo en la ficha y prueben el siguiente.
- `inflating: …/texto-control.txt`: el candidato es correcto. Deténganse y registren la hora de término:

```bash
date '+término %F %T' | tee -a 02_salidas/recuperacion.txt
```

Extraigan el texto de control y calculen su hash:

```bash
cat 02_salidas/recuperado/texto-control.txt
sha256sum 02_salidas/recuperado/texto-control.txt | tee -a 02_salidas/recuperacion.txt
unzip -v | head -1
```

Comparen el SHA-256 obtenido con el valor publicado por el docente.

| Campo | Contenido |
| --- | --- |
| Herramienta y versión | `unzip` [versión] |
| Lista utilizada | `candidatos-autorizados.txt` (sin ampliarla) |
| Inicio y término | [Completar desde `recuperacion.txt`] |
| Candidatos probados y candidato que recuperó | [Completar en orden] |
| SHA-256 del texto de control | [Completar] |
| ¿Coincide con el publicado? | [Sí / No] |

```text
¿Qué demuestra la recuperación y qué NO demuestra (autoría, intención, licitud)?:
¿Por qué esta autorización no permite probar otros contenedores o cuentas?:
```

Condiciones de detención: hash inesperado, lista agotada o contenido distinto del esperado.

## 4. Veredicto provisional y respaldo

Redacten un veredicto que separe observación de interpretación:

| Campo | Contenido |
| --- | --- |
| Resultado observado | [Solo lo que muestran tipo, cadenas y recuperación] |
| Interpretación | [Qué significa para la pregunta del caso] |
| Confianza (alta / media / baja) | [Con una razón] |
| Limitación | [Qué no permite el análisis estático] |
| Prueba dinámica recomendada | [Diseñada, no ejecutada: aislamiento, qué monitorear, cuándo detenerla y cómo revertir, en dos líneas] |

Calculen los hashes de las salidas, respáldenlas junto con la ficha y verifiquen la copia:

```bash
sha256sum 00_control/*.txt 02_salidas/*.txt 02_salidas/recuperado/texto-control.txt | tee 03_anexos/hashes-salidas.txt
cp -r 00_control 02_salidas 03_anexos "$SALIDA/"
(cd "$SALIDA" && sha256sum -c 03_anexos/hashes-salidas.txt)
```

La ficha `00_control/ficha-l13.md` se sigue editando hasta el final, por eso no entra en el manifiesto.

**Solo SAINF43-2, al terminar el bloque del martes:** respalden lo avanzado con `cp -r 00_control 02_salidas "$SALIDA/"`. El miércoles, repitan la definición de variables y comprueben que la carpeta de trabajo sigue completa antes de continuar.

## Preguntas de cierre

- ¿Qué indicador clasificaron distinto entre parejas y por qué?
- ¿Qué cambiaría si las cadenas estuvieran ofuscadas o cifradas?
- Si la lista se hubiera agotado sin abrir el contenedor, ¿qué informarían?
- ¿Qué tendría que observar la prueba dinámica de la lección 14 para subir la confianza?

## Comprobación final

- [ ] La muestra y el contenedor terminaron en `OK` antes de analizarse.
- [ ] El tipo real se determinó por firma, no por la extensión.
- [ ] La muestra nunca se ejecutó ni se subió a un servicio en línea.
- [ ] Cuatro cadenas quedaron clasificadas, cada una con un falso positivo posible.
- [ ] Las hipótesis usan "podría" o "es compatible con", no un veredicto de malware.
- [ ] La recuperación usó solo la lista autorizada y registró candidatos, tiempos y hash.
- [ ] El SHA-256 del texto recuperado se comparó con el valor publicado.
- [ ] El veredicto separa resultado, interpretación, confianza y limitación, y la prueba dinámica quedó diseñada, no ejecutada.
- [ ] El respaldo contiene control, salidas y anexos, y supera la comprobación de hashes.
