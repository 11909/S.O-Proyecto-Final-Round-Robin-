#include "round_robin/proceso.h"
//DEFINICIÓN DE FUNCIONES PARA PROCESO
void *unirse_memoria_compartida(const char *path, int id, size_t size){
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

sem_t *acceder_semaforo(const char *path){
    sem_t *semaforo;

    semaforo = sem_open(path, 0);

    if(semaforo == SEM_FAILED){
        perror("ERROR: sem_open() en acceder_semaforo");
        return NULL;
    }

    return semaforo;
}

void registrar_proceso(sem_t *semaforo_shm, sem_t *semaforo_proceso, pid_t pid_proceso, SHM_Planificador *planificador, volatile sig_atomic_t *signal_planificador){
    pid_t pid_planificador;

    sem_wait(semaforo_proceso);
    sem_wait(semaforo_shm);
    pid_planificador = planificador->planificador_pid;
    planificador->pid_proceso = pid_proceso;
    sem_post(semaforo_shm);

    kill(pid_planificador, REGISTRAR_PROCESO);

    while(*signal_planificador == 0){
        usleep(1000);
    }
    *signal_planificador = 0;

    sem_post(semaforo_proceso);
}

void realizar_proceso(volatile sig_atomic_t *signal_planificador, volatile sig_atomic_t *signal_proceso){
    if(*signal_planificador == 1){
        *signal_proceso = 1;
    }
    *signal_planificador = 0;
}

void detener_ejecucion(volatile sig_atomic_t *signal_planificador, volatile sig_atomic_t *signal_proceso){
    if(*signal_planificador == 1){
        *signal_proceso = 0;
    }
    *signal_planificador = 0;
}