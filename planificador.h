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
#include <sys/types.h>
#include <unistd.h>

//DECLARACIÓN DE CONSTANTES
#define SHM_PATH "/tmp"

//DECLARACIÓN DE ESTRUCUTRA PARA UN PROCESO
//Se debe considerar que se usara una cola con apuntadores genericos,
//entonces se debe usar casting para la interacción entre planificador y estructura
typedef struct Proceso{
    int planificador_pid;
}Proceso;

//DECLARACIÓN DE ESTRUCTURA PARA LA MEMORIA COMPARTIDAS
typedef struct SHM_Planificador{
    int planificador_pid;
    int pid_proceso;
    //Semaforos
}SHM_Planificador;

//DEFINICIÓN DE FUNCIONES PARA PLANIFICADOR
int inicializar_memoria_compartida();   //Inicializar memoria compartida: Crear espacios de memoria 
                                        //que seran usados por el planificador y procesos
int inicializar_semaforos();            //Inicializar semaforos: Inicializar semaforos dentro de la memoria
                                        //compartida para evitar condiciones de carrera y administrar procesos
void encolar_proceso(pid_t pid);        //Encolar proceso(): Encola la estructura de un proceso 
Proceso desencolar_proceso();
void ejecutar_proceso(pid_t pid);
void detener_proceso(pid_t pid);
int iniciar_planificador();
void limpiar();

//DEFINICIÓN DE FUNCIONES PARA PROCESO
int unirse_memoria_compartida();
int registrar_proceso();
int realizar_proceso();
