//
//  ArbolBmas.h
//	[ Insertar documentacion ]
//


#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H


#include <stack>
#include "fisica_ArchivoBloques.h"
#include "logica_ArbolBmasNodo.h"
#include "logica_ArbolBmasNodoHoja.h"
#include "logica_ArbolBmasNodoInterno.h"
#include "domain_RegistroGenerico.h"
#include "lib_ListaFija.h"



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

private:

	// Estructura con informacion del arbol. 
	// Se utiliza solamente para almacenar o para levantar los metadatos del
	// arbol desde un archivo
	struct Metadata {
		uint raiz;					// Bloque en el que se encuentra la raiz
		uint nivel;					// Contador del nivel actual del árbol
		uint contBloques;			// Contador de bloques existentes
	};


	ArchivoBloques *archivo;				// Archivo donde se almacena
	Nodo *raiz;								// Nodo de la raiz
	uint nivel;								// Contador del nivel actual
	uint contBloques;						// Contador de bloques existentes


	// Carga la metadata del arbol desde el archivo.
	// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
	void cargarMetadata();

	// Guarda la metadata actual del arbol en el archivo, actualizando info.
	// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
	void guardarMetadata();


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
};

#endif
