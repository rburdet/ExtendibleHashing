#ifndef INDICETITULO_H_INCLUDED
#define INDICETITULO_H_INCLUDED

#include <iostream>
#include "domain_AutorOcurrencia.h"
#include "domain_TituloReferencias.h"
#include <list>
#include "logica_BlockTable.h"



/**
*   Autor: Pablo Rodriguez
*
*   Es la clase que contiene la indexacion por Titulo. Posee un Hash Extensible.
*/
class IndiceTitulo{
    public:
        IndiceTitulo(std::string path);
        IndiceTitulo();
        ~IndiceTitulo();
        int agregar(unsigned int posArch, std::string titulo);
        int recuperar(std::string titulo, std::list<unsigned int> *lista);
        int recuperarPlus(std::string titulo, std::list<unsigned int> *lista);
        int eliminarTodo();
        int pack();
        int packAppend();
    private:
        unsigned int obtenerId(std::string palabra);
        int buscarId(std::string titulo);
        int guardarOcurrencia(AutorOcurrencia* ocur);
        int printOcurrencias();
        unsigned int getRefLista();
        int guardarRefLista(std::list<unsigned int>* lista);
        int copylist(unsigned int reflista,std::list<unsigned int>*lista);
        BlockTable<TituloReferencias>* hash;

        std::string temporalOcurrencias;
        std::string titulos;
        std::string listaRefs;
        std::string tableName;
        std::string blocksName;

};

#endif // INDICETITULO_H_INCLUDED
