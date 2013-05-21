//
//  ArbolBmasNodo.cpp
//	[ Insertar documentacion ]
//


#include "logica_ArbolBmasNodo.h"
#include "domain_RegistroGenerico.h"
#include "fisica_ArchivoBloques.h"



/* ****************************************************************************
 * DEFINICIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


// Constructor
Nodo::Nodo() { }


// Destructor
Nodo::~Nodo() 
{

}


// Inicializa el nodo.
// PRE: 'numBloque' es el numero de bloque en el que se encuentra
// almacenado; 'nivel' es el nivel en el que se encuentra el nodo.
void Nodo::inicializar(uint numBloque, uint nivel)
{
	this->numBloque = numBloque;
	this->nivel = nivel;
	this->cantClaves = 0;
}


// Devuelve el numero de bloque en el que se encuentra el nodo
uint Nodo::getNumBloque()
{
	return this->numBloque;
}
