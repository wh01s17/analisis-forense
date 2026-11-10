---
title: "Práctica guiada 14 - Análisis dinámico de una muestra en entorno controlado"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA3 - Metodología forense
lesson: "14"
author: Jordy
start: 2026-11-10
end: 2026-11-16
created_at: 2026-11-08
aliases:
  - "Práctica guiada 14 - Análisis dinámico de una muestra en entorno controlado"
---

# Actividad Lección 14 - Análisis dinámico de una muestra en entorno controlado

- **Modalidad:** parejas.
- **Duración:** 100 minutos.
- **Caso:** `AR-CASO06` - ANDES SOFT SPA (empresa ficticia), fase dinámica.
- **Entorno:** `KALI-L14` (`10.14.0.10`) y `WIN-10-L14` (`10.14.0.20`) en la red interna `lab_malware`, **sin Internet**, importadas desde `LAB14.ova`.
- **Entrega:** `plantilla-entrega-leccion-14.docx`, una por pareja, con sus 10 capturas.
- **Carácter:** formativo, sin calificación.

Consulten la [guía práctica de comandos](guia-comandos-leccion-14.md) cuando necesiten saber dónde está una opción, qué muestra una vista o qué hacer ante un error.

Las tablas y preguntas de esta actividad se responden en la plantilla, que completan en su computador. Las capturas se toman en el computador, con su herramienta de capturas de pantalla, y se pegan en el recuadro de la plantilla con el mismo número. Cada una debe mostrar la ventana completa de la máquina virtual, con su nombre en la barra de VirtualBox.

> [!warning] Seguridad del laboratorio
> La muestra es una **variante didáctica benigna**, pero se trata como malware: se ejecuta **solo en `WIN-10-L14`**, después de restaurar `LAB-L14`, con `lab_malware` como única red, sin NAT, puente ni carpetas compartidas. No se extrae en el computador anfitrión ni se sube a servicios en línea. Si el aislamiento falla, no ejecuten nada y avisen al docente.

## 0. Importar el laboratorio

1. Importen `LAB14.ova` en VirtualBox (*Archivo > Importar servicio virtualizado*). Contiene `KALI-L14` y `WIN-10-L14` ya configuradas.
2. Con las dos máquinas apagadas, tomen en cada una un snapshot llamado `LAB-L14`.
3. Enciendan Kali y ejecuten `ip -br addr`. La interfaz con `10.14.0.10/24` (en el OVA ensayado, `eth1`) es la que usarán en Wireshark. Si no aparece, avisen al docente.

## El caso

En la lección 13, el laboratorio de ANDES SOFT SPA analizó, sin ejecutarlo, el adjunto `actualizacion-seguridad.pdf`, que había llegado por correo como una actualización de seguridad de un proveedor. Sus cadenas apuntaban a persistencia y comunicación de red, pero el veredicto quedó en **confianza media**: nada se había ejecutado, y una explicación benigna, un instalador legítimo mal nombrado, seguía en pie. El informe recomendó una prueba dinámica.

El laboratorio obtuvo una **variante ejecutable** de la misma muestra, `actualizacion-andes.exe`, con las mismas cadenas e indicadores, pero con otro hash: es otro archivo. La jefatura de Seguridad de la Información autoriza tres cosas y nada más:

1. **ejecutarla una vez por pareja**, solo en `WIN-10-L14`, con `lab_malware` como única red y después de restaurar `LAB-L14`;
2. **monitorearla** con Process Monitor, Process Explorer, Autoruns, Wireshark e INetSim, y exportar la evidencia a Kali;
3. **revertir** `WIN-10-L14` al terminar.

Queda excluido afirmar autoría, intención o daño a terceros. Si la VM tiene salida a Internet, aparece tráfico fuera de `10.14.0.0/24`, el hash no coincide o la muestra sigue activa después de 6 minutos, se detiene la ejecución y se avisa al docente.

| Archivo (Kali, `/home/kali/LAB14_ENTREGA`) | Contenido |
| --- | --- |
| `caso/autorizacion.txt` | Objetivo, lo autorizado, lo excluido y las condiciones de detención |
| `muestra/AR-CASO06-variante.zip` | La muestra, comprimida con la contraseña `infected` |
| `muestra/AR-CASO06-variante.sha256` | SHA-256 del ZIP y del ejecutable |
| `notas/ficha-tecnica.txt` | Naturaleza de la variante didáctica y manejo de la muestra |
| `notas/predicciones-l13.txt` | Veredicto del análisis estático (L13) y sus seis predicciones |
| `notas/referencia-attack.txt` | Extracto local de MITRE ATT&CK |
| `respaldo/` | Evidencia grabada por el docente, solo si una máquina falla |

