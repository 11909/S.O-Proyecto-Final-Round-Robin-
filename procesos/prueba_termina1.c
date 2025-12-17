#include "round_robin/proceso.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Para EXIT_SUCCESS

// El contexto global del proceso para acceder a las funciones de IPC
ContextoProceso proceso_prueba;

// Definición de cuántas iteraciones durará el proceso (Quantum * 3)
#define MAX_ITERACIONES 15

int main(){
    pid_t pid_proceso;
    int contador = 0;

    // 1. Inicialización y Registro
    pid_proceso = getpid();
    printf("Proceso [PID: %d] iniciando y registrándose en el Planificador.\n", pid_proceso);
    
    // Asume que inicializar_proceso() se encarga del registro vía SHM
    if (inicializar_proceso(&proceso_prueba, pid_proceso) == -1) {
        fprintf(stderr, "ERROR: Falló la inicialización o el registro del proceso.\n");
        return EXIT_FAILURE;
    }
    
    // 2. Bucle de Ejecución Limitada
    while(contador < MAX_ITERACIONES){
        
        // Sección Crítica: Ejecución del Quantum
        printf(">>> [PID: %d] EJECUTANDO (Iteración %d/%d)...\n", getpid(), contador + 1, MAX_ITERACIONES);
        fflush(stdout);
        
        // El proceso realiza su trabajo por una duración corta (no usa sleep(quantum) para no bloquear)
        // Puedes usar un ciclo for o sleep(1) para simular trabajo
        sleep(1); 
        
        contador++;
        
        // Envía la señal de pausa al planificador (el planificador lo manejará)
        // No es necesario si el planificador usa 'sleep(quantum)' y luego pausa al proceso actual.
        // Si el planificador maneja la pausa implícita por el fin del quantum, esta línea no es necesaria aquí.
    }
    
    // 3. Terminación del Proceso
    printf("Proceso [PID: %d] ha completado su trabajo. Enviando SOLICITUD_ELIMINADO.\n", getpid());
    
    // Esta función debe enviar un registro de tipo SOLICITUD_ELIMINADO al planificador
    // y luego desenlazar los recursos del proceso.
    terminar_proceso(proceso_prueba);
    
    printf("Proceso [PID: %d] TERMINADO y recursos liberados.\n", getpid());
    
    return EXIT_SUCCESS;
}