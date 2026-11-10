---
title: "Guía práctica de comandos Lección 14 - Análisis dinámico"
tags:
  - nota
  - course
  - curso
  - guia-comandos
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "14"
author: Jordy
start: 2026-11-10
end: 2026-11-16
created_at: 2026-11-08
aliases:
  - "Guía práctica de comandos Lección 14 - Análisis dinámico"
---

# Guía práctica de comandos Lección 14 - Análisis dinámico

Esta guía acompaña la actividad de la Lección 14. Explica, en el orden de la actividad, para qué sirve cada herramienta, dónde está cada opción, qué resultado esperar y qué hacer si algo falla.

| Máquina | Dirección | Rol |
| --- | --- | --- |
| Kali | `10.14.0.10` | Estación del analista: simula Internet (INetSim y dnsmasq), captura la red (Wireshark) y recibe la evidencia |
| `WIN-10-L14` | `10.14.0.20` | Máquina de análisis (*sandbox*): ejecuta la muestra y la monitorea (Sysinternals) |

Ambas están en la red interna `lab_malware`, sin salida a Internet. `WIN-10-L14` usa a Kali como puerta de enlace y como DNS: todo lo que la muestra intente contactar llega a Kali.

> [!warning] Antes de ejecutar
> Restauren `LAB-L14`, revisen que `WIN-10-L14` tenga un solo adaptador (red interna `lab_malware`) y que INetSim y Wireshark estén corriendo en Kali. Si algo de eso falla, no ejecuten la muestra.

## Herramientas

| Herramienta | Máquina | Origen | Para qué sirve |
| --- | --- | --- | --- |
| Snapshot | Anfitrión | Función de VirtualBox | Volver al estado limpio de una VM |
| INetSim | Kali | Instalado en la VM | Simular servicios web y registrar cada solicitud |
| dnsmasq | Kali | Instalado en la VM | Responder el DNS con la IP de Kali y registrar cada consulta |
| Wireshark | Kali | Instalado en la VM | Ver qué viajó por la red y con qué contenido |
| Impacket (smbserver) | Kali | Instalado en la VM | Recibir la evidencia que envía Windows |
| Autoruns | WIN-10-L14 | Microsoft Sysinternals; no viene con Windows, está copiado en `C:\Tools` | Ver qué quedó configurado para iniciarse solo (persistencia) |
| Process Monitor | WIN-10-L14 | Microsoft Sysinternals; no viene con Windows, está copiado en `C:\Tools` | Registrar qué archivos, claves, procesos y conexiones tocó cada proceso |
| Process Explorer | WIN-10-L14 | Microsoft Sysinternals; no viene con Windows, está copiado en `C:\Tools` | Ver los procesos en vivo, su padre y sus objetos abiertos |
| 7-Zip | WIN-10-L14 | Instalado en la VM | Extraer el ZIP con contraseña |

Ninguna herramienta lo ve todo: Procmon no muestra *mutex*, Process Explorer no guarda historia, Wireshark no sabe qué proceso envió cada paquete. Por eso se combinan.

## Paso 1. Preparar el laboratorio

### Orientarse entre máquinas, carpetas y terminales

El anfitrión es su computador físico; Kali y Windows son las dos VM dentro de VirtualBox. Para abrir una terminal de Kali usen `Ctrl+Alt+T`; para PowerShell, búsquenlo en Inicio de Windows. No ejecuten las órdenes de una máquina en la otra.

| Expresión | Significa |
| --- | --- |
| `~/` | Carpeta personal del usuario de Kali, aquí `/home/kali/` |
| `CASO` | Variable que apunta a su carpeta de trabajo |
| `COMPARTIDA` | Variable que apunta al paquete local `/home/kali/LAB14_ENTREGA`; no es una carpeta compartida de VirtualBox |
| `00_control/` | Manifiesto de hashes de la evidencia |
| `02_salidas/` | Capturas, registros e imágenes que conservarán |
| `%APPDATA%` | En el Explorador de Windows, carpeta de datos del usuario |
| `$env:APPDATA` | La misma carpeta, escrita para PowerShell |
| `HKCU` | Rama del registro de Windows que contiene la configuración del usuario actual |

