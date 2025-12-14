/*
LIBRERIA: IMPLEMENTACIÓN DE LIBRERIA PARA PLANIFICADOR ROUND ROBIN
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
#include "planificador.h"

/*
void *inicializar_memoria_compartida(const char *path, int id, size_t size)
Descripción: Inicializa una memoria compartida y retonar la dirección donde se encuentra.
Recibe: const char *path (Ruta al archivo base para generar la clave),
        int id (Clave del proyecto para crear llave),
        size_t size (Tamaño de la estructura para el segmento a crear)
Devuelve: void * (Puntero generico al inicio del segmento de memoria compartida)
Observaciones:  La función debe hacer uso de las constantes definidos en la cabecera del planificador
                en "planificador.h" para que se tenga la misma llave para acceder a la memoria compartida
*/
void *inicializar_memoria_compartida(const char *path, int id, size_t size){
    key_t clave;
    int shm_id;
    void *shm_ptr;

    key_t clave;
    int shm_id;
    void *shm_ptr = NULL;

    clave = ftok(path, id);
    if(clave == (key_t) -1){
        perror("ERROR: ftok() en unirse_memoria_compartida()");
        return NULL;
    }

    shm_id = shmget(clave, size, IPC_CREAT|0666);
    if(shm_id == -1){
        perror("ERROR: shmget() en inicializar_memoria_compartida()");
        return NULL;
    }

    shm_ptr = shmat(shm_id, NULL, 0);
    if(shm_id == (void *) -1){
        perror("ERROR: shmat() en inicializar_memoria_compartida()");
        return NULL;
    }

    return shm_ptr;
}

/*
void inicializar_semaforos()
Descripción: Recibe la ruta hacía un semaforo con nombre y lo inicializa
Recibe: const char *path (Ruta al archivo del semaforo con nombre),
        int init (Valor con el que se inicializa el semaforo)
Devuelve:   sem_t *(Puntero al semaforo inicializado)
Observaciones:  El semaforo debe usar la ruta definida de forma global en 
*/
sem_t *inicializar_semaforo(const char *path, int init){
    sem_t *semaforo;
    semaforo = sem_open(path, O_CREAT, 0644, init);

    if(semaforo == SEM_FAILED){
        perror("ERROR: sem_open() en inicializar_semaforos()");
        return NULL;
    }

    return semaforo;
}

/*
void inicializar_cola_procesos(ColaProcesos *cola_procesos)
Descripción: Reciba una cola de procesos y la inicializa como una cola dinamica
Recibe: colaProcesos *cola_procesos (Dirección/Referencia de la cola dinamica)
Devuelve:
Observaciones:  
*/
void inicializar_cola_procesos(ColaProcesos *cola_procesos){
    Dyn_Initialize(cola_procesos);
}

/*
void inicializar_cola_registros(ColaRegistros *cola_registro)
Descripción: Reciba una cola de registros y la inicializa como una cola estatica circular
Recibe: colaRegistro *cola_registro (Dirección/Referencia de la cola estatica)
Devuelve:
Observaciones:  
*/
void inicializar_cola_registros(ColaRegistros *cola_registro){
    Est_Initialize(cola_registro);
}

//Desencolar registro(): Desencola un registro que se encuentra la cola estatica de
//registros. 
Registro desencolar_registro(ColaRegistros *cola_registro){

}

/*
void encolar_proceso(pid_t pid, cola_dinamica *cola_procesos)
Descripción: Recibe un pid y el estado acutal del proceso, para despues crear
             un elemento de Proceso y encolarlo en la cola de procesos.
Recibe: pid_t pid (PID del proceso a encolar en la cola de procesos),
        ColaProcesos *cola_procesos (Dirección/Referencia de la cola de procesos para encolar),
        EstadoProceso estado_proceso (Valor enteror para representar el estado con el que el proceso se encolara)
Devuelve:
Observaciones:  
*/
void encolar_proceso(pid_t pid, ColaProcesos *cola_procesos, EstadoProceso estado_proceso){
    Proceso *proceso = (Proceso *) calloc(1, sizeof(Proceso));

    if(proceso == NULL){
        perror("ERROR: calloc() en encolar_proceso()");
        return NULL;
    }

    proceso->pid = (int) pid;

    Dyn_Queue(cola_procesos, (ElementoColaProcesos)proceso);
}

