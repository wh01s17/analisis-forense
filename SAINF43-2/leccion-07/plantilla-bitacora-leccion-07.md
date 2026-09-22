---
title: "Plantilla de bitácora Lección 07 - WIN10-CASO01"
course: INF43 - Análisis Forense
lesson: "07"
---

# Bitácora Lección 07 - WIN10-CASO01

## Identificación

| Dato                  | Registro    |
| --------------------- | ----------- |
| Integrantes           | [Completar] |
| Sección               | [Completar] |
| Fecha                 | [Completar] |
| Zona horaria de Kali  | [Completar] |
| Zona horaria del caso | [Completar] |
| Versión de Chainsaw   | [Completar] |

## 1. Control de la evidencia

| Control | Resultado | Evidencia breve |
| --- | --- | --- |
| `LAB07_ENTREGA` compartida como `ro` | [Sí / No] | [Opciones mostradas por `findmnt`] |
| SHA-256 de E01 a E08 | [Todos OK / discrepancia] | [Completar] |
| Procedencia pedagógica | [Completar] | [Completar] |
| Sector inicial de Windows | [Completar] | [Valor de `mmls`] |
| Tamaño del sector | [Completar] | [Completar] |
| Offset calculado | [Completar] | [Completar] |
| Montaje de Windows como `ro` | [Sí / No] | [Opciones mostradas por `findmnt`] |

## 2. Fuente exportada

| Archivo | Ruta dentro de la imagen | Tamaño | Fecha de modificación | SHA-256 de la copia |
| --- | --- | ---: | --- | --- |
| `Security.evtx` | `C:\Windows\System32\winevt\Logs\Security.evtx` | [ ] | [ ] | [ ] |

## 3. Línea temporal de ocho evidencias

La columna `#` ya viene numerada: es la posición temporal, donde `1` es el hecho más antiguo. Escribe cada evidencia en la posición que le corresponde según su hora normalizada, no en el orden en que la encontraste. Si un campo no existe, escribe `no registrado`. Oculta cualquier contraseña como `[REDACTADO]`.

| # | Selección | Hora original | Hora normalizada | Fuente | ID | Cuenta o SID | Logon ID | Proceso, tarea u objeto | Hecho observado | Inferencia | Confianza |
| ---: | --- | --- | --- | --- | ---: | --- | --- | --- | --- | --- | --- |
| 1 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [Alta/Media/Baja] |
| 2 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 3 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 4 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 5 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 6 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 7 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 8 | [ ] | [ ] | [ ] | `Security.evtx` | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |

## 4. Correlación 1 - Uso de credenciales y sesión

```text
Evidencia A:
Evidencia B:
Campo de unión:
Relación observada:
Inferencia:
Confianza:
Explicación alternativa o dato faltante:
```

## 5. Correlación 2 - Creación de cuenta y privilegios

```text
Evidencia A:
Evidencia B:
Campo de unión:
Relación observada:
Inferencia:
Confianza:
Explicación alternativa o dato faltante:
```

## 6. Artefacto de corroboración

| Dato | Registro |
| --- | --- |
| Nombre de tarea obtenido en 4698 | [Completar] |
| Ruta buscada | [Completar] |
| Resultado | [Presente / No localizado] |
| Campo que coincide con 4698 | [Completar] |
| Qué apoya | [Completar] |
| Qué no demuestra | [Completar] |

## 7. Conclusión

Escribe una conclusión breve que distinga hecho observado, inferencia y límite:

> [Completar]

## 8. Limitaciones

- [Campo ausente, fuente no disponible o explicación alternativa].
- [Límite de atribución o de interpretación].

## Comprobación de entrega

- [ ] Conservé las horas originales y normalizadas.
- [ ] Ordené las ocho filas por tiempo.
- [ ] Usé campos concretos para las dos correlaciones.
- [ ] Separé hechos, inferencias y limitaciones.
- [ ] Reemplacé cualquier contraseña por `[REDACTADO]`.
- [ ] Copié esta bitácora a `LAB07_SALIDA` y comprobé que ambas copias coincidían.