Una variable de Bash solo existe en la terminal donde la definieron. Para retomar el trabajo en una terminal nueva:

```bash
# En Kali (terminal)
CASO=~/forense/LAB-14/AR-CASO06
COMPARTIDA=/home/kali/LAB14_ENTREGA
cd "$CASO"
```

No cierren una terminal que mantiene un servicio activo. `Ctrl+C` detiene el programa de esa terminal; no detiene los de las demás.

| Terminal de Kali | Durante la ejecución | Cuándo detenerla |
| --- | --- | --- |
| Servidor HTTP en puerto 8000 | Ya debe estar detenido | Después de descargar el ZIP en Windows |
| A: `sudo inetsim` | Abierta, simulando servicios web | Paso 4, después de que termine la muestra |
| B: `sudo dnsmasq ...` | Abierta, registrando consultas DNS | Paso 4, después de que termine la muestra |
| C: órdenes y recepción SMB | Libre para comprobar archivos; luego mantiene SMB activo | SMB se detiene tras exportar el CSV del paso 5 |

Wireshark es otra ventana: debe seguir capturando hasta el paso 4. El docente ya instaló las herramientas y configuró ambas VM; estos pasos comprueban esa preparación.

### Comprobar el hash en Kali

```bash
# En Kali (terminal)
(cd "$COMPARTIDA/muestra" && sha256sum -c --ignore-missing AR-CASO06-variante.sha256)
```

El manifiesto tiene dos líneas: el ZIP y el ejecutable. En Kali solo está el ZIP; `--ignore-missing` evita que el ejecutable aparezca como falla. **Resultado esperado:** `AR-CASO06-variante.zip: OK`.

### Llevar la muestra a WIN-10-L14

```bash
# En Kali (terminal)
python3 -m http.server 8000 --bind 10.14.0.10 --directory "$COMPARTIDA/muestra"
```

| Parte | Qué hace |
| --- | --- |
| `python3 -m http.server 8000` | Levanta un servidor web mínimo en el puerto 8000 |
| `--bind 10.14.0.10` | Escucha solo en la interfaz del laboratorio |
| `--directory ...` | Sirve únicamente la carpeta de la muestra |

Se usa la red y no una carpeta compartida porque una carpeta compartida conecta la máquina de análisis con el anfitrión: si la muestra la usara, saldría del laboratorio.

En `WIN-10-L14`:

```powershell
# En WIN-10-L14 (PowerShell)
Invoke-WebRequest http://10.14.0.10:8000/AR-CASO06-variante.zip -OutFile C:\Lab\muestra\AR-CASO06-variante.zip
& 'C:\Program Files\7-Zip\7z.exe' x C:\Lab\muestra\AR-CASO06-variante.zip -oC:\Lab\muestra -pinfected
Get-FileHash C:\Lab\muestra\actualizacion-andes.exe
```

| Parte | Qué hace |
| --- | --- |
| `Invoke-WebRequest URL -OutFile ruta` | Descarga el archivo |
| `Get-FileHash` | Calcula SHA-256 (por defecto). Muestra el hash en mayúsculas: se compara igual |
| `& '...\7z.exe' x` | `&` ejecuta un programa cuya ruta tiene espacios. `x` extrae conservando rutas; `-o` es la carpeta de destino y `-p` la contraseña, ambas sin espacio |

**El ZIP con contraseña `infected`** identifica un paquete que debe manipularse con cuidado y exige extraerlo deliberadamente. La contraseña es pública: no protege información confidencial ni garantiza que un antivirus no inspeccione o bloquee el archivo. Extraerlo no equivale a ejecutarlo. Comparen los 64 caracteres del hash del ejecutable con su línea del manifiesto en Kali; ante una diferencia, deténganse.

