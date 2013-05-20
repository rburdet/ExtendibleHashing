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
	RegistroGenerico() { }

	// Setea el identificador (clave) del registro
	void setClave(unsigned int clave) { this->clave = clave; }

	// Devuelve el identificador (clave) del registro
	unsigned int getClave() { return this->clave; }
};

#endif
