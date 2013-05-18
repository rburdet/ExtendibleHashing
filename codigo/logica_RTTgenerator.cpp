#include <string>
#include <fstream>
#include "logica_RTTgenerator.h"
#include "logica_RTTocurrencia.h"
#include <string.h>



RTTgenerator::RTTgenerator(std::string path){
    //this->arbol = new ArbolBmas();
    this->temporalOcurrencias=path+".ocurrencias";
    this->nonIndexableWords=path+ ".palabrasNoIndexables";
    this->listasInvertidasDocumentos=path+ ".listaDocumentos";
    this->listasInvertidasPosiciones=path+ ".listaPosiciones";
}

RTTgenerator::~RTTgenerator(){
//    delete arbol;
}

int RTTgenerator::indexarPalabra(std::string palabra, unsigned int song, unsigned int position){
    if(esIndexable(palabra)){
        unsigned int idPalabra = this->obtenerId(palabra);
        RTTocurrencia ocurrencia(idPalabra, song, position);
        this->guardarOcurrencia(&ocurrencia);
    }
    return 0;
}

int RTTgenerator::esIndexable(std::string){
    return 1;
}

unsigned int RTTgenerator::obtenerId(std::string palabra){
    return 0;
}

int RTTgenerator::guardarOcurrencia(RTTocurrencia* ocur){
    std::ofstream file;
    file.open(temporalOcurrencias.c_str(),std::fstream::app);
    unsigned int tam = sizeof(*ocur);
    const char* data = new char[tam];
    memcpy(ocur,data,tam);
    file.write(data, tam);
    file.close();
    delete[] data;
    return 0;
}


