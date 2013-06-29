//
//  logica_ArbolBmas.h
//	ARBOL B+
//	***************************************************************************
//
//	Implementacion de un Arbol B+. En este se almacenan registros que deben
//	ser estrictamente objetos que hereden de la clase RegistroGenerico. Esto
//	obliga al usuario a implementar los metodos de serializacion y
//	deserializacion, lo cual sera utilizado para la persistencia del arbol.
//	Ademas, la clave que debe especificarse en ciertos metodos es aquella
//	que se encuentra ligada al registro, siendo esta almacenada dentro del
//	mismo RegistroGenerico.
//
//
//	CONFIGURACION
//
//	Es configurable el tamanio del bloque, el cual representa a un nodo
//	del arbol (ya sea interno u hoja). De acuerdo al valor especificado
//	se calcularan la cantidad maxima de registros que pueden insertarse
//	en cada nodo hoja y nodo interno. Tengase en cuenta que al modificar
//	el tamanio del bloque, debe modificarse el tamanio del buffer.
//	Es configurable el bloque en el que se debe guardar la metadata, pero
//	siguiendo la restriccion de que el bloque de metadata siempre debe
//	encontrarse por debajo del numero de bloque en el que se encuentra la
//	raiz inicial. Esto se debe a cuestiones de disenio e implementacion.
//


#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint; 


/* ****************************************************************************
 * CONFIGURACION
 * ***************************************************************************/

namespace {

	// Constantes para los numeros de bloque
	const uint NUM_BLOQUE_METADATA = 0;
	const uint NUM_BLOQUE_RAIZ_INICIAL = 1;
}




/* ****************************************************************************
 * INCLUDES
 * ***************************************************************************/


#include "logica_ArbolBmas.h"
#include "fisica_SerialBuffer.h"
#include "fisica_ArchivoBloques.h"
#include "logica_ArbolBmasNodo.h"
#include "logica_ArbolBmasNodoHoja.h"
#include "logica_ArbolBmasNodoInterno.h"
#include "runtimeConfig.h"




/* ****************************************************************************
 * DECLARACIÓN DE LA CLASE
 * ***************************************************************************/


template < typename Tipo >
class ArbolBmas
{

private:

	ArchivoBloques *archivo;		// Archivo donde se almacena
	SerialBuffer *buffer;			// Buffer utilizado para serializacion
	Nodo< Tipo > *raiz;				// Nodo de la raiz
	uint nivel;						// Contador del nivel actual
	uint contBloques;				// Contador de bloques existentes
	bool estaAbierto;				// Flag para sensar si se abrio el arbol

	size_t MAX_HOJA;				// Cantidad maxima de registros en hoja
	size_t MAX_INTERNO;				// Cantidad maxima de registros en nodo
									// interno


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

	// Abre un arbol.
	// PRE: 'nombre_archivo' es la ruta del archivo donde se almacena el arbol.
	// POST: Si existe el archivo, se carga el arbol existente, y si no existe
	// se crea un arbol nuevo en dicho archivo.
	void abrir(const char* nombre_archivo);

	// Cierra el arbol
	// POST: se guardo en el archivo con el que fue abierto la informacion
	// actual del arbol.
	void cerrar();

	// Inserta un registro nuevo en el árbol
	// PRE: 'clave' es la clave o id con el que se identifica el registro;
	// 'registro' es el registro que se desea ingresar, el cual debe ser un
	// RegistroGenerico
	void insertar(uint clave, Tipo & registro);

	// Busca un registro en el arbol
	// PRE: 'clave' es la clave o id que identifica al registro a buscar;
	// 'registro' es un puntero a un almacenador en donde se insertara el
	// resultado de la busqueda
	// POST: Si se encontró el registro, se devuelve true y se almacena en
	// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
	// en 'registro' el registro superior mas proximo al buscado.
	bool buscar(const uint clave, Tipo & registro);

	// Actualiza un registro existente en el arbol.
	// PRE: 'clave' es la clave o id que identifica al registro a actualizar;
	// 'registro' es una referencia al nuevo registro que sera almacenado en
	// lugar del existente
	// POST: se reemplazo el registro de clave 'clave' por el pasado por
	// parametro.
	bool actualizar(const uint clave, Tipo & registro);

	// Elimina un arbol por completo.
	// POST: se borro el archivo almacenado en disco con los datos del arbol. 
	void eliminar();

	// Imprime el arbol sobre la salida estandar. Las tabulaciones indican
	// el nivel, siendo el nivel cero aquel que se encuentra con la mayor
	// tabulacion.
	void imprimir();
};





