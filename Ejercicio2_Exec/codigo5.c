#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Definimos el comando 'ls' y su argumento '-l' para listar archivos
    char *programa = "/bin/ls";

    printf("Padre (PID %d): Iniciando proceso hijo...\n", getpid()); // Identifica al padre

    // Creación del proceso hijo
    int pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso");
        exit(1);
    } else if (pid == 0) {
        // --- Lógica del proceso hijo ---
        printf("Hijo (PID %d): Ejecutando comando ls...\n", getpid());
        
        // Reemplazo de la memoria del hijo con el ejecutable /bin/ls
        execl(programa, "ls", "-l", NULL);
        
        // Solo se ejecuta si execl falla
        perror("Error al ejecutar exec");
        exit(0);
    } else {
        // --- Lógica del proceso padre ---
        wait(0); // El padre espera a que el hijo termine
        printf("Padre: El proceso hijo ha finalizado exitosamente.\n");
    }

    return 0;
}