> ¿Qué hace realmente la muestra cuando se ejecuta, con qué evidencia se demuestra y qué cambia respecto del análisis estático?

Roles: **operador de `WIN-10-L14`** y **analista de red en Kali**. Intercambien los roles en el paso 5.

## 1. Preparar el laboratorio

**En Kali**, creen la carpeta de trabajo, lean la autorización, verifiquen el hash del ZIP y publíquenlo para Windows:

```bash
# En Kali (terminal)
CASO=~/forense/LAB-14/AR-CASO06
COMPARTIDA=/home/kali/LAB14_ENTREGA
mkdir -p "$CASO"/{00_control,02_salidas}
cd "$CASO"
cat "$COMPARTIDA/caso/autorizacion.txt"
(cd "$COMPARTIDA/muestra" && sha256sum -c --ignore-missing AR-CASO06-variante.sha256)
python3 -m http.server 8000 --bind 10.14.0.10 --directory "$COMPARTIDA/muestra"
```

Debe aparecer `AR-CASO06-variante.zip: OK` (**captura 1**). El servidor ocupa esa terminal; abran otra para seguir.

**En VirtualBox**, restauren `LAB-L14` de `WIN-10-L14` e inícienla. **En PowerShell**, descarguen y extraigan la muestra, sin ejecutarla:

```powershell
# En WIN-10-L14 (PowerShell)
Invoke-WebRequest http://10.14.0.10:8000/AR-CASO06-variante.zip -OutFile C:\Lab\muestra\AR-CASO06-variante.zip
& 'C:\Program Files\7-Zip\7z.exe' x C:\Lab\muestra\AR-CASO06-variante.zip -oC:\Lab\muestra -pinfected
Get-FileHash C:\Lab\muestra\actualizacion-andes.exe
```

El hash del ejecutable debe coincidir con su línea del manifiesto en Kali (**captura 2**). Si no coincide, no sigan.

**En Kali**, detengan el servidor con `Ctrl+C` y arranquen la red simulada, cada servicio en una terminal que queda abierta:

```bash
# En Kali (terminal)
sudo inetsim
```

```bash
# En Kali (terminal)
cd ~/forense/LAB-14/AR-CASO06
sudo dnsmasq -d -q -R -h -z --listen-address=10.14.0.10 -A /#/10.14.0.10 2>&1 | tee 02_salidas/dns-consultas.log
```

Comprueben la red antes de ejecutar: en PowerShell, `Test-NetConnection 10.14.0.10 -Port 80` debe mostrar `TcpTestSucceeded : True`, y `Resolve-DnsName prueba.invalid` debe responder `10.14.0.10`.

Abran **Wireshark** en Kali, elijan la interfaz `eth1` (la que tiene `10.14.0.10`), escriban el filtro de captura `host 10.14.0.20` e inicien la captura.

## 2. Línea base antes de ejecutar

Todo este paso se hace **en `WIN-10-L14`**. Abran cada herramienta con doble clic desde `C:\Tools` y acepten el aviso de UAC.

1. **Autoruns** (`Autoruns64.exe`): esperen a que la barra inferior diga *Ready*. Guarden el estado con *File > Save* como `C:\Lab\salida\antes.arn`. Luego abran la pestaña **Logon** y anoten qué entradas aparecen bajo `HKCU\Software\Microsoft\Windows\CurrentVersion\Run`.
2. **Process Monitor** (`Procmon64.exe`): si aparece la ventana *Process Monitor Filter*, presionen **OK** sin cambiar nada. La captura empieza sola: en la barra inferior, el contador *Showing … of … events* debe ir subiendo. Déjenlo capturando.
3. **Process Explorer** (`procexp64.exe`): déjenlo abierto, mostrando el árbol de procesos. Ahí aparecerá la muestra en el paso 3.

```text
Entradas de Logon bajo HKCU\...\Run antes de ejecutar:
```

Justo antes de ejecutar la muestra, vuelvan a Process Monitor y limpien la vista con `Ctrl+X`.

## 3. Ejecutar y observar en vivo

Ejecuten `C:\Lab\muestra\actualizacion-andes.exe` con doble clic normal, sin elevar, y anoten la hora. La muestra trabaja unos **4 minutos** y termina sola; lo que sigue se hace **mientras corre**.

- **Process Explorer:** ubiquen la muestra en el árbol y anoten sus procesos hijos. Busquen el mutex con *Find > Find Handle or DLL* (`Ctrl+F`), escribiendo `ASUpd`.
- **Wireshark:** filtro de visualización `dns || http.request`; observen qué nombre consulta `WIN-10-L14` y cada cuánto aparecen las solicitudes.
- **Autoruns:** actualicen con `F5` y comparen con *File > Compare* contra `antes.arn`.