/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
template < typename Tipo >
ArbolBmas< Tipo >::ArbolBmas() : nivel(0), 
	contBloques(NUM_BLOQUE_RAIZ_INICIAL), estaAbierto(false)
{
	// Creamos el buffer
	this->buffer = new SerialBuffer(arbolBlockSize());

	// Calculamos la cantidad maxima de registros por bloques en nodos hoja y
	// en nodos internos del arbol
	this->MAX_INTERNO = ((arbolBlockSize() - 30) / (12)) - 2;
	this->MAX_HOJA = ((arbolBlockSize() - 36) / (6+Tipo::getTamanioEnBytes()))-1;
}


// Destructor
template < typename Tipo >
ArbolBmas< Tipo >::~ArbolBmas() 
{
	// Liberamos la memoria utilizada para el buffer de serializacion
	delete this->buffer;
	
	// Si no esta abierto no hay mas nada que liberar
	if(!this->estaAbierto) return;

	// Liberamos la memoria utilizada por el archivo
	delete this->archivo;
	// Liberamos la memoria utilizada para mantener la raiz en memoria
	delete this->raiz;
}


// Abre un arbol.
// PRE: 'nombre_archivo' es la ruta del archivo donde se almacena el arbol.
// POST: Si existe el archivo, se carga el arbol existente, y si no existe
// se crea un arbol nuevo en dicho archivo.
template < typename Tipo >
void ArbolBmas< Tipo >::abrir(const char* nombre_archivo)
{
	// Comprobamos que no se encuentre ya abierto
	if(this->estaAbierto) return;

	this->estaAbierto = true;

	// Creamos un archivo de bloques
	this->archivo = new ArchivoBloques(arbolBlockSize(), nombre_archivo);

	// Inicializamos el archivo de bloques o lo levantamos si ya existia
	if(!this->archivo->existe())
	{
		// Seteamos valores iniciales
		this->nivel = 0;
		this->contBloques = NUM_BLOQUE_RAIZ_INICIAL;

		// Creamos el nodo raiz
		this->raiz = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
		this->raiz->inicializar(NUM_BLOQUE_RAIZ_INICIAL, 0);

		// Guardamos metadata del arbol con valores iniciales
		guardarMetadata();

		// Guardamos la raiz inicial
		this->raiz->guardar(this->archivo);

		return;
	}
	
	// Cargamos metadata
	cargarMetadata();
}


// Cierra el arbol
// POST: se guardo en el archivo con el que fue abierto la informacion
// actual del arbol.
template < typename Tipo >
void ArbolBmas< Tipo >::cerrar()
{
	if(!this->estaAbierto) return;
	this->guardarMetadata();
	this->estaAbierto = false;
	delete this->raiz;
	delete this->archivo;
}


// Inserta un registro nuevo en el árbol
// PRE: 'clave' es la clave o id con el que se identifica el registro;
// 'registro' es el registro que se desea ingresar, el cual debe ser un
// RegistroGenerico
template < typename Tipo >
void ArbolBmas< Tipo >::insertar(uint clave, Tipo & registro)
{
	// Corroboramos que se haya creado el arbol
	if(!this->archivo)
		throw "ArbolBmas::insertar() ERROR: no se ha abierto el arbol";

	// Insertamos. Si no hubo overflow, retornamos
	if(!this->raiz->insertar(clave, registro, this->archivo, 
		this->contBloques))
	{
		// Caso en que la raiz es un nodo hoja, la cual debe guardarse.
		if(this->nivel == 0) this->raiz->guardar(this->archivo);
		return;
	}

	// Hubo overflow, pasamos a partir la raiz y generar una nueva raiz.
	Nodo< Tipo > *nuevoHermano;

	if(this->nivel == 0)
		nuevoHermano = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
	else
		nuevoHermano = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	NodoInterno< Tipo > *nuevaRaiz
	 = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	// Inicializamos al nuevo hermano
	this->contBloques++;
	nuevoHermano->inicializar(this->contBloques, this->nivel);

	// Inicializamos a la nueva raiz
	this->contBloques++;
	this->nivel++;
	nuevaRaiz->inicializar(this->contBloques, this->nivel);

	// Dividimos al nodo
	uint claveQueSube = this->raiz->dividir(nuevoHermano);

	// Insertamos clave e hijos en la nueva raiz
	nuevaRaiz->insertarHijosIniciales(claveQueSube, 
		this->raiz->getNumBloque(), nuevoHermano->getNumBloque());

	// Escribimos los nodos
	nuevaRaiz->guardar(this->archivo);
	nuevoHermano->guardar(this->archivo);
	this->raiz->guardar(this->archivo);

	delete this->raiz;
	delete nuevoHermano;
	
	// Guardamos en el arbol la nueva raiz
	this->raiz = nuevaRaiz;
}


