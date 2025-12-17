#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//DECLARAR FUNCIONES
void crear_hijo_millenial(const char* nombre);
void crear_hijo_z(const char* nombre);
void crear_hijo_alfa(const char* nombre);


int main(){
	//PROCESO DE GENERACION X
	pid_t pid = getpid();
	
	printf("\nSoy el proceso de generacion X\n");
	printf("Mi PID es: %d\n", pid);
	printf("No tengo proceso padre\n");
	
	//CREAR PROCESOS HIJOS
	crear_hijo_millenial("MILLENIAL");
	crear_hijo_millenial("MILLENIAL");
	
	// Esperar a todos los procesos hijos (Millenials)
    	while (wait(NULL) > 0); // Recolecta todos los hijos que hayan terminado

    	printf("\n[Main] Todos los procesos han terminado.\n");
    	return 0;
}

void crear_hijo_millenial(const char* nombre){
	pid_t pid = fork();
	
	//ERROR EN PROCESO
	if(pid < 0){
		perror("fork: void crear_hijo_millenial (nombre)");
		exit(1);
	}
	//PROCESO HIJO
	else if(pid == 0){
		pid_t pid_hijo = getpid();
		pid_t pid_padre = getppid();
		
		printf("\nSoy un proceso de generacion Millenial\n");
		printf("Mi PID es: %d\n", pid_hijo);
		printf("El PID de mi padre es: %d\n", pid_padre);
		
		//CREAR PROCESOS HIJOS
		crear_hijo_z("Z");
		crear_hijo_z("Z");
		crear_hijo_z("Z");
		
		//TERMINAR PROCESO
		_exit(42);
	}
	//PROCESO PADRE 
	//pid=pid de hijo
	else{
		
	}
}

void crear_hijo_z(const char* nombre){
	pid_t pid = fork();
	
	//ERROR EN PROCESO
	if(pid < 0){
		perror("fork: void crear_hijo_z (nombre)");
		exit(1);
	}
	//PROCESO HIJO
	else if(pid == 0){
		pid_t pid_hijo = getpid();
		pid_t pid_padre = getppid();
		
		printf("\nSoy un proceso de generacion Z\n");
		printf("Mi PID es: %d\n", pid_hijo);
		printf("El PID de mi padre es: %d\n", pid_padre);
		
		//CREAR PROCESOS HIJOS
		crear_hijo_alfa("ALFA");
		crear_hijo_alfa("ALFA");
		
		//TERMINAR PROCESO
		_exit(42);
	}
	//PROCESO PADRE 
	//pid=pid de hijo
	else{
		
	}
}

void crear_hijo_alfa(const char* nombre){
	pid_t pid = fork();
	
	//ERROR EN PROCESO
	if(pid < 0){
		perror("fork: void crear_hijo_alfa (nombre)");
		exit(1);
	}
	//PROCESO HIJO
	else if(pid == 0){
		pid_t pid_hijo = getpid();
		pid_t pid_padre = getppid();
		
		printf("\nSoy un proceso de generacion Alfa\n");
		printf("Mi PID es: %d\n", pid_hijo);
		printf("El PID de mi padre es: %d\n", pid_padre);
		
		//ULTIMO PROCESO DE ARBOL
		sleep(5);
		
		//TERMINAR PROCESO
		_exit(42);
	}
	//PROCESO PADRE 
	//pid=pid de hijo
	else{
		
	}
}
