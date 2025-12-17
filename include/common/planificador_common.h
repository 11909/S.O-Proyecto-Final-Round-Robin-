/*
LIBRERIA: Cabecera de ATRIBUTOS EN COMÚN PARA PLANIFICADOR ROUND ROBIN
AUTOR:  Rodriguez Guarneros Hector Daniel 
        Mendez Rosales Miguel Angel
        (C) Noviembre 2025
VERSIÓN: 1.0 Diciembre 2024

DESCRIPCIÓN: Atributos Comunes para Planificador Round Robin.
Definición de estructuras, constantes y tipos de variables que
hara uso el planificador y los procesos que formen parte del planificador,
o vayan a formar parte del planificador.

OBSERVACIONES:  
*/

#ifndef PLANIFICADOR_COMMON_H
#define PLANIFICADOR_COMMON_H

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
#include "tad/queue_estatica.h"
#include "tad/queue_dinamica.h"

//DECLARACIÓN DE CONSTANTES
#define SHM_PATH "/tmp/planificador_key"
#define SHM_KEY 'A'
#define SEM_SHM_PATH "/planificador_semaforo"
#define INITIAL_SEM_VALUE 1

//SEÑALES DE PLANIFICADOR -> PROCESO
#define SIGNAL_CONTINUAR_PROCESO SIGCONT
#define SIGNAL_PAUSAR_PROCESO SIGSTOP
#define SIGNAL_MATAR_PROCESO SIGTERM

//SEÑALES PROCESO -> PLANIFICADOR
#define SIGNAL_ENVIO_REGISTRO SIGUSR1

//ESTADOS DE PROCESO
#define LISTO 0
#define EJECUTANDO 1
#define PAUSADO 2
#define TERMINADO 3

//DECLARACIÓN DE TIPOS PARA EL PLANIFICADOR
typedef elemento_estatico Registro;
typedef elemento_dinamico ElementoColaProcesos;
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
    ColaRegistros cola_registros;
}SHM_Planificador;

#endif