/*
Proceso *desencolar_proceso(cola *cola_procesos)
Descripción: Recibe una cola de proesos, comprueba que tenga elementos que
             pueda desencolar. Retorna el elemento desencolado como un Proceso
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola dinamica a desencolar)
Devuelve: Proceso * (Dirección/Referencia de Proceso desencolado)
Observaciones: Dado que se retorna la dirección/referencia del proceso desencolado, se puede
               hacer modificaciones directas.
*/
Proceso *desencolar_proceso(ColaProcesos *cola_procesos){
    ElementoColaProcesos e;
    Proceso *p;

    if(Dyn_Empty(cola_procesos)){
        printf("ERROR: La cola esta vacia. No hay procesos por sacar\n");
        return;
    }
    
    e = Dyn_Dequeue(cola_procesos);

    p = (Proceso *) e;

    return p;
}

//Buscar pid(): Busca un PID dentro de la cola de procesos, retorna la posición
//donde se encuentra el Proceso que tiene el mismo PID
int buscar_pid(ColaProcesos *cola_procesos, int pid){
    
}

//Cambiar estado de pid(): Va a la posición indicada en la cola y obtiene el Proceso en
//esa posición, despues cambia el estado del proceso.
void cambiar_estado_pid(ColaProcesos *cola_procesos, int posicion, EstadoProceso estado_nuevo){

}

/*
void ejecutar_proceso(pid_t pid)
Descripción: Envia una señal al proceso con ese PID para indicar que puede ejecutarse
Recibe: pid_t pid (PID del proceso que puede ejecutarse en ese momento)
Devuelve:
Observaciones:
*/
void ejecutar_proceso(pid_t pid){
    kill(pid, CONTINUAR_PROCESO);
}

/*
void detener_proceso(pid_t pid)
Descripción: Envia una señal al proceso con ese PID para indicar que debe pausar su ejecución
Recibe: pid_t pid (PID del proceso que debe pausar en ese momento)
Devuelve:
Observaciones:  
*/
void detener_proceso(pid_t pid){
    kill(pid, PAUSAR_PROCESO);
}

/*
SHM_Planificador *iniciar_planificador(pid_t pid_planificador)
Descripción: Inicializa a la estructura del planificado en la memoria compartida.
Recibe: pid_t pid (PID del planificador)
Devuelve: SHM_Planificador * (Dirección/Referencia del segmento donde se enceuntra la memoria compartidad con la estructura)
Observaciones:  
*/
SHM_Planificador *iniciar_planificador(pid_t pid_planificador){
    SHM_Planificador *planificador;

    planificador = (SHM_Planificador *) inicializar_memoria_compartida(SHM_PATH, SHM_KEY, sizeof(SHM_Planificador));
    
    planificador->procesos_registrados = 0;
    planificador->planificador_pid = (int) pid_planificador;
    
    inicializar_cola_registros(&planificador->cola_registros);

    return planificador;
}

/*
void limpiar_planificador(ColaProcesos *cola_procesos, SHM_Planificador *planificador)
Descripción: Separa, elimina y limpia las estructuras que usa el planificador.
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola de procesos con procesos registrados)
        SHM_Planificador *planificador (Dirección/Referencia de segmento con memoria compartida)
Devuelve:
Observaciones:  
*/
void limpiar_planificador(ColaProcesos *cola_procesos, SHM_Planificador *planificador){
    Proceso *proceso;
    pid_t pid_proceso;
    while(!Empty(cola_procesos)){
        proceso = desencolar_proceso(cola_procesos);
        pid_proceso = (pid_t) proceso->pid;
        kill(pid_proceso, MATAR_PROCESO);
    }

    Dyn_Destroy(cola_procesos);

    Est_Destroy(&planificador->cola_registros);

    if(sem_unlink(SEM_SHM_PATH) == -1){
        perror("ERROR: sem_unlink() en limpiar_planificador()");
        return NULL;
    }

    if(shmdt(planificador) == -1){
        perror("ERROR: shmdt en limpiar_planificador()");
        return NULL;
    }

    // if(shmctl(shm_id, IPC_RMID, NULL) == -1){
    //     perror("ERROR: shmctl() en limpiar_planificador()");
    //     return NULL;
    // }
}

