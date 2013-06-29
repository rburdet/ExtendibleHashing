//
//  logica_ArbolBmasNodoHoja.h
//	NODO HOJA - ARBOL B+
//	***************************************************************************
//
//	Implementacion de la estructura del Nodo Hoja utilizado en el Arbol B+.
//	Debe considerarse esta estructura como privada del arbol, es decir, se
//	prohibe su uso fuera del ambito de la clase del ArbolBmas.
//


#ifndef ARBOLBMAS_NODO_HOJA_H
#define ARBOLBMAS_NODO_HOJA_H


#include "fisica_ArchivoBloques.h"
#include "logica_ArbolBmasNodo.h"
#include "lib_ListaFija.h"


// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint; 




/* ****************************************************************************
 * DECLARACIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


template < typename Tipo >
struct NodoHoja : public Nodo< Tipo >
{
	ListaFija< uint > *claves;		// Claves de registros
	ListaFija< Tipo > *registros;	// Lista de registros
	uint nodoHermano;				// Referencia al nodo hermano
	size_t MAX_HOJA;				// Cantidad maxima de registros en hoja
	size_t MAX_INTERNO;				// Cantidad maxima de registros en nodo
									// interno

	// Constructor
	NodoHoja(size_t max_hoja, size_t max_interno);

	// Destructor
	~NodoHoja();

	// Inserta el registro en el nodo.
	// PRE: 'clave' es la clave a insertar; 'registro' es el registro
	// asociado a dicha clave; 'archivo' es un puntero al archivo donde
	// se almacena el arbol; 'contBloques' es el contador de bloques del
	// arbol.
	// POST: devuelve true si queda en overflow o false en caso contrario
	virtual bool insertar(uint& clave, Tipo & registro, 
		ArchivoBloques *archivo, uint& contBloques);

	// Busca un registro.
	// PRE: 'clave' es la clave o id asociado al registro a buscar; 'registro'
	// es un puntero a un almacenador en donde se insertara el resultado de la
	// busqueda.
	// POST: Si se encontró el registro, se devuelve true y se almacena en
	// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
	// en 'registro' el registro superior mas proximo al buscado.
	virtual bool buscar(const uint clave, Tipo & registro, 
		ArchivoBloques *archivo);

	// Actualiza un registro existente.
	// PRE: 'clave' es la clave o id que identifica al registro a actualizar;
	// 'registro' es una referencia al nuevo registro que sera almacenado en
	// lugar del existente
	// POST: se reemplazo el registro de clave 'clave' por el pasado por
	// parametro. Si no se encuentra la clave, no se realiza ninguna 
	// actualizacion y se devuelve false. En caso de exito se devuelve true.
	virtual bool actualizar(const uint clave, Tipo & registro, 
		ArchivoBloques *archivo);

	// Obtiene el menor de los registros contenido en el nodo.
	// PRE: 'registro' es donde se almacena el registro menor.
	// POST: si se encuentra vacio el nodo se lanza una excepcion.
	void obtenerMenorRegistro(Tipo & registro);

	// Reparte su contenido con su nodoHermano, pasandole la mitad.
	// PRE: 'nodoHermano' es un nodo con el que se hara la division
	// POST: devuelve la clave del registro inferior de 'nodoHermano'
	virtual uint dividir(Nodo< Tipo > *nodoHermano);

	// Carga el nodo desde un archivo.
	// PRE: 'archivo' es donde se almacena el nodo que se desea cargar, el
	// cual se encuentra en el numero de bloque con el que fue inicializado.
	// POST: se han cargado todos los atributos internos del nodo
	virtual void cargar(ArchivoBloques *archivo);

	// Guarda el nodo en un archivo.
	// PRE: 'archivo' es donde se almacenaran los datos del nodo, los cuales
	// se guardaran en el numero de bloque con el cual fue inicializado.
	// POST: se guardo el estado del nodo en el archivo.
	virtual void guardar(ArchivoBloques *archivo);

	// Se imprime el nodo en la salida estandar con su contenido.
	// FORMATO: "[nivel], [numero_bloque]: ([clave1])..([claveN])[nodo_hermano]
	virtual void imprimir(uint& nivelDelArbol, ArchivoBloques *archivo);
};





/* ****************************************************************************
 * DEFINICIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


// Constructor
template < typename Tipo >
NodoHoja< Tipo >::NodoHoja(size_t max_hoja, size_t max_interno) : 
	MAX_HOJA(max_hoja), MAX_INTERNO(max_interno)
{
	this->cantMaxClaves = MAX_HOJA;
	this->nodoHermano = 0;
	this->claves = new ListaFija< uint >(MAX_HOJA + 1);
	this->registros = new ListaFija< Tipo >(MAX_HOJA + 1);
}


// Destructo
template < typename Tipo >
NodoHoja< Tipo >::~NodoHoja() 
{
	delete this->claves;
	delete this->registros;
}


// Inserta el registro en el nodo.
// PRE: 'clave' es la clave a insertar; 'registro' es el registro
// asociado a dicha clave; 'archivo' es un puntero al archivo donde
// se almacena el arbol; 'contBloques' es el contador de bloques del
// arbol.
// POST: devuelve true si queda en overflow o false en caso contrario
template < typename Tipo >
bool NodoHoja< Tipo >::insertar(uint& clave, 
	Tipo & registro, ArchivoBloques *archivo, uint& contBloques)
{
	for(size_t i = 0; i < this->claves->tamanio()+1; i++)
	{
		// Si esta vacia insertamos directamente
		if(this->claves->estaVacia())
		{
			this->claves->insertarUltimo(clave);
			this->registros->insertarUltimo(registro);
			break;
		}
		// Comprobamos que no hayan claves iguales
		else if((*this->claves)[i] == clave)
			throw "ERROR: Claves iguales en arbol.";
		// Si la clave es mas grande que el actual, insertamos en ese lugar
		else if((*this->claves)[i] > clave)
		{
			this->claves->insertar(clave, i);
			this->registros->insertar(registro, i);
			break;
		}
		// Si llegamos a la ultima posicion y no hubo elemento mayor,
		// insertamos clave y registro al final de las listas.
		else if(i == (this->claves->tamanio() - 1))
		{
			this->claves->insertarUltimo(clave);
			this->registros->insertarUltimo(registro);
			break;
		}
	}

	// Verificamos si entro en overflow y devolvemos de acuerdo a esto
	if(this->claves->tamanio() > this->cantMaxClaves) return true;
	return false;
}


// Busca un registro.
// PRE: 'clave' es la clave o id asociado al registro a buscar; 'registro'
// es un puntero a un almacenador en donde se insertara el resultado de la
// busqueda.
// POST: Si se encontró el registro, se devuelve true y se almacena en
// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
// en 'registro' el registro superior mas proximo al buscado.
template < typename Tipo >
bool NodoHoja< Tipo >::buscar(const uint clave, Tipo & registro, 
	ArchivoBloques *archivo)
{
	// Iteramos sobre las claves del nodo
	for(size_t i = 0; i < this->claves->tamanio(); i++)
	{
		// Caso en que esta vacia la lista de claves
		if(this->claves->estaVacia())
			return false;

		// Caso en que coincide la clave buscada con una clave del nodo
		else if((*this->claves)[i] == clave)
		{
			// Devolvemos la clave asociada al registro.
			registro = (*this->registros)[i];
			return true;
		}
		else if((*this->claves)[i] > clave)
		{
			registro = (*this->registros)[i];
			return false;
		}
	}

	// Si no tiene hermano derecho, devolvemos false sin cargar registro
	if(this->nodoHermano == 0) return false;

	// Cargamos nodo hermano y devolvemos el primero de sus registros
	NodoHoja< Tipo > nodoHermano(MAX_HOJA, MAX_INTERNO);
	nodoHermano.setNumBloque(this->nodoHermano);
	nodoHermano.cargar(archivo);
	nodoHermano.obtenerMenorRegistro(registro);

	// Retornamos false por no haber encontrado el registro buscado
	return false;
}


// Actualiza un registro existente.
// PRE: 'clave' es la clave o id que identifica al registro a actualizar;
// 'registro' es una referencia al nuevo registro que sera almacenado en
// lugar del existente
// POST: se reemplazo el registro de clave 'clave' por el pasado por
// parametro. Si no se encuentra la clave, no se realiza ninguna 
// actualizacion y se devuelve false. En caso de exito se devuelve true.
template < typename Tipo >
bool NodoHoja< Tipo >::actualizar(const uint clave, Tipo & registro, 
	ArchivoBloques *archivo)
{
	// Caso en que esta vacia la lista de claves
	if(this->claves->estaVacia())
		return false;

	// Iteramos sobre las claves del nodo
	for(size_t i = 0; i < this->claves->tamanio(); i++)
	{
		// Caso en que coincide la clave buscada con una clave del nodo
		if((*this->claves)[i] == clave)
		{
			// Actualizamos el registro en la lista y guardamos cambios
			this->registros->reemplazar(registro, i);
			this->guardar(archivo);
			return true;
		}
	}

	return false;
}


// Obtiene el menor de los registros contenido en el nodo.
// PRE: 'registro' es donde se almacena el registro menor.
// POST: si se encuentra vacio el nodo se lanza una excepcion.
template < typename Tipo >
void NodoHoja< Tipo >::obtenerMenorRegistro(
	Tipo & registro)
{
	// Corroboramos que no este vacia
	if(this->registros->estaVacia())
		throw "ERROR: El nodo no contiene elementos.";

	registro = (*this->registros)[0];
}


// Reparte su contenido con su nodoHermano, pasandole la mitad.
// PRE: 'nodoHermano' es un nodo con el que se hara la division
// POST: devuelve la clave del registro inferior de 'nodoHermano'
template < typename Tipo >
uint NodoHoja< Tipo >::dividir(
	Nodo< Tipo > *nodoHermano)
{
	// Casteamos para poder tratarlo como nodo hoja
	NodoHoja *nodoHojaHermano = (NodoHoja< Tipo >*)
		nodoHermano;
		
	// Calculamos posicion de corte
	int pos_corte = this->claves->tamanio() / 2;

	// Transferimos elementos de la segunda mitad hacia las listas del
	// nodo hermano
	this->claves->transferir(nodoHojaHermano->claves, pos_corte, 
		this->claves->tamanio() - 1);
	this->registros->transferir(nodoHojaHermano->registros, pos_corte,
		this->registros->tamanio() - 1);

	// Apuntamos al nodo hermano
	nodoHojaHermano->nodoHermano = this->nodoHermano;
	this->nodoHermano = nodoHojaHermano->numBloque;

	// Retornamos la clave menor del nodo hermano
	return nodoHojaHermano->claves->verPrimero();
}


// Carga el nodo desde un archivo.
// PRE: 'archivo' es donde se almacena el nodo que se desea cargar, el
// cual se encuentra en el numero de bloque con el que fue inicializado.
// POST: se han cargado todos los atributos internos del nodo
template < typename Tipo >
void NodoHoja< Tipo >::cargar(ArchivoBloques *archivo)
{
	this->buffer->clear();
	archivo->leerBloque(this->buffer->getBuffer(), this->numBloque);

	uint numBloque, nivel, cantMaxClaves, nodoHermano;

	this->buffer->unpack(&numBloque);
	this->buffer->unpack(&nivel);
	this->buffer->unpack(&cantMaxClaves);
	this->buffer->unpack(&nodoHermano);

	this->numBloque = numBloque;
	this->nivel = nivel;
	this->cantMaxClaves = cantMaxClaves;
	this->nodoHermano = nodoHermano;

	// Deserializamos las claves
	this->claves->deserializar(this->buffer);
	// Deserializamos los registrosthis
	this->registros->deserializar(this->buffer);
}


// Guarda el nodo en un archivo.
// PRE: 'archivo' es donde se almacenaran los datos del nodo, los cuales
// se guardaran en el numero de bloque con el cual fue inicializado.
// POST: se guardo el estado del nodo en el archivo.
template < typename Tipo >
void NodoHoja< Tipo >::guardar(ArchivoBloques *archivo)
{
	this->buffer->clear();

	uint numBloque = this->numBloque;
	uint nivel = this->nivel;
	uint cantMaxClaves = this->cantMaxClaves;
	uint nodoHermano = this->nodoHermano;

	// Serializamos atributos
	this->buffer->pack(&numBloque, sizeof(uint));
	this->buffer->pack(&nivel, sizeof(uint));
	this->buffer->pack(&cantMaxClaves, sizeof(uint));
	this->buffer->pack(&nodoHermano, sizeof(uint));

	// Serializamos las claves
	this->claves->serializar(this->buffer);
	// Serializamos los registros
	this->registros->serializar(this->buffer);

	// Escribimos bloque y limpiamos buffer
	archivo->escribirBloque(this->buffer->getBuffer(), this->numBloque);
	this->buffer->clear();
}


// Se imprime el nodo en la salida estandar con su contenido.
// FORMATO: "[nivel], [numero_bloque]: ([clave1])...([claveN])[nodo_hermano] 
template < typename Tipo >
void NodoHoja< Tipo >::imprimir(uint& nivelDelArbol, 
	ArchivoBloques *archivo)
{
	// Tabulamos de acuerdo al nivel
	int tabs = nivelDelArbol - this->nivel;
	std::cout << std::string(tabs, '\t');
	std::cout << this->nivel << ", " << this->numBloque << ": ";

	// Iteramos sobre las claves
	for(size_t i = 0; i < this->claves->tamanio(); i++)
		std::cout << "(" << (*this->claves)[i] << ")";

	std::cout << this->nodoHermano << std::endl;

	return;
}


#endif
