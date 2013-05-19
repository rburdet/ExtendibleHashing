//
//  ArbolBmas.h
//	[ Insertar documentacion ]
//


#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H


#include <stack>
#include <list>
#include "fisica_ArchivoBloques.h"
#include "domain_RegistroGenerico.h"



// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint; 




/* ****************************************************************************
 * CONFIGURACION
 * ***************************************************************************/

namespace {

	// Cantidad de registros en nodos
	const int CANT_REG_NODO_INTERNO = 5;
	const int CANT_REG_NODO_HOJA = 6;
}




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


class ArbolBmas
{

public:

	// Constructor
	ArbolBmas();

	// Destructor
	~ArbolBmas();

	// Abre un arbo.
	// PRE: 'nombre_archivo' es la ruta del archivo donde se almacena el arbol.
	// POST: Si existe el archivo, se carga el arbol existente, y si no existe
	// se crea un arbol nuevo en dicho archivo.
	void abrir(const char* nombre_archivo);

	// Inserta un registro nuevo en el árbol
	// PRE: 'clave' es la clave o id con el que se identifica el registro;
	// 'registro' es el registro que se desea ingresar, el cual debe ser un
	// RegistroGenerico
	void insertar(const uint clave, RegistroGenerico& registro);

	// Busca un registro en el arbol
	// PRE: 'clave' es la clave o id que identifica al registro a buscar;
	// 'registro' es una referencia a una almacenador en donde se insertara el
	// resultado de la busqueda
	// POST: Si se encontró el registro, se devuelve true y se almacena en
	// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
	// en 'registro' el registro superior mas proximo al buscado.
	bool buscar(const uint clave, RegistroGenerico& registro);

	// Cierra el archivo si se encuentra abierto
	void cerrar();


private:

	// Estructura con informacion del arbol. 
	// Se utiliza solamente para almacenar o para levantar los metadatos del
	// arbol desde un archivo
	struct Metadata {
		uint raiz;					// Bloque en el que se encuentra la raiz
		uint nivel;					// Contador del nivel actual del árbol
		uint contBloques;			// Contador de bloques existentes
	};


	// Estructura interna que representa un nodo del arbol. Dependiendo del 
	// nivel en el que se encuentra, se comportara como un nodo hoja o como un 
	// nodo interno, es decir, sus metodos realizaran distintas acciones 
	// dependiendo de cual de los dos sea
	struct Nodo
	{
		uint numBloque;							// Numero de bloque del nodo
		uint nivel;								// Nivel en el que se 
												// encuentra el nodo
		uint cantClaves;						// Cantidad actual de claves
		uint cantMaxClaves;						// Cantidad maxima de claves
												// permitidas
		uint nodoHermano;						// Referencia al nodo hermano
												// (se usa si es nodo hoja)
		list< uint > claves;					// Claves de registros
		list< uint > hijos;						// Referencias a hijos (se usa
												// si es un nodo interno)
		list< RegistroGenerico > registros;		// Lista de registros (se usa 
												// si es un nodo hoja)

		// Constructor
		Nodo();

		// Destructor
		~Nodo();

		// Inicializa el nodo.
		// PRE: 'numBloque' es el numero de bloque en el que se encuentra
		// almacenado; 'nivel' es el nivel en el que se encuentra el nodo;
		// 'cantMaxClaves' es la cantidad maxima de claves que puede contener
		// el nodo (sobrepasado este limite se considera overflow).
		void inicializar(uint numBloque, uint nivel, uint cantMaxClaves);

		// Inserta el registro en el nodo.
		// PRE: 'clave' es la clave a insertar; 'registro' es el registro
		// asociado a dicha clave; 'archivo' es un puntero al archivo donde
		// se almacena el arbol; 'contBloques' es el contador de bloques del
		// arbol.
		// POST: devuelve true si queda en overflow o false en caso contrario
		bool insertar(uint& clave, RegistroGenerico& registro, 
			ArchivoBloques *archivo, uint& contBloques);

		// Reparte su contenido con su nodoHermano, pasandole la mitad.
		// PRE: 'nodoHermano' es un nodo con el que se hara la division
		// POST: devuelve la clave del registro inferior de 'nodoHermano'
		uint dividir(Nodo& nodoHermano);

		// Devuelve el numero de bloque en el que se encuentra el nodo
		uint getNumBloque();

		// Se imprime el nodo en la salida estandar con su contenido
		void imprimir(uint nivelDelArbol);
	};


	ArchivoBloques *archivo;				// Archivo donde se almacena
	Nodo raiz;								// Nodo de la raiz
	uint nivel;								// Contador del nivel actual
	uint contBloques;						// Contador de bloques existentes
	stack<Nodo> ramaNodos;					// Almacenador para rama


	// Carga la metadata del arbol desde el archivo.
	// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
	void cargarMetadata();

	// Guarda la metadata actual del arbol en el archivo, actualizando info.
	// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
	void guardarMetadata();
};

#endif
