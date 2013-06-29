#ifndef BUSCADOR_H_INCLUDED
#define BUSCADOR_H_INCLUDED

#include "logica_IndiceAutor.h"
#include "logica_IndiceTitulo.h"
#include "logica_RTTgenerator.h"
#include "logica_Estadista.h"


/**
*   Autor: Pablo Rodriguez
*
*   Es la clase madre en donde se resuelven las busquedas por autor, titulo y RTT. Delega parte de su
*   comportamiento en los diferentes indices.
*/
class Buscador{
    public:
        Buscador(Estadista *estadista);
        ~Buscador();
        int buscarPorAutor(std::string autor);
        int buscarPorTitulo(std::string titulo);
        int buscarPorFrase(std::string frase);
        int listarTodo();
    private:
        int imprimirCancion(unsigned int ref);
        IndiceAutor *autores;
        IndiceTitulo *titulos;
        RTTgenerator *rtt;
        Estadista *estadista;
        std::string dest;
        std::string outName;
};


#endif // BUSCADOR_H_INCLUDED
