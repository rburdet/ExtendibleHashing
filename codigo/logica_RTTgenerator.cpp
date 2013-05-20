#include <string>
#include <fstream>
#include "logica_RTTgenerator.h"
#include "domain_RTTocurrencia.h"
#include <string.h>
#include "logica_SortExterno.h"



RTTgenerator::RTTgenerator(std::string path){
    //this->arbol = new ArbolBmas();
    this->temporalOcurrencias=path+".ocurrencias";
    this->palabras=path+".palabras";
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
    std::fstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(palabras.c_str(), std::fstream::in | std::fstream::out);
    if(file.good()){
        while(!file.eof() && !found){
            id = file.tellg();
            getline(file,p);
            if(!p.compare(palabra)){
                found=1;
            }
        }
    }
    file.close();
    if(!found){
        std::ofstream file2(palabras.c_str(), std::fstream::app);
        file2 << palabra << endl;
        file2.close();
    }

    return id;
}

int RTTgenerator::guardarOcurrencia(RTTocurrencia* ocur){
    std::ofstream file;
    file.open(temporalOcurrencias.c_str(),std::fstream::app);
    file << *ocur;
    file.close();
    return 0;
}


int RTTgenerator::pack(){
    SortExterno<RTTocurrencia>* sort = new SortExterno<RTTocurrencia>(this->temporalOcurrencias,4096);
    this->printOcurrencias();
    std::cout << "ORDENANDO" << endl;
    sort->ordenar();
    this->printOcurrencias();
    return  0;
}

int RTTgenerator::printOcurrencias(){
    std::ifstream file;
    file.open(temporalOcurrencias.c_str(), std::fstream::binary);
    RTTocurrencia* o = new RTTocurrencia;
    while(!file.eof()){
        file >> *o;
        std::cout << "(" << o->getPalabraId() << "," << o->getDocumentoId() << "," << o->getPosition() << ")";
    }
    file.close();
    return 0;
}
