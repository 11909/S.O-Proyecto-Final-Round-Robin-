/*
LIBRERIA: IMPLEMENTACIÓN DE LIBRERIA PARA PROCESO DE UN PLANIFICADOR ROUND ROBIN
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2024  
VERSIÓN: 2.0 Diciembre 2024

DESCRIPCIÓN: Proceso para un Planificador Round Robin.
Todo proceso que se vaya a comunicar con un planificador Round Robin debe
de ser capaz de comunicarse mediante señales con el planificador para atender
su registro y su eliminación. Así como debe de atender las señales del planificador
para poder pausar su ejecución o continuarla.

OBSERVACIONES:  
Dado que se usara una cola de registros dentro de la memoria compartida del planificador,
es necesario que toda comunicación de un proceso al planificador sea tratada como un registro.
Entonces debe de encolarse dentro de la cola estatica circular en la memoria compartida y avisar
al planificador que hay registros pendientes.
*/

//LLAMADA DE LIBRERIAS
#include "round_robin/proceso.h"
#include <sys/types.h>
#include <signal.h>

//DEFINICIÓN DE FUNCIONES PARA PROCESO
/*
void *unirse_memoria_compartida(const char *path, int id, size_t size)
Descripción: Recibe los parametros para unirse a un segmento de memoria compartida que ya fue creada,
             devolviendo el apuntador generico a ese segmento de memoria.
Recibe: const char *path (Ruta al archivo base para generar la clave),
        int id (Clave del proyecto para crear llave),
        size_t size (Tamaño de la estructura para el segmento a crear)
Devuelve: void * (Puntero generico al inicio del segmento de memoria compartida)
Observaciones: La memoria de compartida ya tuvo que ser creada para poder unirse a ella.
*/
void *unirse_memoria_compartida(const char *path, int id, size_t size){
    key_t clave;
    int shm_id;
    void *shm_ptr = NULL;

    int fd = open(path, O_CREAT, 0666); 
    if (fd == -1) {
        perror("Error al crear el archivo clave para SHM");
        exit(EXIT_FAILURE);
    }
    close(fd);

    clave = ftok(path, id);
    if(clave == (key_t) -1){
        perror("ERROR: ftok() en unirse_memoria_compartida()");
        return NULL;
    }

    shm_id = shmget(clave, size, 0666);
    if(shm_id == -1){
        perror("ERROR: shmget() en inicializar_memoria_compartida()");
        return NULL;
    }

    shm_ptr = shmat(shm_id, NULL, 0);
    if(shm_ptr == (void *) -1){
        perror("ERROR: shmat() en inicializar_memoria_compartida()");
        return NULL;
    }

    return shm_ptr;
}

/*
sem_t *acceder_semaforo(const char *path)
Descripción: Función que se una a un semaforo con nombre ya inicializado.
Recibe: const char *path (Ruta de semaforo con nombre)
Devuelve: sem_t * (Apuntador/Referencia de semaforo al que se ha unido)
Observaciones:
*/
sem_t *acceder_semaforo(const char *path){
    sem_t *semaforo;

    semaforo = sem_open(path, 0);

    if(semaforo == SEM_FAILED){
        perror("ERROR: sem_open() en acceder_semaforo");
        return NULL;
    }

    return semaforo;
}

/*
int inicializar_proceso(ContextoProceso *contexto, pid_t pid)
Descripción: Inicializa los parametros de un proceso para que pueda iniciar su
             comunicación con el Planificador. Se une a la memoria compartida y semaforo,
             así como envia su PID al planificador.
Recibe: ContextoProceso *contexto (Dirección/Referencia de estructura que tiene los parametros necesario para el manejo del proceso),
        pid_t pid (PID del proceso que sera admnistrado por el planificador)
Devuelve: int (Valor para indicar si el proceso se cumplio o tuvo errores)
Observaciones: La función solamente puede cometer errores en inicializar los parametros, entonces un
               error siginifica que no se ha registrado. Se recomienda hacer un exit().
*/
int inicializar_proceso(ContextoProceso *proceso, pid_t pid){
    proceso->shm = (SHM_Planificador*) unirse_memoria_compartida(SHM_PATH, SHM_KEY, sizeof(SHM_Planificador));
    if(proceso->shm == (SHM_Planificador *)NULL) return -1;
    
    proceso->semaforo_shm = acceder_semaforo(SEM_SHM_PATH);
    if(proceso->semaforo_shm == NULL) return -1;

    proceso->pid_proceso = pid;

    registrar_proceso(*proceso);
    printf("[PID %d] Proceso ha solicitud de registro\n", (int)pid);
    return 0;
}

/*
void encolar_registro(ContextoProceso proceso, int solicitud)
Descripción: Función que encola un registro en la cola de registros de la memoria compartida
             con el planificador, así como avisa al planificador que hay registros pendientes.
Recibe: ContextoProceso proceso (Estructura con los parametros necesarios del proceso),
        int solicitud (Valor numerico para representar el tipo de solicitud que hara al planificador).
Devuelve: 
Observaciones: 
*/
void encolar_registro(ContextoProceso proceso, int solicitud){
    Registro registro;

    registro.pid = proceso.pid_proceso;
    registro.solicitud_proceso = solicitud;

    sem_wait(proceso.semaforo_shm);
    Est_Queue(&proceso.shm->cola_registros, registro);
    kill( (pid_t) proceso.shm->planificador_pid, SIGNAL_ENVIO_REGISTRO);
    sem_post(proceso.semaforo_shm);
    
    return;
}

/*
void registrar_proceso(ContextoProceso proceso)
Descripción: Función que envia una solicitud de registrar el proceso a la oola de procesos
             del planificador.
Recibe: ContextoProceso proceso (Estructura con los parametros necesarios del proceso)
Devuelve: 
Observaciones: 
*/
void registrar_proceso(ContextoProceso proceso){
    encolar_registro(proceso, SOLICITUD_REGISTRO);
}

/*
void terminar_proceso(ContextoProceso proceso)
Descripción: Función que envia una solicitud de eliminar el proceso a la oola de procesos
             del planificador. Para que sea retirado de la cola de procesos,
Recibe: ContextoProceso proceso (Estructura con los parametros necesarios del proceso)
Devuelve: 
Observaciones: 
*/
void terminar_proceso(ContextoProceso proceso){
    encolar_registro(proceso, SOLICITUD_ELIMINADO);
}