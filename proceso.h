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
#include "planificador_common.h"

//DEFINICIÓN DE FUNCIONES PARA PROCESO

//Unirse a SHM(): Función para que un proceso se una a la memoria compartida para
//comunicarse con el planificador.
void *unirse_memoria_compartida(const char *path, int id, size_t size);

//Unirse a semaforo(): Permite que el proceso se pueda unir al semaforo con nombre para
//manejar el acceso a las estructuras compartidas
sem_t *acceder_semaforo(const char *path);

//Inicializar proceso(): Inicializa todas las variables y estructuras que usara el proceso
//para su comunicación con el planificador
SHM_Planificador *inicializa_proceso();

//Encolar registro(): Función que encola un registro en la cola de registros para que
//el planificador atienda el registro de acuerdo a la solicitud: REGISTRAR o ELIMINAR.
void encolar_registro(ColaRegistros *cola_registro, int pid, int solicitud);

//Registrar proceso(): Función para enviar el PID del proceso al planificador e indicar
//que hay proceso por registrar.
void registrar_proceso(ColaRegistros *cola_registro, int pid);

//Terminar proceso(): Función para enviar el pid del proceso al planificador, luego de haber
//terminado y que el planificador deje de considerar el PID del proceso para la planificación.
void terminar_proceso(ColaRegistros *cola_registro, int pid);

//Realizar proceso(): Función que empieza 
void realizar_proceso(volatile sig_atomic_t *signal_planificador, volatile sig_atomic_t *signal_proceso);

//Detener ejecución(): Función que modifica los semaforos y detiene la ejecución de un
//proceso al recibir la señal para detenerse sin haber terminado.
void detener_ejecucion(volatile sig_atomic_t *signal_planificador, volatile sig_atomic_t *signal_proceso);
