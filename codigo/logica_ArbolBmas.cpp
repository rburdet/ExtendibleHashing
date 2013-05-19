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
	const uint NUM_BLOQUE_RAIZ_INICIAL = 2;
}




/* ****************************************************************************
 * DEFINICIÓN DE LA CLASE
 * ***************************************************************************/


// Constructor
ArbolBmas::ArbolBmas() : nivel(0), contBloques(1) { }


// Destructo
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

		// Inicializamos el nodo raiz
		this->raiz.inicializar(NUM_BLOQUE_RAIZ_INICIAL, 0, CANT_REG_NODO_HOJA);

		// Creamos metadata del arbol con valores iniciales
		guardarMetadata();

		// Guardamos nodo raiz inicial
		this->archivo->escribirBloque(&this->raiz, this->raiz.getNumBloque(),
			sizeof(Nodo));

		// this->archivo->leerBloque(&nodo, 2);

		// std::cout << nodo.getNumBloque() << std::endl;
		// std::cout << "Tomo bien" << std::endl;

		return;
	}
	
	// Cargamos metadata
	cargarMetadata();

	// std::cout << this->nivel << " " << this->contBloques  << std::endl;
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
	// Cargamos la raiz
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
ArbolBmas::Nodo::Nodo() { } 


// Destructor
ArbolBmas::Nodo::~Nodo() { }


// Inicializa el nodo.
// PRE: 'numBloque' es el numero de bloque en el que se encuentra
// almacenado; 'nivel' es el nivel en el que se encuentra el nodo;
// 'cantMaxClaves' es la cantidad maxima de claves que puede contener
// el nodo (sobrepasado este limite se considera overflow).
void ArbolBmas::Nodo::inicializar(uint numBloque, uint nivel, 
	uint cantMaxClaves)
{
	this->numBloque = numBloque;
	this->nivel = nivel;
	this->cantMaxClaves = cantMaxClaves;
}


// Inserta el registro en el nodo.
// PRE: 'clave' es la clave a insertar; 'registro' es el registro
// asociado a dicha clave; 'archivo' es un puntero al archivo donde
// se almacena el arbol; 'contBloques' es el contador de bloques del
// arbol.
// POST: devuelve true si queda en overflow o false en caso contrario
bool ArbolBmas::Nodo::insertar(uint& clave, RegistroGenerico& registro, 
	ArchivoBloques *archivo, uint& contBloques)
{
	// Creamos iteradores
	list< uint >::iterator it_claves;


	/*
	 *  Caso en el que es un nodo hoja 
	 */

	if(this->nivel == 0)
	{
		list< RegistroGenerico >::iterator it_registros;
		it_registros = this->registros.begin();

		// Flag para saber si se ingreso elemento en el medio de la lista
		bool inserto_en_medio = false;

		for(it_claves = this->claves.begin(); it_claves != this->claves.end();
			++it_claves)
		{
			if(*it_claves == clave)
				throw "ERROR: Claves iguales en arbol.";
			
			// Si la clave es mas grande que el actual, insertamos en ese lugar
			else if(*it_claves > clave)
			{
				this->claves.insert(it_claves, clave);
				this->registros.insert(it_registros, registro);
				inserto_en_medio = true;
				break;
			}

			++it_registros;
		}

		// Si no se inserto en medio, lo insertamos al final
		if(!inserto_en_medio) 
		{
			this->claves.push_back(clave);
			this->registros.push_back(registro);
		}

		this->cantClaves++;

		// Corroboramos si entro en overflow y devolvemos de acuerdo a esto
		if(this->cantClaves > this->cantMaxClaves) return true;
		return false;
	}


	/*
	 *  Si no es nodo hoja accionamos de forma distinta
	 */

	list< uint >::iterator it_hijos;
	Nodo nodo;

	// Posicionamos iterador de las claves al inicio de la lista
	it_claves = this->claves.begin();

	// Iteramos sobre los hijos
	for(it_hijos = this->hijos.begin(); it_hijos != this->hijos.end();
		++it_hijos)
	{
		// Si llegamos al hijo final insertamos allí, pero si se encuentra
		// una clave mayor a la que se desea insertar, insertamos en el 
		// hijo izquierdo de la primer clave mayor que aparezca
		if(it_hijos == --this->hijos.end() || clave < *it_claves)
		{
			// Cargamos el nodo en memoria
			archivo->leerBloque(&nodo, *it_hijos);

			// Insertamos la clave y el registro en el nodo, y verificamos
			// si se produjo overflow en este, en cual caso, accionaremos
			if(nodo.insertar(clave, registro, archivo, contBloques))
			{
				/* Se produjo overflow en el nodo */

				// Creamos nodo hermano
				Nodo nodoHermano;
				nodoHermano.inicializar(NUM_BLOQUE_RAIZ_INICIAL + contBloques,
					this->nivel, this->cantMaxClaves);

				// Incrementamos el contador de bloques del arbol
				contBloques++;

				// Dividimos el contenido entre nodo y nodo hermano e
				// insertamos la clave retornada a la lista de claves
				this->claves.insert(it_claves, nodo.dividir(nodoHermano));

				// Agregamos el numero de bloque del nodo hermano a la lista
				// de hijos
				this->hijos.insert(it_hijos, nodoHermano.getNumBloque());

				// Escribimos los nodos en el archivo
				archivo->escribirBloque(&nodo, nodo.getNumBloque(), 
					sizeof(Nodo));
				archivo->escribirBloque(&nodoHermano, 
					nodoHermano.getNumBloque(), sizeof(Nodo));

				// Verificamos si hay overblow y devolvemos true o false
				if(this->cantClaves > this->cantMaxClaves) return true;
				return false;
			}

			// Escribimos el nodo en el archivo
			archivo->escribirBloque(&nodo, nodo.getNumBloque(), 
				sizeof(Nodo));

			break;
		}

		++it_claves;
	}

	return false;
}


