/*
IMPLEMENTACION DE LA LIBRERIA DEL TAD COLA DINÁMICA (TADColaEst.h)
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


COMPILACIÓN PARA GENERAR EL CÓDIGO OBJETO: gcc -c queue.c
*/

//LIBRERAS
#include "tad/queue_dinamica.h"
#include <stdlib.h>
#include <stdio.h>
//DEFINICIÓN DE FUNCIONES

/*
void Initialize(cola *c);
Descripción: Inicializar cola (Iniciar una cola para su uso)
Recibe: cola *c (Referencia a la cola "c" a operar)
Devuelve:
Observaciones: El usuario a creado una cola y c tiene la referencia a ella, 
si esto no ha pasado se ocasionara un error.
*/
void Dyn_Initialize(cola_dinamica * c)
{
	c->num_elem=0;
	c->frente=NULL;
	c->final=NULL;
	

	return;
}

/*
void Queue(cola * c, elemento e);
Descripción: Recibe una cola y agrega un elemento al final de ella.
Recibe: cola *c (Referencia a la cola "c" a operar) elemento e (Elemento a encolar)
Devuelve:
Observaciones: El usuario a creado una cola y ha sido inicializada y c tiene la referencia a ella, 
si esto no ha pasado se ocasionara un error.
*/
void Dyn_Queue(cola_dinamica * c, elemento_dinamico e)
{
	nodo* aux;
	aux= malloc(sizeof(nodo));
	if (aux==NULL){
		printf("ERROR: Queue (c,e) desbordamiento de cola");
		exit(1);
	}
	aux->e = e;
	aux->siguiente = NULL;
	if (c->num_elem > 0)
		c->final->siguiente = aux;
	c->final = aux;
	c->num_elem++;
	if (c->num_elem == 1)
		c->frente = c->final;
	return;
}

/*
elemento Dequeue(cola * c);
Descripción: Recibe una cola y devuelve el elemento que se encuentra al 
frente de esta, quitándolo de la cola.
Recibe: cola *c (Referencia a la cola "c" a operar)
Devuelve: elemento (Elemento desencolado)
Observaciones: El usuario a creado e inicializado una cola, ademas la cola tiene 
elementos, si no es así se genera un error y se termina el programa. (Subdesboramiento de Cola)
*/
elemento_dinamico Dyn_Dequeue(cola_dinamica * c)
{
	nodo *aux;
	elemento_dinamico e;//Elemento a retornar 
	if(c->num_elem==0)
	{
		printf("ERROR: Dequeue (c) subdesbordamiento de cola");
		exit(1);
	}
	else 
	{
		e=c->frente->e;
		aux=c->frente;
		c->frente=c->frente->siguiente;
		free(aux);
		c->num_elem--;
		if(c->num_elem==0)
			c->final=NULL;
	}

	//Retornar al elemento
	return e;
}

/*
boolean Empty(cola * c);
Descripción: Recibe una cola y TRUE si la cola esta vacia y FALSE en caso contrario
Recibe: cola *c (Referencia a la cola "c" a verificar)
Devuelve: boolean TRUE O FALSE
Observaciones: El usuario a creado una cola y la cola fue correctamente inicializada
*/
boolean Dyn_Empty(cola_dinamica * c)
{
	
	return (c->num_elem==0)?TRUE:FALSE;
}

/*
elemento Front(cola * c);
Descripción: Recibe una cola y devuelve el elemento que se encuentra al frente de esta.
Recibe: cola *c (Referencia a la cola "c")
Devuelve: elemento del frente de la cola
Observaciones: El usuario a creado una cola,la cola fue correctamente inicializada, esta 
tiene elementos de lo contrario devolvera ERROR. *No se modifica el TAD
*/
elemento_dinamico Dyn_Front(cola_dinamica * c)
{
	elemento_dinamico e;
	
	e=c->frente->e;
	
	return e;
}

/*
elemento Final(cola * c);
Descripción: Recibe una cola y devuelve el elemento que se encuentra al final de esta.
Recibe: cola *c (Referencia a la cola "c")
Devuelve: elemento del final de la cola
Observaciones: El usuario a creado una cola,la cola fue correctamente inicializada, esta 
tiene elementos de lo contrario devolvera ERROR. *No se modifica el TAD
*/
elemento_dinamico Dyn_Final(cola_dinamica * c)
{
	elemento_dinamico e;	
	e=c->final->e;
	return e;
}

