/*
LIBRERIA: Cabecera de FUNCIONES PARA PROCESOS
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2025
VERSIÓN: 1.0 Diciembre 2025

DESCRIPCIÓN: Proceso de un Planificador Round Robin.
Todo proceso que forme parte de un planificador Round Robin debe
de: Ser capaz de comunicarse con el planificador mediante el uso de 
una memoria compartida, y señales. El proceso debe de responder a las
señales recibidas por el planificador para pausarse, ejecutarse o eliminarse.

OBSERVACIONES:  
 
*/
//LLAMADA DE LIBRERIAS
#include "common/planificador_common.h"

//DEFINICIÓN DE ESTRUCTURAS
typedef struct ContextoProceso{
        SHM_Planificador *shm;
        sem_t *semaforo_shm;
        pid_t pid_proceso;
}ContextoProceso;

//DEFINICIÓN DE FUNCIONES PARA PROCESO

//Unirse a SHM(): Función para que un proceso se una a la memoria compartida para
//comunicarse con el planificador.
void *unirse_memoria_compartida(const char *path, int id, size_t size);

//Unirse a semaforo(): Permite que el proceso se pueda unir al semaforo con nombre para
//manejar el acceso a las estructuras compartidas
sem_t *acceder_semaforo(const char *path);

//Inicializar proceso(): Inicializa todas las variables y estructuras que usara el proceso
//para su comunicación con el planificador
int inicializar_proceso(ContextoProceso *contexto, pid_t pid);

//Encolar registro(): Función que encola un registro en la cola de registros para que
//el planificador atienda el registro de acuerdo a la solicitud: REGISTRAR o ELIMINAR.
void encolar_registro(ContextoProceso proceso, int solicitud);

//Registrar proceso(): Función para enviar el PID del proceso al planificador e indicar
//que hay proceso por registrar.
void registrar_proceso(ContextoProceso proceso);

//Terminar proceso(): Función para enviar el pid del proceso al planificador, luego de haber
//terminado y que el planificador deje de considerar el PID del proceso para la planificación.
void terminar_proceso(ContextoProceso proceso);
