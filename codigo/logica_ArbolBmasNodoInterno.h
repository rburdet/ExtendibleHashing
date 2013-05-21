//
//  ArbolBmasNodoHoja.h
//	[ Insertar documentacion ]
//


#ifndef ARBOLBMAS_NODO_INTERNO_H
#define ARBOLBMAS_NODO_INTERNO_H


#include "fisica_ArchivoBloques.h"
#include "logica_ArbolBmasNodo.h"
#include "logica_ArbolBmasNodoInterno.h"
#include "logica_ArbolBmasNodoHoja.h"
#include "lib_ListaFija.h"


// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint;




/* ****************************************************************************
 * DECLARACIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


template < size_t MAX_HOJA, size_t MAX_INTERNO >
struct NodoInterno : public Nodo
{
	// Claves de registros
	ListaFija< uint, MAX_INTERNO + 1 > claves;

	// Referencias a hijos (se usa si es un nodo interno)
	ListaFija< uint, MAX_INTERNO + 2 > hijos;


	// Constructor
	NodoInterno();

	// Destructor
	~NodoInterno();	

	// Inserta el registro en el nodo.
	// PRE: 'clave' es la clave a insertar; 'registro' es el registro
	// asociado a dicha clave; 'archivo' es un puntero al archivo donde
	// se almacena el arbol; 'contBloques' es el contador de bloques del
	// arbol.
	// POST: devuelve true si queda en overflow o false en caso contrario
	virtual bool insertar(uint& clave, RegistroGenerico& registro, 
		ArchivoBloques *archivo, uint& contBloques);

	// Reparte su contenido con su nodoHermano, pasandole la mitad.
	// PRE: 'nodoHermano' es un nodo con el que se hara la division
	// POST: devuelve la clave del registro inferior de 'nodoHermano'
	virtual uint dividir(Nodo *nodoHermano);


	// Se imprime el nodo en la salida estandar con su contenido
	// FORMATO: "[nivel], [numero_bloque]: [hijo1]([clave1])..[hijoN]([claveN])
	// [hijoN+1] 
	virtual void imprimir(uint& nivelDelArbol);
};




/* ****************************************************************************
 * DEFINICIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


// Constructor
template < size_t MAX_HOJA, size_t MAX_INTERNO >
NodoInterno< MAX_HOJA, MAX_INTERNO >::NodoInterno()
{
	this->cantMaxClaves = MAX_INTERNO;
}


// Destructor
template < size_t MAX_HOJA, size_t MAX_INTERNO >
NodoInterno< MAX_HOJA, MAX_INTERNO >::~NodoInterno() 
{

}

// Inserta el registro en el nodo.
// PRE: 'clave' es la clave a insertar; 'registro' es el registro
// asociado a dicha clave; 'archivo' es un puntero al archivo donde
// se almacena el arbol; 'contBloques' es el contador de bloques del
// arbol.
// POST: devuelve true si queda en overflow o false en caso contrario
template < size_t MAX_HOJA, size_t MAX_INTERNO >
bool NodoInterno< MAX_HOJA, MAX_INTERNO >::insertar(uint& clave, 
	RegistroGenerico& registro, ArchivoBloques *archivo, uint& contBloques)
{
	Nodo *nodo;

	if(this->nivel == 1)
		nodo = new NodoHoja< MAX_HOJA, MAX_INTERNO >;
	else
		nodo = new NodoInterno< MAX_INTERNO, MAX_INTERNO >;

	// Iteramos sobre los hijos
	for(size_t i = 0; i < this->hijos.tamanio(); i++)
	{
		// Si llegamos al hijo final insertamos allí, pero si se encuentra
		// una clave mayor a la que se desea insertar, insertamos en el 
		// hijo izquierdo de la primer clave mayor que aparezca
		if(i == (this->hijos.tamanio()-1) || clave < this->claves[i])
		{
			// Cargamos el nodo en memoria
			archivo->leerBloque(nodo, this->hijos[i]);

			// Insertamos la clave y el registro en el nodo, y verificamos
			// si se produjo overflow en este, en cual caso, accionaremos
			if(nodo->insertar(clave, registro, archivo, contBloques))
			{
				/* Se produjo overflow en el nodo */

				Nodo *nodoHermano;

				// Creamos nodo hermano
				if(this->nivel == 1)
					nodoHermano = new NodoHoja< MAX_HOJA, MAX_INTERNO >;
				else
					nodoHermano = new NodoInterno< MAX_HOJA, MAX_INTERNO >;
				
				// Inicializamos los datos del nodo hermano
				nodoHermano->inicializar(contBloques, nodo->nivel);

				// Incrementamos el contador de bloques del arbol
				contBloques++;

				// Dividimos el contenido entre nodo y nodo hermano e
				// insertamos la clave retornada a la lista de claves
				this->claves.insertar(nodo->dividir(nodoHermano), i);

				// Agregamos el numero de bloque del nodo hermano a la lista
				// de hijos
				this->hijos.insertar(nodoHermano->getNumBloque(), i);

				// Escribimos los nodos en el archivo
				archivo->escribirBloque(nodo, nodo->getNumBloque(), 
					sizeof(*nodo));
				archivo->escribirBloque(nodoHermano, 
					nodoHermano->getNumBloque(), sizeof(*nodoHermano));

				delete nodo;
				delete nodoHermano;

				// Verificamos si hay overblow y devolvemos true o false
				if(this->cantClaves > this->cantMaxClaves) return true;
				return false;
			}

			// Escribimos el nodo en el archivo
			archivo->escribirBloque(nodo, nodo->getNumBloque(), 
				sizeof(*nodo));

			break;
		}
	}

	delete nodo;
	return false;
}


