#include "planificador.h"

//DECLARACIÓN DE VARIABLES GLOBALES
volatile sig_atomic_t signal_proceso = 0;
volatile sig_atomic_t signal_terminar = 0;
SHM_Planificador *shm_planificador;
cola cola_planificador;

//DECLARACIÓN DE FUNCIONES PARA ROUND ROBIN
void configurar_manejadores();
void bucle_planificador();
void terminar_planificador();
void imprimir_cola_procesos(cola *cola_procesos);

//DECLARACIÓN DE FUNCIONES PARA MANEJAR SEÑALES
void signal_registrar_proceso(int signal){
    signal_proceso = 1;
}

void signal_terminar_planificador(int signal){
    signal_terminar = 1;
}

//FUNCIÓN PRINCIPAL DE PLANIFICADOR
int main(int argc, char *argv[]){
    signal(REGISTRAR_PROCESO, signal_registrar_proceso);
    signal(SIGTERM, signal_terminar_planificador);
    
    unsigned int quantum; 
    pid_t pid_planificador;
    sem_t *semaforo_shm;
    sem_t *semaforo_procesos;
    
    if(argc > 2){
        perror("ERROR: Debe ingresar ./round_robin segundos_quantum");
        exit(1);
    }

    quantum = (unsigned int) atoi(argv[1]);
    pid_planificador = getpid();

    if(shm_planificador == NULL){
        perror("ERROR: Asignación de memoria fallidad con calloc()");
        exit(1);
    }

    configurar_manejadores(shm_planificador, pid_planificador, &cola_planificador, &semaforo_shm, &semaforo_procesos);
    bucle_planificador(quantum, semaforo_shm, shm_planificador, &cola_planificador);
    terminar_planificador();
}

void configurar_manejadores(SHM_Planificador *planificador, pid_t pid_planificador, cola *cola_procesos, sem_t **semaforo_memoria, sem_t **semaforo_procesos){
    planificador = iniciar_planificador(pid_planificador);
    inicializar_cola_procesos(cola_procesos);
    semaforo_memoria = inicializar_semaforos(SEM_SHM_PATH, INITIAL_SEM_VALUE);
    semaforo_procesos = inicializar_semaforos(SEM_PROCESS_PATH, INITIAL_SEM_VALUE);
}

void bucle_planificador(unsigned int quantum, sem_t *semaforo_SHM, SHM_Planificador *planificador, cola *cola_procesos){
    pid_t pid_proceso_actual;
    pid_t pid_proceso_registrar;
    Proceso *proceso;

    while(1){
        if(signal_proceso == 1){
            signal_proceso = 0;
            sem_wait(semaforo_SHM);
            pid_proceso_registrar = planificador->pid_proceso;
            encolar_proceso(pid_proceso_registrar, cola_procesos);
            printf("Proceso encolado: [%d]\n", (int) pid_proceso_registrar);
            planificador->procesos_registrados += 1;
            sem_post(semaforo_SHM);
        }

        if(signal_terminar == 1){
            terminar_planificador();
        }

        sem_wait(semaforo_SHM);
        if(planificador->procesos_registrados > 0){
            proceso = desencolar_proceso(cola_procesos);
            pid_proceso_actual = (pid_t) proceso->pid;
            
            planificador->estado_proceso = EJECUTANDO;
            
            printf("Proceso a ejecutar: [%d]", (int) pid_proceso_actual);
            imprimir_cola_procesos(cola_procesos);
            sem_post(semaforo_SHM);

            kill(pid_proceso_actual, CONTINUAR_PROCESO);

            sleep(quantum);

            kill(pid_proceso_actual, DETENER_PROCESO);

            sem_wait(semaforo_SHM);
            if(planificador->estado_proceso == TERMINADO){
                printf("Proceso terminado: [%d]", (int) pid_proceso_actual);
            }
            else if(planificador->estado_proceso == PAUSADO){
                encolar_proceso(pid_proceso_actual, cola_procesos);
            }
            sem_post(semaforo_SHM);
        }
        else{
            sem_post(semaforo_SHM);
        }
    }
}

void terminar_planificador(){
    limpiar_planificador(&cola_planificador, shm_planificador);
}

void imprimir_cola_procesos(cola *cola_procesos){
    int i;
    int pid_proceso;
    Proceso *auxiliar;

    printf("Proceso en cola: \n");
    printf("Inicio -> ");
    for(i = 1; i <= Size(cola_procesos); i++){
        auxiliar = (Proceso *) Element(cola_procesos, i);
        pid_proceso = auxiliar->pid;
        printf("%d \t", pid_proceso);
    }
    printf("<- Final");
}