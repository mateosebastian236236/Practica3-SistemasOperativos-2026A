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
                hijoHasAlgo(i); // cada hijo recibe su número de iteración
                /* MODIFICACIÓN 1: Se agregó exit(0).
                 * FUNCIÓN: Evita que el proceso hijo continúe ejecutando el bucle 'for'
                 * del main() y cree sus propios procesos (prevención de Fork Bomb). 
                 * Obliga al hijo a terminar inmediatamente después de su tarea.
                 */
                exit(0);
                
            default:
                /* MODIFICACIÓN 2: Se ajustó a wait(NULL) y se eliminó exit(0).
                 * FUNCIÓN: El padre se detiene a esperar que el hijo actual termine. 
                 * Al eliminar el exit(0) que estaba aquí originalmente, se permite 
                 * que el padre continúe iterando en el bucle 'for' para crear 
                 * los siguientes hijos.
                 */
                wait(NULL); 
                printf("Mi hijo %d ha terminado.\n", i);
                break;
        }
    }
    return 0;
}

void hijoHasAlgo(int numero) {
    printf("\nEjecutando el hijo %d (PID: %d)\n", numero, getpid());
    
    /* MODIFICACIÓN 3: Se implementó una estructura condicional (if-else).
     * FUNCIÓN: Rompe la homogeneidad del código original. En lugar de que todos 
     * los procesos hagan lo mismo, el sistema lee el identificador lógico ('numero') 
     * asignado por el bucle del padre y bifurca la lógica de ejecución para 
     * asignar una tarea matemática específica a cada clon.
     */
    if (numero == 1) {
        // Hijo 1: imprime del 1 al 10
        printf("Hijo 1 - Conteo ascendente:\n");
        for (int i = 1; i <= 10; i++)
            printf("%d\t", i);
        printf("\n");

    } else if (numero == 2) {
        // Hijo 2: imprime los cuadrados del 1 al 10
        printf("Hijo 2 - Cuadrados:\n");
        for (int i = 1; i <= 10; i++)
            printf("%d\t", i * i);
        printf("\n");

    } else if (numero == 3) {
        // Hijo 3: imprime del 10 al 1 (descendente)
        printf("Hijo 3 - Conteo descendente:\n");
        for (int i = 10; i >= 1; i--)
            printf("%d\t", i);
        printf("\n");
    }
}