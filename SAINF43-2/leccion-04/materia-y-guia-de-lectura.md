---
title: "Materia y guía de lectura - Adquisición, duplicación e integridad"
tags:
  - nota
  - course
  - curso
  - materia
  - guia-de-lectura
  - adquisicion-forense
  - materia-y-guia-de-lectura
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "04"
author: Jordy
start: 2026-08-31
end: 2026-09-02
created_at: "2026-08-07 21:08"
aliases:
  - "Materia y guía de lectura - Adquisición, duplicación e integridad"
---

# Materia y guía de lectura - Adquisición, duplicación e integridad

```toc
```

## Propósito

Esta lección enseña a adquirir, verificar, duplicar y documentar una imagen forense sin trabajar sobre el original.

## El encargo

Trabajarás sobre el caso `PCL-2026-018` de PUERTO CLARO SPA: la misma empresa cuyo servidor administrativo revisaste en la Lección 02, donde quedó la sospecha de que se extrajo una planilla. La evidencia `EV-01` es un pendrive USB rotulado a mano «RESPALDO ADMIN», que estuvo conectado a un equipo del sector durante el período investigado y llegó sellado al laboratorio el 26 de agosto.

La autorización de la gerencia cubre **adquirir y preservar**. No cubre examinar el contenido: eso corresponde a una etapa posterior. Por eso durante toda la lección tendrás la evidencia disponible y no la abrirás.

Esa restricción no es una formalidad de clase. Quien abre el original antes de la adquisición modifica tiempos de acceso, no puede demostrar después en qué estado lo recibió, y entrega a la contraparte una pregunta que no sabrá responder: ¿quién accedió a la evidencia, cuándo y con qué autorización?

El pendrive mide 512 MiB, lo que permite ejecutar el procedimiento completo dentro del bloque. La imagen Windows de 50 GiB queda reservada para la Lección 07, donde se entrega ya adquirida y la carga se concentra en artefactos, eventos y cronología.

## El mapa del caso

```text
PENDRIVE_ADMIN_01.raw
          |
          | exposición como /dev/nbd0 en modo RO
          v
adquisición con Guymager realizada por la pareja
          |
          v
PENDRIVE_ADMIN_01.E01 en adquisicion/
          |
          | ewfverify + comparación RAW/ewf1
          v
copia E01 verificada en trabajo/
```

La secuencia evita que el tiempo de copia o adquisición de una imagen pesada sustituya el aprendizaje. La evidencia es pequeña, pero conserva todas las decisiones esenciales: identificar la fuente, comprobar solo lectura, registrar la herramienta, verificar la E01 y separar la adquisición de la copia de trabajo.

## Resultado de aprendizaje

Al finalizar deberías poder:

- distinguir fuente RAW, dispositivo expuesto, imagen E01 y copia de trabajo;
- realizar una adquisición autorizada sobre una fuente pequeña;
- reconocer adquisición lógica y física;
- verificar una E01 y sus segmentos;
- relacionar cada hash con el objeto exacto sobre el que fue calculado;
- crear una copia de trabajo sin alterar la adquisición preservada;
- documentar una discrepancia sin ocultarla.

## 1. Adquirir no es solamente copiar

Una adquisición forense es un procedimiento autorizado y documentado para obtener datos desde una fuente. Debe permitir responder:

1. ¿Cuál fue la fuente?
2. ¿Quién realizó el procedimiento?
3. ¿Cuándo y con qué zona horaria?
4. ¿Qué herramienta y versión se utilizó?
5. ¿Qué objeto se produjo?
6. ¿Cómo se verificó?
7. ¿Qué errores o límites existieron?

La calidad del resultado depende de los datos obtenidos y de la trazabilidad del procedimiento.

## 2. Tipos de adquisición

### Adquisición lógica

Obtiene objetos que el sistema presenta como archivos, carpetas, registros o exportaciones.

Puede ser apropiada cuando el alcance es específico y no se necesitan estructuras internas. Puede omitir:

- espacio no asignado;
- archivos eliminados;
- estructuras del sistema de archivos;
- metadatos no expuestos;
- datos fuera de los filtros aplicados.

### Adquisición física o de bajo nivel

Busca representar los sectores disponibles de un medio o disco virtual. Puede conservar particiones, sistemas de archivos, espacio asignado y no asignado y artefactos que una copia lógica no incluye.

Requiere más almacenamiento, tiempo y control técnico. En esta lección cada pareja adquiere un volumen sintético pequeño; en la Lección 07, el docente entrega el caso Windows pesado ya adquirido.

### Adquisición en vivo

Se realiza mientras el sistema funciona. Puede ser necesaria para memoria, procesos, conexiones o volúmenes cifrados abiertos. Siempre produce cambios y esos cambios deben justificarse y registrarse.

