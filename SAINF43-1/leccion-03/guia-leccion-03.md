---
title: "Guía de comandos Lección 03 - Preparación del laboratorio forense"
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
  - "Guía de comandos Lección 03 - Preparación del laboratorio forense"
---
# Guía de comandos Lección 03 - Preparación del laboratorio forense

## Índice

- [Objetivo](#objetivo)
- [1. Configuración inicial en VirtualBox](#1-configuración-inicial-en-virtualbox)
  - [¿Qué debería quedar?](#qué-debería-quedar)
- [2. Kali: identificar el sistema](#2-kali-identificar-el-sistema)
  - [Nombre del equipo y datos básicos](#nombre-del-equipo-y-datos-básicos)
  - [Versión del sistema](#versión-del-sistema)
  - [Kernel](#kernel)
- [3. Kali: revisar la red](#3-kali-revisar-la-red)
  - [Ver interfaces y direcciones IP](#ver-interfaces-y-direcciones-ip)
  - [Ver rutas](#ver-rutas)
- [4. Kali: configurar IP estática](#4-kali-configurar-ip-estática)
- [5. Windows: revisar la red](#5-windows-revisar-la-red)
  - [CMD](#cmd)
  - [Ver rutas](#ver-rutas-1)
- [6. Windows: configurar IP estática](#6-windows-configurar-ip-estática)
  - [Opción gráfica recomendada](#opción-gráfica-recomendada)
  - [PowerShell para comprobar](#powershell-para-comprobar)
- [7. Comprobar comunicación entre las VMs](#7-comprobar-comunicación-entre-las-vms)
  - [Desde Kali hacia Windows](#desde-kali-hacia-windows)
  - [Desde Windows hacia Kali](#desde-windows-hacia-kali)
  - [Si no responde](#si-no-responde)
- [8. Comprobar aislamiento](#8-comprobar-aislamiento)
  - [Kali](#kali)
  - [Windows](#windows)
  - [Prueba complementaria](#prueba-complementaria)
- [9. Registrar fecha, hora y zona horaria](#9-registrar-fecha-hora-y-zona-horaria)
  - [Kali](#kali-1)
  - [Windows](#windows-1)
- [10. Crear la estructura del LAB-03](#10-crear-la-estructura-del-lab-03)
- [11. Reconocer herramientas básicas](#11-reconocer-herramientas-básicas)
  - [Hashing](#hashing)
  - [Identificar el tipo de archivo](#identificar-el-tipo-de-archivo)
  - [Mostrar discos y particiones](#mostrar-discos-y-particiones)
  - [Guymager](#guymager)
- [12. Herramientas adicionales disponibles](#12-herramientas-adicionales-disponibles)
  - [EWF](#ewf)
  - [Sleuth Kit](#sleuth-kit)
- [13. Prueba sencilla de integridad](#13-prueba-sencilla-de-integridad)
  - [¿Qué significa?](#qué-significa)
- [14. Demostración de una modificación](#14-demostración-de-una-modificación)
- [15. Protección contra escritura del original](#15-protección-contra-escritura-del-original)
- [16. Guardar resultados en `notas/`](#16-guardar-resultados-en-notas)
- [17. Crear snapshot final](#17-crear-snapshot-final)
- [18. Checklist técnico de cierre](#18-checklist-técnico-de-cierre)
- [Resultado esperado](#resultado-esperado)

## Objetivo

Esta guía sirve como apoyo durante la preparación del laboratorio. No es necesario memorizar todos los comandos.

La idea es que puedas:

- identificar el sistema;
- revisar la red;
- comprobar el aislamiento;
- crear la estructura de trabajo;
- reconocer herramientas;
- calcular hashes;
- dejar el laboratorio preparado para las siguientes prácticas.

Arquitectura del LAB-03:

```text
Red interna VirtualBox: lab_forense

Kali Linux
192.168.77.10/24

Windows 10
192.168.77.20/24

Gateway: ninguno
DNS: ninguno
```

---

## 1. Configuración inicial en VirtualBox

Con ambas VMs apagadas:

```text
Configuración
→ Red
→ Adaptador 1
→ Habilitar adaptador de red
→ Conectado a: Red interna
→ Nombre: lab_forense
```

Verifica también:

```text
Configuración
→ General
→ Avanzado

Portapapeles compartido: Deshabilitado
Arrastrar y soltar: Deshabilitado
```

Y revisa:

```text
Configuración
→ Carpetas compartidas
```

No debe haber carpetas compartidas configuradas.

Evita dejar adaptadores adicionales en:

```text
NAT
Adaptador puente
Red NAT
Solo-anfitrión
```

### ¿Qué debería quedar?

```text
Kali ---------------- Windows
        lab_forense

Sin conexión directa a Internet
Sin conexión a la LAN real
```

---

## 2. Kali: identificar el sistema

### Nombre del equipo y datos básicos

```bash
hostnamectl
```

Ejemplo de información que puedes encontrar:

```text
Static hostname: kali
Operating System: Kali GNU/Linux
Architecture: x86-64
```

### Versión del sistema

```bash
cat /etc/os-release
```

Ejemplo:

```text
NAME="Kali GNU/Linux"
VERSION="2026.x"
```

### Kernel

```bash
uname -a
```

No necesitas interpretar toda la línea. Solo reconoce que muestra información del kernel y de la arquitectura.

---

## 3. Kali: revisar la red

### Ver interfaces y direcciones IP

```bash
ip -br addr
```

Ejemplo esperado:

```text
lo      UNKNOWN  127.0.0.1/8
eth0    UP       192.168.77.10/24
```

El nombre de la interfaz puede variar. Podría llamarse:

```text
eth0
enp0s3
enp0s8
```

Lo importante es encontrar:

```text
192.168.77.10/24
```

### Ver rutas

```bash
ip route
```

Ejemplo esperado:

```text
192.168.77.0/24 dev eth0 scope link
```

En este laboratorio **no debería existir** una línea similar a:

```text
default via 192.168.1.1
```

Una línea que empieza con `default via` normalmente indica una ruta para salir hacia otras redes.

---

## 4. Kali: configurar IP estática

> Esta sección es orientativa. Si la IP ya está correctamente configurada, no es necesario modificarla.

### Opción simple: usar `ip` sin entrar a NetworkManager

Ejemplo para asignar `192.168.56.20/24` directamente a `eth0`:

```bash
sudo ip addr add 192.168.56.20/24 dev eth0
sudo ip link set eth0 up
```

Comprobar:

```bash
ip -br addr show eth0
```

> Si la interfaz tiene otro nombre, reemplaza `eth0` por el nombre que muestre `ip -br link`. Esta configuración es temporal y se pierde al reiniciar Kali. El comando anterior usa `192.168.56.20/24` como ejemplo; para el direccionamiento propuesto en este LAB-03, reemplázala por `192.168.77.10/24`.

### Opción persistente: editar `/etc/network/interfaces`

Usa esta opción si quieres conservar la dirección después de reiniciar Kali sin configurar un perfil de NetworkManager.

1. Edita el archivo:

```bash
sudo vi /etc/network/interfaces
```

2. Agrega la configuración de `eth0`:

```text
auto eth0
iface eth0 inet static
    address 192.168.56.20
    netmask 255.255.255.0
```

3. Guarda el archivo y aplica la configuración:

```bash
sudo ifdown --force eth0
sudo ifup eth0
```

4. Comprueba:

```bash
ip -br addr show eth0
ip route
```

> En este laboratorio no se configura `gateway`, porque la red debe permanecer aislada. En una red con salida, `gateway` debe ser la dirección del router y nunca la misma dirección IP de Kali. Reemplaza `eth0` si tu interfaz tiene otro nombre.

> El bloque usa `192.168.56.20` como ejemplo. Para seguir el direccionamiento propuesto en este LAB-03, usa `address 192.168.77.10`.

> Elige un solo método persistente: `/etc/network/interfaces` o NetworkManager. No configures ambos sobre la misma interfaz.

### Alternativa persistente: usar NetworkManager

Primero revisa las conexiones:

```bash
nmcli connection show
```

Ejemplo:

```text
NAME                TYPE      DEVICE
Wired connection 1  ethernet  eth0
```

En este ejemplo, el nombre de la conexión es:

```text
Wired connection 1
```

Entonces el comando sería:

```bash
sudo nmcli connection modify "Wired connection 1"   ipv4.method manual   ipv4.addresses 192.168.77.10/24   ipv4.gateway ""   ipv4.dns ""
```

Evitar DNS automático:

```bash
sudo nmcli connection modify "Wired connection 1" ipv4.ignore-auto-dns yes
```

Reiniciar la conexión:

```bash
sudo nmcli connection down "Wired connection 1"
sudo nmcli connection up "Wired connection 1"
```

Comprobar:

```bash
ip -br addr
ip route
```

> Sustituye `Wired connection 1` por el nombre que aparezca en tu equipo.

---

## 5. Windows: revisar la red

### CMD

```cmd
ipconfig /all
```

Busca el adaptador Ethernet.

Ejemplo esperado:

```text
Dirección IPv4. . . . . . . . . . : 192.168.77.20
Máscara de subred . . . . . . . . : 255.255.255.0
Puerta de enlace predeterminada . . :
```

La puerta de enlace debería aparecer vacía.

### Ver rutas

```cmd
route print
```

Busca la tabla IPv4.

En el laboratorio no debería existir una ruta predeterminada funcional del tipo:

```text
0.0.0.0    0.0.0.0    192.168.x.x
```

---

## 6. Windows: configurar IP estática

### Opción gráfica recomendada

```text
Panel de control
→ Redes e Internet
→ Centro de redes y recursos compartidos
→ Cambiar configuración del adaptador
→ Ethernet
→ Propiedades
→ Protocolo de Internet versión 4 (TCP/IPv4)
```

Configura:

```text
Dirección IP:       192.168.77.20
Máscara:            255.255.255.0
Puerta de enlace:   dejar vacía
DNS preferido:      dejar vacío
DNS alternativo:    dejar vacío
```

### PowerShell para comprobar

```powershell
Get-NetIPConfiguration
```

Otro comando útil:

```powershell
Get-NetIPAddress -AddressFamily IPv4
```

Ejemplo de lo que debes identificar:

```text
IPAddress : 192.168.77.20
```

---

## 7. Comprobar comunicación entre las VMs

### Desde Kali hacia Windows

```bash
ping -c 4 192.168.77.20
```

Ejemplo de respuesta correcta:

```text
64 bytes from 192.168.77.20: icmp_seq=1 ttl=128 time=1.2 ms
```

### Desde Windows hacia Kali

```cmd
ping 192.168.77.10
```

Ejemplo:

```text
Respuesta desde 192.168.77.10: bytes=32 tiempo<1ms TTL=64
```

### Si no responde

Un `ping` fallido puede deberse al firewall.

Primero revisa:

```text
1. ¿Ambas VMs están encendidas?
2. ¿Ambas usan la red interna lab_forense?
3. ¿Las IP son correctas?
4. ¿Ambas tienen máscara /24?
```

No cambies configuraciones al azar.

---

## 8. Comprobar aislamiento

### Kali

```bash
ip route
```

Correcto:

```text
192.168.77.0/24 dev eth0 scope link
```

Sospechoso para este laboratorio:

```text
default via 10.0.2.2 dev eth0
```

### Windows

```cmd
route print
```

También puedes revisar:

```powershell
Get-NetRoute -AddressFamily IPv4
```

### Prueba complementaria

```bash
ping -c 4 8.8.8.8
```

Si falla, es compatible con un laboratorio aislado.

Pero recuerda:

> Un ping fallido por sí solo no demuestra aislamiento. La revisión de la configuración y de las rutas es más importante.

---

## 9. Registrar fecha, hora y zona horaria

### Kali

```bash
date --iso-8601=seconds
```

Ejemplo:

```text
2026-08-24T09:15:20-04:00
```

Zona horaria:

```bash
timedatectl
```

Busca algo similar a:

```text
Time zone: America/Santiago
```

### Windows

PowerShell:

```powershell
Get-Date
```

Zona horaria:

```cmd
tzutil /g
```

Ejemplo:

```text
Pacific SA Standard Time
```

No cambies los relojes solo para hacerlos coincidir.

---

## 10. Crear la estructura del LAB-03

Crear las carpetas:

```bash
mkdir -p ~/forense/LAB-03/{original,trabajo,exportados,notas}
```

Comprobar:

```bash
find ~/forense/LAB-03 -maxdepth 1 -type d
```

Resultado esperado:

```text
/home/kali/forense/LAB-03
/home/kali/forense/LAB-03/original
/home/kali/forense/LAB-03/trabajo
/home/kali/forense/LAB-03/exportados
/home/kali/forense/LAB-03/notas
```

Si tienes `tree` instalado:

```bash
tree ~/forense/LAB-03
```

Salida esperada:

```text
LAB-03
├── exportados
├── notas
├── original
└── trabajo
```

---

## 11. Reconocer herramientas básicas

Los comandos de esta sección comienzan con `which`. Este comando no abre ni ejecuta la herramienta: solamente muestra la ruta de su ejecutable.

```text
Salida con una ruta → el comando está disponible.
Sin salida           → el comando no fue encontrado en las rutas del usuario.
```

### Hashing

```bash
which sha256sum
```

Ejemplo:

```text
/usr/bin/sha256sum
```

Función:

```text
Calcular el hash SHA-256 de un archivo.
```

Uso:

```bash
sha256sum <ARCHIVO>
```

Ejemplo:

```bash
sha256sum ~/forense/LAB-03/original/prueba.txt
```

La salida tiene dos partes:

```text
<hash SHA-256 de 64 caracteres>  <nombre del archivo>
```

Si el hash del original y el de su copia son iguales, ambos tienen el mismo contenido. Si son diferentes, algún byte cambió. El hash detecta el cambio, pero no dice cuál fue ni quién lo realizó.

### Identificar el tipo de archivo

```bash
which file
```

Ejemplo:

```text
/usr/bin/file
```

Uso simple:

```bash
file /etc/passwd
```

`file` examina el contenido y no confía únicamente en la extensión. Por eso puede ayudar a reconocer un archivo que fue renombrado para ocultar su tipo real.

Ejemplo de salida:

```text
/etc/passwd: ASCII text
```

Esta descripción es una identificación inicial; no reemplaza un análisis posterior del archivo.

### Mostrar discos y particiones

```bash
which lsblk
which fdisk
```

`lsblk` muestra discos, particiones y puntos de montaje sin modificar su contenido. Para obtener columnas útiles durante la identificación:

```bash
lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,MODEL,SERIAL
```

Ejemplo simplificado:

```text
NAME   SIZE TYPE
sda     80G disk
├─sda1  79G part
└─sda2   1G part
```

`fdisk -l` entrega más detalles sobre las tablas de particiones:

```bash
sudo fdisk -l
```

Antes de trabajar con un dispositivo, compara su nombre, tamaño, modelo y número de serie. No ejecutes `fdisk /dev/sdX` en modo interactivo durante esta actividad, porque ese modo permite modificar las particiones.

### Guymager

```bash
which guymager
```

Si está instalado, podría devolver:

```text
/usr/bin/guymager
```

Guymager se utiliza para adquisición forense de medios de almacenamiento.

Es una aplicación gráfica que permite seleccionar un dispositivo de origen, crear una imagen forense y calcular hashes de verificación. En esta lección, `which guymager` se usa únicamente para comprobar que el ejecutable está disponible; todavía no debes adquirir un disco real.

---

## 12. Herramientas adicionales disponibles

Estas herramientas se utilizarán con mayor profundidad en lecciones posteriores.

### EWF

```bash
which ewfacquire
which ewfinfo
which ewfverify
```

Funciones generales:

```text
ewfacquire → adquirir un dispositivo o archivo y crear una imagen EWF
ewfinfo    → mostrar los metadatos registrados en una imagen EWF
ewfverify  → recalcular y comprobar la integridad de una imagen EWF
```

Una adquisición EWF suele producir segmentos con nombres como `imagen.E01`, `imagen.E02`, etc. Para consultar y verificar una imagen ya existente:

```bash
ewfinfo imagen.E01
ewfverify imagen.E01
```

`ewfinfo` ayuda a revisar datos como el tamaño y la descripción del caso. `ewfverify` informa si la comprobación de integridad finalizó correctamente.

> No ejecutes `ewfacquire` sobre un disco real en esta lección. Primero debes identificar con certeza el origen, el destino y los controles contra escritura.

### Sleuth Kit

```bash
which mmls
which fls
```

Funciones generales:

```text
mmls → mostrar las particiones y sus sectores de inicio
fls  → listar archivos y directorios contenidos en un sistema de archivos
```

Uso inicial:

```bash
mmls <IMAGEN>
fls -o <SECTOR_INICIAL> -r <IMAGEN>
```

Primero, `mmls` permite encontrar el sector donde comienza una partición. Después, ese número se entrega a `fls` mediante `-o`. La opción `-r` solicita un listado recursivo.

Si faltan herramientas:

```bash
sudo apt update
sudo apt install ewf-tools sleuthkit guymager
```

`apt update` actualiza la lista de paquetes disponibles. `apt install` instala `ewf-tools`, Sleuth Kit y Guymager. Ambos comandos necesitan acceso a los repositorios, por lo que deben ejecutarse antes de aislar la VM en la red interna.

> No es necesario utilizar todas estas herramientas durante la actividad de la Lección 03.

---

## 13. Prueba sencilla de integridad

Crear un archivo:

```bash
echo "Prueba LAB-03" > /tmp/prueba.txt
```

Ver contenido:

```bash
cat /tmp/prueba.txt
```

Resultado:

```text
Prueba LAB-03
```

Copiar a `original/`:

```bash
cp /tmp/prueba.txt ~/forense/LAB-03/original/
```

Crear copia de trabajo:

```bash
cp ~/forense/LAB-03/original/prueba.txt ~/forense/LAB-03/trabajo/
```

Calcular hash del original:

```bash
sha256sum ~/forense/LAB-03/original/prueba.txt
```

Ejemplo:

```text
a1b2c3...  /home/kali/forense/LAB-03/original/prueba.txt
```

Calcular hash de la copia:

```bash
sha256sum ~/forense/LAB-03/trabajo/prueba.txt
```

Los dos valores deberían ser iguales.

### ¿Qué significa?

```text
Hash original = Hash copia
        ↓
El contenido comparado es idéntico.
```

---

## 14. Demostración de una modificación

Modifica únicamente la copia de trabajo:

```bash
echo "cambio" >> ~/forense/LAB-03/trabajo/prueba.txt
```

Calcula nuevamente:

```bash
sha256sum ~/forense/LAB-03/trabajo/prueba.txt
```

Ahora deberías observar:

```text
Hash original ≠ Hash copia modificada
```

Esto demuestra que una modificación en el contenido produce un hash diferente.

No modifiques el archivo de:

```text
~/forense/LAB-03/original/
```

---

## 15. Protección contra escritura del original

Esta sección es opcional, pero muestra una buena práctica.

Evitar escritura sobre el archivo:

```bash
chmod a-w ~/forense/LAB-03/original/prueba.txt
```

Comprobar permisos:

```bash
ls -l ~/forense/LAB-03/original/prueba.txt
```

Ejemplo:

```text
-r--r--r-- 1 kali kali 14 Aug 24 09:30 prueba.txt
```

Para devolver permiso de escritura al propietario:

```bash
chmod u+w ~/forense/LAB-03/original/prueba.txt
```

> En una investigación real, la preservación de evidencia requiere procedimientos adicionales. Esta demostración solo introduce el concepto.

---

## 16. Guardar resultados en `notas/`

Guardar información del sistema:

```bash
hostnamectl > ~/forense/LAB-03/notas/sistema-kali.txt
```

Guardar la red:

```bash
ip -br addr > ~/forense/LAB-03/notas/red-kali.txt
ip route >> ~/forense/LAB-03/notas/red-kali.txt
```

Guardar hora:

```bash
date --iso-8601=seconds > ~/forense/LAB-03/notas/tiempo-kali.txt
```

Guardar hash:

```bash
sha256sum ~/forense/LAB-03/original/prueba.txt   > ~/forense/LAB-03/notas/hash-original.txt
```

Ver el archivo guardado:

```bash
cat ~/forense/LAB-03/notas/hash-original.txt
```

---

## 17. Crear snapshot final

Apaga correctamente ambas VMs.

En VirtualBox:

```text
Seleccionar VM
→ Instantáneas / Snapshots
→ Tomar
```

Nombre recomendado:

```text
LAB-03-configurado
```

Créalo tanto para Kali como para Windows.

El snapshot sirve para:

```text
Volver a un estado conocido
Repetir una práctica
Recuperarse de una configuración incorrecta
```

No debe confundirse con:

```text
Imagen forense
Copia de evidencia
Backup formal de evidencia
```

---

## Resultado esperado

```text
Kali <---- Red interna lab_forense ----> Windows 10

Kali:
192.168.77.10/24

Windows:
192.168.77.20/24

Sin gateway
Sin DNS
Sin NAT
Sin acceso a Internet
```
