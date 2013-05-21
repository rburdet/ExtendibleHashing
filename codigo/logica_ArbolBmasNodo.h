//
//  ArbolBmasNodo.h
//	[ Insertar documentacion ]
//


#ifndef ARBOLBMAS_NODO_H
#define ARBOLBMAS_NODO_H


class RegistroGenerico;
class ArchivoBloques;

// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint; 




/* ****************************************************************************
 * DECLARACIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


struct Nodo
{
	uint numBloque;							// Numero de bloque del nodo
	uint nivel;								// Nivel en el que se 
											// encuentra el nodo
	uint cantClaves;						// Cantidad actual de claves
	uint cantMaxClaves;						// Cantidad maxima de claves
											// permitidas

	// Constructor
	Nodo();

	// Destructor
	virtual ~Nodo();

	// Inicializa el nodo.
	// PRE: 'numBloque' es el numero de bloque en el que se encuentra
	// almacenado; 'nivel' es el nivel en el que se encuentra el nodo.
	void inicializar(uint numBloque, uint nivel);

	// Devuelve el numero de bloque en el que se encuentra el nodo
	uint getNumBloque();

	// Inserta el registro en el nodo.
	// PRE: 'clave' es la clave a insertar; 'registro' es el registro
	// asociado a dicha clave; 'archivo' es un puntero al archivo donde
	// se almacena el arbol; 'contBloques' es el contador de bloques del
	// arbol.
	// POST: devuelve true si queda en overflow o false en caso contrario
	virtual bool insertar(uint& clave, RegistroGenerico& registro, 
		ArchivoBloques *archivo, uint& contBloques) = 0;

	// Reparte su contenido con su nodoHermano, pasandole la mitad.
	// PRE: 'nodoHermano' es un nodo con el que se hara la division
	// POST: devuelve la clave del registro inferior de 'nodoHermano'
	virtual uint dividir(Nodo *nodoHermano) = 0;


	// Se imprime el nodo en la salida estandar con su contenido
	virtual void imprimir(uint& nivelDelArbol) = 0;
};

#endif
