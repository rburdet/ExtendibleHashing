//
//  RegistroGenerico.h
//	[ Insertar documentacion ]
//


#ifndef REGISTRO_GENERICO_H
#define REGISTRO_BENERICO_H



/* ****************************************************************************
 * DEFINICION DE LA CLASE
 * ***************************************************************************/


class RegistroGenerico
{

private:

	unsigned int clave;			// Identificador del registro

public:

	// Constructor
	RegistroGenerico(unsigned int clave) : clave(clave) { }

	// Devuelve el identificador (clave) del registro
	unsigned int getClave() { return this->clave; }
};

#endif
