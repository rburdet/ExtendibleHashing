//
//  ListaFija.h
//	Implementación de una lista de tamaño máximo fijo, la cual imita las
//  funcionalidades de la librería list de la STL. 
//


#ifndef LISTA_FIJA_H
#define LISTA_FIJA_H


#include <string>


/* ****************************************************************************
 * DECLARACIÓN DEL TEMPLATE
 * ***************************************************************************/


template < typename Tipo, size_t Tamanio > 
class ListaFija 
{

private:
	
	Tipo lista[Tamanio];			// Arreglo que representa a la lista
	size_t tamanioParcial;			// Tamanio real de la lista, la cual será
									// siempre menor o igual a Tamanio.

public:

	// Constructor
	ListaFija();

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
	void transferir(ListaFija< Tipo, Tamanio >& lista, size_t pos_ini, 
		size_t pos_fin);
};




/* ****************************************************************************
 * DEFINICIÓN DEL TEMPLATE
 * ***************************************************************************/


// Constructor
template <typename Tipo, size_t Tamanio >
ListaFija< Tipo, Tamanio >::ListaFija() : tamanioParcial(0) { }


// Destructor
template <typename Tipo, size_t Tamanio >
ListaFija< Tipo, Tamanio >::~ListaFija() { }


// Inserta un nuevo elemento en una posicion especifica de la lista
// desplazando los demas elementos hacia adelante
// PRE: 'elemento' es el elemento a insertar; 'pos' es la posicion
// en la que se desea insertar.
template <typename Tipo, size_t Tamanio >
void ListaFija< Tipo, Tamanio >::insertar(Tipo elemento, const size_t pos)
{
	// Corroboramos que la posicion sea valida
	if(pos > this->tamanioParcial)
		throw "ERROR: Posicion de lista inválido.";
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
template <typename Tipo, size_t Tamanio >
void ListaFija< Tipo, Tamanio >::insertarUltimo(const Tipo elemento)
{
	// Corroboramos que no este llena
	if(this->estaLlena()) 
		throw "ERROR: Lista llena. No puede insertarse elemento.";

	this->lista[tamanioParcial] = elemento;
	++tamanioParcial;
}


// Devuelve un puntero al primer elemento
template <typename Tipo, size_t Tamanio >
Tipo ListaFija< Tipo, Tamanio >::verPrimero()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No se puede ver el primer elemento.";

	return this->lista[0];
}


// Devuelve el ultimo elemento de la lista.
template <typename Tipo, size_t Tamanio >
Tipo ListaFija< Tipo, Tamanio >::verUltimo()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No se puede ver el primer elemento.";

	return this->lista[this->tamanioParcial - 1];
}


// Elimina el ultimo elemento de la lista decrementando una unidad su 
// tamanio.
template <typename Tipo, size_t Tamanio >
void ListaFija< Tipo, Tamanio >::eliminarUltimo()
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No puede eliminarse el primer elemento.";

	--this->tamanioParcial;
	this->lista[this->tamanioParcial] = 0;
}


// Devuelve la cantidad de elementos contenidos en la lista.
template <typename Tipo, size_t Tamanio >
size_t ListaFija< Tipo, Tamanio >::tamanio()
{
	return this->tamanioParcial;
}


// Verifica si una lista se encuentra vacía.
// POST: Devuelve verdadero si la lista se encuentra vacía o falso en 
// caso contrario.
template <typename Tipo, size_t Tamanio >
bool ListaFija< Tipo, Tamanio >::estaVacia()
{
	return (tamanioParcial == 0);
}


// Verifica si una lista se encuentra llena (condicion que se da cuando
// se igualan la cantidad de elementos con el valor de Tamanio).
// POST: Devuelve verdadero si la lista se encuentra llena o falso en caso
// contrario.
template <typename Tipo, size_t Tamanio >
bool ListaFija< Tipo, Tamanio >::estaLlena()
{
	return (tamanioParcial == Tamanio);
}


// Operador []
// Permite acceder a los índices de la lista mediante la notación lista[i], 
// donde i es un número entero comprendido entre [0, n-1], siendo n el tamaño
// de la lista.
template <typename Tipo, size_t Tamanio >
Tipo ListaFija< Tipo, Tamanio >::operator[] (const size_t indice)
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
template <typename Tipo, size_t Tamanio >
void ListaFija< Tipo, Tamanio >::transferir(ListaFija< Tipo, Tamanio >& lista, 
	size_t pos_ini, size_t pos_fin)
{
	// Corroboramos que no este vacia
	if(this->estaVacia())
		throw "ERROR: Lista vacia. No puede realizarse la transferencia.";
	// Corroboramos que las posiciones sean validas
	else if(pos_ini > this->tamanioParcial || pos_fin > this->tamanioParcial)
		throw "ERROR: Una o ambas posiciones de transferencia son inválidas";
	// Corroboramos que no este llena
	else if(lista.estaLlena()) 
		throw "ERROR: La lista destino en transferencia se encuentra llena.";

	// Iteramos y transferimos elementos
	for(size_t i = pos_ini; i <= pos_fin; i++)
	{
		lista.insertarUltimo(this->lista[i]);
		--this->tamanioParcial;
	}
}

#endif