| Observación en vivo | Herramienta | Resultado |
| --- | --- | --- |
| Procesos hijos de la muestra | Process Explorer | [Completar] |
| Mutex: proceso, tipo y nombre | Process Explorer | [Completar] |
| Nombre consultado por DNS y respuesta | Wireshark | [Completar] |
| Entrada nueva de inicio automático (ubicación, nombre, ruta) | Autoruns | [Completar] |

Mientras la muestra corre, tomen la **captura 3** (árbol de procesos), la **captura 4** (mutex encontrado) y la **captura 5** (comparación de Autoruns). Esperen a que la muestra desaparezca de Process Explorer antes de continuar. Si sigue activa a los 6 minutos, deténganla con *Kill Process Tree* y avisen al docente.

## 4. Exportar la evidencia y revertir

Lo que no se exporta antes de revertir, se pierde.

**En `WIN-10-L14`**, detengan Procmon (`Ctrl+E`) y guarden con *File > Save*, opción **All events**, formato **PML**, como `C:\Lab\salida\captura-l14.pml`. En Autoruns, guarden el estado actual como `C:\Lab\salida\despues.arn`.

**En Kali**, detengan Wireshark y guarden la captura como `~/forense/LAB-14/AR-CASO06/02_salidas/red-l14.pcapng`. En otra terminal, abran la carpeta de recepción:

```bash
# En Kali (terminal)
cd ~/forense/LAB-14/AR-CASO06
sudo impacket-smbserver -smb2support -username lab -password lab14 salida "$PWD/02_salidas"
```

**En `WIN-10-L14`**, copien la evidencia a Kali:

```powershell
# En WIN-10-L14 (PowerShell)
net use \\10.14.0.10\salida /user:lab lab14
copy C:\Lab\salida\* \\10.14.0.10\salida\
```

**En Kali**, detengan INetSim y dnsmasq con `Ctrl+C` (el servidor SMB sigue abierto: con él traerán la captura de vuelta a Windows), recuperen el registro del simulador y calculen los hashes:

```bash
# En Kali (terminal)
cd ~/forense/LAB-14/AR-CASO06
sudo cp /var/log/inetsim/service.log 02_salidas/inetsim-service.log
sudo chown -R "$USER" 02_salidas
ls -lh 02_salidas
sha256sum 02_salidas/* | tee 00_control/hashes-evidencia.txt
```

Deben estar el PML, los dos `.arn`, el pcap y los dos registros, ninguno vacío (**captura 6**). Si falta algo, no reviertan.

**En VirtualBox**, apaguen `WIN-10-L14` y restauren `LAB-L14`. Al iniciarla, comprueben que `AndesUpdate` no aparece en Autoruns y que `Test-Path "$env:APPDATA\andes"` devuelve `False` (**captura 7**). Kali no se revierte: ahí queda la evidencia.

```text
¿Qué evidencia se habría perdido si revertían antes de exportar?:
Resultado de la verificación de la reversión:
```

## 5. Aislar la muestra en la captura de Procmon

**En `WIN-10-L14`** (ya limpia), traigan la captura desde Kali y ábranla en Procmon con *File > Open*:

```powershell
# En WIN-10-L14 (PowerShell)
net use \\10.14.0.10\salida /user:lab lab14
copy \\10.14.0.10\salida\captura-l14.pml C:\Lab\
```

1. *Tools > Process Tree*: seleccionen `actualizacion-andes.exe` y usen **Include Subtree** para dejar solo la muestra y sus descendientes.
2. Agreguen un filtro `Operation is …` (`Ctrl+L`) para cada fila de la tabla, conservando el del árbol. Elijan eventos con resultado `SUCCESS` y lean la columna **Detail**.

| Hora | Operación | Proceso | Ruta o detalle | Qué muestra |
| --- | --- | --- | --- | --- |
| [Completar] | `Process Start` | `actualizacion-andes.exe` | Ruta de la imagen | Inicio de la muestra |
| [Completar] | `CreateFile` | [ ] | [Ruta de la copia] | [Completar] |
| [Completar] | `RegSetValue` | [ ] | [Clave y valor] | [Completar] |
| [Completar] | `Process Create` | [ ] | [Línea de comandos del hijo] | [Completar] |
| [Completar] | `CreateFile` / `WriteFile` | [ ] | [Archivo que escribe el hijo] | [Completar] |
| [Completar] | `TCP Connect` | [ ] | [Destino y puerto] | [Completar] |

```text
¿Por qué la consulta DNS no aparece como operación de actualizacion-andes.exe? (Quiten el filtro y busquen UDP Send al puerto 53):
Un proceso de la captura que NO pertenece a la muestra y cómo lo descartaron:
```

