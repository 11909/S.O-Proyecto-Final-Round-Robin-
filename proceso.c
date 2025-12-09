#include "planificador.h"
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

void registrar_proceso(sem_t *semaforo, pid_t pid_planificador, pid_t pid_proceso){
    
}

void realizar_proceso(){

}

void detener_ejecucion(){

}