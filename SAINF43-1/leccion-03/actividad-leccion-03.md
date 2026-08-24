---
title: "Actividad Lección 03 - Preparación de un laboratorio forense seguro"
tags:
  - nota
  - course
  - curso
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA1 - Las bases del análisis forense
lesson: "03"
author: Jordy
start: 2026-08-24
end: 2026-08-26
created_at: 2026-08-21
aliases:
  - "Actividad Lección 03 - Preparación de un laboratorio forense seguro"
---
# Actividad Lección 03 - Preparación de un laboratorio forense seguro

**Duración:** 80 minutos.  
**Modalidad sugerida:** parejas, con entrega individual.  
**Carácter:** actividad evaluada con calificación. La actividad tendrá un puntaje definido y será evaluada de acuerdo con los criterios establecidos para la entrega.

## Tema de la lección

Sistemas y herramientas utilizadas en forense; preparación, aislamiento y verificación básica de un laboratorio virtual seguro.

## Objetivo

Preparar un laboratorio básico de análisis forense con Kali Linux y Windows 10 en VirtualBox, comprobando que ambas máquinas estén aisladas de Internet, organizando el espacio de trabajo y verificando la integridad de un archivo mediante SHA-256.

En esta lección **no se realiza todavía una adquisición forense completa ni se analizan eventos de Windows**.

## Escenario

Se trabajará con dos máquinas virtuales:

```text
Kali Linux
└── estación analista
    IP: 192.168.77.10/24

Windows 10
└── sistema objetivo
    IP: 192.168.77.20/24
```

Ambas máquinas deben conectarse mediante una red interna de VirtualBox llamada:

```text
lab_forense
```

Configuración esperada:

```text
Kali Linux  <------ lab_forense ------>  Windows 10
192.168.77.10/24                         192.168.77.20/24

Internet: sin acceso
Gateway: ninguno
DNS: ninguno
```

---

## Parte 1 - Revisar el laboratorio

Registra la siguiente información de ambas máquinas virtuales:

- nombre de la VM;
- sistema operativo;
- RAM asignada;
- cantidad de CPU virtuales;
- dirección IP.

Completa un diagrama simple:

```text
Computador físico
   |
   +-- VirtualBox
         |
         +-- Red interna: lab_forense
               |
               +-- Kali Linux: __________________
               |
               +-- Windows 10: __________________
```

---

## Parte 2 - Comprobar el aislamiento

Con las máquinas virtuales apagadas, revisa en VirtualBox:

```text
Red: Red interna
Nombre: lab_forense

Portapapeles compartido: Deshabilitado
Arrastrar y soltar: Deshabilitado
Carpetas compartidas: Ninguna
```

Verifica que no exista otro adaptador de red activo con:

```text
NAT
Adaptador puente
Red NAT
Solo-anfitrión
```

### Pregunta

¿Por qué es importante evitar que el sistema objetivo se conecte directamente a Internet o a la red real?

**Respuesta:**

[Completar]

---

## Parte 3 - Verificar la red entre Kali y Windows

La configuración esperada es:

```text
Kali Linux
IP: 192.168.77.10/24

Windows 10
IP: 192.168.77.20/24
```

Comprueba la dirección IP en ambos sistemas.

Luego prueba la comunicación:

Desde Kali hacia Windows:

```bash
ping -c 4 192.168.77.20
```

Desde Windows hacia Kali:

```cmd
ping 192.168.77.10
```

Registra:

| Prueba | Resultado |
| --- | --- |
| IP de Kali | [Completar] |
| IP de Windows | [Completar] |
| Kali puede comunicarse con Windows | [ ] Sí [ ] No |
| Windows puede comunicarse con Kali | [ ] Sí [ ] No |
| Acceso esperado a Internet | [ ] Sí [ ] No |

> Si un `ping` falla, no cambies configuraciones al azar. Revisa primero la guía de comandos y consulta al docente.

---

## Parte 4 - Preparar el espacio de trabajo

En Kali crea la estructura:

```text
~/forense/LAB-03/
├── original/
├── trabajo/
├── exportados/
└── notas/
```

Utiliza:

