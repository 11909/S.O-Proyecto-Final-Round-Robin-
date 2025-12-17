// ESCOM | SISTEMAS OPERATIVOS
// MENDEZ ROSALES MIGUEL ANGEL, RODRIGUEZ GUARNEROS HECTOR DANIEL
#include "round_robin/proceso.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

ContextoProceso proceso;

int N; //NUmero de sillas
int sillas_libres; // Número de sillas libres

// Semáforos
sem_t clientes;        // Cuenta clientes esperando
sem_t barbero;         // Señaliza cuando el barbero está listo
sem_t mutex;           // Protege la sección crítica

void *barbero_thread(void* arg); 
void *cliente_thread(void* arg); 

int main() {
    int numClientes=0; 
    printf("Introduce la cantidad de clientes a esperar"); 
    scanf("%d",&numClientes);
    printf("Introduce el numero de sillas: "); 
    scanf("%d",&N);
    sillas_libres=N; 

    pthread_t barbero_tid;
    pthread_t clientes_tid[numClientes];
    
    // Inicializa semáforos
    sem_init(&clientes, 0, 0);  // Sin clientes inicialmente
    sem_init(&barbero, 0, 0);   // Barbero no está listo inicialmente
    sem_init(&mutex, 0, 1);     // Mutex para sección crítica
    
    pid_t pid_proceso;

    pid_proceso = getpid();
    inicializar_proceso(&proceso, pid_proceso);

    printf("Barbería abierta con %d sillas de espera\n\n", N);
    
    // Crea el hilo del barbero
    pthread_create(&barbero_tid, NULL, barbero_thread, NULL);
    
    // Simula la llegada de clientes
    for(int i = 1; i <= numClientes; i++) {
        sleep(rand() % 3);  // Clientes llegan aleatoriamente
        
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&clientes_tid[i-1], NULL, cliente_thread, id);
    }
    
    // Espera a que todos los clientes terminen
    for(int i = 0; i < numClientes; i++) {
        pthread_join(clientes_tid[i], NULL);
    }
    
    printf("\nTodos los clientes han sido atendidos. Cerrando barbería...\n");
    
    // Cancela el hilo del barbero
    pthread_cancel(barbero_tid);
    pthread_join(barbero_tid, NULL);
    
    // Limpia semáforos
    sem_destroy(&clientes);
    sem_destroy(&barbero);
    sem_destroy(&mutex);
    

    terminar_proceso(proceso);
    return 0;
}

void* barbero_thread(void* arg) {
    while(1) {
        printf("Barbero: No hay clientes, me voy a dormir...\n");
        
        // Espera a que llegue un cliente (barbero durmiendo)
        sem_wait(&clientes);
        
        printf("Barbero: Me despertaron! Hay un cliente esperando.\n");
        
        // Protege acceso a sillas_libres
        sem_wait(&mutex);
        sillas_libres++;
        printf("Barbero: atendiendo cliente. Sillas libres: %d\n", sillas_libres);
        sem_post(&mutex);
        
        // Señaliza que está listo para cortar el cabello
        sem_post(&barbero);
        
        // Simula el corte de cabello
        printf("Barbero: cortando cabello...\n");
        sleep(2);
        printf("Barbero: cliente terminado!\n\n");
    }
    return NULL;
}

void* cliente_thread(void* arg) {
    int id = *(int*)arg;
    
    printf("Cliente %d: llegando a la barbería...\n", id);
    
    // Protege acceso a sillas_libres
    sem_wait(&mutex);
    
    if(sillas_libres > 0) {
        // Hay sillas disponibles
        sillas_libres--;
        printf("Cliente %d: sentándose. Sillas libres: %d\n", id, sillas_libres);
        
        // Señaliza que hay un cliente esperando
        sem_post(&clientes);
        sem_post(&mutex);
        
        // Espera a que el barbero esté listo
        sem_wait(&barbero);
        
        printf("Cliente %d: recibiendo corte de cabello...\n", id);
        
    } else {
        // No hay sillas disponibles
        printf("Cliente %d: no hay sillas, se va...\n", id);
        sem_post(&mutex);
    }
    
    free(arg);
    return NULL;
}
