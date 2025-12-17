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

//DEFINICIÓN DE ESTRUCTURAS
typedef struct ContextoPlanificador{
        SHM_Planificador *shm;
        sem_t *semaforo_shm;
        ColaProcesos cola_procesos;
        Proceso *proceso_actual;
        int procesos_registrados;
        int quantum;
        volatile sig_atomic_t signal_registros;
        volatile sig_atomic_t signal_terminar;
}ContextoPlanificador;

//DEFINICIÓN DE FUNCIONES PARA PLANIFICADOR

void *inicializar_memoria_compartida(const char *path, int id, size_t size);      //Inicializar memoria compartida: Crear espacios de memoria que seran usados por el planificador y procesos

sem_t *inicializar_semaforo(const char *path, int init);        //Inicializar semaforos: Inicializar semaforos dentro de la memoria compartida para evitar condiciones de carrera y administrar procesos

void inicializar_cola_procesos(cola_dinamica *cola_procesos);   //Inicializar cola de proceso: Inicializa la cola dinamica que usara el planificador para ordenar la ejecución de los procesos

int desencolar_registro(ColaRegistros *cola_registro, Registro *registro_obtenido);     //Desencolar registro(): Desencola un registro que se encuentra la cola estatica de registros. 

void encolar_proceso(pid_t pid, ColaProcesos *cola_procesos, EstadoProceso estado_proceso);     //Encolar proceso(): Encola la estructura de un proceso 

Proceso *desencolar_proceso(ColaProcesos *cola_procesos);       //Desencolar proceso(): Obtiene el proceso siguiente que se debe ejecutar despues lo retorna a la cola tran ejecutar el quantum.

int buscar_pid(ColaProcesos *cola_procesos, int pid);   //Buscar pid(): Busca un PID dentro de la cola de procesos, retorna la posición donde se encuentra el Proceso que tiene el mismo PID

void cambiar_estado_pid(ColaProcesos *cola_procesos, int posicion, EstadoProceso estado_nuevo); //Cambiar estado de pid(): Va a la posición indicada en la cola y obtiene el Proceso en esa posición, despues cambia el estado del proceso.

void ejecutar_proceso(pid_t pid);       //Ejecutar proceso(): Realiza un quantum sobre el proceso actual a ejecutar enviando una señal para indicar ejecución al proceso.

void pausar_proceso(pid_t pid); //Detener proceso(): Envia una señal a un proceso para indicar que termino su tiempo de ejecución, iniciando el proceso de seleccionar el siguiente proceso

int iniciar_planificador(ContextoPlanificador *planificador, pid_t pid_planificador, int quantum);      //Iniciar planificador(): Inicializa los valores, estructuras, memoria compartida y semaforos para el planificador

Proceso *obtener_siguiente_proceso(ContextoPlanificador *planificador); //Obtener siguiente proceso(): Maneja la logica para obtener el siguiente proceso de acuerdo al estado del Proceso 

void tratar_registro(ContextoPlanificador *planificador);      //Tratar registro(): Atiende la cola de registros y realiza las solicitudes de los registros obtenidos.

int planificador_proceso_disponible(ContextoPlanificador *planificador);       //Procesos disponible(): Función que revisa si hay procesos disponibles o por atender en la cola de procesos.

int planificador_ejecuta_proceso(ContextoPlanificador *planificador);  //Ejecutar proceso(): Planificador obtiene el siguiente proceso a ejecutar.

int planificador_pausa_proceso(ContextoPlanificador *planificador);    //Planificador pausa proceso(): Planificador pausa un proceso que acaba de terminar de ejecutar.

void limpiar_planificador(ContextoPlanificador *planificador); //Limpiar(): Función que borra, limpia y termina todas la estructuras usadas por el planificador.

void imprimir_cola_procesos(ContextoPlanificador *planificador);        //Imprimir cola(): Se imprime el estado actual de la cola de procesos