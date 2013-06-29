//
//  lib_ListaFija.h
//	ListaFija
//	***************************************************************************
//
//	Implementacion de una lista de tamanio maximo fijo, la cual imita las
//  funcionalidades de la libreria list de la STL.
//
//  RESTRICCIONES: Para la utilizacion de la lista es necesario que, de
//  almacenarse tipos no primitivos, se defina dentro de este ultimo los
//  siguientes metodos:
//   
//  	- serializar(SerialBuffer *buffer),
//  	- deserializar(SerialBuffer *buffer).
//
//  Esto le permitira a la lista ser serializable y persistible.
//
//	TIPOS NATIVOS SOPORTADOS: Esta primer version de ListaFija solamente
//	soporta el tipo nativo unsigned int.
//


#ifndef LISTA_FIJA_H
#define LISTA_FIJA_H


#include <string>
#include "fisica_SerialBuffer.h"


// Definicion de tipo uint para utilizar nombre mas corto
typedef unsigned int uint; 





/* ****************************************************************************
 * DECLARACIÓN DEL TEMPLATE
 * ***************************************************************************/


template < typename Tipo > 
class ListaFija 
{

private:
	
	size_t Tamanio;
	Tipo *lista;					// Arreglo que representa a la lista
	size_t tamanioParcial;			// Tamanio real de la lista, la cual será
									// siempre menor o igual a Tamanio.

public:

	// Constructor
	ListaFija(size_t Tamanio);

	// Destructor
	~ListaFija();

	// Inserta un nuevo elemento en una posicion especifica de la lista
	// desplazando los demas elementos hacia adelante
	// PRE: 'elemento' es el elemento a insertar; 'pos' es la posicion
	// en la que se desea insertar.
	void insertar(Tipo elemento, const size_t pos);

	// Inserta un nuevo elemento al final de la lista.
	// PRE: 'elemento' es el dato a insertar.
	void insertarUltimo(const Tipo elemento);

	// Reemplaza el contenido de una posicion con un nuevo elemento.
	// PRE: 'elemento' es el elemento nuevo; 'pos' es la posicion
	// en la que se desea reemplazar.
	void reemplazar(Tipo elemento, const size_t pos);

	// Devuelve un puntero al primer elemento
	Tipo verPrimero();

	// Devuelve el ultimo elemento de la lista.
	Tipo verUltimo();

	// Elimina el ultimo elemento de la lista decrementando una unidad su 
	// tamanio.
	void eliminarUltimo();

	// Devuelve la cantidad de elementos contenidos en la lista.
	size_t tamanio();

	// Verifica si una lista se encuentra vacía.
	// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
	// caso contrario.
	bool estaVacia();

	// Verifica si una lista se encuentra llena (condicion que se da cuando
	// se igualan la cantidad de elementos con el valor de Tamanio).
	// POST: Devuelve verdadero si la lista se encuentra llena o falso en caso
	// contrario.
	bool estaLlena();

	// Operador []
	// Permite acceder a los índices de la lista mediante la notación 
	// lista[i], donde i es un número entero comprendido entre [0, n-1],
	// siendo n el tamanio de la lista.
	Tipo operator[] (const size_t indice);

	// Transfiere los elementos de la lista a una lista pasada por parametro.
	// Los elementos a transferir se van colocando en orden al final de la
	// lista externa.
	// PRE: 'lista' es la lista a la que se transferiran los elementos, la cual
	// debe poseer el mismo tamanio que Tamanio;
	// 'pos_ini' es la posicion inicial desde la que se transferira; 'pos_fin'
	// es la posicion final de transferencia, incluyendo el elemento de dicha
	// posicion.
	// POST: la lista desde la que se realiza la transferencia disminuye su
	// tamaño de acuerdo a la cantidad de elementos que se transfirieron.
	void transferir(ListaFija< Tipo > *lista, size_t pos_ini, 
		size_t pos_fin);

	// Serializa la lista.
	// PRE: 'buffer' es el buffer en donde se serializara la lista para poder
	// ser almacenada en algun medio.
	void serializar(SerialBuffer *buffer);

	// Deserializa una lista.
	// PRE: 'buffer' es el buffer en donde se encuentra serializada la lista.
	// POST: el estado de la lista se actualiza con los datos obtenidos del
	// buffer.
	void deserializar(SerialBuffer *buffer);

	// Devuelve el tamanio en bytes que ocuparia persistir la clase.
	static size_t getBytesUsados();
};




