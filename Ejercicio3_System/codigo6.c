#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (){
//obtener el PID del proceso principal (padre) antes de aplicar fork
	pid_t pid_padre= getpid();
	printf ("----Iniciando ejecuciòn del sistema de procesos---\n");
	printf("PRINCIPAL ----PID DEL PROCESO ORIGINAL: %d\n\n",pid_padre);
// creamos un hijo manualmente mediante el uso de fork ()
	pid_t pid =fork ();

	if (pid <0){
	//aquì se realiza un manejo de errores para indicar mediante un PID
	//negativo que que el proceso HIJO no pudo crearse
   		perror ("Error al crear el PROCESO FORK (fork)");
   		exit (EXIT_FAILURE);

	}if (pid ==0){
	// aqui se encuentra el bloque del proceso hijo , se usa getpid() como el
	// identificador del proceso HIJO y getppid () como identificador del proceso padre
	printf ("--HIJO--IDENTIFICADOR (PID): %d\n", getpid());
	printf ("--HIJO--IDENTIFICADOR DE MI PADRE (PIDD): %d\n",getppid());
	printf ("--HIJO-- Mostrando estados de PROCESOS activos..........\n");

// metodo de verificaciòn de los procesos relevantes <<NOTESE USO DE SYSTEM>>>>>
// El comando "ps -f --forest" es un comando base que le dice al sistema operativo 
// que nos brinde los procesos que se encuentran ejecutandose en el momento de forma que podamos observar  
// aspectos que incluyen la hora, el usuario , los PPID en forma de una jerarquìa que es representado con ramas
//
 	system ("ps -f --forest");

// PROCESO CON PSTREE
	printf ("<<HIJO>>-----> Generando arbol visual con pstree \n");

//Aquì se crea un buffer de memoria para reservar espacio en RAM
	char arreglo[100] ;
//Se realiza la filtracion para ver los procesos desde el abuelo hasta el hijo
//La funciòn sprintf convierte todo en una sola unidad de texto que la funciòn pueda entender y ejecutar correctamente en la SHEll de Linux
	sprintf (arreglo , " pstree -p -A %d\n", getppid());
	system (arreglo);

 	printf ("--------------------------------------\n");
 	exit (0); //aqui termina el HIJO  de forma exitosa
 	 } else {

	//Bloque de procesamiento del PADRE
	printf ("--PADRE-- identificador (PID) %d\n",getpid());
	printf ("--PADRE-- ME HE ENCARGADO DE CREAR UN HIJO CON EL PID %d\n",pid);
	printf ("<<PADRE>> ----> esperando a que el hijo finalice \n\n");
	// importante para la prevenciòn de procesos ZOMBIES
	
	wait (NULL);

	printf ("\n <<PADRE>> El proceso hijo ha terminado . Finalizando el programa\n");
         }

return 0;

}

