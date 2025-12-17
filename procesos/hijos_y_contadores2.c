#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid1, pid2;

    // Crear primer hijo
    pid1 = fork();

    if (pid1 < 0) {
        perror("Error en fork 1");
        return 1;
    }

    if (pid1 == 0) {
        // ===== HIJO 1 =====
        printf("Hijo 1 - PID: %d | Padre: %d\n", getpid(), getppid());

        for (int i = 1; i <= 50; i++) {
            printf("Hijo 1 [%d] -> Contador: %d\n", getpid(), i);
            sleep(1);
        }
        return 0;
    }

    // Crear segundo hijo (solo el padre llega aquí)
    pid2 = fork();

    if (pid2 < 0) {
        perror("Error en fork 2");
        return 1;
    }

    if (pid2 == 0) {
        // ===== HIJO 2 =====
        printf("Hijo 2 - PID: %d | Padre: %d\n", getpid(), getppid());

        for (int i = 1; i <= 50; i++) {
            printf("Hijo 2 [%d] -> Contador: %d\n", getpid(), i);
            sleep(1);
        }
        return 0;
    }

    // ===== PROCESO PADRE =====
    printf("Padre - PID: %d\n", getpid());

    for (int i = 1; i <= 30; i++) {
        printf("Padre [%d] -> Contador: %d\n", getpid(), i);
        sleep(2);
    }

    return 0;
}