### INetSim y dnsmasq

```bash
# En Kali (terminal)
sudo inetsim
```

INetSim simula servicios web en `10.14.0.10`: su HTTP y su HTTPS aceptan cualquier solicitud y devuelven una página genérica. **Resultado esperado:** la terminal termina con `Simulation running.` Se detiene con `Ctrl+C`; el registro queda en `/var/log/inetsim/service.log`.

```bash
# En Kali (terminal)
sudo dnsmasq -d -q -R -h -z --listen-address=10.14.0.10 -A /#/10.14.0.10 2>&1 | tee 02_salidas/dns-consultas.log
```

| Opción | Forma larga | Qué hace |
| --- | --- | --- |
| `-d` | `--no-daemon` | Queda en primer plano y escribe su registro en la terminal; se detiene con `Ctrl+C` |
| `-q` | `--log-queries` | Registra cada consulta: qué nombre, qué tipo y desde qué IP |
| `-R` | `--no-resolv` | No reenvía nada a un DNS real |
| `-h` | `--no-hosts` | No usa el archivo `hosts` de Kali |
| `-z` y `--listen-address=10.14.0.10` | `--bind-interfaces` | Escucha solo en la red del laboratorio |
| `-A /#/10.14.0.10` | `--address=/#/10.14.0.10` | Responde **cualquier** nombre (`#`) con la IP de Kali |
| `2>&1 \| tee 02_salidas/dns-consultas.log` | — | Además de mostrarlo, lo guarda como evidencia |

En este laboratorio el DNS dirige los nombres consultados al simulador. Se utiliza la configuración que el docente probó en las VM entregadas; no hay que instalar ni modificar el sistema durante la actividad.

Así la muestra cree tener Internet: cualquier nombre que consulte la lleva a Kali, donde INetSim la atiende. El DNS lo hace dnsmasq porque el servicio DNS de INetSim 1.3.2 no arranca con la versión actual de Kali. **Resultado esperado:** `dnsmasq: started` y, apenas Windows haga algo, líneas `query[A] … from 10.14.0.20`. Las consultas de tipo `HTTPS` aparecen como `REFUSED`: es normal.

### Wireshark: capturar

1. En una terminal de Kali ejecuten `ip -br addr`. Busquen la fila con `10.14.0.10/24`; su primer campo es el nombre de la interfaz. En el OVA ensayado es **`eth1`**. En *Capture > Options* elijan esa interfaz, aunque su nombre sea distinto.
2. En **Capture filter**, `host 10.14.0.20`: solo se guarda el tráfico de `WIN-10-L14`.
3. *Start*.

Si no ven paquetes, comprueben que Windows está encendida y que eligieron la interfaz con la IP del laboratorio. No seleccionen `lo` (la comunicación de Kali consigo misma).

Antes de ejecutar, con los dos servicios encendidos, prueben en PowerShell `Test-NetConnection 10.14.0.10 -Port 80` (debe mostrar `TcpTestSucceeded : True`) y `Resolve-DnsName prueba.invalid` (debe responder `10.14.0.10`). Si el DNS falla, prueben en Kali `dig @10.14.0.10 prueba.invalid +short` para saber si el problema es dnsmasq o Windows.

El **filtro de captura** decide qué se guarda; el **filtro de visualización** (la barra verde) decide qué se muestra de lo guardado. Se usan los dos.

## Paso 2. Línea base con Autoruns

Autoruns reúne en una sola vista todos los lugares desde donde Windows ejecuta programas automáticamente: claves `Run`, carpeta de inicio, servicios, tareas programadas y otros.

| Acción | Dónde |
| --- | --- |
| Ver las claves `Run` y la carpeta de inicio | Pestaña **Logon** |
| Guardar el estado | *File > Save* (archivo `.arn`) |
| Comparar con un estado guardado | *File > Compare* |
| Actualizar | `F5` |
| Ocultar entradas firmadas por Microsoft | *Options > Hide Microsoft Entries* |

