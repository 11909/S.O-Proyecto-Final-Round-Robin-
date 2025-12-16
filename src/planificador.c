/*
LIBRERIA: IMPLEMENTACIÓN DE LIBRERIA PARA PLANIFICADOR ROUND ROBIN
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2024  
VERSIÓN: 4.0 Diciembre 2024

DESCRIPCIÓN: Planificador Round Robin.
Algoritmo de planificación que hace uso de una cola
de manera que todos los procesos que quieren se ejecutados
entran a la cola, y el algoritmo realiza un quantum para
ejecutar en el procesador una cantidad especifica de tiempo
y despues devolver a la cola.

OBSERVACIONES:  
 
*/
#include "round_robin/planificador.h"

/*
void *inicializar_memoria_compartida(const char *path, int id, size_t size)
Descripción: Inicializa una memoria compartida y retonar la dirección donde se encuentra.
Recibe: const char *path (Ruta al archivo base para generar la clave),
        int id (Clave del proyecto para crear llave),
        size_t size (Tamaño de la estructura para el segmento a crear)
Devuelve: void * (Puntero generico al inicio del segmento de memoria compartida)
Observaciones:  La función debe hacer uso de las constantes definidos en la cabecera del planificador
                en "planificador.h" para que se tenga la misma llave para acceder a la memoria compartida
*/
void *inicializar_memoria_compartida(const char *path, int id, size_t size){
    key_t clave;
    int shm_id;
    void *shm_ptr = NULL;

    clave = ftok(path, id);
    if(clave == (key_t) -1){
        perror("ERROR: ftok() en unirse_memoria_compartida()");
        return NULL;
    }

    shm_id = shmget(clave, size, IPC_CREAT|0666);
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
void inicializar_semaforos()
Descripción: Recibe la ruta hacía un semaforo con nombre y lo inicializa
Recibe: const char *path (Ruta al archivo del semaforo con nombre),
        int init (Valor con el que se inicializa el semaforo)
Devuelve:   sem_t *(Puntero al semaforo inicializado)
Observaciones:  El semaforo debe usar la ruta definida de forma global en 
*/
sem_t *inicializar_semaforo(const char *path, int init){
    sem_t *semaforo;
    semaforo = sem_open(path, O_CREAT, 0644, init);

    if(semaforo == SEM_FAILED){
        perror("ERROR: sem_open() en inicializar_semaforos()");
        return NULL;
    }

    return semaforo;
}

/*
void inicializar_cola_procesos(ColaProcesos *cola_procesos)
Descripción: Reciba una cola de procesos y la inicializa como una cola dinamica
Recibe: colaProcesos *cola_procesos (Dirección/Referencia de la cola dinamica)
Devuelve:
Observaciones:  
*/
void inicializar_cola_procesos(ColaProcesos *cola_procesos){
    Dyn_Initialize(cola_procesos);
}

/*
void inicializar_cola_registros(ColaRegistros *cola_registro)
Descripción: Reciba una cola de registros y la inicializa como una cola estatica circular
Recibe: colaRegistro *cola_registro (Dirección/Referencia de la cola estatica)
Devuelve:
Observaciones:  
*/
void inicializar_cola_registros(ColaRegistros *cola_registro){
    Est_Initialize(cola_registro);
}

/*
Registro desencolar_registro(ColaRegistros *cola_registro)
Descripción: Función desencola un registro de la cola de registros y lo devuelve.
Recibe: ColaRegistros *cola_registro (Dirección/Referencia de cola estatica circular con registros)
Devuelve: Registro (Elemento estatico que tiene los datos del Registro)
Observaciones:  
*/
Registro desencolar_registro(ColaRegistros *cola_registro){
    Registro r;

    if(Est_Empty(cola_registro)){
        perror("ERROR: La cola de registros esta vacia\n");
        exit(1);
    }

    r = Est_Dequeue(cola_registro);
    return r;
}

/*
void encolar_proceso(pid_t pid, ColaProcesos *cola_procesos, EstadoProceso estado_proceso)
Descripción: Recibe un pid y el estado acutal del proceso, para despues crear
             un elemento de Proceso y encolarlo en la cola de procesos.
Recibe: pid_t pid (PID del proceso a encolar en la cola de procesos),
        ColaProcesos *cola_procesos (Dirección/Referencia de la cola de procesos para encolar),
        EstadoProceso estado_proceso (Valor enteror para representar el estado con el que el proceso se encolara)
Devuelve:
Observaciones:  
*/
void encolar_proceso(pid_t pid, ColaProcesos *cola_procesos, EstadoProceso estado_proceso){
    Proceso *proceso = (Proceso *) calloc(1, sizeof(Proceso));

    if(proceso == NULL){
        perror("ERROR: calloc() en encolar_proceso()");
        return;
    }

    proceso->pid = (int) pid;
    proceso->estado = estado_proceso;

    Dyn_Queue(cola_procesos, (ElementoColaProcesos)proceso);
    return;
}

/*
Proceso *desencolar_proceso(cola *cola_procesos)
Descripción: Recibe una cola de proesos, comprueba que tenga elementos que
             pueda desencolar. Retorna el elemento desencolado como un Proceso
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola dinamica a desencolar)
Devuelve: Proceso * (Dirección/Referencia de Proceso desencolado)
Observaciones: Dado que se retorna la dirección/referencia del proceso desencolado, se puede
               hacer modificaciones directas.
*/
Proceso *desencolar_proceso(ColaProcesos *cola_procesos){
    ElementoColaProcesos e;
    Proceso *p;

    if(Dyn_Empty(cola_procesos)){
        printf("ERROR: La cola esta vacia. No hay procesos por sacar\n");
        return NULL;
    }
    
    e = Dyn_Dequeue(cola_procesos);

    p = (Proceso *) e;

    return p;
}

/*
int buscar_pid(ColaProcesos *cola_procesos, int pid)
Descripción: Busca un proceso dentro de la cola de procesos que tenga el mismo PID que
             el que se busca. En caso de encontrarlo retorna su posición, en caso contrario retorna 0.
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola dinamica donde se encuentran los procesos),
        int pid (Valor del PID a buscar en al cola)
Devuelve: int (Posición dentro de la cola dinamica donde se encuentra el PID)
Observaciones: La función comprueba que la cola no este vacia entes de hacer la busqueda, en caso de estarlo
               realiza un exit(). Solo hacerlo cuando la cola este llena.
*/
int buscar_pid(ColaProcesos *cola_procesos, int pid){
    int tamano;
    int i;
    int pos;
    Proceso *proceso_auxiliar;

    if(Dyn_Empty(cola_procesos)){
        perror("ERROR: La cola de procesos esta vacia. No hay Procesos a buscar\n");
        exit(1);
    }

    tamano = Dyn_Size(cola_procesos);
    pos = 0;
    for(i = 1; i <= tamano; i++ ){
        proceso_auxiliar = (Proceso *) Dyn_Element(cola_procesos, i);

        if(proceso_auxiliar->pid == pid){
            pos = i;
            break;
        }
    }

    return pos;
}

/*
void cambiar_estado_pid(ColaProcesos *cola_procesos, int posicion, EstadoProceso estado_nuevo)
Descripción: Reciba la posición de un elemento dentro de la cola de procesos y un nuevo estado para ese proceso.
             modifica el estado del proceso en esa posición por el nuevo estado indicado.
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola dinamica donde se encuentran los procesos),
        int posicion (Posición actual del proceso dentro de la cola),
        EstadoProceso estado_nuevo (Nuevo estado con el que se modificara al proceso)
Devuelve:
Observaciones: 
*/
void cambiar_estado_pid(ColaProcesos *cola_procesos, int pid, EstadoProceso estado_nuevo){
    int tamano;
    Proceso *proceso_a_modificar;
    int posicion;

    tamano = Dyn_Size(cola_procesos);
    posicion = buscar_pid(cola_procesos, pid);

    if(posicion <= 0 || posicion > tamano){
        perror("ERROR: La posición esta fuera del rango de la cola. No se puede cambiar su estado");
        return;
    }

    proceso_a_modificar = (Proceso *) Dyn_Element(cola_procesos, posicion);
    proceso_a_modificar->estado = estado_nuevo;

    return;
}

/*
void ejecutar_proceso(pid_t pid)
Descripción: Envia una señal al proceso con ese PID para indicar que puede ejecutarse
Recibe: pid_t pid (PID del proceso que puede ejecutarse en ese momento)
Devuelve:
Observaciones:
*/
void ejecutar_proceso(pid_t pid){
    kill(pid, SIGNAL_CONTINUAR_PROCESO);
}

/*
void detener_proceso(pid_t pid)
Descripción: Envia una señal al proceso con ese PID para indicar que debe pausar su ejecución
Recibe: pid_t pid (PID del proceso que debe pausar en ese momento)
Devuelve:
Observaciones:  
*/
void detener_proceso(pid_t pid){
    kill(pid, SIGNAL_PAUSAR_PROCESO);
}

/*
SHM_Planificador *iniciar_planificador(pid_t pid_planificador)
Descripción: Inicializa a la estructura del planificado en la memoria compartida.
Recibe: pid_t pid (PID del planificador)
Devuelve: SHM_Planificador * (Dirección/Referencia del segmento donde se enceuntra la memoria compartidad con la estructura)
Observaciones:  
*/
SHM_Planificador *iniciar_planificador(pid_t pid_planificador){
    SHM_Planificador *planificador;

    planificador = (SHM_Planificador *) inicializar_memoria_compartida(SHM_PATH, SHM_KEY, sizeof(SHM_Planificador));
    
    planificador->procesos_registrados = 0;
    planificador->planificador_pid = (int) pid_planificador;
    
    inicializar_cola_registros(&planificador->cola_registros);

    return planificador;
}

/*
void limpiar_planificador(ColaProcesos *cola_procesos, SHM_Planificador *planificador)
Descripción: Separa, elimina y limpia las estructuras que usa el planificador.
Recibe: ColaProcesos *cola_procesos (Dirección/Referencia de cola de procesos con procesos registrados)
        SHM_Planificador *planificador (Dirección/Referencia de segmento con memoria compartida)
Devuelve:
Observaciones:  
*/
void limpiar_planificador(ColaProcesos *cola_procesos, SHM_Planificador *planificador){
    Proceso *proceso;
    pid_t pid_proceso;
    while(!Dyn_Empty(cola_procesos)){
        proceso = desencolar_proceso(cola_procesos);
        pid_proceso = (pid_t) proceso->pid;
        kill(pid_proceso, SIGNAL_MATAR_PROCESO);
    }

    Dyn_Destroy(cola_procesos);

    Est_Destroy(&planificador->cola_registros);

    if(sem_unlink(SEM_SHM_PATH) == -1){
        perror("ERROR: sem_unlink() en limpiar_planificador()");
        return NULL;
    }

    if(shmdt(planificador) == -1){
        perror("ERROR: shmdt en limpiar_planificador()");
        return NULL;
    }

    // if(shmctl(shm_id, IPC_RMID, NULL) == -1){
    //     perror("ERROR: shmctl() en limpiar_planificador()");
    //     return NULL;
    // }
}

