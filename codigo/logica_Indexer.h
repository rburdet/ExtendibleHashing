#ifndef INDEXER_H_INCLUDED
#define INDEXER_H_INCLUDED

#include <iostream>
#include "logica_IndiceAutor.h"
#include "logica_IndiceTitulo.h"
#include "logica_RTTgenerator.h"


/**
*   Autor: Pablo Rodriguez
*
*   Es la clase madre de la Indexacion. Copia los temas al archivo maestro y va agregando las ocurrencias
*   a los diferentes titulos
*/
class Indexer{
    public:
        Indexer();
        ~Indexer();
        int indexarCancionesDesde(int mode); //0 scratch ; 1 append
        int eliminarTodo();
    private:
        int estaIndexado(std::string);
        int copyToMaster(std::string from, std::string to);
        int indexarAutores(std::string header, unsigned int songPosition);
        int indexarTitulo(std::string header, unsigned int songPosition);
        int generateRTT(std::string songPath, unsigned int songPosition);
        IndiceAutor *autores;
        IndiceTitulo *titulos;
        RTTgenerator *rtt;
        std::string fromPath;
        std::string dest;
        std::string masterName;
};



#endif // INDEXER_H_INCLUDED
