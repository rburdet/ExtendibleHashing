#ifndef LOGICA_RTTOCURRENCIA_H_INCLUDED
#define LOGICA_RTTOCURRENCIA_H_INCLUDED

#include "logica_Comparable.h"

class RTTocurrencia : public Comparable{
    public:
        RTTocurrencia(unsigned int palabraId, unsigned int documentoId, unsigned int position);
        ~RTTocurrencia();
        virtual int comparar(Comparable *c) const;
        unsigned int getPalabraId() const;
        unsigned int getDocumentoId() const;
        unsigned int getPosition() const;
    private:
        unsigned int palabraId;
        unsigned int documentoId;
        unsigned int position;

};

#endif // LOGICA_RTTOCURRENCIA_H_INCLUDED