### Comparación breve

| Método | Unidad principal | Ventaja | Límite típico |
| --- | --- | --- | --- |
| Lógico | Archivos u objetos seleccionados | Rápido y acotado | Omite datos no expuestos |
| Físico | Sectores del medio | Conserva más estructura | Mayor tiempo y tamaño |
| En vivo | Estado de un sistema activo | Accede a datos volátiles | Modifica el sistema |

No existe un método superior para todos los casos. La elección depende del objetivo, autorización, estado de la fuente y recursos disponibles.

## 3. RAW, dispositivo de bloque, E01 y copia no son lo mismo

### RAW

`PENDRIVE_ADMIN_01.raw` contiene una secuencia directa de bytes que representa el volumen sintético. Es el archivo fuente distribuido para la práctica.

### Dispositivo de bloque

`qemu-nbd --read-only` expone el RAW como `/dev/nbd0`. Guymager adquiere ese dispositivo. Antes de conectarlo debe comprobarse que esté libre; después debe verificarse su tamaño y que `blockdev --getro` devuelva `1`.

### OVA

Una OVA es un paquete para distribuir una máquina virtual. Normalmente contiene una descripción OVF y uno o más discos virtuales. Puede incluir configuración de hardware virtual además del disco.

### VMDK

El VMDK representa el disco virtual de una máquina. En la preparación de la Lección 07, el docente expone el VMDK de Windows en modo de solo lectura y lo adquiere con Guymager. No es la fuente práctica de esta lección.

### E01

E01 es un contenedor de evidencia. Puede:

- dividir la adquisición en segmentos;
- incorporar metadatos del caso;
- aplicar compresión;
- almacenar controles de integridad;
- permitir verificación con herramientas compatibles.

El formato E01 no vuelve correcto un procedimiento por sí solo. También se necesitan identificación, protección, verificación y documentación.

### Copia de trabajo

Es una duplicación verificada de todos los segmentos E01. Se utiliza para el examen posterior. La adquisición preservada permanece separada y sin uso analítico cotidiano.

## 4. Por qué la VM Windows no se usa para practicar esta adquisición

Windows modifica el disco durante el arranque y el uso normal. Puede cambiar:

- registros de eventos;
- archivos temporales;
- Prefetch;
- Registry;
- marcas temporales;
- tareas y servicios;
- archivos de usuario.

Dos estudiantes que importaron la misma OVA pueden terminar con discos distintos. Por eso, para el caso pesado:

- la OVA maestra se conserva;
- el docente extrae el VMDK sin iniciar la VM;
- se genera una sola E01 docente;
- el curso recibe una copia de análisis en la Lección 07.

Importar una OVA normalmente crea archivos nuevos para la VM. El archivo OVA conservado no debería usarse como máquina de trabajo ni reemplazarse por una exportación posterior.

## 5. Proteger la fuente: bloqueadores de escritura

Conectar un medio a un computador no es una operación pasiva. El sistema operativo puede montar el volumen automáticamente, actualizar tiempos de acceso, escribir archivos de índice o reparar estructuras que considere inconsistentes. Nada de eso requiere que tú lo pidas, y todo modifica la evidencia.

Un **bloqueador de escritura por hardware** resuelve ese problema en el punto correcto: es un adaptador que se interpone entre el dispositivo y el computador, permite las lecturas y descarta los comandos de escritura antes de que alcancen el medio. Modelos habituales para USB son el *Tableau Forensic USB 3.0 Bridge* (T8u), el *CRU WiebeTech USB WriteBlocker* y el *Digital Intelligence UltraBlock*, que además indican el estado de bloqueo con un LED o una pantalla.

