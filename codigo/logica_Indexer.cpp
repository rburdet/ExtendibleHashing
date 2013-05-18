#include <iostream>
#include <fstream>
#include <sstream>
#include "fisica_DirHandler.h"
#include "logica_Indexer.h"
#include "logica_FileHandler.h"
#include "logica_Validator.h"
#include "logica_IndicePrimario.h"
#include "logica_Utils.h"

Indexer::Indexer(){
    fromPath="/home/pablo/tpDatos/7506-tp-grupo07/temas";
    destPath="/home/pablo/tpDatos/dest/.master";
    rtt = new RTTgenerator(destPath);
}

Indexer::~Indexer(){
    delete rtt;
}

int Indexer::eliminarTodo(){
    return 0;
}

int Indexer::indexarCancionesDesde(int mode){
    DirHandler *dirH = new DirHandler(fromPath.c_str());
    FileHandler *fileH = new FileHandler();
    dirH->getFiles(fromPath.c_str());
    std::list<string> *archivos = dirH->getFileList();
    std::list<string>::iterator it;
    for(it = archivos->begin(); it != archivos->end(); it++){
        cout << "Indexando archivo..  " << *it << endl;
        std::string header = fileH->getFirstLine(*it);
        cout << "Validando Header.. " << header << endl;
        int size;
        if(!(size = Validator::validateHeader(header))){
            std::cout << "Error en header. Descartado." << endl;
        }else{
            std::cout << "Header OK." << endl << "Verificando si ya esta indexada..";
            if (this->estaIndexado(header)){
                std::cout << "Archivo ya indexado. Descartado" << endl;
            }else{
                std::cout << "OK." << endl << "Indexando..";
                unsigned int songPosition = this->copyToMaster(*it, destPath);
                this->indexarAutores(header,songPosition);
                this->indexarTitulo(header,songPosition);
                this->generateRTT(*it,songPosition);


            }
        }
    }
    if(mode){
        //COSAS DE APPEND
    }
    delete dirH;
    return 0;
}


int Indexer::estaIndexado(std::string header){
    IndicePrimario *ind = new IndicePrimario;
    std::string s;
    s = Utils::getClaveFromHeader(header);
    std::cout << s << endl;
    if(ind->recuperar(s)){
        return 1;
    }
    return 0;
}

int Indexer::copyToMaster(std::string from, std::string to){
    std::ifstream source(from.c_str());
    std::ofstream dest(to.c_str(), std::fstream::app);
    unsigned int pos = dest.tellp();
    source.seekg(0,std::ios_base::end);
    unsigned int tam = source.tellg();
    source.seekg(0,std::ios_base::beg);
    dest.write((char*)&tam, sizeof(tam));
    //dest << tam;
    dest << source.rdbuf();
    source.close();
    dest.close();
    return pos;
}

int Indexer::indexarAutores(std::string header, unsigned int songPosition){
    std::list<std::string>* autores = new std::list<std::string>;
    Utils::getAutoresFromHeader(header,autores);
    std::list<std::string>::iterator it;
    IndiceAutor *indiceA = new IndiceAutor;
    for(it = autores->begin(); it != autores->end();it++){

    }
    delete autores;
    delete indiceA;
    return 0;
}

int Indexer::indexarTitulo(std::string header, unsigned int songPosition){
    std::string titulo = Utils::getTituloFromHeader(header);
    IndiceTitulo *indiceT = new IndiceTitulo;
    std::cout << "TITULO: " << titulo << endl;
    delete indiceT;
    return 0;
}

int Indexer::generateRTT(std::string songPath, unsigned int songPosition){
    std::ifstream f;
    std::string palabra;
    std::string linea;
    unsigned int position = 0;
    f.open(songPath.c_str());
    getline(f,linea); //descarto el header
    while(getline(f,linea)){
        std::istringstream lineaS(linea);
        while(getline(lineaS,palabra,' ')){
            palabra = Utils::uniformizarString(palabra);
            std::cout << palabra << endl;
            position++;
            rtt->indexarPalabra(palabra, songPosition,position);

        }
    }
    return 0;
}



