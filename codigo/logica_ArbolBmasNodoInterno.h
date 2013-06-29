//
//  logica_ArbolBmasNodoInterno.h
//	NODO INTERNO - ARBOL B+
//	***************************************************************************
//
//	Implementacion de la estructura del Nodo Interno utilizado en el Arbol B+.
//	Debe considerarse esta estructura como privada del arbol, es decir, se
//	prohibe su uso fuera del ambito de la clase del ArbolBmas.
//


#ifndef ARBOLBMAS_NODO_INTERNO_H
#define ARBOLBMAS_NODO_INTERNO_H


#include "fisica_ArchivoBloques.h"
#include "logica_ArbolBmasNodo.h"
#include "logica_ArbolBmasNodoHoja.h"
#include "lib_ListaFija.h"


// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint;




/* ****************************************************************************
 * DECLARACIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


template < typename Tipo >
struct NodoInterno : public Nodo< Tipo >
{
	ListaFija< uint > *claves;		// Claves de registros
	ListaFija< uint > *hijos;		// Referencias a hijos
	size_t MAX_HOJA;				// Cantidad maxima de registros en hoja
	size_t MAX_INTERNO;				// Cantidad maxima de registros en nodo
									// interno


	// Constructor
	NodoInterno(size_t max_hoja, size_t max_interno);

	// Destructor
	~NodoInterno();	

	// Inserta el registro en el nodo.
	// PRE: 'clave' es la clave a insertar; 'registro' es el registro
	// asociado a dicha clave; 'archivo' es un puntero al archivo donde
	// se almacena el arbol; 'contBloques' es el contador de bloques del
	// arbol.
	// POST: devuelve true si queda en overflow o false en caso contrario
	virtual bool insertar(uint& clave, Tipo & registro, 
		ArchivoBloques *archivo, uint& contBloques);

	// Agrega una primer clave al nodo con sus respectivos hijos. Sirve
	// para inicializar un nuevo nodo luego de dividir nodos hijos.
	// PRE: 'clave' es la clave que tendra los hijos; 
	// POST: Se agregaron la clave y los hijos al nodo.
	void insertarHijosIniciales(uint clave, uint numBloqueIzq, 
		uint numBloqueDer);

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

	// Se imprime el nodo en la salida estandar con su contenido
	// FORMATO: "[nivel], [numero_bloque]: [hijo1]([clave1])..[hijoN]([claveN])
	// [hijoN+1] 
	virtual void imprimir(uint& nivelDelArbol, ArchivoBloques *archivo);
};




/* ****************************************************************************
 * DEFINICIÓN DE LA ESTRUCTURA
 * ***************************************************************************/


// Constructor
template < typename Tipo >
NodoInterno< Tipo >::NodoInterno(size_t max_hoja, size_t max_interno) :
	MAX_HOJA(max_hoja), MAX_INTERNO(max_interno)
{
	this->cantMaxClaves = MAX_INTERNO;
	this->claves = new ListaFija< uint >(MAX_INTERNO + 1);
	this->hijos = new ListaFija< uint >(MAX_INTERNO + 2);
}


// Destructor
template < typename Tipo >
NodoInterno< Tipo >::~NodoInterno() 
{
	delete this->claves;
	delete this->hijos;
}


