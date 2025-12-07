#include "planificador.h"

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
   
}

Proceso desencolar_proceso(){
    
}

void ejecutar_proceso(pid_t pid){

}

void detener_proceso(pid_t pid){
    
}

int iniciar_planificador(){

}

void limpiar(){

}

//DEFINICIÓN DE FUNCIONES PARA PROCESO
int unirse_memoria_compartida(){

}

int registrar_proceso(){

}

int realizar_proceso(){

}

int detener_ejecucion(){

}