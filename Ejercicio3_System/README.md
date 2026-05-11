# Ejercicio 3 — Uso de `system()`

> **Práctica 3 · Laboratorio de Sistemas Operativos 2026A**
> Escuela Politécnica Nacional · Facultad de Ingeniería en Sistemas
> **Integrante:** Stefanie Ortega (Ejercicio 3.3.1)

---

## Descripción

Este ejercicio cubre el uso de la función `system()` para listar los procesos activos del sistema desde un programa en C. Se crea un proceso hijo mediante `fork()` que llama a `system()` para ejecutar comandos de monitoreo (`ps -f --forest` y `pstree`), mientras el padre espera al hijo para evitar procesos zombie.

---

## Archivos

| Archivo      | Ejercicio | Descripción                                              |
|--------------|-----------|----------------------------------------------------------|
| `codigo6.c`  | 3.3.1     | Proceso hijo que lista los procesos del sistema con `system()` |

---

## ¿Qué es `system()`?

`system()` es una función de la biblioteca estándar de C (`<stdlib.h>`) que permite ejecutar un comando de shell directamente desde un programa en C/C++. Sirve para lanzar comandos del sistema, scripts u otros ejecutables sin salir del programa actual.

**Prototipo:**
```c
int system(const char *command);
```

### ¿Para qué se usa?

- Ejecutar comandos rápidos de consola.
- Lanzar scripts de Bash o binarios del sistema.
- Realizar tareas de administración sencillas desde un programa.

### Características

| Característica | Descripción |
|----------------|-------------|
| **Abstracción** | Es una función de alto nivel que abstrae la complejidad de usar `fork`, `execl` y `wait` manualmente. |
| **Eficiencia de desarrollo** | Reduce líneas de código, aunque sacrifica algo de control sobre el proceso exacto. |
| **Dependencia del shell** | Siempre depende de que `/bin/sh` esté disponible en el sistema. |

> ⚠️ **Advertencia de seguridad:** No debe usarse con cadenas provenientes del usuario (riesgo de inyección de comandos). Para producción, se prefiere `execvp()` con argumentos controlados.

### ¿Cómo funciona internamente?

`system()` usa `fork()` para crear un proceso hijo que ejecuta el comando de shell indicado, usando `execl()` de la siguiente forma:

```c
execl("/bin/sh", "sh", "-c", command, (char *) 0);
```

`system()` regresa después de que el comando termina de ejecutarse. Esto significa que **bloquea al proceso que lo llama** hasta que el comando de la terminal termina.

Debido a que `system()` solo recibe un parámetro de tipo `const char *`, es indispensable el uso de un buffer intermedio para formatear datos numéricos (como PIDs) en una cadena de texto ejecutable por el shell.

---

## Ejercicio 3.3.1 — Proceso hijo que lista los procesos del sistema

### Funciones utilizadas

| Función        | Propósito                                                          |
|----------------|--------------------------------------------------------------------|
| `fork()`       | Crea el proceso hijo                                               |
| `getpid()`     | Retorna el PID del proceso actual                                  |
| `getppid()`    | Retorna el PID del proceso padre                                   |
| `system()`     | Ejecuta un comando del shell desde el proceso hijo                 |
| `sprintf()`    | Construye un comando dinámico con el PID del padre                 |
| `wait(NULL)`   | El padre espera al hijo antes de terminar (evita proceso zombie)   |
| `exit()`       | Termina el proceso hijo de forma controlada                        |

### Código — `codigo6.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>  /* Necesaria para pid_t */