// Inserta el registro en el nodo.
// PRE: 'clave' es la clave a insertar; 'registro' es el registro
// asociado a dicha clave; 'archivo' es un puntero al archivo donde
// se almacena el arbol; 'contBloques' es el contador de bloques del
// arbol.
// POST: devuelve true si queda en overflow o false en caso contrario
template < typename Tipo >
bool NodoInterno< Tipo >::insertar(uint& clave, 
	Tipo & registro, ArchivoBloques *archivo, uint& contBloques)
{
	Nodo< Tipo > *nodo;

	if(this->nivel == 1)
		nodo = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
	else
		nodo = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	// Iteramos sobre los hijos
	for(size_t i = 0; i < this->hijos->tamanio(); i++)
	{
		// Si llegamos al hijo final insertamos allí, pero si se encuentra
		// una clave mayor a la que se desea insertar, insertamos en el 
		// hijo izquierdo de la primer clave mayor que aparezca
		if(i == (this->hijos->tamanio()-1) || clave < (*this->claves)[i])
		{
			// Cargamos el nodo en memoria
			nodo->setNumBloque((*this->hijos)[i]);
			nodo->cargar(archivo);

			// Insertamos la clave y el registro en el nodo, y verificamos
			// si se produjo overflow en este, en cual caso, accionaremos
			if(nodo->insertar(clave, registro, archivo, contBloques))
			{
				/* Se produjo overflow en el nodo */

				Nodo< Tipo > *nodoHermano;

				// Creamos nodo hermano
				if(this->nivel == 1)
					nodoHermano = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
				else
					nodoHermano = 
						new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);
				
				// Incrementamos el contador de bloques del arbol
				contBloques++;
				// Inicializamos los datos del nodo hermano
				nodoHermano->inicializar(contBloques, nodo->nivel);

				// Dividimos el contenido entre nodo y nodo hermano e
				// insertamos la clave retornada a la lista de claves
				this->claves->insertar(nodo->dividir(nodoHermano), i);

				// Agregamos el numero de bloque del nodo hermano a la lista
				// de hijos
				this->hijos->insertar(nodoHermano->getNumBloque(), i+1);

				// Guardamos cambios en disco
				nodo->guardar(archivo);
				nodoHermano->guardar(archivo);
				this->guardar(archivo);

				delete nodo;
				delete nodoHermano;

				// Verificamos si hay overblow y devolvemos true o false
				if(this->claves->tamanio() > this->cantMaxClaves) return true;
				return false;
			}

			// Escribimos el nodo en el archivo
			nodo->guardar(archivo);

			break;
		}
	}

	delete nodo;

	// Verificamos si entro en overflow y devolvemos de acuerdo a esto
	if(this->claves->tamanio() > this->cantMaxClaves) return true;
	return false;
}


// Agrega una primer clave al nodo con sus respectivos hijos. Sirve
// para inicializar un nuevo nodo luego de dividir nodos hijos.
// PRE: 'clave' es la clave que tendra los hijos; 
// POST: Se agregaron la clave y los hijos al nodo.
template < typename Tipo >
void NodoInterno< Tipo >::insertarHijosIniciales(
	uint clave, uint numBloqueIzq, uint numBloqueDer)
{
	this->claves->insertarUltimo(clave);
	this->hijos->insertarUltimo(numBloqueIzq);
	this->hijos->insertarUltimo(numBloqueDer);
}


