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

int shm_planificador_id;

/*
void inicializar_memoria_compartida();
Descripción: Inicializa la memoria compartida para la comunicación del planificador con procesos
Recibe:
Devuelve:
Observaciones:  La función hace uso de las constantes definidos en la cabecera del planificador
                en "planificador.h"
*/
void inicializar_memoria_compartida(){
    key_t clave;
    int shm_id;
    void *shm_ptr;

    clave = ftok(SHM_PATH, SHM_KEY);
    if(clave == (key_t) -1){
        perror("ERROR: ftok() en inicializar_memoria_compartida()");
        return NULL;
    }

    shm_id = shmget(clave, sizeof(SHM_Planificador), IPC_CREAT|0666);
    if(shm_id == -1){
        perror("ERROR: shmget() en inicializar_memoria_compartida()");
        return NULL;
    }
    shm_planificador_id = shm_id;

    shm_ptr = shmat(shm_id, NULL, 0);
    if(shm_id == (void *) -1){
        perror("ERROR: shmat() en inicializar_memoria_compartida()");
        return NULL;
    }

    shm_planificador = (SHM_Planificador*) shm_ptr;
}

/*
void inicializar_semaforos();
Descripción: Inicializa al semaforo que servira para indicar el acceso a la memoria compartida
Recibe:
Devuelve:
Observaciones:  El semoforo esta declarado en la cabecera, entonces es de acceso global para planificador
                y procesos.
*/
void inicializar_semaforos(){
    semaforo_shm = sem_open(SEM_SHM_PATH, O_CREAT, 0644, INITIAL_SEM_VALUE);

    if(semaforo_shm == SEM_FAILED){
        perror("ERROR: sem_open() en inicializar_semaforos()");
        return NULL;
    }
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
    kill(pid, SIGCONT);
}

void detener_proceso(pid_t pid){
    kill(pid, SIGSTOP);
}

void iniciar_planificador(){
    inicializar_memoria_compartida();
    inicializar_semaforos();
}

void limpiar_planificador(cola *cola_procesos){
    Proceso *proceso;
    pid_t pid_proceso;
    while(!Empty(cola_procesos)){
        proceso = desencolar_proceso(cola_procesos);
        pid_proceso = proceso->pid;
        kill(pid_proceso, SIGTERM);
    }

    if(sem_unlink(SEM_SHM_PATH) == -1){
        perror("ERROR: sem_unlink() en limpiar_planificador()");
        return NULL;
    }

    if(shmdt(shm_planificador) == -1){
        perror("ERROR: shmdt en limpiar_planificador()");
        return NULL;
    }

    if(shmctl(shm_planificador_id, IPC_RMID, NULL) == -1){
        perror("ERROR: shmctl() en limpiar_planificador()");
        return NULL;
    }
}