```bash
mkdir -p ~/forense/LAB-03/{original,trabajo,exportados,notas}
```

Comprueba que fue creada:

```bash
find ~/forense/LAB-03 -maxdepth 1 -type d
```

Relaciona cada carpeta con su función:

| Carpeta | Función |
| --- | --- |
| `original/` | Conservar archivos de referencia sin trabajar directamente sobre ellos |
| `trabajo/` | Guardar las copias sobre las que se realizarán pruebas |
| `exportados/` | Guardar resultados generados durante el análisis |
| `notas/` | Guardar hashes, comandos y documentación |

---

## Parte 5 - Reconocer herramientas básicas

En Kali comprueba si existen las siguientes herramientas:

```bash
which sha256sum
which file
which lsblk
which guymager
```

Completa:

| Herramienta | ¿Está disponible? | ¿Para qué sirve? |
| --- | --- | --- |
| `sha256sum` | [ ] Sí [ ] No | [Completar] |
| `file` | [ ] Sí [ ] No | [Completar] |
| `lsblk` | [ ] Sí [ ] No | [Completar] |
| `guymager` | [ ] Sí [ ] No | [Completar] |

En esta lección solo debes **reconocer estas herramientas**. Su uso forense completo se trabajará posteriormente.

---

## Parte 6 - Prueba sencilla de integridad

Crea un archivo de prueba:

```bash
echo "Prueba LAB-03" > /tmp/prueba.txt
```

Cópialo a `original/`:

```bash
cp /tmp/prueba.txt ~/forense/LAB-03/original/
```

Crea una copia en `trabajo/`:

```bash
cp ~/forense/LAB-03/original/prueba.txt ~/forense/LAB-03/trabajo/
```

Calcula el SHA-256 de ambos archivos:

```bash
sha256sum ~/forense/LAB-03/original/prueba.txt
sha256sum ~/forense/LAB-03/trabajo/prueba.txt
```

Registra los resultados:

```text
SHA-256 original:
[Completar]

SHA-256 copia de trabajo:
[Completar]
```

### Preguntas

1. ¿Los hashes coinciden?

   [ ] Sí  
   [ ] No

2. Si coinciden, ¿qué significa?

   [Completar]

3. ¿Qué ocurriría con el hash si modificáramos la copia de trabajo?

   [Completar]

### Prueba opcional

Modifica **solo la copia de trabajo**:

```bash
echo "cambio" >> ~/forense/LAB-03/trabajo/prueba.txt
```

Calcula nuevamente:

```bash
sha256sum ~/forense/LAB-03/trabajo/prueba.txt
```

Compara el nuevo valor con el hash del archivo ubicado en `original/`.

---

## Parte 7 - Crear el estado final del laboratorio

Con ambas VMs apagadas, crea un snapshot en cada máquina con el nombre:

```text
LAB-03-configurado
```

Registra:

| VM | Snapshot creado |
| --- | --- |
| Kali Linux | [ ] Sí [ ] No |
| Windows 10 | [ ] Sí [ ] No |

### Pregunta

¿Para qué puede servir este snapshot en las próximas prácticas?

[Completar]

> Un snapshot permite volver a un estado conocido de una máquina virtual. No es una imagen forense ni reemplaza una copia de evidencia.

---

# Entrega

Entrega una bitácora breve que incluya **capturas de pantallas**:

1. inventario básico de Kali y Windows;
2. diagrama del laboratorio;
3. evidencia de la configuración de red interna;
4. IP de ambas máquinas y prueba de comunicación;
5. estructura `~/forense/LAB-03/`;
6. herramientas básicas identificadas;
7. SHA-256 del original y de la copia;
8. respuestas de la prueba de integridad;
9. evidencia del snapshot `LAB-03-configurado`.

No es necesario realizar un informe forense formal.

## Criterio principal

Al finalizar la actividad debes poder explicar:

- por qué el laboratorio debe estar aislado;
- cómo se comunican Kali y Windows sin utilizar Internet;
- por qué se separan los archivos originales de las copias de trabajo;
- para qué sirve un hash SHA-256;
- para qué sirve un snapshot.
