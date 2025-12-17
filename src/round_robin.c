#define _POSIX_C_SOURCE 200809L

#include "round_robin/planificador.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//DECLARACIÓN DE VARIABLES GLOBALES
ContextoPlanificador planificador;

//DECLARACIÓN DE FUNCIONES PARA MANEJAR SEÑALES
void handler_registros(int signum) {
    (void)signum;
    planificador.signal_registros = 1;
}

void handler_terminar(int signum) {
    (void)signum;
    planificador.signal_terminar = 1;
}

//FUNCIÓN PRINCIPAL DE PLANIFICADOR
int main(int argc, char *argv[]){    
    struct sigaction sa_registros;
    struct sigaction sa_terminar;

    // ---------- Señal de registros ----------
    memset(&sa_registros, 0, sizeof(sa_registros));
    sa_registros.sa_handler = handler_registros;
    sigemptyset(&sa_registros.sa_mask);

    if (sigaction(SIGNAL_ENVIO_REGISTRO, &sa_registros, NULL) == -1) {
        perror("sigaction SIGNAL_ENVIO_REGISTRO");
        exit(1);
    }

    // ---------- Señal de terminación ----------
    memset(&sa_terminar, 0, sizeof(sa_terminar));
    sa_terminar.sa_handler = handler_terminar;
    sigemptyset(&sa_terminar.sa_mask);

    if (sigaction(SIGTERM, &sa_terminar, NULL) == -1 ||
        sigaction(SIGINT,  &sa_terminar, NULL) == -1) {
        perror("sigaction SIGTERM/SIGINT");
        exit(1);
    }


    int quantum; 
    pid_t pid_planificador;
    int bool_planificador = 0;
    
    if(argc != 2){
        fprintf(stderr, "Uso: %s <quantum>\n", argv[0]);
        exit(1);
    }

    quantum = (unsigned int) atoi(argv[1]);
    if(quantum <= 0){
        fprintf(stderr, "Quantum invalido\n");
        exit(1);
    }

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
            exit(0);
        }

        if(planificador_proceso_disponible(&planificador)){
            if(planificador_ejecuta_proceso(&planificador) == 1){
                sleep(planificador.quantum);
                planificador_pausa_proceso(&planificador);
            }
        }
        else{
            pause();
        }
    }
}