Con el filtro `RegSetValue` aplicado, abran la fila con doble clic y tomen la **captura 8** con su *Detail* visible. Quiten los filtros de operación, conserven el del árbol y exporten a Kali: *File > Save*, opción **Events displayed using current filter**, formato **CSV**, como `\\10.14.0.10\salida\procmon-muestra.csv`. Detengan el servidor SMB en Kali con `Ctrl+C` y agreguen el CSV a los hashes:

```bash
# En Kali (terminal)
cd ~/forense/LAB-14/AR-CASO06
sudo chown "$USER" 02_salidas/procmon-muestra.csv
sha256sum 02_salidas/procmon-muestra.csv | tee -a 00_control/hashes-evidencia.txt
```

## 6. Analizar la red con Wireshark, dnsmasq e INetSim

**En Kali**, abran `02_salidas/red-l14.pcapng` en Wireshark.

1. Filtro `dns.qry.name contains "andes"`: anoten la consulta y la respuesta.
2. Filtro `http.request && http.host == "c2-telemetria.andes-soft.invalid"`: cuenten las solicitudes, sus rutas y su *User-Agent*.
3. Sobre `/v1/checkin`, *Follow > HTTP Stream*: lean qué envió la muestra (**captura 9**). Al cerrar, vuelvan a escribir el filtro `http.request && http.host == "c2-telemetria.andes-soft.invalid"`.
4. *View > Time Display Format > Seconds Since Previous Displayed Packet*: midan el intervalo entre solicitudes (**captura 10**).

Corroboren con los registros de Kali:

```bash
# En Kali (terminal)
grep -i 'andes' 02_salidas/dns-consultas.log
grep -i 'andes' 02_salidas/inetsim-service.log
```

| Pregunta | Respuesta y fuente |
| --- | --- |
| ¿Qué dominio y qué IP? | [Completar] |
| ¿Cuántas solicitudes, a qué rutas y cada cuánto? | [Completar] |
| ¿Qué contenía `/v1/checkin`? | [Completar] |
| ¿Qué contienen las balizas `/v1/beat`? | [Completar] |
| ¿dnsmasq e INetSim registran lo mismo que Wireshark? | [Completar] |
| Tráfico de `WIN-10-L14` que no es de la muestra | [Completar] |

## 7. Predicción frente a observación

Clasifiquen cada predicción de `notas/predicciones-l13.txt` como **confirmada** (hay evidencia), **no observada** (las herramientas podían verla y no apareció) o **no evaluable** (ninguna herramienta usada podía verla). Citen la herramienta.

| Predicción | Estado | Evidencia (herramienta y dato) |
| --- | --- | --- |
| P1 Crear `cache.dat` | [Completar] | [Completar] |
| P2 Valor de autoarranque en `...\Run` | [Completar] | [Completar] |
| P3 Comunicación con el dominio | [Completar] | [Completar] |
| P4 Mutex `Global\ASUpd_7731` | [Completar] | [Completar] |
| P5 Iniciar otros procesos | [Completar] | [Completar] |
| P6 Instalador legítimo (explicación benigna) | [Completar] | [Completar] |

Mapeen **tres** comportamientos observados a técnicas de `notas/referencia-attack.txt`, citando la evidencia. La referencia incluye técnicas que no corresponden.

| Comportamiento observado | Técnica ATT&CK | Evidencia |
| --- | --- | --- |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |
| [Completar] | [Completar] | [Completar] |

```text
P2: ¿qué confirmaron y qué siguen sin comprobar?:
```

## 8. Veredicto provisional

| Campo | Contenido |
| --- | --- |
| Comportamiento observado | [Hechos, con la herramienta que los muestra] |
| Interpretación | [Qué significa para la pregunta del caso] |
| Confianza y cambio respecto del análisis estático | [`notas/predicciones-l13.txt` la dejó en confianza media: ¿sube, baja o se mantiene, y por qué?] |
| Explicación benigna (P6) | [El análisis estático no descartó que fuera un instalador legítimo: ¿sigue en pie después de ejecutarla? ¿Por qué?] |
| Limitación | [Qué no permite concluir esta prueba] |
| Recomendación | [Contención, búsqueda o prueba adicional] |

Entreguen la plantilla completa, con las 10 capturas, como indique el docente.

**Si falla una máquina:** avisen al docente y sigan el apartado de respaldo de la guía de comandos desde el paso 5, declarando en la plantilla que analizan la evidencia del ensayo docente.

## Preguntas de cierre

- ¿Qué vieron en vivo que no habrían visto en la captura de Procmon, y al revés?
- ¿Qué habría que hacer para comprobar que la persistencia realmente ejecuta la copia?
- Si la muestra hubiera detectado la máquina virtual y no hubiera hecho nada, ¿qué informarían?
