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
void inicializar_memoria_compartida();
Descripción: Inicializa la memoria compartida para la comunicación del planificador con procesos
Recibe:
Devuelve:
Observaciones:  La función hace uso de las constantes definidos en la cabecera del planificador
                en "planificador.h"
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
void inicializar_semaforos();
Descripción: Inicializa al semaforo que servira para indicar el acceso a la memoria compartida
Recibe:
Devuelve:
Observaciones:  El semoforo esta declarado en la cabecera, entonces es de acceso global para planificador
                y procesos.
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

void inicializar_cola_procesos(cola *cola_procesos){
    Initialize(cola_procesos);
}

void encolar_proceso(pid_t pid, cola *cola_procesos){
    Proceso *proceso = (Proceso *) calloc(1, sizeof(Proceso));

    if(proceso == NULL){
        perror("ERROR: calloc() en encolar_proceso()");
        return NULL;
    }

    proceso->pid = (int) pid;

    Queue(cola_procesos, (elemento)proceso);
}

Proceso *desencolar_proceso(cola *cola_procesos){
    elemento e;
    Proceso *p;

    if(Empty(cola_procesos)){
        printf("ERROR: La cola esta vacia. No hay procesos por sacar\n");
        return;
    }
    
    e = Dequeue(cola_procesos);

    p = (Proceso *) e;

    return p;
}

void ejecutar_proceso(pid_t pid){
    kill(pid, CONTINUAR_PROCESO);
}

void detener_proceso(pid_t pid){
    kill(pid, DETENER_PROCESO);
}

SHM_Planificador *iniciar_planificador(pid_t pid_planificador){
    SHM_Planificador *planificador;

    planificador = (SHM_Planificador *) calloc(1, sizeof(SHM_Planificador *));

    planificador = (SHM_Planificador *) inicializar_memoria_compartida(SHM_PATH, SHM_KEY, sizeof(SHM_Planificador));

    planificador->procesos_registrados = 0;
    planificador->planificador_pid = (int) pid_planificador;
    planificador->estado_proceso = VACIO;

    return planificador;
}

void limpiar_planificador(cola *cola_procesos, SHM_Planificador *planificador){
    Proceso *proceso;
    pid_t pid_proceso;
    while(!Empty(cola_procesos)){
        proceso = desencolar_proceso(cola_procesos);
        pid_proceso = (pid_t) proceso->pid;
        kill(pid_proceso, MATAR_PROCESO);
    }

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

