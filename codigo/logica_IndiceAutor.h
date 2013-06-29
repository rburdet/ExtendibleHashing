#ifndef INDICEAUTOR_H_INCLUDED
#define INDICEAUTOR_H_INCLUDED

#include <iostream>
#include "domain_AutorOcurrencia.h"
#include "logica_ArbolBmas.h"
#include "domain_AutorReferencias.h"
#include <list>



/**
*   Autor: Pablo Rodriguez
*
*   Es la clase que contiene la indexacion por Autor. Posee un arbol B+
*/
class IndiceAutor{
    public:
        IndiceAutor(std::string path);
        ~IndiceAutor();
        int agregar(unsigned int posArch, std::string autor);
        int recuperar(std::string autor, std::list<unsigned int> *lista);
        int eliminarTodo();
        int pack();
        int packAppend();
    private:
        unsigned int obtenerId(std::string palabra);
        int buscarId(std::string autor);
        int guardarOcurrencia(AutorOcurrencia* ocur);
        int printOcurrencias();
        unsigned int getRefLista();
        int guardarRefLista(std::list<unsigned int>* lista);
        ArbolBmas<AutorReferencias> *arbol;
        int copylist(unsigned int reflista,std::list<unsigned int>*lista);

        //UN ARBOL
        std::string temporalOcurrencias;
        std::string autores;
        std::string listaRefs;
        std::string arbolName;


};



#endif // INDICEAUTOR_H_INCLUDED
