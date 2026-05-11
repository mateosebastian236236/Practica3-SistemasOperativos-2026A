# Ejercicio 2 — Uso de `exec()`

> **Práctica 3 · Laboratorio de Sistemas Operativos 2026A**
> Escuela Politécnica Nacional · Facultad de Ingeniería en Sistemas
> **Responsable:** Daniela Caiza

---

## Descripción

En este ejercicio se crea un proceso hijo con `fork()` que utiliza la llamada al sistema `execl()` de manera directa para sustituir el espacio de direcciones de memoria del proceso hijo con el ejecutable del comando de listado detallado `ls -l`. El padre espera a que el hijo termine mediante `wait(0)`.

---

## Archivo

| Archivo             | Ejercicio | Descripción                                       |
|---------------------|-----------|---------------------------------------------------|
| `ejercicio_exec.c`  | 3.2.1     | Proceso hijo que ejecuta `ls -l` usando `execl()` |

---

## Funciones del Sistema Utilizadas

| Función    | Descripción                                                                                                                              |
|------------|------------------------------------------------------------------------------------------------------------------------------------------|
| `fork()`   | Duplica el proceso llamador, creando un nuevo proceso hijo que se ejecuta de forma concurrente                                           |
| `execl()`  | Carga y ejecuta un nuevo archivo binario (`/bin/ls`), reemplazando por completo la imagen de memoria del proceso hijo. Requiere una lista de argumentos terminada con un puntero `NULL` |
| `wait(0)`  | Suspende temporalmente la ejecución del proceso padre, manteniéndolo a la espera de que el proceso hijo finalice su tarea               |
| `getpid()` | Devuelve el identificador único de proceso (PID) asignado por el SO al proceso actual                                                    |

---

## Diferencia clave: `fork()` vs `exec()`

| Función    | Comportamiento                                                                  |
|------------|---------------------------------------------------------------------------------|
| `fork()`   | **Crea** un nuevo proceso (clon del actual). Ambos continúan ejecutándose.      |
| `exec()`   | **Reemplaza** el programa del proceso actual. No crea uno nuevo.                |
| Combinados | Patrón estándar UNIX: `fork()` crea el hijo, `exec()` le asigna nuevo programa  |

---

## Código — `ejercicio_exec.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    switch (pid) {
        case -1:
            fprintf(stderr, "No se pudo crear el proceso hijo\n");
            exit(1);

        case 0:
            /* Proceso hijo: reemplaza su imagen con ls -l */
            printf("Hijo (PID %d): Ejecutando comando \"ls\"...\n", getpid());
            execl("/bin/ls", "ls", "-l", NULL);
            /* Solo llega aquí si execl falla */
            fprintf(stderr, "Error al ejecutar execl\n");
            exit(1);

        default:
            /* Proceso padre: espera al hijo */
            printf("Padre (PID %d): Iniciando proceso hijo...\n", getpid());
            wait(0);
            printf("Padre: El proceso hijo ha finalizado exitosamente.\n");
            break;
    }
    return 0;
}
```

---

## Compilación

```bash
gcc ejercicio_exec.c -o ejercicio_exec
```

- `gcc` — compilador que transforma el código fuente en un ejecutable
- `ejercicio_exec.c` — archivo con el código fuente
- `-o ejercicio_exec` — define el nombre del ejecutable de salida

---

## Ejecución

```bash
./ejercicio_exec
```

### Explicación del resultado

| Proceso | PID    | Acción                                                                          |
|---------|--------|---------------------------------------------------------------------------------|
| Padre   | `1189` | Inicia el ciclo, obtiene su PID y ejecuta `fork()`                              |
| Hijo    | `1190` | Creado por `fork()`, su memoria es reemplazada por `/bin/ls` mediante `execl()` |
| Salida  | —      | Se muestran permisos, propietario, tamaño y fecha de los archivos del directorio |
| Padre   | `1189` | Sale de `wait(0)` al recibir la señal de fin del hijo y finaliza                |


---

## Referencias

- `man 2 execl` — documentación de execl
- `man 2 fork` — documentación de fork
- Guía de Práctica 3 — Ing. Diana Martínez, PhD.