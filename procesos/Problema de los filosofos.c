//CENA DE FILOSOFOS
// ESCOM | SISTEMAS OPERATIVOS | MENDEZ ROSALES MIGUEL ANGELL, RODRIGUEZ GUARNEROS HECTOR DANIEL
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define HAMBRIENTO 0
#define COMIENDO 1
#define PENSANDO 2

#define ENDL printf("\n")

#define NUM_FILOSOFOS 8

pthread_mutex_t acceso_estados = PTHREAD_MUTEX_INITIALIZER;

int estados_filosofos[NUM_FILOSOFOS];
pthread_t FILOSOFOS[NUM_FILOSOFOS];

void *filosofo(void *arg);
void pensar(int i);
void tomar_tenedores(int i);
void comer(int i);
void soltar_tenedores(int i);
bool intentar_tomar(int i);

int izquierda(int i);
int derecha(int i);

int main(){
    for(int i = 0; i < NUM_FILOSOFOS; i++){
        estados_filosofos[i] = PENSANDO;
    }

    for(int i = 0; i < NUM_FILOSOFOS; i++){
        pthread_create(&FILOSOFOS[i], NULL, filosofo, (void*)(long)i);
    }

    for(int i = 0; i < NUM_FILOSOFOS; i++){
        pthread_join(FILOSOFOS[i], NULL);
    }

}


void *filosofo(void *arg){
    int i = (long) arg;
    
    while(1){
        pensar(i);
        tomar_tenedores(i);
        comer(i);
        soltar_tenedores(i);
    }

    return NULL;
}

int izquierda(int i){
    return (i + NUM_FILOSOFOS - 1) % NUM_FILOSOFOS;
}

int derecha (int i){
    return (i + 1) % NUM_FILOSOFOS;
}

void pensar(int i){
    pthread_mutex_lock(&acceso_estados);
    estados_filosofos[i] = PENSANDO;
    pthread_mutex_unlock(&acceso_estados);

    printf("[FILOSOFO %d] Esta PENSANDO", i); ENDL;
    //sleep(2);
    sleep(rand() % 3 + 1);
}

void tomar_tenedores(int i){
    int izq = izquierda(i);
    int der = derecha(i);

    pthread_mutex_lock(&acceso_estados);
    estados_filosofos[i] = HAMBRIENTO;
    pthread_mutex_unlock(&acceso_estados);

    printf("[FILOSOFO %d] Esta HAMBRIENTO", i); ENDL;
}

void comer(int i){
    while (1) {
        pthread_mutex_lock(&acceso_estados);
        estados_filosofos[i] = HAMBRIENTO;

        if (intentar_tomar(i)) {
            estados_filosofos[i] = COMIENDO;
            pthread_mutex_unlock(&acceso_estados);
            break;
        }
        pthread_mutex_unlock(&acceso_estados);
        usleep(100000); // Esperar un poco antes de volver a intentar
    }

    printf("[FILOSOFO %d] Está COMIENDO", i); ENDL;
    //sleep(2);
    sleep(rand() % 3 + 1);
}

bool intentar_tomar(int i){
    int izq = izquierda(i);
    int der = derecha(i);

    if(estados_filosofos[i] == HAMBRIENTO &&
       estados_filosofos[izq] != COMIENDO &&
       estados_filosofos[der] != COMIENDO){

        return true;
    }
    else{
        return false;
    }
}

void soltar_tenedores(int i){
    int izq = izquierda(i);
    int der = derecha(i);

    pthread_mutex_lock(&acceso_estados);
    estados_filosofos[i] = PENSANDO;
    pthread_mutex_unlock(&acceso_estados);
    
    printf("[FILOSOFO %d] Termino de COMER", i); ENDL;
}
