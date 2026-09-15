---
title: "Actividad Lección 06 - Mesa de evaluación y preservación de evidencia"
tags:
  - nota
  - course
  - curso
  - actividad
institution: CFT San Antonio
course: INF43 - Análisis Forense
unit: UA2 - La evidencia digital
lesson: "06"
author: Jordy
start: 2026-09-14
end: 2026-09-16
created_at: "2026-08-07 11:56"
aliases:
  - "Actividad Lección 06 - Mesa de evaluación y preservación de evidencia"
---
# Actividad Lección 06 - Mesa de evaluación y preservación de evidencia

- **Modalidad:** equipos de tres, con roles estables.
- **Caso:** `CSA-2026-044` - COSTA AZUL SPA (ficticia).
- **Carácter:** formativo, sin calificación.
- **Producto:** una ficha técnica compacta.

## Pregunta del caso

> ¿Qué fuentes deben preservarse primero y bajo qué condiciones pueden aceptarse para un análisis posterior?

La actividad evalúa decisiones de preservación. No busca reconstruir el incidente ni identificar a una persona.

## Lo que debes demostrar

1. Interpretar una verificación de integridad sin inventar su causa.
2. Evaluar seis fuentes por origen, volatilidad, amenaza y valor potencial.
3. Distinguir propietario, custodio, fuente original y objeto recibido.
4. Proponer preservación con responsable, autorización, integridad y condición de detención.
5. Estimar almacenamiento, considerar la sensibilidad, controlar el acceso y comunicar límites.

## Material y roles

Usa solamente:

- `material/acta-recepcion.md`;
- `material/inventario-fuentes.csv`;
- `material/manifest_entrega.sha256`;
- los cuatro archivos de `material/exportados/`.

| Rol | Responsabilidad |
| --- | --- |
| Operador | Ejecuta los comandos mínimos autorizados. |
| Registrador | Completa la ficha y conserva la salida relevante. |
| Revisor | Comprueba alcance, responsables y límites de las conclusiones. |

Mantengan los roles durante toda la actividad.

## Reglas

```text
Trabaja sobre una copia local del material.
No accedas a sistemas, cuentas ni dispositivos reales.
No modifiques el manifiesto ni los archivos recibidos.
Ante una discrepancia: registra, aísla y escala.
Una prioridad de preservación no demuestra culpabilidad ni intención.
```

## Fase 1 - Alcance e integridad

1. Lean `material/acta-recepcion.md`.
2. Registren integrantes, roles, una acción permitida y una prohibida.
3. Desde `analisis-forense/leccion-06/`, creen la copia y ejecuten solo estos controles:

```bash
mkdir -p ~/forense/LAB-06
cp -a material/. ~/forense/LAB-06/
cd ~/forense/LAB-06
sha256sum -c manifest_entrega.sha256
sed -n '1,/^$/p' exportados/correo-alerta.eml
```

Si un comando falla, consulten la guía técnica o informen al docente. No regeneren el manifiesto.

### E1 - Registro mínimo

```text
Archivo con discrepancia:
Archivos que coinciden con el manifiesto:
La discrepancia demuestra que:
La discrepancia no permite afirmar que:
Cabecera ausente y limitación del EML:
```

## Fase 2 - Evaluación de las seis fuentes

Lean `material/inventario-fuentes.csv`. No copien nuevamente propietario, custodio, tamaño ni retención: consúltenlos allí y completen solo las decisiones.

| ID       | Volatilidad y amenaza principal                                       | Valor potencial y sensibilidad                                           | Estado o acción inmediata y responsable                                                                              |
| -------- | --------------------------------------------------------------------- | ------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------- |
| EV-06-01 | Alta: la RAM cambia durante el uso y se pierde al apagar o reiniciar. | Alto: puede contener procesos, conexiones y secretos; sensibilidad alta. | Pendiente de adquisición: infraestructura mantiene el equipo encendido y un especialista autorizado adquiere la RAM. |
| EV-06-02 | [Alta/media/baja + razón]                                             | [Nivel + razón; dato sensible]                                           | [Decisión + responsable]                                                                                             |
| EV-06-03 | [Alta/media/baja + razón]                                             | [Nivel + razón; dato sensible]                                           | [Decisión + responsable]                                                                                             |
| EV-06-04 | [Alta/media/baja + razón]                                             | [Nivel + razón; dato sensible]                                           | [Decisión + responsable]                                                                                             |
| EV-06-05 | [Alta/media/baja + razón]                                             | [Nivel + razón; dato sensible]                                           | [Decisión + responsable]                                                                                             |
| EV-06-06 | [Alta/media/baja + razón]                                             | [Nivel + razón; dato sensible]                                           | [Decisión + responsable]                                                                                             |

### Tres comprobaciones obligatorias

Respondan con una oración cada una:

1. Elijan EV-06-03 o EV-06-05: ¿quién es propietario, quién es custodio y quién debe autorizar?
2. ¿Por qué `pcap-indice.csv` o `respaldo-bd-inventario.csv` no equivale a la fuente original?
3. Para una fuente, completen: «El dato disponible permite decidir ____, pero antes de afirmar ____ falta ____».

## Fase 3 - Prioridad y preservación

Seleccionen las dos fuentes que requieren la primera acción. Pueden compartir prioridad si intervienen responsables distintos.

| Nivel | ID  | Razón: pérdida + valor | Acción y responsable | Condición previa o de detención |
| ----: | --- | ---------------------- | -------------------- | ------------------------------- |
|    1A |     |                        |                      |                                 |
|    1B |     |                        |                      |                                 |

Formulen un plan breve para esas dos fuentes. No ejecuten las acciones propuestas.

| ID  | Método o formato | Control de integridad | Almacenamiento, acceso y conservación | Detener si... |
| --- | ---------------- | --------------------- | ------------------------------------- | ------------- |
| [ ] |                  |                       |                                       |               |
| [ ] |                  |                       |                                       |               |

### Estimación de almacenamiento

Usen `tamano_estimado` de las dos fuentes seleccionadas:

```text
Suma de las fuentes: ___ GiB
x 2 (copia preservada + copia de trabajo): ___ GiB
+ 20 % de margen: ___ GiB
Medio, cifrado y control de acceso propuestos: ___
```

No usen el tamaño del índice o inventario como si fuera el tamaño de la fuente.

## Fase 4 - Conclusión y salida

### Conclusión del equipo

Redacten entre 80 y 120 palabras. Incluyan:

- alcance y método;
- resultado de integridad;
- dos decisiones de prioridad;
- una consideración de propiedad o almacenamiento;
- una limitación.

## Revisión final

- [ ] Evaluamos las seis fuentes sin copiar todo el inventario.
- [ ] Diferenciamos fuente, objeto recibido, propietario y custodio.
- [ ] La prioridad considera pérdida, valor, autorización y responsable.
- [ ] Los planes incluyen integridad, almacenamiento, acceso y detención.
- [ ] La conclusión comunica un límite y no atribuye acciones a una persona.
