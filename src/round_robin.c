#include "planificador.h"

//DECLARACIÓN DE VARIABLES GLOBALES
ContextoPlanificador planificador;

//DECLARACIÓN DE FUNCIONES PARA MANEJAR SEÑALES
void signal_atender_registros(int signal){
    planificador.signal_registros = 1;
}

void signal_terminar_planificador(int signal){
    planificador.signal_terminar = 1;
}

//FUNCIÓN PRINCIPAL DE PLANIFICADOR
int main(int argc, char *argv[]){
    signal(SIGNAL_ENVIO_REGISTRO, signal_atender_registros);
    signal(SIGTERM, signal_terminar_planificador);
    
    int quantum; 
    pid_t pid_planificador;
    int bool_planificador = 0;
    
    if(argc > 2){
        perror("ERROR: Debe ingresar ./round_robin segundos_quantum");
        exit(1);
    }

    quantum = (unsigned int) atoi(argv[1]);
    pid_planificador = getpid();

    bool_planificador = iniciar_planificador(&planificador, pid_planificador, quantum);
    if(bool_planificador == -1){
        printf("ERROR: Planificador no se inicializo correctamente\n");
        exit(1);
    }

    while(1){
        if(planificador.signal_registros == 1){
            tratar_registro(&planificador);
        }
        if(planificador.signal_terminar == 1){
            limpiar_planificador(&planificador);
        }

        procesar_cola_procesos(&planificador);
    }
}