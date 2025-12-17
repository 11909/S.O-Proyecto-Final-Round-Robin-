#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "round_robin/proceso.h"
ContextoProceso proceso_hijo;
ContextoProceso proceso_padre;


int main() {
    pid_t pid;

    pid = fork();  // Crear proceso hijo

    if (pid < 0) {
        // Error al crear el proceso
        perror("Error en fork");
        return 1;
    }
    else if (pid == 0) {
        // ===== PROCESO HIJO =====
        pid_t pid_proceso;
        pid_proceso = getpid();
        inicializar_proceso(&proceso_hijo, pid_proceso);

        printf("Proceso HIJO - PID: %d\n", getpid());

        for (int i = 1; i <= 50; i++) {
            printf("Hijo [%d] -> Contador: %d\n", getpid(), i);
            sleep(1);  // 1 segundo
        }

        terminar_proceso(proceso_hijo);
    }
    else {
        // ===== PROCESO PADRE =====
        pid_t pid_proceso;
        pid_proceso = getpid();
        inicializar_proceso(&proceso_padre, pid_proceso);

        printf("Proceso PADRE - PID: %d\n", getpid());

        for (int i = 1; i <= 30; i++) {
            printf("Padre [%d] -> Contador: %d\n", getpid(), i);
            sleep(2);  // 2 segundos
        }

        terminar_proceso(proceso_padre);
    }

    return 0;
}