// Reparte su contenido con su nodoHermano, pasandole la mitad.
// PRE: 'nodoHermano' es un nodo con el que se hara la division
// POST: devuelve la clave del registro inferior de 'nodoHermano'
template < size_t MAX_HOJA, size_t MAX_INTERNO >
uint NodoInterno< MAX_HOJA, MAX_INTERNO >::dividir(Nodo *nodoHermano)
{
	// Casteamos para poder tratarlo como nodo hoja
	NodoInterno *nodoInternoHermano = (NodoInterno< MAX_HOJA, MAX_INTERNO >*)
		nodoHermano;

	// Calculamos posicion de corte
	int pos_corte = (this->claves.tamanio() / 2) + 1;

	// Transferimos elementos de la segunda mitad hacia las listas del
	// nodo hermano
	this->claves.transferir(nodoInternoHermano->claves, pos_corte, 
		this->claves.tamanio() - 1);
	this->hijos.transferir(nodoInternoHermano->hijos, pos_corte,
		this->hijos.tamanio() - 1);

	// Tomamos la clave que subira hacia el nodo padre y la borramos de la
	// lista de claves del nodo actual
	uint clave_subir = this->claves.verUltimo();
	this->claves.eliminarUltimo();

	// Actualizamos cantidad de claves en nodos
	this->cantClaves = this->claves.tamanio();
	nodoInternoHermano->cantClaves = nodoInternoHermano->claves.tamanio();

	return clave_subir;
}


// Se imprime el nodo en la salida estandar con su contenido
// FORMATO: "[nivel], [numero_bloque]: [hijo1]([clave1])...[hijoN]([claveN])
// [hijoN+1] 
template < size_t MAX_HOJA, size_t MAX_INTERNO >
void NodoInterno< MAX_HOJA, MAX_INTERNO >::imprimir(uint& nivelDelArbol)
{
	// Tabulamos de acuerdo al nivel
	int tabs = nivelDelArbol - this->nivel;
	std::cout << std::string(tabs, '\t');
	std::cout << this->nivel << ", " << this->numBloque << ": ";

	// Iteramos sobre las claves
	for(size_t i = 0; i < this->claves.tamanio(); i++)
	{
		std::cout << "(" << this->claves[i] << ")";
		std::cout << this->hijos[i] << "(" << this->claves[i] << ")";
	}

	std::cout << this->hijos[this->hijos.tamanio() - 1] << std::endl; 
}

#endif
