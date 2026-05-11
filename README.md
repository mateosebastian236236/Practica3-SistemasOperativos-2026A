# Práctica 3: Creación de Procesos

> **Laboratorio de Sistemas Operativos — 2026A**
> Escuela Politécnica Nacional · Facultad de Ingeniería en Sistemas

---

## Información General

| Campo         | Detalle                                   |
|---------------|-------------------------------------------|
| **Materia**   | Laboratorio de Sistemas Operativos        |
| **Tema**      | Creación de Procesos: `fork`, `exec`, `system` |
| **Profesora** | Ing. Diana Martínez, PhD.                 |
| **Grupo**     | F                                         |
| **Período**   | 2026A                                     |

---

## Objetivos

- Familiarizarse con el uso de las funciones `fork()`, `exec()` y `system()` en C.
- Comprender cómo el sistema operativo gestiona procesos padre e hijo.
- Identificar y resolver escenarios con procesos zombie y huérfanos.
- Implementar programas en C que demuestren la creación y sincronización de procesos.

---

## Estructura del Repositorio

```
Practica3-SistemasOperativos-2026A/
├── README.md                  ← Descripción general del proyecto
│
├── Ejercicio1_Fork/
│   ├── codigo1.c              ← 3.1.1 Identificar PID y PPID
│   ├── codigo2.c   
    ├── codigo3.c 
    ├── codigo4.c     ← 3.1.2 Padre incrementa, hijo escribe en archivo
│   └── README.md              ← Explicación y capturas del ejercicio
│
├── Ejercicio2_Exec/
│   ├── codigo5.c          ← 3.2.1 Proceso hijo con execl()
│   └── README.md
│
└── Ejercicio3_System/
    ├── codigo6.c        ← 3.3.1 Listar procesos con system()
    ├── index.html             ← Informe HTML completo de la práctica
    └── README.md
```

---

## Resumen de Ejercicios

### 3.1 Uso de `fork()`

| Ejercicio | Descripción |
|-----------|-------------|
| **3.1.1** | Ejecutar Figura 1 e identificar los valores PID y PPID de cada proceso |
| **3.1.2** | Padre incrementa variable 10 veces; hijo registra cada valor en `registro.txt` |
| **3.1.3** | Proceso hijo sin espera → proceso huérfano; modificarlo para evitarlo |
| **3.1.4** | Un padre crea 3 hijos, cada uno con una tarea diferente |

### 3.2 Uso de `exec()`

| Ejercicio | Descripción |
|-----------|-------------|
| **3.2.1** | Proceso hijo que usa `execl()` / `execvp()` para ejecutar un comando del sistema |

### 3.3 Uso de `system()`

| Ejercicio | Descripción |
|-----------|-------------|
| **3.3.1** | Proceso hijo que lista los procesos del sistema con `system("ps aux")` |

---

## Conceptos Clave

### `fork()`
Crea un clon del proceso actual. El valor de retorno determina quién es quién:
- `-1` → error
- `0` → proceso hijo
- `> 0` → proceso padre (el valor es el PID del hijo)

### `exec()`
Reemplaza la imagen de memoria del proceso por un nuevo programa. No crea un proceso nuevo; lo sustituye.

### `system()`
Ejecuta un comando del shell desde C. Equivale internamente a `fork()` + `exec("/bin/sh", "-c", cmd)`.

### Proceso zombie
Proceso que ha terminado pero cuyo padre no ha llamado `wait()` para recoger su estado de salida.

### Proceso huérfano
Proceso cuyo padre terminó antes que él. El sistema lo re-adopta bajo `init` / `systemd` (PID 1).

---

## Compilación y Ejecución

```bash
# Compilar un archivo C
gcc codigo1.c -o programa1

# Ejecutar
./programa1
```

---

## Distribución de Tareas — Grupo F

| Integrante       | Responsabilidad                                      |
|------------------|------------------------------------------------------|
| Sebastian Riios | Integración GitHub, informe HTML, documentación  |
| Emily Tituaña   | Ejercicios 3.1.1 y 3.1.2 (Figuras 1 y 2)            |
| Gabriel Quilachamin    | Ejercicios 3.1.3 y 3.1.4 (Figuras 3 y 4)            |
| Daniela Caiza    | Ejercicio 3.2.1 — uso de `exec()` (Figura 6)         |
| Stefanie Ortega  | Ejercicio 3.3.1 — uso de `system()`                  |

---

*Escuela Politécnica Nacional · 2026A*