**Por qué una línea base:** sin el estado anterior, una entrada nueva se confunde con una legítima. Con *Compare*, Autoruns resalta lo que apareció.

## Paso 2. Process Monitor: empezar a capturar

| Acción | Atajo |
| --- | --- |
| Iniciar o detener la captura | `Ctrl+E` |
| Limpiar la vista | `Ctrl+X` |
| Filtros | `Ctrl+L` |
| Árbol de procesos de la captura | *Tools > Process Tree* (`Ctrl+T`) |

Procmon registra cada operación de archivos, registro, procesos y red de **todos** los procesos. Captura miles de eventos por minuto: no se intenta leer todo en vivo, se filtra después (paso 5). Necesita privilegios de administrador; acepten el aviso de UAC. Esto eleva Procmon, no la muestra: ejecuten la muestra mediante doble clic normal. Antes de ejecutar: detengan con `Ctrl+E`, limpien con `Ctrl+X` y reanuden con `Ctrl+E`; debe aumentar el número de eventos. Dejen **Filter > Drop Filtered Events** desmarcado para que ocultar un evento no lo descarte de la captura.

## Paso 3. Process Explorer: observar en vivo

Process Explorer es un administrador de tareas avanzado: muestra los procesos en árbol, con su padre, y los objetos que cada uno tiene abiertos.

| Acción | Dónde |
| --- | --- |
| Ver el árbol | Vista por defecto (*View > Show Process Tree*) |
| Procesos que aparecen o terminan | Se resaltan en verde (nuevo) y rojo (terminado) por un segundo |
| Buscar un objeto abierto | *Find > Find Handle or DLL* (`Ctrl+F`) |
| Ver los objetos de un proceso | Seleccionar el proceso y `Ctrl+H` (panel inferior) |
| Detener una muestra | Clic derecho > **Kill Process Tree** |

