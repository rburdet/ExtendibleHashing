//
//  ArbolBmas.cpp
//	[ Insertar documentacion ]
//



#include "logica_ArbolBmas.h"



/* ****************************************************************************
 * CONSTANTES
 * ***************************************************************************/

namespace {

	// Constantes para los numeros de bloque
	const uint NUM_BLOQUE_METADATA = 1;
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ArbolBmas::ArbolBmas() : raiz(0,0,0), nivel(0), contBloques(0) { }


// Destructor
ArbolBmas::~ArbolBmas() 
{
	cerrar();
	delete this->archivo;
}


// Abre un arbol ya existente
// PRE: 'nombre_archivo' es la ruta del archivo donde se almacena el arbol.
// POST: si no existe el archivo se crea uno nuevo y se inicializa el arbol,
void ArbolBmas::abrir(const char* nombre_archivo)
{
	// Creamos un archivo de bloques
	this->archivo = new ArchivoBloques(512, nombre_archivo);

	// Inicializamos el archivo de bloques o lo levantamos si ya existia
	if(this->archivo->abrirArchivo() == -1)
	{
		// El archivo no existe, lo creamos
		this->archivo->crearArchivo();
		this->archivo->abrirArchivo();

		// Creamos metadata del arbol con valores iniciales
		guardarMetadata();

		// // Creamos el nodo raiz
		// this->raiz = new Nodo(this->nivel, NUM_BLOQUE_RAIZ);
		// this->archivo->escribirBloque(&this->raiz, NUM_BLOQUE_RAIZ, sizeof(*this->raiz));

		return;
	}
	
	// Cargamos metadata
	cargarMetadata();

	std::cout << this->nivel << " " << this->contBloques << std::endl;

	// // Cargamos nodo raiz
	// this->archivo->leerBloque(&this->raiz, NUM_BLOQUE_RAIZ);

	// // Apilamos puntero a nodo raiz para comenzar rama
	// this->ramaNodos.push(this->raiz);
}


// Inserta un registro nuevo en el árbol
// PRE: 'clave' es la clave o id con el que se identifica el registro;
// 'registro' es el registro que se desea ingresar, el cual debe ser un
// RegistroGenerico
void ArbolBmas::insertar(const uint clave, RegistroGenerico& registro)
{
	// Corroboramos que se haya creado el arbol
	if(!this->archivo)
		throw "ArbolBmas::insertar() ERROR: no se ha abierto el arbol";
}


// Busca un registro en el arbol
// PRE: 'clave' es la clave o id que identifica al registro a buscar;
// 'registro' es una referencia a una almacenador en donde se insertara el
// resultado de la busqueda
// POST: Si se encontró el registro, se devuelve true y se almacena en
// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
// en 'registro' el registro superior mas proximo al buscado.
bool ArbolBmas::buscar(const uint clave, RegistroGenerico& registro)
{
	// Corroboramos que se haya creado el arbol
	if(!this->archivo)
		throw "ArbolBmas::buscar() ERROR: no se ha abierto el arbol";

	return false;
}


// Cierra el archivo si se encuentra abierto
void ArbolBmas::cerrar()
{
	// Corroboramos que esté abierto el archivo
	if(this->archivo->estaAbierto())
		this->archivo->cerrarArchivo();

}




/**********************************
 *  METODOS PRIVADOS
 **********************************/


// Carga la metadata del arbol desde el archivo.
// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
void ArbolBmas::cargarMetadata()
{
	// Corroboramos que esté abierto el archivo
	if(!this->archivo->estaAbierto()) return;

	// Levantamos la metadata del archivo
	Metadata metadata;
	this->archivo->leerBloque(&metadata, NUM_BLOQUE_METADATA);

	// Cargamos datos en atributos
	this->nivel = metadata.nivel;
	this->contBloques = metadata.contBloques;
	this->archivo->leerBloque(&this->raiz, metadata.raiz);
}


// Guarda la metadata actual del arbol en el archivo, actualizando info.
// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
void ArbolBmas::guardarMetadata()
{
	// Corroboramos que esté abierto el archivo
	if(!this->archivo->estaAbierto()) return;

	// Creamos la metadata a almacenar
	Metadata metadata;
	metadata.raiz = this->raiz.getNumBloque();
	metadata.nivel = this->nivel;
	metadata.contBloques = this->contBloques;

	// Escribimos metadata en archivo
	this->archivo->escribirBloque(&metadata, NUM_BLOQUE_METADATA, 
		sizeof(Metadata));
}




/*************************************
 *  ESTRUCTURAS INTERNAS DE LA CLASE
 *************************************/


// Constructor Nodo
ArbolBmas::Nodo::Nodo(uint numBloque, uint nivel, uint cantMaxClaves) : 
	numBloque(numBloque), nivel(nivel), cantMaxClaves(cantMaxClaves) { }


// Inserta el registro en el nodo.
// PRE: 'clave' es la clave a insertar; 'registro' es el registro
// asociado a dicha clave.
// POST: devuelve true si queda en overflow o false en caso contrario
bool ArbolBmas::Nodo::insertar(uint clave, RegistroGenerico registro)
{
	return true;
}

// Reparte su contenido con su nodoHermano, pasandole la mitad.
// PRE: 'nodoHermano' es un nodo con el que se hara la division
// POST: devuelve la clave del registro inferior de 'nodoHermano'
uint ArbolBmas::Nodo::dividir(Nodo& nodoHermano)
{
	return 0;
}


// Devuelve el numero de bloque en el que se encuentra el nodo
uint ArbolBmas::Nodo::getNumBloque()
{
	return this->numBloque;
}