/* ****************************************************************************
 * DEFINICIÓN DEL TEMPLATE
 * ***************************************************************************/



/* PRIVADO
 * Utilizado para serializar dependiendo de si se esta conteniendo elementos de
 * tipo primitivo o no.
 */
namespace {

	// Serializacion de tipos no primitivos
	template <typename Tipo >
	struct Serializacion
	{
		// Serializacion
		void serializar(SerialBuffer *buffer, Tipo lista[], 
			size_t tamanioParcial)
		{
			// Iteramos sobre cada elemento y los vamos serializando en orden
			for(size_t i = 0; i < tamanioParcial; i++)
				lista[i].serializar(buffer);
		}

		// Deserializacion
		void deserializar(SerialBuffer *buffer, 
			ListaFija< Tipo> *estaLista)
		{
			size_t tamanioParcial;

			// Tomamos el tamanio parcial de la lista
			buffer->unpack(&tamanioParcial);

			// Cargamos los elementos
			for(size_t i = 0; i < tamanioParcial; i++)
			{
				Tipo e;
				e.deserializar(buffer);
				estaLista->insertarUltimo(e);
			}
		}
	};


	// Serializacion del tipo primitivo UNSIGNED INT
	template <>
	struct Serializacion< uint > 
	{
		// Serializacion
		void serializar(SerialBuffer *buffer, uint lista[], 
			size_t tamanioParcial)
		{
			// Iteramos sobre cada elemento y los vamos serializando en orden
			for(size_t i = 0; i < tamanioParcial; i++)
				buffer->pack(&(lista[i]), sizeof(unsigned int));
		}

		// Deserializacion
		void deserializar(SerialBuffer *buffer, 
			ListaFija< uint > *estaLista)
		{
			size_t tamanioParcial;

			// Tomamos el tamanio parcial de la lista
			buffer->unpack(&tamanioParcial);

			// Cargamos los elementos
			for(size_t i = 0; i < tamanioParcial; i++)
			{
				uint e;
				buffer->unpack(&e);
				estaLista->insertarUltimo(e);
			}
		}
	};
}
/* FIN PRIVADO */




// Constructor
template <typename Tipo >
ListaFija< Tipo >::ListaFija(size_t Tamanio) : Tamanio(Tamanio), 
	tamanioParcial(0) 
{
	this->lista = new Tipo[this->Tamanio];
}


// Destructor
template <typename Tipo >
ListaFija< Tipo >::~ListaFija() 
{
	delete [] this->lista;
}


// Inserta un nuevo elemento en una posicion especifica de la lista
// desplazando los demas elementos hacia adelante
// PRE: 'elemento' es el elemento a insertar; 'pos' es la posicion
// en la que se desea insertar.
template <typename Tipo >
void ListaFija< Tipo >::insertar(Tipo elemento, const size_t pos)
{
	// Corroboramos que la posicion sea valida
	if(pos > this->tamanioParcial)
		throw "ERROR: Posicion de lista inválida.";
	// Corroboramos que no este llena
	else if(this->estaLlena()) 
		throw "ERROR: Lista llena. No puede insertarse elemento.";

	Tipo b = elemento;
	Tipo a;

	for(size_t i = pos; i <= this->tamanioParcial; i++)
	{
		a = this->lista[i];
		this->lista[i] = b;
		b = a;
	}

	++this->tamanioParcial;
}


// Inserta un nuevo elemento al final de la lista.
// PRE: 'elemento' es el dato a insertar.
template <typename Tipo >
void ListaFija< Tipo >::insertarUltimo(const Tipo elemento)
{
	// Corroboramos que no este llena
	if(this->estaLlena()) 
		throw "ERROR: Lista llena. No puede insertarse elemento.";

	this->lista[this->tamanioParcial] = elemento;
	++tamanioParcial;
}


// Reemplaza el contenido de una posicion con un nuevo elemento.
// PRE: 'elemento' es el elemento nuevo; 'pos' es la posicion
// en la que se desea reemplazar.
template <typename Tipo >
void ListaFija< Tipo >::reemplazar(Tipo elemento, const size_t pos)
{
	// Corroboramos que la posicion sea valida
	if(pos > this->tamanioParcial)
		throw "ERROR: Posicion de lista inválida.";

	this->lista[pos] = elemento;
}


// Devuelve un puntero al primer elemento
template <typename Tipo >
Tipo ListaFija< Tipo >::verPrimero()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No se puede ver el primer elemento.";

	return this->lista[0];
}