// Busca un registro en el arbol
// PRE: 'clave' es la clave o id que identifica al registro a buscar;
// 'registro' es un puntero a un almacenador en donde se insertara el
// resultado de la busqueda
// POST: Si se encontró el registro, se devuelve true y se almacena en
// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
// en 'registro' el registro superior mas proximo al buscado.
template < typename Tipo >
bool ArbolBmas< Tipo >::buscar(const uint clave, Tipo & registro)
{
	// Corroboramos que se haya creado el arbol
	if(!this->archivo)
		throw "ArbolBmas::buscar() ERROR: no se ha abierto el arbol";

	// Buscamos a partir del nodo raiz y devolvemos resultado
	return this->raiz->buscar(clave, registro, this->archivo);
}


// Actualiza un registro existente en el arbol.
// PRE: 'clave' es la clave o id que identifica al registro a actualizar;
// 'registro' es una referencia al nuevo registro que sera almacenado en
// lugar del existente
// POST: se reemplazo el registro de clave 'clave' por el pasado por
// parametro. Si no se encuentra la clave, no se realiza ninguna actualizacion
// y se devuelve false. En caso de exito se devuelve true.
template < typename Tipo >
bool ArbolBmas< Tipo >::actualizar(const uint clave, Tipo & registro)
{
	// Corroboramos que se haya creado el arbol
	if(!this->archivo)
		throw "ArbolBmas::actualizar() ERROR: no se ha abierto el arbol";

	// Damos la orden de actualizar a partir de la raiz y devolvemos resultado.
	return this->raiz->actualizar(clave, registro, this->archivo);
}


// Elimina un arbol por completo.
// POST: se borro el archivo almacenado en disco con los datos del arbol.
template < typename Tipo > 
void ArbolBmas< Tipo >::eliminar()
{
	this->estaAbierto = false;
	this->archivo->borrarArchivo();
	delete this->raiz;
	delete this->archivo;
}


// Imprime el arbol sobre la salida estandar. Las tabulaciones indican
// el nivel, siendo el nivel cero aquel que se encuentra con la mayor
// tabulacion.
template < typename Tipo >
void ArbolBmas< Tipo >::imprimir()
{
	this->raiz->imprimir(this->nivel, this->archivo);
}




/**********************************
 *  METODOS PRIVADOS
 **********************************/


// Carga la metadata del arbol desde el archivo.
// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
template < typename Tipo >
void ArbolBmas< Tipo >::cargarMetadata()
{
	// Limpiamoss buffer
	this->buffer->clear();

	// Levantamos la metadata del archivo
	this->archivo->leerBloque(this->buffer->getBuffer(), NUM_BLOQUE_METADATA);
	// Variables auxiliares para deserializacion
	uint nivel;
	uint contBloques;
	uint bloqueRaiz;

	// Deserializamos metadata del arbol
	this->buffer->unpack(&nivel);
	this->buffer->unpack(&contBloques);
	this->buffer->unpack(&bloqueRaiz);

	// Cargamos datos en atributos
	this->nivel = nivel;
	this->contBloques = contBloques;

	// Cargamos la raiz
	// Si el nivel es cero, la raiz es un nodo hoja, sino es nodo interno
	if(this->nivel == 0)
		this->raiz = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
	else
		this->raiz = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	// Seteamos el numero de bloque de la raiz y cargamos la raiz
	this->raiz->setNumBloque(bloqueRaiz);
	this->raiz->cargar(this->archivo);
}


// Guarda la metadata actual del arbol en el archivo, actualizando info.
// POST: si todavia no ha sido abierto o creado el archivo, no hace nada.
template < typename Tipo >
void ArbolBmas< Tipo >::guardarMetadata()
{
	uint nivel = this->nivel;
	uint contBloques = this->contBloques;
	uint bloqueRaiz = this->raiz->getNumBloque();

	// Limpiamoss buffer
	this->buffer->clear();

	// Serializamos metadata del arbol
	this->buffer->pack(&nivel, sizeof(uint));
	this->buffer->pack(&contBloques, sizeof(uint));
	this->buffer->pack(&bloqueRaiz, sizeof(uint));

	// Escribimos metadata en archivo
	this->archivo->escribirBloque(this->buffer->getBuffer(), 
		NUM_BLOQUE_METADATA);
}


#endif