**Mutex:** un *mutex* es un objeto con nombre que un programa crea para no ejecutarse dos veces a la vez. Windows lo llama **Mutant**. Al buscar `ASUpd`, Process Explorer debe mostrar el proceso dueño y el nombre `\BaseNamedObjects\ASUpd_7731` (`Global\` se guarda en `\BaseNamedObjects`). Solo se ve **mientras el proceso existe**: por eso se busca en vivo.

`cmd.exe` y `systeminfo.exe` duran pocos segundos. Si no alcanzan a verlos en Process Explorer, la captura de Procmon los registra igual.

## Paso 4. Exportar y revertir

### Guardar en Procmon

*File > Save*:

| Opción | Cuándo |
| --- | --- |
| **All events**, formato **PML** | Respaldo completo; se vuelve a abrir en Procmon |
| **Events displayed using current filter**, formato **CSV** | Lo que se cita en la plantilla de entrega; se abre en cualquier editor |

### Recibir la evidencia en Kali

```bash
# En Kali (terminal)
sudo impacket-smbserver -smb2support -username lab -password lab14 salida "$PWD/02_salidas"
```

| Parte | Qué hace |
| --- | --- |
| `impacket-smbserver` | Servidor de carpetas compartidas de Windows (SMB) escrito en Python |
| `-smb2support` | Habilita SMB2, que Windows 10 exige |
| `-username lab -password lab14` | Windows 10 no acepta conexiones anónimas |
| `salida "$PWD/02_salidas"` | Nombre del recurso y carpeta de Kali donde quedan los archivos |

Necesita `sudo` porque usa el puerto 445. En `WIN-10-L14`:

```powershell
# En WIN-10-L14 (PowerShell)
net use \\10.14.0.10\salida /user:lab lab14
copy C:\Lab\salida\* \\10.14.0.10\salida\
```

**Resultado esperado:** `Se ha completado el comando correctamente` y los archivos copiados. Los archivos llegan a Kali como propiedad de root; `sudo chown -R "$USER" 02_salidas` los devuelve a su usuario.

### Comprobar lo recibido antes de revertir

En Kali, desde la carpeta del caso, ejecuten `ls -lh 02_salidas`. Deben existir el PML, los dos `.arn`, el pcap y los registros DNS e INetSim, ninguno con tamaño 0. Un hash correcto por sí solo no demuestra que se capturó lo necesario.

El CSV `procmon-muestra.csv` se crea en el paso 5, después de calcular los hashes: su hash se agrega al final de `hashes-evidencia.txt` (`tee -a`).

### Revertir y verificar

En VirtualBox: apagar `WIN-10-L14` (*Apagar*, no guardar estado) y *Snapshots > LAB-L14 > Restaurar*. Después, en Autoruns, comprobar que `AndesUpdate` no está y, en el Explorador, que `%APPDATA%\andes` no existe.

**Por qué verificar:** una reversión que no se comprueba es un supuesto. Si algo sobrevivió, la próxima prueba ya no parte de un estado limpio.

## Paso 5. Filtrar la captura de Procmon

### Aislar la muestra

1. *Tools > Process Tree* (`Ctrl+T`).
2. Seleccionen `actualizacion-andes.exe`.
3. Botón **Include Subtree**: agrega a los filtros la muestra **y todos sus descendientes** (`cmd.exe`, `systeminfo.exe`).
4. Cierren el árbol: la vista ya muestra solo la muestra.

Hacerlo por árbol y no por nombre es lo que separa la muestra del ruido: hay muchos `cmd.exe` en Windows, pero solo uno es hijo de la muestra.

### Filtros por operación

*Filter > Filter* (`Ctrl+L`): `Operation` `is` `RegSetValue` → *Add* → *OK*. Para la siguiente fila, desmarquen ese filtro y agreguen otro.

| Operación | Qué registra | Columnas que importan |
| --- | --- | --- |
| `Process Start` | Inicio de un proceso | *Detail*: PID del padre y línea de comandos |
| `Process Create` | Un proceso crea otro | *Path*: imagen del hijo; *Detail*: línea de comandos |
| `CreateFile` | Abrir o crear un archivo | *Detail*: `Disposition` (`Create`, `OverwriteIf`, `Open`) |
| `WriteFile` | Escribir en un archivo | *Path* y *Detail* (tamaño) |
| `SetEndOfFileInformationFile` | Fijar el tamaño de un archivo | Aparece al copiar: Windows copia con `CopyFile` y Procmon muestra `CreateFile` + `SetEndOfFileInformationFile`, no `WriteFile` |
| `RegSetValue` | Escribir un valor del registro | *Path* (clave y valor) y *Detail* (dato) |
| `TCP Connect`, `TCP Send` | Conexiones y envíos de red | *Path*: origen → destino:puerto; *Detail*: longitud |

Atajos útiles: clic derecho sobre una celda > **Include 'valor'** agrega ese valor como filtro. *Tools > Count Occurrences* cuenta valores de una columna. La columna *Time of Day* muestra la hora con segundos y fracción: es la que se cita en la plantilla de entrega.

**Cómo elegir un evento:** lean *Process Name*, *PID*, *Operation*, *Path*, *Result* y *Detail*. `SUCCESS` indica que esa operación tuvo éxito; `NAME NOT FOUND` indica que no encontró el objeto, no que lo creó. `CreateFile` puede abrir un archivo ya existente: revisen el detalle y otras operaciones sobre la misma ruta. La evidencia de autocopia se refuerza comparando el hash del original y de la copia; sin esa comparación, describan los eventos observados y eviten afirmar identidad de bytes.

Antes de guardar `procmon-muestra.csv`, quiten los filtros de **Operation** y conserven los de la muestra y sus descendientes. Así exportan toda esa actividad, no únicamente la última operación buscada.

### La consulta DNS

En el ensayo de estas VM, las consultas DNS salen del servicio **Cliente DNS** (`svchost.exe`), que resuelve el nombre solicitado por la muestra. Por eso, con el filtro de la muestra, no aparece ninguna consulta. Quiten el filtro y busquen `UDP Send` con destino `10.14.0.10:53`: el proceso será un `svchost.exe`. Wireshark sí la muestra, porque ve la red, no los procesos.

## Paso 6. Analizar la red

### Filtros de visualización

| Filtro | Muestra |
| --- | --- |
| `dns` | Todas las consultas y respuestas DNS |
| `dns.qry.name contains "andes"` | Solo las consultas por el dominio del caso |
| `http.request` | Todas las solicitudes HTTP, incluido el ruido de Windows |
| `http.request && http.host == "c2-telemetria.andes-soft.invalid"` | Las solicitudes al dominio del caso: usen este filtro para contar y medir intervalos |
| `http.request.method == "POST"` | Solo los envíos |
| `!(ip.addr == 10.14.0.10 && tcp.port == 445) && !(tcp.port == 8000)` | Oculta el tráfico de la descarga y del SMB del laboratorio |

### Ver el contenido

Clic derecho sobre la solicitud > *Follow > HTTP Stream*. Distingan la petición de Windows (`POST`, ruta y cabeceras) de la respuesta del servidor (`HTTP/1.1`, código y cabeceras). Los colores habituales son rojo para el cliente y azul para el servidor, pero pueden cambiar según el tema visual. Es el mismo *follow* de la lección 08, ahora en la interfaz gráfica.

Al cerrar *Follow*, puede quedar aplicado un filtro como `tcp.stream eq ...`: solo verán una conversación. Vuelvan a escribir el filtro del dominio antes de contar todas las solicitudes o medir intervalos.

### Medir el intervalo

Con `http.request && http.host == "c2-telemetria.andes-soft.invalid"` aplicado: *View > Time Display Format > Seconds Since Previous Displayed Packet*. La columna *Time* muestra el tiempo entre una solicitud y la anterior visible. Lean desde la segunda solicitud y anoten varios intervalos. Si usan todas las solicitudes HTTP, el tráfico ajeno puede alterar la medición.

### Corroborar con INetSim

```bash
# En Kali (terminal)
grep -i 'andes' 02_salidas/inetsim-service.log
```

dnsmasq registra cada consulta DNS e INetSim cada solicitud HTTP que recibió. Comparen en dnsmasq el nombre, la IP cliente y la hora con el DNS de Wireshark; comparen en INetSim las solicitudes web con el HTTP capturado. Los registros se complementan: dnsmasq no registra el cuerpo de HTTP. Para revisar líneas vecinas a un dominio, abran `less 02_salidas/inetsim-service.log`, busquen `/andes`, avancen con `n` y salgan con `q`.

**Ruido esperado:** Windows intenta comprobar su conexión a Internet (`msftconnecttest.com`, `www.msftncsi.com`), SmartScreen, `wpad` y otros servicios. dnsmasq e INetSim también les responden. No los atribuyan a la muestra por compartir IP: correlacionen dominio, ruta, hora y conexiones en Procmon. Estar fuera del árbol es un dato de atribución, pero servicios como Cliente DNS pueden actuar por encargo de la muestra.

## Interpretar sin exceder la evidencia

| Observación | Afirmación respaldada | Afirmación que excede la evidencia |
| --- | --- | --- |
| Autoruns muestra `AndesUpdate` en `HKCU\...\Run` | La muestra registró su copia para iniciarse con la sesión | La muestra se ejecuta en cada inicio (no se reinició) |
| Procmon: operaciones exitosas sobre `asupd.exe` y Autoruns apuntando a ella | Hay actividad sobre el archivo y una entrada de inicio que lo referencia; hay que revisar los detalles de creación | La copia se ejecutó o tiene bytes idénticos sin verificar su hash |
| Process Explorer: Mutant `ASUpd_7731` | La muestra crea un mutex con ese nombre | La muestra evita a propósito el análisis |
| Wireshark: `POST /v1/checkin` con la salida de `systeminfo` | La muestra envió la información del sistema a su dominio | Un atacante recibió y usó esa información |
| INetSim respondió `200` | El simulador aceptó la solicitud | Existe un servidor real |
| Sin descargas en 4 minutos | No se observaron descargas en esa ejecución | La muestra nunca descarga nada |

## Si algo falla

| Síntoma | Qué revisar |
| --- | --- |
| `Invoke-WebRequest` no conecta | ¿Está corriendo `http.server` en Kali? ¿La IP es `10.14.0.10`? |
| `7z.exe` dice *Wrong password* | La contraseña va pegada a `-p`: `-pinfected` |
| La muestra no hace nada | ¿Existe `C:\Lab\autorizado.txt`? ¿Defender la borró? Avisen al docente |
| No aparece la consulta DNS | ¿Está corriendo dnsmasq? ¿El filtro de captura es `host 10.14.0.20`? Prueben en Kali `dig @10.14.0.10 prueba.invalid +short` |
| Wireshark no muestra la interfaz | Cierren sesión y vuelvan a entrar (permiso de captura) o avisen al docente |
| No encuentran el mutex | La muestra ya terminó; aparece solo mientras corre. Registren «no observado en vivo» o repitan desde el snapshot |
| `net use` falla | ¿Sigue corriendo `impacket-smbserver`? Usuario `lab`, clave `lab14` |
| Procmon abre la captura vacía | Guardaron con la vista filtrada; usen **All events** |
| La muestra sigue activa a los 6 minutos | Process Explorer > *Kill Process Tree*, revertir y avisar al docente |
| Su `WIN-10-L14` no funciona | Sigan el apartado de respaldo de la actividad. El PML necesita una VM Windows operativa con Procmon; si no la tienen, trabajen con `procmon-muestra.csv` en Kali y declárenlo. En Kali pueden analizar el pcap, CSV y registros. Declaren la procedencia docente y las limitaciones |

## Referencias de las opciones de interfaz

Consultadas el 08-11-2026: [Process Monitor](https://learn.microsoft.com/en-us/sysinternals/downloads/procmon), [Autoruns](https://learn.microsoft.com/en-us/sysinternals/downloads/autoruns), [formatos de tiempo de Wireshark](https://www.wireshark.org/docs/wsug_html_chunked/ChWorkTimeFormatsSection.html) y [Follow Stream](https://www.wireshark.org/docs/wsug_html_chunked/ChAdvFollowStreamSection.html). Las direcciones IP, interfaces y resultados del caso proceden de la preparación y del ensayo docente local.

## Si una máquina falla: trabajar con el respaldo

Avisen al docente y declaren en la plantilla **«análisis de evidencia del ensayo docente»**: no presenten esa ejecución como propia. Copien los archivos sin extraer ni ejecutar la muestra:

```bash
# En Kali (terminal)
CASO=~/forense/LAB-14/AR-CASO06
mkdir -p "$CASO"/{00_control,02_salidas}
cp /home/kali/LAB14_ENTREGA/respaldo/* "$CASO/02_salidas/"
cd "$CASO/02_salidas"
sha256sum -c /home/kali/LAB14_ENTREGA/notas/respaldo.sha256
```

Continúen desde el paso 5. `captura-l14.zip` contiene el PML de Procmon: extráiganlo con 7-Zip en una **VM Windows de análisis operativa** y ábranlo allí. Si Windows no arranca, trabajen con `procmon-muestra.csv` en Kali y declaren que no revisaron el PML. En Kali abran `red-l14.pcapng` con Wireshark y consulten los registros de DNS e INetSim. El respaldo trae `autoruns-antes.csv` y `autoruns-despues.csv`, que se comparan como texto, en lugar de los `.arn` de una ejecución propia.

Sin una observación en vivo, el respaldo no permite confirmar el mutex. Declaren P4 **no evaluable con este respaldo**. Registren también que la reversión corresponde al ensayo docente y no fue comprobada por ustedes. Conserven los hashes originales del respaldo y calculen aparte los de sus nuevas salidas.