// Devuelve el ultimo elemento de la lista.
template <typename Tipo >
Tipo ListaFija< Tipo >::verUltimo()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No se puede ver el primer elemento.";

	return this->lista[this->tamanioParcial - 1];
}


// Elimina el ultimo elemento de la lista decrementando una unidad su 
// tamanio.
template <typename Tipo >
void ListaFija< Tipo >::eliminarUltimo()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No puede eliminarse el primer elemento.";

	--this->tamanioParcial;
	this->lista[this->tamanioParcial] = 0;
}


// Devuelve la cantidad de elementos contenidos en la lista.
template <typename Tipo >
size_t ListaFija< Tipo >::tamanio()
{
	return this->tamanioParcial;
}


// Verifica si una lista se encuentra vacía.
// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
// caso contrario.
template <typename Tipo >
bool ListaFija< Tipo >::estaVacia()
{
	return (tamanioParcial == 0);
}


// Verifica si una lista se encuentra llena (condicion que se da cuando
// se igualan la cantidad de elementos con el valor de Tamanio).
// POST: Devuelve verdadero si la lista se encuentra llena o falso en caso
// contrario.
template <typename Tipo >
bool ListaFija< Tipo >::estaLlena()
{
	return (tamanioParcial == Tamanio);
}


// Operador []
// Permite acceder a los índices de la lista mediante la notación lista[i], 
// donde i es un número entero comprendido entre [0, n-1], siendo n el tamaño
// de la lista.
template <typename Tipo >
Tipo ListaFija< Tipo >::operator[] (const size_t indice)
{	
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No puede eliminarse el primer elemento.";
	// Corroboramos que el índice sea válido
	else if(indice >= this->tamanioParcial)
		throw "ERROR: Indice de lista inválido.";
		
	return this->lista[indice];
}


// Transfiere los elementos de la lista a una lista pasada por parametro.
// Los elementos a transferir se van colocando en orden al final de la
// lista externa.
// PRE: 'lista' es la lista a la que se transferiran los elementos, la cual
// debe poseer el mismo tamanio que Tamanio;
// 'pos_ini' es la posicion inicial desde la que se transferira; 'pos_fin'
// es la posicion final de transferencia, incluyendo el elemento de dicha
// posicion.
// POST: la lista desde la que se realiza la transferencia disminuye su
// tamaño de acuerdo a la cantidad de elementos que se transfirieron.
template <typename Tipo >
void ListaFija< Tipo >::transferir(ListaFija< Tipo >* lista, 
	size_t pos_ini, size_t pos_fin)
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No puede realizarse la transferencia.";
	// Corroboramos que las posiciones sean validas
	else if(pos_ini > this->tamanioParcial || pos_fin > this->tamanioParcial)
		throw "ERROR: Una o ambas posiciones de transferencia son inválidas";
	// Corroboramos que no este llena
	else if(lista->estaLlena()) 
		throw "ERROR: La lista destino en transferencia se encuentra llena.";

	// Iteramos y transferimos elementos
	for(size_t i = pos_ini; i <= pos_fin; i++)
	{
		lista->insertarUltimo(this->lista[i]);
		--this->tamanioParcial;
	}
}


// Serializa la lista.
// PRE: 'buffer' es el buffer en donde se serializara la lista para poder
// ser almacenada en algun medio.
template <typename Tipo >
void ListaFija< Tipo >::serializar(SerialBuffer *buffer)
{
	size_t tamanioParcial = this->tamanioParcial;

	// Serializamos primero el tamanio y despues el arreglo
	buffer->pack(&tamanioParcial, sizeof(tamanioParcial));

	Serializacion< Tipo > s;
	s.serializar(buffer, this->lista, tamanioParcial);
}


// Deserializa una lista.
// PRE: 'buffer' es el buffer en donde se encuentra serializada la lista.
// POST: el estado de la lista se actualiza con los datos obtenidos del
// buffer.
template <typename Tipo >
void ListaFija< Tipo >::deserializar(SerialBuffer *buffer)
{
	Serializacion< Tipo > s;
	s.deserializar(buffer, this);
}


// Devuelve el tamanio en bytes que ocuparia persistir la clase.
template <typename Tipo >
size_t ListaFija< Tipo >::getBytesUsados()
{
	// Retornamos la cantidad de bytes usadas para saber el tamanio parcial de
	// la lista
	return sizeof(size_t);
}


#endif
