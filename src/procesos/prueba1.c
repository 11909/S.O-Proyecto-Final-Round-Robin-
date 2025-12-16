#include "round_robin/proceso.h"

volatile sig_atomic_t signal_pausado = 0;
volatile sig_atomic_t signal_ejecutar = 0;
volatile sig_atomic_t signal_matar = 0;

ContextoProceso proceso;

void handle_pausa(int sig){
    signal_pausado = 1;
    signal_ejecutar = 0;
}

void handle_ejecucion(int sig){
    signal_ejecutar = 1;
}

void handle_muerte(int sig){
    signal_matar = 1;
}


int main(){
    signal(SIGNAL_PAUSAR_PROCESO, handle_pausa);
    signal(SIGNAL_CONTINUAR_PROCESO, handle_ejecucion);
    signal(SIGNAL_MATAR_PROCESO, handle_muerte);

    pid_t pid_proceso;

    pid_proceso = getpid();
    inicializar_proceso(&proceso, pid_proceso);

    while(1){
        if(signal_pausado){
            pause();
        }

        if(signal_ejecutar){
            funcion();
        }
    }

}

void funcion(){

}