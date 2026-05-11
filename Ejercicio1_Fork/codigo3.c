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
            /* MODIFICACIÓN 1: Se eliminó wait(0) de este bloque.
             * FUNCIÓN: Evita un bloqueo o error lógico, ya que el proceso hijo 
             * no tiene otros subprocesos a los cuales deba esperar para terminar.
             */
            break;
            
        default:
            printf("\nSoy el padre, mi PID es %d y el PID de mi hijo es %d", getpid(), pid);
            /* MODIFICACIÓN 2: Se agregó wait(NULL) en este bloque.
             * FUNCIÓN: Sincroniza los procesos. Obliga al proceso padre a pausar 
             * su ejecución hasta que el hijo termine (exit). Esto evita que el 
             * hijo se convierta en un proceso "zombie" y asegura el orden de salida.
             */
            wait(NULL); 
            break;
    }
    
    printf("\nFinal de ejecución de %d \n", getpid());
    exit(0);
}