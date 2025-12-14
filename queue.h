/*
LIBRERIA: Cabecera de el TAD COLA DINÁMICA
AUTOR: Edgardo Adrián Franco Martínez (C) Noviembre 2022
IMPLEMENTA: Rodriguez Guarneros Hector Daniel 
VERSIÓN: 1.0 Mayo 2024

DESCRIPCIÓN: TAD cola o Queue.
Estructura de datos en la que se cumple:
Los elementos se insertan en un extremo (el posterior) y 
la supresiones tienen lugar en el otro extremo (frente).

OBSERVACIONES: Hablamos de una Estructura de datos dinámica 
cuando se le asigna memoria a medida que es necesitada, 
durante la ejecución del programa. 
 
*/

//DEFINICIONES DE CONSTANTES
#define TRUE	1
#define FALSE	0

//DEFINICIONES DE TIPOS DE DATO

//Definir un boolean (Se modela con un "char")
typedef unsigned char boolean;

//Definir un elemento (Se modela con una estructura "elemento")
//Se usa un apuntador generico (void *) para que la estructura TAD Cola funcione
//con cualquier caso, pero es necesario hacer casting.
typedef void* elemento_dinamico;

//Definir la estructura de un nodo dinámico
typedef struct nodo
{
	elemento_dinamico e;
	struct nodo *siguiente;
}nodo;

//Definir una cola 
typedef struct cola_dinamica
{
	int num_elem;
	nodo *frente;
	nodo *final;
} cola_dinamica;

//DECLARACIÓN DE FUNCIONES	
void Dyn_Initialize(cola_dinamica * c);						//Inicializar cola (Initialize): Recibe una cola y la inicializa para su trabajo normal.
void Dyn_Queue(cola_dinamica * c, elemento_dinamico e);		//Encolar (Queue): Recibe una cola y agrega un elemento al final de ella. 
elemento_dinamico Dyn_Dequeue(cola_dinamica * c);			//Desencolar (Dequeue): Recibe una cola y remueve el elemento del frente retornándolo.
boolean Dyn_Empty(cola_dinamica * c);						//Es vacía (Empty): Recibe la cola y devuelve verdadero si esta esta vacía.
elemento_dinamico Dyn_Front(cola_dinamica * c);				//Frente (Front): Recibe una cola y retorna el elemento del frente.	
elemento_dinamico Dyn_Final(cola_dinamica * c);				//Final (Final): Recibe una cola y retorna el elemento del final.
elemento_dinamico Dyn_Element(cola_dinamica *c, int i); 	// Recibe una cola y un número de elemento de 1 al tamaño de la cola y retorna el elemento de esa posición.
int Dyn_Size(cola_dinamica *c);								//Tamaño (Size): Retorna el tamaño de la cola 	
void Dyn_Destroy(cola_dinamica * c);						//Eliminar cola (Destroy): Recibe una cola y la libera completamente.
void Dyn_Flip(cola_dinamica *c);							//Voltear cola (Flip): Recibe una cola voltea sus elementos, donde el frente sera el
															//final, y el final el frente.
