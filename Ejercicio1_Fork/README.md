# Ejercicio 1 — Uso de `fork()`

> **Práctica 3 · Laboratorio de Sistemas Operativos 2026A**
> Escuela Politécnica Nacional · Facultad de Ingeniería en Sistemas
> **Integrantes:** Emily Tituaña (3.1.1 – 3.1.2) · Gabriel Quilachamin (3.1.3 – 3.1.4)

---

## Descripción

Este ejercicio cubre los cuatro casos de uso de `fork()`: identificación de procesos mediante PID y PPID, comunicación indirecta entre padre e hijo a través de un archivo de texto, comportamiento de procesos huérfanos, y creación de un padre con múltiples hijos que realizan tareas distintas.

---

## Archivos

| Archivo     | Ejercicio | Descripción                                              |
|-------------|-----------|----------------------------------------------------------|
| `codigo1.c` | 3.1.1     | Identificar PID y PPID de proceso padre e hijo           |
| `codigo2.c` | 3.1.2     | Padre incrementa variable; hijo registra en archivo      |
| `codigo3.c` | 3.1.3     | Proceso hijo sin espera — proceso huérfano               |
| `codigo4.c` | 3.1.4     | Un padre y tres procesos hijo con tareas distintas       |

---

## Ejercicio 3.1.1 — Identificar PID y PPID

### Funciones utilizadas

| Función     | Propósito                                                    |
|-------------|--------------------------------------------------------------|
| `fork()`    | Crea un nuevo proceso hijo (clon del padre)                  |
| `getpid()`  | Retorna el PID del proceso actual                            |
| `getppid()` | Retorna el PID del proceso padre (PPID)                      |
| `sleep(1)`  | Pausa la ejecución 1 segundo para observar el comportamiento |

### Código — `codigo1.c`

```c
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Ejemplo de fork.\n");
    printf("Inicio del proceso padre. PID=%d\n", getpid());

    if (fork() == 0)
    {
        /* Proceso hijo */
        printf("Inicio proceso hijo. PID=%d, PPID=%d\n",
               getpid(), getppid());
        sleep(1);
    }
    else
    {
        /* Proceso padre */
        printf("Continuacion del padre. PID=%d\n", getpid());
        sleep(1);
    }

    printf("Fin del proceso %d\n", getpid());
    return 0;
}
```

### Compilar y ejecutar

```bash
gcc codigo1.c -o programa1
./programa1
```

### Resultado obtenido

```
Ejemplo de fork.
Inicio del proceso padre. PID=1327
Continuacion del padre.   PID=1327
Inicio proceso hijo.      PID=1328, PPID=1327
Fin del proceso 1327
Fin del proceso 1328
```

> Los valores exactos de PID varían en cada ejecución.

### Análisis de PIDs

| Proceso | PID    | PPID   | Observación                                      |
|---------|--------|--------|--------------------------------------------------|
| Padre   | `1327` | shell  | Inicia el programa y llama a `fork()`            |
| Hijo    | `1328` | `1327` | Creado por `fork()`; su PPID es el PID del padre |

---

## Ejercicio 3.1.2 — Padre incrementa; hijo registra en archivo

### Funciones utilizadas

| Función      | Propósito                                                    |
|--------------|--------------------------------------------------------------|
| `fork()`     | Crea el proceso hijo                                         |
| `fopen()`    | Abre `registro.txt` en modo escritura                        |
| `fprintf()`  | Escribe el valor de la iteración en el archivo               |
| `fflush()`   | Fuerza la escritura en disco en cada iteración               |
| `fclose()`   | Cierra el archivo al finalizar                               |
| `wait(NULL)` | El padre espera al hijo antes de terminar (evita zombie)     |
| `sleep(1)`   | Pausa de 1 segundo para sincronización visual                |

