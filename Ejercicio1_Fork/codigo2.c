#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    int pid = fork();
    int variable = 0;

    if (pid == 0) {

        FILE *f = fopen("registro.txt", "w");
        if (f == NULL) exit(1);

        for (int i = 1; i <= 10; i++) {

            variable += 10;

            /* Aqui escribe en el archivo */
            fprintf(f, "iteracion %d: %d\n", i, variable);

            /* aqui se asegura que se guarda */
            fflush(f);

            printf("Hijo registra: %d\n", variable);

            sleep(1);
        }

        fclose(f);
        exit(0);

    } else {

        /* Padre */
        for (int i = 1; i <= 10; i++) {

            variable += 10;

            printf("Padre incrementa: %d\n", variable);

            sleep(1);
        }

        /* El padre espera a que el hijo termine */
        wait(NULL);
    }

    return 0;
}