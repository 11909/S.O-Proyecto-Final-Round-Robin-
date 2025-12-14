/*
LIBRERIA: Cabecera de el TAD COLA ESTÁTICA
AUTOR: Edgardo Adrián Franco Martínez (C) Noviembre 2022
IMPLEMENTA: Rodriguez Guarneros Hector Daniel 
VERSIÓN: 1.0 Mayo 2024

DESCRIPCIÓN: TAD cola o Queue.
Estructura de datos en la que se cumple:
Los elementos se insertan en un extremo (el posterior) y 
la supresiones tienen lugar en el otro extremo (frente).

OBSERVACIONES: Hablamos de una Estructura de datos estática cuando 
se le asigna una cantidad fija de memoria para esa estructura 
antes de la ejecución del programa. 
*/

//LIBRERIA CON DEFINICIONES EN COMÚN
#include "queue_common.h"

//DEFINICIONES DE CONSTANTES
#define MAX_ELEMENT 100
#define REGISTRAR_PROCESO 1
#define ELIMINAR_PROCESO 2

//Definir un elemento (Se modela con una estructura "elemento")
typedef struct elemento_estatico
{
	int pid;
	int solicitud_proceso;
} elemento_estatico;

//Definir una cola (Se modela con una estructura que incluye un arreglo estatico de "elemento",
//de MAX_ELEMENT de longitud, así como dos subindices para controlar el frente y final dela cola)
typedef struct cola_estatica
{
	elemento_estatico elementos[MAX_ELEMENT];
	int frente;
	int final;
	int num_elem;
} cola_estatica;

//DECLARACIÓN DE FUNCIONES
void Est_Initialize(cola_estatica * c);			            //Inicializar cola (Initialize): Recibe una cola y la inicializa para su trabajo normal.
void Est_Queue(cola_estatica * c, elemento_estatico e);	    //Encolar (Queue): Recibe una cola y agrega un elemento al final de ella. 
elemento_estatico Est_Dequeue(cola_estatica * c);			//Desencolar (Dequeue): Recibe una cola y remueve el elemento del frente retornándolo.
boolean Est_Empty(cola_estatica * c);			            //Es vacía (Empty): Recibe la cola y devuelve verdadero si esta esta vacía.
elemento_estatico Est_Front(cola_estatica * c);			    //Frente (Front): Recibe una cola y retorna el elemento del frente.	
elemento_estatico Est_Final(cola_estatica * c);			    //Final (Final): Recibe una cola y retorna el elemento del final.
elemento_estatico Est_Element(cola_estatica *c, int i); 	// Recibe una cola y un número de elemento de 1 al tamaño de la cola y retorna el elemento de esa posición.
int Est_Size(cola_estatica *c);					            //Tamaño (Size): Retorna el tamaño de la cola 	
elemento_estatico Est_Element(cola_estatica * c, int i);	//Recibe una cola y un número de elemento de 1 al tamaño de la cola y retorna el elemento de esa posición
void Est_Destroy(cola_estatica * c);				        //Eliminar cola (Destroy): Recibe una cola y la libera completamente.
void Est_Flip(cola_estatica *c);					        //Voltear cola (Flip): Recibe una cola voltea sus elementos, donde el frente sera el
									                        //final, y el final el frente.