### Código — `codigo2.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int pid = fork();
    int variable = 0;

    if (pid == 0) {
        /* Proceso hijo: escribe en archivo */
        FILE *f = fopen("registro.txt", "w");
        if (f == NULL) exit(1);

        for (int i = 1; i <= 10; i++) {
            variable += 10;
            fprintf(f, "iteracion %d: %d\n", i, variable);
            fflush(f);  /* asegura escritura inmediata */
            printf("Hijo registra: %d\n", variable);
            sleep(1);
        }

        fclose(f);
        exit(0);

    } else {
        /* Proceso padre: incrementa e imprime */
        for (int i = 1; i <= 10; i++) {
            variable += 10;
            printf("Padre incrementa: %d\n", variable);
            sleep(1);
        }
        wait(NULL);  /* espera al hijo antes de terminar */
    }

    return 0;
}
```

### Compilar y ejecutar

```bash
gcc codigo2.c -o programa2
./programa2
```

### Resultado en consola

```
Padre incrementa: 10
Hijo registra: 10
Padre incrementa: 20
Hijo registra: 20
...
Padre incrementa: 100
Hijo registra: 100
```

### Contenido de `registro.txt` generado

```
iteracion 1: 10
iteracion 2: 20
iteracion 3: 30
iteracion 4: 40
iteracion 5: 50
iteracion 6: 60
iteracion 7: 70
iteracion 8: 80
iteracion 9: 90
iteracion 10: 100
```

### Nota sobre memoria de procesos

Después de `fork()`, padre e hijo tienen **copias independientes** de las variables. Los incrementos del padre no afectan la copia del hijo. Ambos llevan el mismo contador de 10 en 10, de forma paralela e independiente.

---

## Ejercicio 3.1.3 — Proceso hijo sin espera (Proceso Huérfano)

### Código original — `codigo3.c`

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int pid;

int main() {
    pid = fork();
    switch (pid) {
        case -1:
            printf("\nNo he podido crear el proceso hijo");
            break;
        case 0:
            printf("\nSoy el hijo, mi PID es %d y mi PPID es %d", getpid(), getppid());
            wait(0);
            break;
        default:
            printf("\nSoy el padre, mi PID es %d y el PID de mi hijo es %d", getpid(), pid);
    }
    printf("\nFinal de ejecución de %d \n", getpid());
    exit(0);
}
```

### Compilar y ejecutar

```bash
gcc codigo3.c -o programa3
./programa3
```

### Preguntas del ejercicio

**a) ¿Cuál es el PID del proceso padre y del proceso hijo creado?**

Del proceso padre es **1065** y del proceso hijo es **1066** (valores obtenidos en la ejecución real; varían en cada corrida).

**b) Verifique a qué proceso corresponde el ID encontrado.**

- PID **1065** → proceso padre: es el proceso original que ejecuta el programa.
- PID **1066** → proceso hijo: creado mediante `fork()`.

El padre termina antes que el hijo, por lo que el SO reasigna al hijo bajo `init` / `systemd` (PID 1). Cabe mencionar que el hijo alcanza a capturar el PPID real justo antes de que el sistema operativo lo reasigne.

**c) ¿Cómo se denomina al tipo de proceso hijo?**

Al proceso hijo se lo denomina **proceso huérfano** (*orphan process*), ya que en el momento de la ejecución el padre termina primero. El sistema operativo lo re-adopta automáticamente bajo `systemd` / `init` (PID 1).

| Pregunta | Respuesta |
|----------|-----------|
| PPID del hijo tras quedar huérfano | `1` (adoptado por `systemd`/`init`) |
| Proceso correspondiente a ese ID | `systemd` o `init`, primer proceso del sistema |
| Denominación del tipo de proceso | **Proceso huérfano** (*orphan process*) |

**d) Código modificado para evitar el proceso huérfano**

Se agrega `wait(NULL)` en el bloque `default` (padre) para que espere al hijo antes de terminar:

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int pid;