Un bloqueador no se acepta por confianza en la marca. NIST define en su programa CFTT la [especificación que un bloqueador debe cumplir](https://www.nist.gov/document/cftt-hwb-hardware-write-block-specs-version-20) y publica los [informes de prueba independientes de cada modelo](https://www.nist.gov/itl/ssd/software-quality-group/computer-forensics-tool-testing-program-cftt/cftt-technical/hardware). Es la misma lógica que aplicas al resto del procedimiento: una afirmación vale lo que vale la evidencia que la sostiene.

También existen bloqueadores por software y opciones de montaje en solo lectura. Son útiles, pero dependen de la configuración correcta del sistema en que se ejecutan; el bloqueo por hardware no.

En esta lección no hay bloqueador físico disponible. La fuente se expone en solo lectura mediante `qemu-nbd --read-only`, y por eso `blockdev --getro` debe devolver `1` **antes** de abrir Guymager. Con un bloqueador la garantía la da el equipo; aquí la da tu comprobación, y sin ella no puedes sostener que no escribiste sobre el original.

### Dos niveles que no hay que confundir

El archivo RAW se te entrega además sin permiso de escritura (`-r--r--r--`). Esa es una barrera distinta y menor:

| Nivel | Qué impide | Cómo se comprueba |
| --- | --- | --- |
| Permiso del archivo | Que tú o un programa sobrescriban el RAW por accidente | `ls -l`, `test -w` |
| Solo lectura del dispositivo | Que llegue cualquier escritura al medio que Guymager está leyendo | `blockdev --getro` devuelve `1` |

El permiso es conveniencia; el modo del dispositivo es la evidencia. Un examinador que declare «la fuente estaba protegida» mostrando solo un `ls -l` no ha demostrado lo que afirma: el permiso de un archivo lo puede cambiar su propietario en un segundo, mientras que el dispositivo expuesto en solo lectura rechaza la escritura a nivel del núcleo.

Si recibes la fuente con permisos de escritura, no la corrijas por tu cuenta. Regístralo e informa: alterar el estado de una evidencia recibida -aunque sea para protegerla mejor- es una acción sobre la evidencia y debe quedar documentada y autorizada.

## 6. Separación de objetos

```text
LAB-04/
├── fuente/          PENDRIVE_ADMIN_01.raw entregado
├── adquisicion/     E01 creada y preservada por la pareja
├── trabajo/         duplicado verificado para examen posterior
├── notas/           ficha y hash de la fuente
└── registros/       salidas y bitácora de la pareja
```

La carpeta `fuente/` representa el material de origen del ejercicio. `adquisicion/` conserva el resultado de Guymager y `trabajo/` recibe la copia que puede examinarse. Ninguna carpeta contiene un computador físico original.

## 7. Funciones hash

Una función hash transforma una secuencia de bytes en un valor de longitud fija. Un cambio en los bytes debería producir un valor diferente.

En un procedimiento forense, un hash se usa para comprobar igualdad entre objetos claramente definidos. No demuestra por sí solo:

- quién creó el contenido;
- si el contenido es verdadero o benigno;
- la intención de una persona;
- que no existe evidencia fuera de lo adquirido;
- que todo el procedimiento fue correcto.

### Algoritmos de esta lección

| Algoritmo | Uso didáctico |
| --- | --- |
| MD5 | Compatibilidad histórica y controles presentes en algunos formatos o herramientas |
| SHA-1 | Compatibilidad histórica y controles EWF frecuentes |
| SHA-256 | Control principal para distribuir y duplicar los segmentos E01 |

MD5 y SHA-1 ya no son adecuados para aplicaciones modernas que dependen de resistencia criptográfica a colisiones. En esta lección se interpretan como controles de integridad y compatibilidad dentro de un conjunto documentado, mientras SHA-256 se usa como control externo principal.

En el laboratorio calcularás los tres sobre el mismo archivo. Observa qué cambia y qué no:

| Observación | Qué significa |
| --- | --- |
| Los tres valores son distintos | Son funciones diferentes, no versiones de un mismo resultado. |
| Sus longitudes son 32, 40 y 64 caracteres hexadecimales | Cada función produce un resumen de tamaño fijo propio. |
| Los tres coinciden con su manifiesto a la vez | El archivo no cambió; es un mismo hecho comprobado tres veces, no tres hechos. |
| Solo uno discrepa | Hubo un error de cálculo o de objeto: se hasheó otra cosa. Nunca es “el algoritmo débil fallando”. |

La distinción práctica es esta: una colisión hay que **construirla** con esfuerzo deliberado sobre un archivo diseñado para ello. Verificar que una copia conserva los bytes de su origen es un problema distinto, y por eso el `OK` de MD5 sigue siendo evidencia útil contra el error accidental. Elegir SHA-256 como control externo del caso responde al escenario adverso, no al habitual.

## 8. La regla más importante sobre hashes

Antes de comparar, responde:

> ¿El mismo algoritmo fue calculado sobre el mismo objeto?

| Hash | Objeto |
| --- | --- |
| SHA-256 del RAW | Bytes del archivo fuente `PENDRIVE_ADMIN_01.raw` |
| SHA-256 de `ewf1` | Contenido adquirido expuesto desde la E01 |
| Hashes EWF | Contenido o controles registrados por el formato durante la adquisición |
| SHA-256 de los segmentos | Bytes de cada archivo `.E01`, `.E02` y siguientes |

Estos valores no deben coincidir entre sí porque representan objetos diferentes.

### Ejemplo

Si el SHA-256 de `PENDRIVE_ADMIN_01.raw` coincide con el hash de `ewf1`, puedes afirmar que la E01 expone el mismo contenido adquirido desde la fuente. Si el manifiesto de segmentos valida `trabajo/`, puedes afirmar que esos contenedores copiados son idénticos a los preservados en `adquisicion/`.

No puedes afirmar que:

- el SHA-256 del contenedor `.E01` debería coincidir con el del RAW;
- la coincidencia demuestra quién creó los archivos del volumen;
- la adquisición contiene datos que nunca estuvieron en la fuente;
- la evidencia demuestra por sí sola una conducta maliciosa.

## 9. Dos niveles de verificación

### Nivel 1 - Archivos contenedores

`sha256sum -c` verifica los archivos `.E01`, `.E02` y siguientes contra el manifiesto creado sobre la adquisición. Detecta cambios durante la duplicación.

### Nivel 2 - Controles EWF

`ewfinfo` muestra metadatos y hashes disponibles. `ewfverify` recorre la adquisición y verifica los controles que soporte el contenedor.

Los dos niveles se complementan:

```text
SHA-256 externo -> comprueba los archivos entregados
ewfverify        -> comprueba la estructura y controles EWF
```

## 10. Registro mínimo

La bitácora debe contener:

- caso y evidencia;
- responsables y roles;
- estación utilizada;
- fecha, hora y zona;
- herramientas y versiones;
- nombres y tamaños de segmentos;
- resultado del hash entregado para la fuente;
- metadatos relevantes de `ewfinfo`;
- resultado de `ewfverify`;
- comparación RAW/`ewf1` y resultado de la copia de trabajo;
- errores, decisiones y limitaciones.

Una captura de pantalla sin contexto no reemplaza una bitácora. Debe poder relacionarse cada salida con un objeto, una herramienta y un momento.

## 11. Manejo de discrepancias

Una discrepancia es evidencia del procedimiento y no debe ocultarse.

```text
detener
  -> conservar objetos y salidas
  -> identificar qué comparación falló
  -> informar
  -> documentar
  -> continuar solo con una decisión autorizada
```

No corresponde:

- editar el manifiesto;
- generar una referencia nueva desde la copia fallida;
- omitir un segmento;
- continuar el análisis como si la verificación hubiera sido satisfactoria.

## 12. Puente hacia la Lección 07

La transferencia entre ambas lecciones es metodológica, no una reutilización del mismo archivo:

```text
Lección 04: aprendemos a crear y verificar una adquisición pequeña.
Lección 07: aplicamos la disciplina de preservación al analizar
            una copia Windows pesada adquirida previamente por el docente.
```

En la Lección 07 el curso recibirá `WIN10-CASO01.E01` precargada. Se comprobará su entrega y se montará en solo lectura, pero no se repetirá la adquisición ni la duplicación de 50 GiB.

## Guía de estudio

### Antes de la clase

1. Dibuja el flujo RAW -> dispositivo de solo lectura -> E01 -> copia de trabajo.
2. Escribe una diferencia entre adquisición lógica y física.
3. Explica por qué debe comprobarse `RO 1` antes de abrir Guymager.
4. Relaciona cada hash con su objeto.
5. Anticipa qué esperas ver al calcular MD5, SHA-1 y SHA-256 sobre un mismo archivo.

### Durante la clase

Mantén visible esta pregunta:

> ¿Qué objeto estoy verificando ahora?

No avances si no puedes responderla con un nombre y una ubicación exactos.

### Después de la clase

Sin consultar la guía, explica:

1. por qué no analizamos `adquisicion/`;
2. por qué la copia de trabajo usa el mismo manifiesto;
3. qué diferencia existe entre `sha256sum -c` y `ewfverify`;
4. por qué los tres algoritmos dan valores distintos del mismo archivo;
5. qué harías ante un segmento con `FAILED`.

## Autoevaluación

- [ ] Puedo distinguir adquisición lógica, física y en vivo.
- [ ] Puedo explicar RAW, dispositivo de bloque, E01 y copia de trabajo.
- [ ] Sé por qué el caso práctico usa una fuente pequeña.
- [ ] Relaciono cada hash con un objeto exacto.
- [ ] Explico por qué MD5, SHA-1 y SHA-256 del mismo archivo son distintos entre sí.
- [ ] Justifico qué algoritmo usaría como control externo y frente a qué escenario.
- [ ] Interpreto una coincidencia sin exagerar sus conclusiones.
- [ ] Puedo registrar y manejar una discrepancia.

## Recursos de la lección

- `actividad-leccion-04.md`
- `guia-comandos-leccion-04.md`
- [NIST SP 800-86](https://csrc.nist.gov/pubs/sp/800/86/final)
- [NIST CFTT - Hardware Write Block: especificación e informes de prueba por modelo](https://www.nist.gov/itl/ssd/software-quality-group/computer-forensics-tool-testing-program-cftt/cftt-technical/hardware)
- [libewf](https://github.com/libyal/libewf)
