/*
LIBRERIA: Cabecera de FUNCIONES PARA PLANIFICADOR ROUND ROBIN
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2025
VERSIÓN: 1.0 Diciembre 2025

DESCRIPCIÓN: Proceso de un Planificador Round.
Algoritmo de planificación que hace uso de una cola de manera 
que todos los procesos que quieren se ejecutados entran a la cola,
y el algoritmo realiza un quantum para ejecutar en el procesador 
una cantidad especifica de tiempo y despues volver a encolar en la cola.

OBSERVACIONES:  
 
*/
//LLAMADA DE LIBRERIAS
#include "common/planificador_common.h"

//DEFINICIÓN DE FUNCIONES PARA PLANIFICADOR

//Inicializar memoria compartida: Crear espacios de memoria 
//que seran usados por el planificador y procesos
void *inicializar_memoria_compartida(const char *path, int id, size_t size);      

//Inicializar semaforos: Inicializar semaforos dentro de la memoria
//compartida para evitar condiciones de carrera y administrar procesos
sem_t *inicializar_semaforo(const char *path, int init);

//Inicializar cola de proceso: Inicializa la cola dinamica que usara el planificador
//para ordenar la ejecución de los procesos
void inicializar_cola_procesos(cola_dinamica *cola_procesos);

//Desencolar registro(): Desencola un registro que se encuentra la cola estatica de
//registros. 
Registro desencolar_registro(ColaRegistros *cola_registro);

//Encolar proceso(): Encola la estructura de un proceso 
void encolar_proceso(pid_t pid, ColaProcesos *cola_procesos, EstadoProceso estado_proceso);

//Desencolar proceso(): Obtiene el proceso siguiente que se debe ejecutar
//despues lo retorna a la cola tran ejecutar el quantum.
Proceso *desencolar_proceso(ColaProcesos *cola_procesos); 

//Buscar pid(): Busca un PID dentro de la cola de procesos, retorna la posición
//donde se encuentra el Proceso que tiene el mismo PID
int buscar_pid(ColaProcesos *cola_procesos, int pid);

//Cambiar estado de pid(): Va a la posición indicada en la cola y obtiene el Proceso en
//esa posición, despues cambia el estado del proceso.
void cambiar_estado_pid(ColaProcesos *cola_procesos, int posicion, EstadoProceso estado_nuevo);

//Ejecutar proceso(): Realiza un quantum sobre el proceso actual a ejecutar
//enviando una señal para indicar ejecución al proceso.
void ejecutar_proceso(pid_t pid);           

//Detener proceso(): Envia una señal a un proceso para indicar que termino
//su tiempo de ejecución, iniciando el proceso de seleccionar el siguiente proceso
void detener_proceso(pid_t pid);

//Iniciar planificador(): Inicializa los valores, estructuras, memoria compartida y
//semaforos para el planificador
SHM_Planificador *iniciar_planificador(pid_t pid_planificador);

//Limpiar(): Función que borra, limpia y termina todas la estructuras usadas por el
//planificador.
void limpiar_planificador(ColaProcesos *cola_procesos, SHM_Planificador *planificador);