int main() {
    pid = fork();
    switch (pid) {
        case -1:
            printf("\nNo he podido crear el proceso hijo");
            break;
        case 0:
            printf("\nSoy el hijo, mi PID es %d y mi PPID es %d", getpid(), getppid());
            /* Se retira wait(0) del hijo */
            break;
        default:
            printf("\nSoy el padre, mi PID es %d y el PID de mi hijo es %d", getpid(), pid);
            wait(NULL);  /* ← añadir esta línea evita el huérfano */
            break;
    }
    printf("\nFinal de ejecución de %d \n", getpid());
    exit(0);
}
```
### Resultado en consola
Soy el padre, mi PID es 1065 y el PID de mi hijo es 1066
Final de ejecución de 1065

Soy el hijo, mi PID es 1066 y mi PPID es 1065
Final de ejecución de 1066

---

> **Verificación:** Con `wait(NULL)` en el padre, cuando el hijo imprime su PPID, este sigue siendo el PID del padre real (no 1), confirmando que el proceso ya no queda huérfano.

---




## Ejercicio 3.1.4 — Un padre y tres procesos hijo con tareas distintas

### Código original (Figura 4)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROC 3

void hijoHasAlgo(int numero);

int main() {
    int i, pid;
    for (i = 1; i <= NUM_PROC; i++) {
        pid = fork();
        switch(pid) {
            case -1:
                fprintf(stderr, "Error al crear el proceso\n");
                break;
            case 0:
                hijoHasAlgo(i);
                break;
            default:
                printf("Ejecutando el padre\n");
                wait(0);
                printf("Mi hijo %d ha terminado. \n", i);
                exit(0);
        }
    }
}

void hijoHasAlgo(int numero) {
    int i, MAX = 10;
    printf("Ejecutando el hijo %d: \n", numero);
    for (i = 1; i <= MAX; i++)
        printf("%d\t", i);
    printf("\n");
}
```

### Modificación — cada hijo realiza una tarea diferente

**a) Código modificado — `codigo4.c`**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROC 3

void hijoHasAlgo(int numero);

int main() {
    int i, pid;
    for (i = 1; i <= NUM_PROC; i++) {
        pid = fork();
        switch(pid) {
            case -1:
                fprintf(stderr, "Error al crear el proceso\n");
                break;
            case 0:
                hijoHasAlgo(i); /* cada hijo recibe su número */
                exit(0);
            default:
                wait(NULL);     /* el padre espera a cada hijo */
                printf("Mi hijo %d ha terminado.\n", i);
                break;
        }
    }
    return 0;
}

void hijoHasAlgo(int numero) {
    printf("\nEjecutando el hijo %d (PID: %d)\n", numero, getpid());

    if (numero == 1) {
        /* Hijo 1: imprime del 1 al 10 (conteo ascendente) */
        printf("Hijo 1 - Conteo ascendente:\n");
        for (int i = 1; i <= 10; i++)
            printf("%d\t", i);
        printf("\n");

    } else if (numero == 2) {
        /* Hijo 2: imprime los cuadrados del 1 al 10 */
        printf("Hijo 2 - Cuadrados:\n");
        for (int i = 1; i <= 10; i++)
            printf("%d\t", i * i);
        printf("\n");

    } else if (numero == 3) {
        /* Hijo 3: imprime del 10 al 1 (conteo descendente) */
        printf("Hijo 3 - Conteo descendente:\n");
        for (int i = 10; i >= 1; i--)
            printf("%d\t", i);
        printf("\n");
    }
}
```

### Compilar y ejecutar

```bash
gcc codigo4.c -o programa4
./programa4
```

### Resultado esperado

```
Ejecutando el hijo 1 (PID: XXXX)
Hijo 1 - Conteo ascendente:
1    2    3    4    5    6    7    8    9    10
Mi hijo 1 ha terminado.

Ejecutando el hijo 2 (PID: XXXX)
Hijo 2 - Cuadrados:
1    4    9    16    25    36    49    64    81    100
Mi hijo 2 ha terminado.

Ejecutando el hijo 3 (PID: XXXX)
Hijo 3 - Conteo descendente:
10    9    8    7    6    5    4    3    2    1
Mi hijo 3 ha terminado.
```

### Descripción de tareas por hijo

| Hijo | Tarea                          | Descripción                        |
|------|--------------------------------|------------------------------------|
| 1    | Conteo ascendente (1 → 10)    | Imprime los números del 1 al 10    |
| 2    | Cuadrados (1² → 10²)          | Imprime los cuadrados del 1 al 10  |
| 3    | Conteo descendente (10 → 1)   | Imprime los números del 10 al 1    |

---

## Referencias

- `man 2 fork` — documentación de fork
- `man 3 fopen` — documentación de manejo de archivos en C
- Guía de Práctica 3 — Ing. Diana Martínez, PhD.