// Busca un registro.
// PRE: 'clave' es la clave o id asociado al registro a buscar; 'registro'
// es un puntero a un almacenador en donde se insertara el resultado de la
// busqueda.
// POST: Si se encontró el registro, se devuelve true y se almacena en
// 'registro' al mismo. Si no se encontró, se devuelve false y se almacena
// en 'registro' el registro superior mas proximo al buscado.
template < typename Tipo >
bool NodoInterno< Tipo >::buscar(const uint clave, Tipo & registro, 
	ArchivoBloques *archivo)
{
	Nodo< Tipo > *nodo;

	if(this->nivel == 1)
		nodo = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
	else
		nodo = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	// Iteramos sobre los hijos
	for(size_t i = 0; i < this->hijos->tamanio(); i++)
	{
		if(i == (this->hijos->tamanio()-1) || clave < (*this->claves)[i])
		{
			// Cargamos el nodo en memoria
			nodo->setNumBloque((*this->hijos)[i]);
			nodo->cargar(archivo);

			// Buscamos en nodos inferiores
			bool resBusqueda = nodo->buscar(clave, registro, archivo);
			delete nodo;

			// Retornamos el resultado de la busqueda en nodos inferiores
			return resBusqueda;
		}
	}

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
bool NodoInterno< Tipo >::actualizar(const uint clave, Tipo & registro, 
	ArchivoBloques *archivo)
{
	Nodo< Tipo > *nodo;

	if(this->nivel == 1)
		nodo = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
	else
		nodo = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

	// Iteramos sobre los hijos
	for(size_t i = 0; i < this->hijos->tamanio(); i++)
	{
		if(i == (this->hijos->tamanio()-1) || clave < (*this->claves)[i])
		{
			// Cargamos el nodo en memoria
			nodo->setNumBloque((*this->hijos)[i]);
			nodo->cargar(archivo);

			// Buscamos en nodos inferiores
			bool resActualizacion = nodo->actualizar(clave, registro, archivo);
			delete nodo;

			// Retornamos el resultado de la busqueda en nodos inferiores
			return resActualizacion;
		}
	}

	return false;
}


// Reparte su contenido con su nodoHermano, pasandole la mitad.
// PRE: 'nodoHermano' es un nodo con el que se hara la division
// POST: devuelve la clave del registro inferior de 'nodoHermano'
template < typename Tipo >
uint NodoInterno< Tipo >::dividir(
	Nodo< Tipo > *nodoHermano)
{
	// Casteamos para poder tratarlo como nodo hoja
	NodoInterno *nodoInternoHermano = (NodoInterno< Tipo >*) nodoHermano;

	// Calculamos posicion de corte
	int pos_corte = (this->claves->tamanio() / 2) + 1;

	// Transferimos elementos de la segunda mitad hacia las listas del
	// nodo hermano
	this->claves->transferir(nodoInternoHermano->claves, pos_corte, 
		this->claves->tamanio() - 1);
	this->hijos->transferir(nodoInternoHermano->hijos, pos_corte,
		this->hijos->tamanio() - 1);

	// Tomamos la clave que subira hacia el nodo padre y la borramos de la
	// lista de claves del nodo actual
	uint clave_subir = this->claves->verUltimo();
	this->claves->eliminarUltimo();

	return clave_subir;
}


// Carga el nodo desde un archivo.
// PRE: 'archivo' es donde se almacena el nodo que se desea cargar, el
// cual se encuentra en el numero de bloque con el que fue inicializado.
// POST: se han cargado todos los atributos internos del nodo
template < typename Tipo >
void NodoInterno< Tipo >::cargar(
	ArchivoBloques *archivo)
{
	this->buffer->clear();
	archivo->leerBloque(this->buffer->getBuffer(), this->numBloque);

	uint numBloque, nivel, cantMaxClaves;

	this->buffer->unpack(&numBloque);
	this->buffer->unpack(&nivel);
	this->buffer->unpack(&cantMaxClaves);

	this->numBloque = numBloque;
	this->nivel = nivel;
	this->cantMaxClaves = cantMaxClaves;

	// Deserializamos las claves
	this->claves->deserializar(this->buffer);
	// Deserializamos los registros
	this->hijos->deserializar(this->buffer);
}


// Guarda el nodo en un archivo.
// PRE: 'archivo' es donde se almacenaran los datos del nodo, los cuales
// se guardaran en el numero de bloque con el cual fue inicializado.
// POST: se guardo el estado del nodo en el archivo.
template < typename Tipo >
void NodoInterno< Tipo >::guardar(
	ArchivoBloques *archivo)
{
	// Necesitamos un buffer vacio, por lo que lo limpiamos
	this->buffer->clear();

	uint numBloque = this->numBloque;
	uint nivel = this->nivel;
	uint cantMaxClaves = this->cantMaxClaves;

	// Serializamos atributos
	this->buffer->pack(&numBloque, sizeof(uint));
	this->buffer->pack(&nivel, sizeof(uint));
	this->buffer->pack(&cantMaxClaves, sizeof(uint));

	// Serializamos las claves
	this->claves->serializar(this->buffer);
	// Serializamos los registros
	this->hijos->serializar(this->buffer);

	// Escribimos bloque y limpiamos buffer
	archivo->escribirBloque(this->buffer->getBuffer(), this->numBloque);
	this->buffer->clear();
}


// Se imprime el nodo en la salida estandar con su contenido
// FORMATO: "[nivel], [numero_bloque]: [hijo1]([clave1])...[hijoN]([claveN])
// [hijoN+1] 
template < typename Tipo >
void NodoInterno< Tipo >::imprimir(uint& nivelDelArbol, 
	ArchivoBloques *archivo)
{
	// Tabulamos de acuerdo al nivel
	int tabs = nivelDelArbol - this->nivel;
	std::cout << std::string(tabs, '\t');
	std::cout << this->nivel << ", " << this->numBloque << ": ";

	// Iteramos sobre las claves
	for(size_t i = 0; i < this->claves->tamanio(); i++)
	{
		std::cout << (*this->hijos)[i];
		std::cout << "(" << (*this->claves)[i] << ")";
	}

	std::cout << (*this->hijos)[this->hijos->tamanio() - 1] << std::endl;

	// Imprimimos hijos
	for(size_t i = 0; i < this->hijos->tamanio(); i++)
	{
		Nodo< Tipo > *hijo = 0;

		if(this->nivel == 1)
			hijo = new NodoHoja< Tipo >(MAX_HOJA, MAX_INTERNO);
		else
			hijo = new NodoInterno< Tipo >(MAX_HOJA, MAX_INTERNO);

		hijo->setNumBloque((*this->hijos)[i]);
		hijo->cargar(archivo);
		hijo->imprimir(nivelDelArbol, archivo);

		delete hijo;
	}
}


#endif
