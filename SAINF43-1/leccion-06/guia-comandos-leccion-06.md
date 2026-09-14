---
title: "Guía práctica de comandos Lección 06 - Copia y verificación de evidencia"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "06"
author: Jordy
start: 2026-09-14
end: 2026-09-16
created_at: 2026-09-13
aliases:
  - "Guía práctica de comandos Lección 06 - Copia y verificación de evidencia"
---

# Guía práctica de comandos Lección 06 - Copia y verificación de evidencia

## Objetivo y alcance

Usa esta guía solo como apoyo para los comandos de la actividad. Al finalizar tendrás una **copia de trabajo**, el resultado de la **verificación SHA-256** y las **cabeceras autorizadas** de un correo.

No adquirirás memoria, montarás discos, accederás a servicios reales ni analizarás el incidente. Ningún comando requiere `sudo`.

## 1. Crear la copia de trabajo

Ubícate en la carpeta `analisis-forense/leccion-06/` y ejecuta:

```bash
mkdir -p ~/forense/LAB-06
cp -a material/. ~/forense/LAB-06/
cd ~/forense/LAB-06
pwd
```

### ¿Qué hace cada comando?

| Comando | Parte | Explicación |
| --- | --- | --- |
| `mkdir` | `mkdir` | Crea una carpeta. |
|  | `-p` | Crea las carpetas intermedias que falten y no genera un error si ya existen. |
|  | `~/forense/LAB-06` | Indica la ruta que se creará. `~` representa tu carpeta personal. |
| `cp` | `cp` | Copia archivos y carpetas. |
|  | `-a` | Copia recursivamente y conserva la estructura y los metadatos disponibles. |
|  | `material/.` | Es el origen. El punto indica todo el contenido de `material`, incluidos elementos ocultos. |
|  | `~/forense/LAB-06/` | Es la carpeta de destino. |
| `cd` | `cd` | Cambia la ubicación de trabajo de la terminal. |
|  | `~/forense/LAB-06` | Indica que los comandos siguientes se ejecutarán dentro de la copia. |
| `pwd` | `pwd` | Muestra la ruta de la carpeta actual; el resultado debe terminar en `/forense/LAB-06`. |

**Por qué se hace:** la carpeta `material` es el original didáctico. La actividad debe ejecutarse sobre una copia para evitar cambios accidentales.

**Precaución:** si `LAB-06` contiene archivos de una práctica anterior, informa al docente antes de continuar; no mezcles entregas.

### Comprobación opcional del paquete

Si aparece un error de archivo faltante, revisa la copia con:

```bash
find . -maxdepth 2 -type f -print | sort
```

| Parte | Explicación |
| --- | --- |
| `find` | Busca elementos dentro de una ruta. |
| `.` | Inicia la búsqueda en la carpeta actual. |
| `-maxdepth 2` | Limita la búsqueda a la carpeta actual y dos niveles de profundidad. |
| `-type f` | Selecciona solamente archivos regulares. |
| `-print` | Muestra la ruta de cada archivo encontrado. |
| `\|` | Envía la salida de `find` al comando siguiente. |
| `sort` | Ordena los nombres para facilitar su comparación con la lista de materiales. |

Este control confirma qué archivos copiaste; no valida su contenido.

## 2. Verificar el manifiesto SHA-256

Desde `~/forense/LAB-06`, ejecuta:

```bash
sha256sum -c manifest_entrega.sha256
```

### ¿Cómo funciona?

| Parte | Explicación |
| --- | --- |
| `sha256sum` | Calcula huellas SHA-256 o las compara con una referencia. No modifica los archivos examinados. |
| `-c` | Activa el modo **comprobar**: lee las huellas y rutas escritas en un manifiesto. |
| `manifest_entrega.sha256` | Es el manifiesto que contiene las huellas de referencia y los archivos que se verificarán. |

Lee una línea a la vez:

| Salida            | Interpretación válida                                            | Lo que no demuestra                               |
| ----------------- | ---------------------------------------------------------------- | ------------------------------------------------- |
| `archivo: OK`     | La huella calculada coincide con la registrada en el manifiesto. | Que el archivo sea auténtico, verdadero o seguro. |
| `archivo: FAILED` | La huella calculada no coincide con la registrada.               | La causa de la diferencia ni quién la produjo.    |

Registra en la ficha los nombres de los archivos con `OK` y con `FAILED`. Ante una discrepancia, no intentes repararla: **registra, aísla y escala**.

### Comando que no debes ejecutar

```text
sha256sum exportados/* > manifest_entrega.sha256
```

| Parte | Explicación |
| --- | --- |
| `sha256sum` | Calcularía una huella nueva para cada archivo indicado. |
| `exportados/*` | El asterisco seleccionaría todos los objetos dentro de `exportados`. |
| `>` | Redirigiría la salida y **reemplazaría** el contenido del archivo ubicado a la derecha. |
| `manifest_entrega.sha256` | Sería el archivo sobrescrito; se perdería la referencia recibida para documentar discrepancias. |

## 3. Revisar únicamente las cabeceras del EML

Ejecuta:

```bash
sed -n '1,/^$/p' exportados/correo-alerta.eml
```

### ¿Cómo funciona?

| Parte | Explicación |
| --- | --- |
| `sed` | Lee el archivo de texto sin modificarlo. |
| `-n` | Evita mostrar automáticamente todas las líneas. |
| `'1,/^$/'` | Selecciona desde la primera línea hasta la primera línea vacía. En un EML, esa línea separa las cabeceras del cuerpo. |
| `p` | Imprime solamente el rango seleccionado. |
| `exportados/correo-alerta.eml` | Es el archivo que se revisará. |

Observa si aparecen campos como `From`, `To`, `Date`, `Message-ID`, `Subject` y `Received`. Registra el campo ausente y la limitación que produce.

**Límite:** si un campo no aparece en el archivo recibido, solo puedes afirmar que está ausente en **esa copia**. No puedes concluir que nunca existió en el sistema de correo original.

## 4. Qué llevar a la ficha

| Evidencia mínima | Qué registrar |
| --- | --- |
| `pwd` | Confirmación de que trabajaste en la copia. |
| `sha256sum -c` | Archivos coincidentes, archivo discrepante y límite de la conclusión. |
| `sed` sobre el EML | Cabecera ausente y efecto sobre la evaluación de procedencia. |

No necesitas copiar toda la salida ni crear registros adicionales. Conserva únicamente lo solicitado en E1 de la actividad.

## Si aparece un error

| Mensaje o situación | Qué revisar |
| --- | --- |
| `No such file or directory` | Ejecuta `pwd`. Si no estás en `~/forense/LAB-06`, vuelve a ejecutar `cd ~/forense/LAB-06`. |
| `manifest_entrega.sha256: no properly formatted checksum lines found` | No modifiques el manifiesto. Detén el procedimiento e informa al docente. |
| Todos los archivos muestran `FAILED` | Comprueba la ubicación con `pwd` y confirma que copiaste la estructura completa. |
| El EML muestra el cuerpo | Detén la revisión y comprueba que escribiste exactamente `sed -n '1,/^$/p'`. |
| `command not found` | Copia el mensaje e informa al docente; no instales herramientas durante la actividad. |

## Comprobación final

- [ ] Trabajé sobre la copia `~/forense/LAB-06`.
- [ ] No modifiqué el manifiesto ni los archivos recibidos.
- [ ] Interpreté `OK` o `FAILED` sin inventar una causa.
- [ ] Revisé solo las cabeceras del correo.
- [ ] Registré únicamente la evidencia solicitada en la ficha.
