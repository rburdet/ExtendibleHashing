#ifndef LOGICA_RTTOCURRENCIA_H_INCLUDED
#define LOGICA_RTTOCURRENCIA_H_INCLUDED

#include "logica_Comparable.h"
#include <iostream>


/**
*   Autor: Pablo Rodriguez
*
*   RTTocurrencia representa un par de (idPalabra, idDoc,posicion)
*   Se utiliza para generar las listas invertidas
*/
class RTTocurrencia : public Comparable{
    public:
        RTTocurrencia(unsigned int palabraId, unsigned int documentoId, unsigned int position);
        RTTocurrencia();
        virtual ~RTTocurrencia();
        virtual int comparar(Comparable *c) const;
        unsigned int getPalabraId() const;
        unsigned int getDocumentoId() const;
        unsigned int getPosition() const;
        friend std::istream& operator >> (std::istream &out, RTTocurrencia &ocur);
        friend std::ostream& operator << (std::ostream &out, RTTocurrencia &ocur);
    private:
        unsigned int palabraId;
        unsigned int documentoId;
        unsigned int position;

};

#endif // LOGICA_RTTOCURRENCIA_H_INCLUDED
