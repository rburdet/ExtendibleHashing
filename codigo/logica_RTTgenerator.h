#ifndef LOGICA_RTTGENERATOR_H_INCLUDED
#define LOGICA_RTTGENERATOR_H_INCLUDED

#include "logica_ArbolBmas.h"
#include "logica_RTTocurrencia.h"

class RTTgenerator{
    public:
        RTTgenerator(std::string path);
        ~RTTgenerator();
        int indexarPalabra(std::string palabra, unsigned int song, unsigned int position);
    private:
        int esIndexable(std::string palabra);
        unsigned int obtenerId(std::string palabra);
        int guardarOcurrencia(RTTocurrencia* ocur);
        //ArbolBmas<> *arbol;
        std::string temporalOcurrencias;
        std::string nonIndexableWords;
        std::string listasInvertidasDocumentos;
        std::string listasInvertidasPosiciones;
};


#endif // LOGICA_RTTGENERATOR_H_INCLUDED