/*
int Size(cola * c);
Descripción: Recibe una cola y devuelve el número de elemento que se encuentran en esta.
Recibe: cola *c (Referencia a la cola "c")
Devuelve: int (Tamaño de la cola)
Observaciones: El usuario a creado una cola,la cola fue correctamente inicializada, esta 
*No se modifica el TAD
*/
int Dyn_Size(cola_dinamica * c)
{
	int r;
	r=c->num_elem;	
	return r;
}

/*
void Element(cola * c, int i);
Descripción: Recibe una cola y un número de elemento de 1 al tamaño de la cola y retorna el elemento de esa posición.
Devuelve: elemento en la posicion i de la cola
Observaciones: El número i debera estar entre 1 y el tamaño de la cola, si esta es vacia o mas pequeña se provoca un error.
*/
elemento_dinamico Dyn_Element(cola_dinamica * c, int i)
{
	elemento_dinamico r;
	nodo *aux;
	int j;
	
	if(i>c->num_elem || i<=0)
	{
		printf("ERROR: Element(c,i) i se encuentra fuera del rango");
		exit(1);
	}
	aux = c->frente;

	for(j=1;j<i;j++)
	{
		aux = aux->siguiente;
	}
	r = aux->e;

	return r;
}

/*
void Destroy(cola * c);
Descripción: Recibe una cola y la libera completamente.
Recibe: cola *c (Referencia a la cola "c" a operar)
Devuelve:
Observaciones: La cola se destruye y se inicializa.
*/
void Dyn_Destroy(cola_dinamica * c)
{
	nodo *aux;
	
	aux = c->frente;
	while (aux!=NULL)
	{
		c->frente = aux->siguiente;
		free(aux);
		aux = c->frente;
	}
	c->num_elem = 0;
	c->frente = NULL;
	c->final = NULL;
	
	return;
}

/*
void Flip(cola *c);
Descripción: Recibe una cola y voltea el orden de los elementos
			 en la cola.
Recibe: cola *c (Referencia a la cola "c" a operar)
Devuelve:
Observaciones: El elemento al frente de la cola, terminara al final
			   de la cola, y viceversa para el elemento al final.
*/
void Dyn_Flip(cola_dinamica *c){
	cola_dinamica aux_c;
	elemento_dinamico aux_e;
	int i, s, n;
	
	Dyn_Initialize(&aux_c);
	if(Dyn_Empty(c)){
		printf("Error: Flip(c) la cola esta vacia");
		exit(1);
	}
	
	n = Dyn_Size(c);
	
	for(i = 1; i <= n; i++){
		aux_e = Dyn_Dequeue(c);
		Dyn_Queue(&aux_c, aux_e);
	}
	
	s = Dyn_Size(&aux_c);
	
	for(i = s; i > 0; i--){
		aux_e = Dyn_Element(&aux_c, i);
		Dyn_Queue(c, aux_e);
	}
	
	return;
}


/*
void Dyn_Destroy_Node(cola_dinamica *c, int i);
Descripción: Recibe una cola y la posición de un elemento en ella, para eliminar el elemento en
			 esa posición y reodenar los nodos.
Recibe: cola *c (Referencia a la cola "c" a operar)
		int i	(Posición del elemento a eliminar en la cola)
Devuelve:
Observaciones: El elemento anterior al elemento a eliminar, ahora debe de apuntar
			   al elemento posterior al elemento a eliminar.
*/
void Dyn_Destroy_Node(cola_dinamica *c, int i){
	nodo *nodo_anterior = NULL;
	nodo *nodo_a_destruir = NULL;
	int j;

	if(Dyn_Empty(c)){
		printf("ERROR: Dyn_Destroy_Node(c,i) la cola esta vacia\n");
		exit(1);
	}

	if(i > c->num_elem || i <= 0)
	{
		printf("ERROR: Dyn_Destroy_Node(c,i) i se encuentra fuera del rango\n");
		exit(1);
	}

	//SE ELIMINA EL PRIMER ELEMENTO DE LA COLA
	if(i == 1){
		nodo_a_destruir = c->frente;
		c->frente = c->frente->siguiente;

		free(nodo_a_destruir);
		c->num_elem--;
		if(c->num_elem == 0){
			c->final = NULL;
		}
	}
	//LA COLA TIENE MÁS DE UN ELEMENTO
	else{
		nodo_anterior = c->frente;
		
		for(j = 1; j < i - 1; j++){
			nodo_anterior = nodo_anterior->siguiente;
		}
	
		nodo_a_destruir = nodo_anterior->siguiente;
	
		nodo_anterior->siguiente = nodo_a_destruir->siguiente;
	
		if(nodo_a_destruir == c->final){
			c->final = nodo_anterior;
		}
	
		free(nodo_a_destruir);
		c->num_elem--;
	}

	return;
}