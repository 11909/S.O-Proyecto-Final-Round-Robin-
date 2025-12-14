/*
IMPLEMENTACION DE LA LIBRERIA DEL TAD COLA ESTATICA (TADColaEst.h)
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

COMPILACIÓN PARA GENERAR EL CÓDIGO OBJETO: gcc -c TADColaEst.c
*/

//LIBRERAS
#include "queue_estatica.h"
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
void Est_Initialize(cola_estatica * c)
{
	c->frente=-1;
	c->final=-1;
	c->num_elem=0;
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
void Est_Queue(cola_estatica * c, elemento_estatico e)
{
	
	if(c->num_elem == MAX_ELEMENT-1)
	{
		printf("\nERROR: Queue(c,e) desbordamiento de cola.");
		exit(1);
	}
	c->final++;
	if(c->final==MAX_ELEMENT)
		c->final=0;
		
	c->elementos[c->final] = e;
	c->num_elem++;
	
	if(c->frente == -1)
	{
		c->frente++;
	}
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
elemento_estatico Est_Dequeue(cola_estatica * c)
{
	elemento_estatico e;//Elemento a retornar 
	if (c->num_elem>0)
	{
		e=c->elementos[c->frente];
		c->frente++;
		if(c->frente==MAX_ELEMENT)
			c->frente=0;
		c->num_elem--;
		
		if (c->num_elem == 0)
		{
			c->frente = -1;
			c->final = -1;
		}
	}
	else
	{
		printf("Error:Dequeue(c) subdesbordamiento de cola");
		exit(1);
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
boolean Est_Empty(cola_estatica * c)
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
elemento_estatico Est_Front(cola_estatica * c)
{
	elemento_estatico e;
	
	if(c->num_elem > 0){
	e=c->elementos[c->frente];
	}else
	{
		printf("Error:Front(c) subdesbordamiento de cola");
		exit(1);
	}
	
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
elemento_estatico Est_Final(cola_estatica * c)
{
	elemento_estatico e;
	if(c->num_elem > 0){
	e=c->elementos[c->final];
	}else
	{
		printf("Error:Final(c) subdesbordamiento de cola");
		exit(1);
	}
	
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
int Est_Size(cola_estatica * c)
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
elemento_estatico Est_Element(cola_estatica * c, int i)
{
	elemento_estatico r;
	if(i>0 && i<=c->num_elem)
	{
		r=c->elementos[c->frente+(i-1)%MAX_ELEMENT];	
	}
	else
	{
		printf("Error:Element(c) indice i fuera de rango");
		exit(1);
	}
	return r;
}

/*
void Destroy(cola * c);
Descripción: Recibe una cola y la libera completamente.
Recibe: cola *c (Referencia a la cola "c" a operar)
Devuelve:
Observaciones: La cola se destruye y se inicializa.
*/
void Est_Destroy(cola_estatica * c)
{
	c->num_elem=0;
	c->frente=-1;
	c->final=-1;
	return;
}

/*
int Search(cola *c, elemento e);
Descripción: Recibe una cola y un elemento, busca el elemento
			 en la cola, y retorna su posicion en la cola.
Recibe: cola *c (Referencia a la cola "c" donde buscar)
		elemento e (Elemento a buscar en la cola "c")
Devuelve: Posicion del elemento "e" en la cola "c"
Observaciones: En caso de que el elemento "e" no exista
			   en la cola "c", se retorna el valor 0.
*/
int Est_Search(cola_estatica * c, elemento_estatico e){
	int pos = 0;
	int i, n;
	elemento_estatico aux;
	if(Est_Empty(c)){
		printf("Error: Search(c,e) la pila esta vacia");
		exit(1);
	}
	
	for(i = 1; i <= Est_Size(c); i++){
		aux = Est_Element(c, i);
		if(aux.c == e.c && aux.n == e.n && aux.f == e.f){
			pos = i;
			break;
		}
	}
	
	return pos;
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
void Est_Flip(cola_estatica *c){
	cola_estatica aux_c;
	elemento_estatico aux_e;
	int i, s, n;
	
	Est_Initialize(&aux_c);
	if(Est_Empty(c)){
		printf("Error: Flip(c) la pila esta vacia");
		exit(1);
	}
	
	n = Est_Size(c);
	
	for(i = 1; i <= n; i++){
		aux_e = Est_Dequeue(c);
		Est_Queue(&aux_c, aux_e);
	}
	
	s = Est_Size(&aux_c);
	n = aux_c.frente;
	
	for(i = s; i > n; i--){
		aux_e = Est_Element(&aux_c, i);
		Est_Queue(c, aux_e);
	}
	
	return;
}