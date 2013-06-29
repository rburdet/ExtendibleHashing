//
//  RegistroGenerico.h
//	[ Insertar documentacion ]
//


#ifndef REGISTRO_GENERICO_H
#define REGISTRO_GENERICO_H


#include "fisica_SerialBuffer.h"



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

	virtual ~RegistroGenerico(){}

	// Setea el identificador (clave) del registro
	void setClave(unsigned int clave) { this->clave = clave; }

	// Devuelve el identificador (clave) del registro
	unsigned int getClave() { return this->clave; }

	// Serializa el registro.
	// PRE: 'buffer' es el buffer en donde se serializara el registro para
	// poder ser almacenada en algun medio.
	virtual int serializar(SerialBuffer *buffer)
	{
		unsigned int clv = this->clave;
		if(buffer->pack(&clv, sizeof(unsigned int)))
			return 0;
		return 1;
	}

	// Deserializa una lista.
	// PRE: 'buffer' es el buffer en donde se encuentra serializado el registro
	// POST: el estado del registro se actualiza con los datos obtenidos del
	// buffer.
	virtual void deserializar(SerialBuffer *buffer)
	{
		unsigned int clv;
		buffer->unpack(&clv);
		this->clave = clv;
	}

	// Devuelve el tamanio en bytes que ocuparia persistir la clase.
	static size_t getTamanioEnBytes()
	{
		return (sizeof(unsigned int) + sizeof(unsigned short int));
	}
};

#endif
