/*
LIBRERIA: Cabecera de PLANIFICADOR ROUND ROBIN
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2024
VERSIÓN: 1.0 Noviembre 2024

DESCRIPCIÓN: Planificador Round Robin.
Algoritmo de planificación que hace uso de una cola
de manera que todos los procesos que quieren se ejecutados
entran a la cola, y el algoritmo realiza un quantum para
ejecutar en el procesador una cantidad especifica de tiempo
y despues devolver a la cola.

OBSERVACIONES:  
 
*/
//LLAMADA DE LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include "queue.h"

//DECLARACIÓN DE CONSTANTES
#define SHM_PATH "/tmp"
#define SHM_KEY 'A'
#define SEM_SHM_PATH "/tmp/sem_shm"
#define INITIAL_SEM_VALUE 1

//DECLARACIÓN DE ESTRUCUTRA PARA UN PROCESO
//Se debe considerar que se usara una cola con apuntadores genericos,
//entonces se debe usar casting para la interacción entre planificador y estructura
typedef struct Proceso{
    int pid;
}Proceso;

//DECLARACIÓN DE ESTRUCTURA PARA LA MEMORIA COMPARTIDAS
typedef struct SHM_Planificador{
    int planificador_pid;
    int pid_proceso;
}SHM_Planificador;

//DECLARACIÓN DE VARIABLES GLOBALES
SHM_Planificador *shm_planificador;
sem_t *semaforo_shm;

//DEFINICIÓN DE FUNCIONES PARA PLANIFICADOR

//Inicializar memoria compartida: Crear espacios de memoria 
//que seran usados por el planificador y procesos
void inicializar_memoria_compartida();      

//Inicializar semaforos: Inicializar semaforos dentro de la memoria
//compartida para evitar condiciones de carrera y administrar procesos
void inicializar_semaforos();

//Encolar proceso(): Encola la estructura de un proceso 
void encolar_proceso(pid_t pid, cola *cola_procesos);        

//Desencolar proceso(): Obtiene el proceso siguiente que se debe ejecutar
//despues lo retorna a la cola tran ejecutar el quantum.
Proceso *desencolar_proceso(cola *cola_procesos);               

//Ejecutar proceso(): Realiza un quantum sobre el proceso actual a ejecutar
//enviando una señal para indicar ejecución al proceso.
void ejecutar_proceso(pid_t pid);           

//Detener proceso(): Envia una señal a un proceso para indicar que termino
//su tiempo de ejecución, iniciando el proceso de seleccionar el siguiente proceso
void detener_proceso(pid_t pid);

//Iniciar planificador(): Inicializa los valores, estructuras, memoria compartida y
//semaforos para el planificador
void iniciar_planificador();

//Limpiar(): Función que borra, limpia y termina todas la estructuras usadas por el
//planificador.
void limpiar_planificador(cola *cola_procesos);

//DEFINICIÓN DE FUNCIONES PARA PROCESO
//Unirse a SHM(): Función para que un proceso se una a la memoria compartida para
//comunicarse con el planificador.
int unirse_memoria_compartida();        

//Registrar proceso(): Función para enviar el PID del proceso al planificador y sea
//considerando para la planificación con la Queue.
int registrar_proceso();                

//Realizar proceso(): Función que modifica al semaforo y permite al proceso empezar
//su ejecución.
int realizar_proceso();                 

//Detener ejecución(): Función que modifica los semaforos y detiene la ejecución de un
//proceso al recibir la señal para detenerse.
int detener_ejecucion();                
