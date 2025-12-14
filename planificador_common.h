//LLAMADA DE LIBRERIAS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include "queue.h"
#include "queue_estatica.h"

//DECLARACIÓN DE CONSTANTES
#define SHM_PATH "/tmp"
#define SHM_KEY 'A'
#define SEM_SHM_PATH "/tmp/sem_shm"
#define SEM_PROCESS_PATH "/tmp/sem_shm"
#define INITIAL_SEM_VALUE 1

//SEÑALES DE PLANIFICADOR -> PROCESO
#define CONTINUAR_PROCESO SIGCONT
#define PAUSAR_PROCESO SIGSTOP
#define MATAR_PROCESO SIGTERM

//SEÑALES PROCESO -> PLANIFICADOR
#define PROCESO_ENVIO_SOLICITUD SIGUSR1

//ESTADOS DE PROCESO
#define LISTO 0
#define EJECUTANDO 1
#define PAUSADO 2
#define TERMINADO 3

//DECLARACIÓN DE TIPOS PARA EL PLANIFICADOR
typedef elemento_estatico Registro;
typedef cola_estatica ColaRegistros;
typedef cola_dinamica ColaProcesos;
typedef int EstadoProceso;

//DECLARACIÓN DE ESTRUCUTRA PARA UN PROCESO
//Se debe considerar que se usara una cola con apuntadores genericos,
//entonces se debe usar casting para la interacción entre planificador y estructura
typedef struct Proceso{
    int pid;
    EstadoProceso estado;
}Proceso;

//DECLARACIÓN DE ESTRUCTURA PARA LA MEMORIA COMPARTIDAS
typedef struct SHM_Planificador{
    int planificador_pid;
    int procesos_registrados;
    ColaRegistros cola_registros;
}SHM_Planificador;