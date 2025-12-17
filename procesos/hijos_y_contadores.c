#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

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
        printf("Proceso HIJO - PID: %d\n", getpid());

        for (int i = 1; i <= 50; i++) {
            printf("Hijo [%d] -> Contador: %d\n", getpid(), i);
            sleep(1);  // 1 segundo
        }
    }
    else {
        // ===== PROCESO PADRE =====
        printf("Proceso PADRE - PID: %d\n", getpid());

        for (int i = 1; i <= 30; i++) {
            printf("Padre [%d] -> Contador: %d\n", getpid(), i);
            sleep(2);  // 2 segundos
        }
    }

    return 0;
}

