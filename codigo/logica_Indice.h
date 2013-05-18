//
//  Indice.h
//	[ Insertar documentacion ]
//

#ifndef INDICE_H
#define INDICE_H




/* ****************************************************************************
 * DECLARACION DE LA CLASE
 * ***************************************************************************/

template < typename TipoClave >
class Indice
{

protected:

	int maxClaves;					// Numero maximo de claves
	int numClaves;					// Cantidad de claves
	TipoClave *claves;				// Puntero a arreglo de claves
	int *direccionRegistro;			// 
	bool unico;						// Si es true, cada clave debe ser unica			

	// 
	int buscar(const TipoClave clave, const int direccionRegistro = -1,
		const int exacta = 1) const;

	//
	int inicio(const int maxClaves, const int unico);

public:

	// Constructor
	Indice(int maxClaves, int unico = true);

	// Destructor
	~Indice();

	// Remueve todas las claves del indice
	void borrarClaves();

	//
	int insertar(const TipoClave clave, int direccionRegistro);

	//
	int eliminar(const TipoClave clave, const int direccionRegistro = -1);

	//
	int buscar(const TipoClave clave, const int direccionRegistro = -1,
		const int exacta = 1) const;

	// Devuelve la cantidad de claves contenidas en el indice
	int cantidadClaves() const;
};



/* ****************************************************************************
 * DEFINICION DE LA CLASE
 * ***************************************************************************/



#endif
