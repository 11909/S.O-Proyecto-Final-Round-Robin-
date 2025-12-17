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
#include <sys/types.h>
#include <signal.h>

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

    int fd = open(path, O_CREAT, 0666); 
    if (fd == -1) {
        perror("Error al crear el archivo clave para SHM");
        exit(EXIT_FAILURE);
    }
    close(fd);

    clave = ftok(path, id);
    if(clave == (key_t) -1){
        perror("ERROR: ftok() en inicializar_memoria_compartida()");
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
        Registro *registro_obtenido (Dirección/Referencia de Registro donde se asignara el registro desencolado)
Devuelve: int (Numero que representa el estado de salida de la función)
Observaciones:  0 [Salida erronea], 1 [Salida exitosa]
*/
int desencolar_registro(ColaRegistros *cola_registro, Registro *registro_obtenido){
    int resultado;

    resultado = 0;
    if(Est_Empty(cola_registro)){
        perror("ERROR: La cola de registros esta vacia\n");
        resultado = 0;
    }
    else{
        *registro_obtenido = Est_Dequeue(cola_registro);
        resultado = 1;
    }

    return resultado;
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
        perror("ERROR: La cola esta vacia. No hay procesos por sacar\n");
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
Observaciones:  La función en caso de no encontrar el PID dentro de la cola de procesos devuelve -1.
                -1 [Salida erronea]
*/
int buscar_pid(ColaProcesos *cola_procesos, int pid){
    int tamano;
    int i;
    int pos;
    Proceso *proceso_auxiliar;

    if(Dyn_Empty(cola_procesos)){
        perror("ERROR: La cola de procesos esta vacia. No hay Procesos a buscar\n");
        return -1;
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
void pausar_proceso(pid_t pid){
    kill(pid, SIGNAL_PAUSAR_PROCESO);
}

/*
int iniciar_planificador(ContextoPlanificador *planificador)
Descripción: Inicializa los parametros, estructuras y archivos necesarios para que el planificador
             pueda trabajar.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia estructura que tiene los parametros necesario para el manejo del planificador)
        pid_t pid (PID del planificador)
        int quantum (Cantidad de tiempo en segundos que el planificador da de ejecución a cada proceso)
Devuelve: int (Entero para representar el estado de salida de la función)
Observaciones:  -1 [Salida erronea], 0 [Salida correcta]
*/
int iniciar_planificador(ContextoPlanificador *planificador, pid_t pid_planificador, int quantum){
    planificador->shm = (SHM_Planificador *) inicializar_memoria_compartida(SHM_PATH, SHM_KEY, sizeof(SHM_Planificador));
    if(planificador->shm == (SHM_Planificador*)NULL) return -1;

    planificador->shm->planificador_pid = pid_planificador;
    inicializar_cola_registros(&planificador->shm->cola_registros);

    planificador->semaforo_shm = inicializar_semaforo(SEM_SHM_PATH, INITIAL_SEM_VALUE);
    if(planificador->semaforo_shm == NULL) return -1;

    inicializar_cola_procesos(&planificador->cola_procesos);
    planificador->procesos_registrados = 0;

    planificador->proceso_actual = NULL;
    planificador->quantum = quantum;
    planificador->signal_registros = 0;
    planificador->signal_terminar = 0;

    return 0;
}

/*
Proceso *obtener_siguiente_proceso(ContextoPlanificador *planificador)
Descripción: Función que desencola Procesos de la cola de Procesos y los trata, hasta que desencole un
             proceso que este Listo o Pausado para ser ejecutado.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve:   Proceso *proceso (Dirección/Referencia del Proceso desencolado)
Observaciones:    
*/
Proceso *obtener_siguiente_proceso(ContextoPlanificador *planificador){
    int tamano;
    Proceso *proceso_auxiliar;
    ColaProcesos *cola_procesos = &planificador->cola_procesos;

    tamano = Dyn_Size(cola_procesos);
    if(tamano == 0){
        perror("ERROR: La cola de procesos no tiene Procesos.\n");
        return NULL;
    }

    if(planificador->proceso_actual != NULL){
        perror("ERROR: Ya se encuentra manejando un proceso el planificador. \n");
        return NULL;
    }

    while(Dyn_Size(cola_procesos) > 0){
        proceso_auxiliar = desencolar_proceso(cola_procesos);
    
        if(proceso_auxiliar->estado == TERMINADO){
            printf("Un proceso ha terminado. [PID: %d]\n", proceso_auxiliar->pid);
            free(proceso_auxiliar);
            planificador->procesos_registrados--;
            continue;
        }
    
        if(proceso_auxiliar->estado == PAUSADO || proceso_auxiliar->estado == LISTO){
            proceso_auxiliar->estado = LISTO;
            return proceso_auxiliar;
        }

        if(proceso_auxiliar->estado == EJECUTANDO){
            printf("ERROR FATAL: Proceso PID [%d] en cola con estado EJECUTANDO.\n", proceso_auxiliar->pid);
            exit(1);
        }
    }

    printf("Ya no hay procesos por atender\n");
    return NULL;
}

/*
void tratar_registro(ContextoPlanificador *planificador)
Descripción: Función que se encerga de desencolar todos los registros pendientes en la cola de registros.
             Encola todo proceso que solicita un registro, y cambia el estado a Terminado de todo proceso
             que avisa haber terminado.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve: 
Observaciones:  
*/
void tratar_registro(ContextoPlanificador *planificador){
    ColaProcesos *cola_procesos;
    ColaRegistros *cola_registros;
    Registro registro;
    int bool_registro;

    sem_wait(planificador->semaforo_shm);
    cola_procesos = &planificador->cola_procesos;
    cola_registros = &planificador->shm->cola_registros;

    if(Est_Size(cola_registros) == 0){
        printf("La cola de registros no tiene registros por atender\n");
        sem_post(planificador->semaforo_shm);
        return;
    }

    while(Est_Size(cola_registros) > 0){
        bool_registro = desencolar_registro(cola_registros, &registro);

        if(bool_registro == 0){
            printf("ERROR: No se logro desencolar el registro correctamente\n");
            break;
        }

        if(registro.solicitud_proceso == SOLICITUD_REGISTRO){
            encolar_proceso(registro.pid, cola_procesos, LISTO);
            planificador->procesos_registrados++;
            printf("Se ha registrado un nuevo proceso. [PID: %d]\n", registro.pid);
        }

        if(registro.solicitud_proceso == SOLICITUD_ELIMINADO){
            cambiar_estado_pid(cola_procesos, registro.pid, TERMINADO);
        }
    }

    sem_post(planificador->semaforo_shm);
    planificador->signal_registros = 0;
    printf("Ya no hay registro por atender. \n");
    return;
}

/*
int planificador_proceso_disponible(ContextoPlanificador *planificador)
Descripción: Función que comprueba que haya procesos disponibles en la cola de procesos.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve: int   (Estado de salida para indicar disponibilidad de procesos en la cola de procesos)
Observaciones:  0 [Sin procesos disponibles], 1 [Procesos dispnibles]  
*/
int planificador_proceso_disponible(ContextoPlanificador *planificador){
    ColaProcesos *cola_procesos;
    int resultado;

    cola_procesos = &planificador->cola_procesos;

    resultado = 0;
    if(Dyn_Empty(cola_procesos)){
        printf("No hay procesos por tratar en la cola de procesos.\n");
        resultado = 0;
    }
    else{
        resultado = 1;
    }
    
    return resultado;
}

/*
int planificador_ejecuta_proceso(ContextoPlanificador *planificador)
Descripción: Función que ejecuta el proceso actual manejado por el planificador. Cambiando su estado de Listo a
             Ejecutando.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve: int   (Estado de salida de la función)
Observaciones:  0 [Salida erronea], 1 [Salida correcta]  
*/
int planificador_ejecuta_proceso(ContextoPlanificador *planificador){
    Proceso *proceso;
    int resultado = 0;

    proceso = obtener_siguiente_proceso(planificador);
    if(proceso == NULL){
        printf("ERROR: No hay procesos para tratar.\n");
        resultado = 0;
    }
    else if(proceso->estado == LISTO){
        planificador->proceso_actual = proceso;
        planificador->proceso_actual->estado = EJECUTANDO;
        ejecutar_proceso(planificador->proceso_actual->pid);
        printf("Se esta ejecutando un proceso. [PID: %d]\n", planificador->proceso_actual->pid);
        imprimir_cola_procesos(planificador);
        resultado = 1;
    }
    else{
        printf("ERROR: El proceso no estaba listo para ser ejecutado\n");
        resultado = 0;
    }

    return resultado;
}

/*
int planificador_pausa_proceso(ContextoPlanificador *planificador)
Descripción: Función comprueba el estado del proceso actual atendido por el planificador. En caso de estar
             en ejecución, lo pausa y encola el proceso en la cola de procesos. Depues limpia el proceso actual.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve: int   (Estado de salida de la función)
Observaciones:  0 [Salida erronea], 1 [Salida correcta]  
*/
int planificador_pausa_proceso(ContextoPlanificador *planificador){
    Proceso *proceso;
    int resultado;

    resultado = 0;
    proceso = planificador->proceso_actual;
    if(proceso == NULL){
        printf("ERROR: No hay un proceso actual en el planificador\n");
        resultado = 0;
    }
    else if(proceso->estado == EJECUTANDO){
        pausar_proceso(proceso->pid);
        proceso->estado = PAUSADO;
        Dyn_Queue(&planificador->cola_procesos, proceso);
        planificador->proceso_actual = NULL;

        resultado = 1;
    }
    else{
        printf("ERROR: El proceso no estaba listo para ser ejecutado");
        resultado = 0;
    }

    return resultado;
}


/*
void limpiar_planificador(ContextoPlanificador *planificador)
Descripción: Separa, elimina y limpia las estructuras que usa el planificador.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve:
Observaciones:  La función se debe de encargar de liberar todos los espacios de memorias asignados para los procesos
                de la cola de procesos.
*/
void limpiar_planificador(ContextoPlanificador *planificador){
    Proceso *proceso;
    pid_t pid_proceso;

    sem_wait(planificador->semaforo_shm);
    ColaProcesos *cola_procesos = &planificador->cola_procesos;
    ColaRegistros *cola_registros = &planificador->shm->cola_registros;

    while(!Dyn_Empty(cola_procesos)){
        proceso = desencolar_proceso(cola_procesos);
        pid_proceso = (pid_t) proceso->pid;
        kill(pid_proceso, SIGNAL_MATAR_PROCESO);
        free(proceso);
    }

    Dyn_Destroy(cola_procesos);
    Est_Destroy(cola_registros);
    sem_post(planificador->semaforo_shm);

    if(sem_unlink(SEM_SHM_PATH) == -1){
        perror("ERROR: sem_unlink() en limpiar_planificador()");
        return;
    }

    if(shmdt(planificador->shm) == -1){
        perror("ERROR: shmdt en limpiar_planificador()");
        return;
    }

    free(planificador);
    // if(shmctl(shm_id, IPC_RMID, NULL) == -1){
    //     perror("ERROR: shmctl() en limpiar_planificador()");
    //     return NULL;
    // }
}

/*
void imprimir_cola_procesos(ContextoPlanificador *planificador)
Descripción: Accede al estado actual del planificador e imprime su cola de procesos.
Recibe: ContextoPlanificador *planificador (Dirección/Referencia de estructura con parametros del planificador)
Devuelve:
Observaciones:  
*/
void imprimir_cola_procesos(ContextoPlanificador *planificador){
    int i;
    int pid_proceso;
    Proceso *proceso_auxiliar;
    ColaProcesos *cola_procesos;

    cola_procesos = &planificador->cola_procesos;
    printf("Procesos en cola: \n");
    printf("Inicio -> ");
    for(i = 1; i <= Dyn_Size(cola_procesos); i++){
        proceso_auxiliar = (Proceso *) Dyn_Element(cola_procesos, i);
        pid_proceso = proceso_auxiliar->pid;
        printf("[%d] \t", pid_proceso);
    }
    printf("<- Final\n");
}