// Reparte su contenido con su nodoHermano, pasandole la mitad.
// PRE: 'nodoHermano' es un nodo con el que se hara la division
// POST: devuelve la clave del registro inferior de 'nodoHermano'
uint ArbolBmas::Nodo::dividir(Nodo& nodoHermano)
{
	// Calculamos posicion de corte
	int pos_corte = this->claves.size() / 2;

	/* Caso en el que es un nodo hoja */
	if(this->nivel == 0)
	{
		// Creamos iteradores
		list< uint >::iterator it_claves;
		list< RegistroGenerico >::iterator it_registros;

		// Posicionamos iteradores al inicio de las listas
		it_claves = this->claves.begin();
		it_registros = this->registros.begin();

		// Movemos iteradores hacia la posicion de corte
		advance(it_claves, pos_corte);
		advance(it_registros, pos_corte);

		// Transferimos elementos de la segunda mitad hacia las listas del
		// nodo hermano
		nodoHermano.claves.splice(nodoHermano.claves.begin(), this->claves,
			it_claves, this->claves.end());
		nodoHermano.registros.splice(nodoHermano.registros.begin(), 
			this->registros, it_registros, this->registros.end());

		// Apuntamos al nodo hermano
		this->nodoHermano = nodoHermano.numBloque;

		// Actualizamos cantidad de claves en nodos
		this->cantClaves = this->claves.size();
		nodoHermano.cantClaves = nodoHermano.claves.size();

		// Retornamos la clave menor del nodo hermano
		return nodoHermano.claves.front();
	}

	/* Si no es nodo hoja aplicamos otra metodologia */

	// Creamos iteradores
	list< uint >::iterator it_claves, it_hijos;

	// Posicionamos iteradores al inicio de las listas
	it_claves = this->claves.begin();
	it_hijos = this->hijos.begin();

	// Movemos iteradores hacia la posicion de corte
	advance(it_claves, pos_corte + 1);
	advance(it_hijos, pos_corte + 1);

	// Transferimos elementos de la segunda mitad hacia las listas del
	// nodo hermano
	nodoHermano.claves.splice(nodoHermano.claves.begin(), this->claves,
		it_claves, this->claves.end());
	nodoHermano.hijos.splice(nodoHermano.hijos.begin(), this->hijos, it_hijos,
		this->hijos.end());

	// Tomamos la clave que subira hacia el nodo padre y la borramos de la
	// lista de claves del nodo actual
	uint clave_subir = this->claves.back();
	this->claves.pop_back();

	// Actualizamos cantidad de claves en nodos
	this->cantClaves = this->claves.size();
	nodoHermano.cantClaves = nodoHermano.claves.size();

	return clave_subir;
}


// Devuelve el numero de bloque en el que se encuentra el nodo
uint ArbolBmas::Nodo::getNumBloque()
{
	return this->numBloque;
}


// Se imprime el nodo en la salida estandar con su contenido
void ArbolBmas::Nodo::imprimir(uint nivelDelArbol)
{
	// Creamos iteradores
	list< uint >::iterator it_claves;

	// Tabulamos de acuerdo al nivel
	std::cout << std::string((nivelDelArbol - this->nivel), '\t');
	std::cout << this->numBloque << ": ";

	/* Caso en el que es un nodo hoja */
	if(this->nivel == 0)
	{
		for(it_claves = this->claves.begin(); it_claves != this->claves.end();
			++it_claves)
			std::cout << "(" << *it_claves << ")";

		std::cout << this->nodoHermano << std::endl;

		return;
	}

	/* Caso en el que es un nodo interno */

	list< uint >::iterator it_hijos;
	it_hijos = this->hijos.begin();

	for(it_claves = this->claves.begin(); it_claves != this->claves.end();
			++it_claves)
	{
		++it_hijos;
		std::cout << *it_hijos << "(" << *it_claves << ")";
	}

	++it_hijos;
	std::cout << *it_hijos << std::endl; 

	return;
}