int main() {
    /* Obtenemos el PID del proceso principal (padre) antes del fork */
    pid_t pid_padre = getpid();
    printf("--- Iniciando ejecucion del sistema de procesos ---\n");
    printf("[PRINCIPAL] PID del proceso original: %d\n\n", pid_padre);

    pid_t pid = fork();

    if (pid < 0) {
        /* Manejo de errores */
        perror("Error al crear el proceso (fork)");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* ── Bloque del PROCESO HIJO ── */
        printf("[HIJO]  Identificador (PID): %d\n", getpid());
        printf("[HIJO]  PID de mi padre (PPID): %d\n", getppid());
        printf("[HIJO]  Mostrando estado de procesos activos...\n");
        printf("--------------------------------------------------\n");

        /*
         * ps -f --forest: muestra los procesos activos con jerarquía en ramas.
         * Incluye: UID, PID, PPID, uso de CPU, hora de inicio y comando.
         */
        system("ps -f --forest");

        printf("--------------------------------------------------\n");

        /* pstree: genera un árbol visual con el PID del padre como raíz */
        printf("[HIJO] Generando arbol visual con pstree:\n");

        /*
         * Se usa un buffer char[] para construir el comando dinámico.
         * sprintf convierte el PID (entero) a cadena para que system() lo ejecute.
         * Esto es necesario porque system() solo acepta un único argumento (const char *).
         */
        char arreglo[100];
        sprintf(arreglo, "pstree -p -A %d", getppid());
        system(arreglo);

        printf("--------------------------------------\n");
        exit(0);  /* El hijo termina de forma exitosa */

    } else {
        /* ── Bloque del PROCESO PADRE ── */
        printf("[PADRE] Identificador (PID): %d\n", getpid());
        printf("[PADRE] He creado un hijo con el PID: %d\n", pid);
        printf("[PADRE] Esperando a que el hijo finalice...\n\n");

        wait(NULL);  /* Espera al hijo — prevención de proceso zombie */

        printf("\n[PADRE] El proceso hijo ha terminado. Finalizando programa.\n");
    }

    return 0;
}
```

### Compilar y ejecutar

```bash
gcc codigo6.c -o programa6
./programa6
```

### Resultado esperado en consola

```
--- Iniciando ejecucion del sistema de procesos ---
[PRINCIPAL] PID del proceso original: 2041

[PADRE] Identificador (PID): 2041
[PADRE] He creado un hijo con el PID: 2042
[PADRE] Esperando a que el hijo finalice...

[HIJO]  Identificador (PID): 2042
[HIJO]  PID de mi padre (PPID): 2041
[HIJO]  Mostrando estado de procesos activos...
--------------------------------------------------
UID        PID  PPID  C STIME TTY          TIME CMD
user      2041  1023  0 10:00 pts/0    00:00:00 ./programa6
user      2042  2041  0 10:00 pts/0    00:00:00  \_ /bin/sh -c ps -f --forest
...
--------------------------------------------------
[HIJO] Generando arbol visual con pstree:
programa6(2041)---programa6(2042)---sh(2043)---pstree(2044)
--------------------------------------

[PADRE] El proceso hijo ha terminado. Finalizando programa.
```

---

## Herramientas de monitoreo utilizadas

### 1. `ps -f --forest`
### 2. `pstree -p -A`
---

## Análisis

### Genealogía de procesos

En el código se implementó un `fork()` manual con el objetivo de crear una estructura jerárquica que permite demostrar la genealogía de procesos. Cuando el proceso hijo llama a `system()`, este a su vez hace un `fork()` invisible que usa `execl()` para transformarse en `/bin/sh`, y desde allí el shell ejecuta `pstree` o `ps`, permitiendo observar la estructura gráfica completa.

### Función `sprintf` y el buffer dinámico

`system()` es especial porque solo recibe **un único argumento** de tipo `const char *`. Se utiliza `sprintf` para unir variables en la cadena de comandos y construir instrucciones dinámicas basadas en los PIDs del sistema. Esto permite que el programa sea flexible y pueda monitorear su propio árbol de procesos sin importar qué PID le asigne el sistema en cada ejecución.

### Bloqueo de `system()`

`system()` bloquea al proceso que lo llama hasta que el comando de la terminal termina. En este caso el hijo se queda pausado mientras `pstree` dibuja el árbol. Una vez que el árbol termina de imprimirse, `system()` regresa y el hijo puede continuar a la siguiente línea de código (o terminar con `exit`).

---

## Referencias

[1] DataFlair Team, "Process in Linux - Linux Process Management Tutorial," *DataFlair*. Disponible en: https://data-flair.training/blogs/process-in-linux/ [Accedido: 10-may-2026].

[2] "system(3) — Linux manual page," *man7.org*, marzo de 2024. Disponible en: https://man7.org/linux/man-pages/man3/system.3.html