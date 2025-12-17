#include <stdio.h>
#include <unistd.h>
#include "round_robin/proceso.h"
ContextoProceso proceso;

int main(){
    pid_t pid_proceso;
    pid_proceso = getpid();
    inicializar_proceso(&proceso, pid_proceso);

    int i = 0;
    while(1){
        printf("PID %d -> %d\n", getpid(), i++);
        fflush(stdout);
        sleep(3);
    }

    terminar_proceso(proceso);
}