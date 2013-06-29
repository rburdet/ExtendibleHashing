#include "logica_IndiceAutor.h"
#include "logica_SortExterno.h"
#include "domain_AutorId.h"
#include "domain_AutorReferencias.h"
#include "logica_Utils.h"
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <climits>

IndiceAutor::IndiceAutor(std::string path){
    this->temporalOcurrencias = path + ".ocurrenciasAutor";
    this->autores = path + ".autores";
    this->listaRefs = path + ".autoresRef";
    this->arbolName = path + ".arbolAutores";
    this->arbol = new ArbolBmas<AutorReferencias>();

}

IndiceAutor::~IndiceAutor(){
    delete arbol;
}


int IndiceAutor::agregar(unsigned int songPosition, std::string autor){
    unsigned int autorId = this->obtenerId(autor);
    AutorOcurrencia* ocur = new AutorOcurrencia(autorId, songPosition);
    this->guardarOcurrencia(ocur);
    delete ocur;
    return 0;
}

unsigned int IndiceAutor::obtenerId(std::string autor){
    std::ifstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(this->autores.c_str(), std::fstream::in | std::fstream::out);
    AutorId* aui = new AutorId();
    if(file.good()){
        file >> *aui;
        while(!file.eof() && !found){
            id = aui->getId();
            const char* c=aui->getAutor();
            if(!strcmp(c,autor.c_str())){
                found=1;
            }
            file >> *aui;
        }
    }
    file.close();
    delete aui;
    if(!found){
        std::ofstream file2(autores.c_str(), std::fstream::app);
        id = file2.tellp();
        AutorId* aid = new AutorId(autor.c_str(),id);
        file2 << *aid;
        delete aid;
        file2.close();
    }
    return id;
}

int IndiceAutor::buscarId(std::string autor){
    std::ifstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(this->autores.c_str());
    if(!file.good()){
        return -1;
    }
    AutorId* aui = new AutorId();
    if(file.good()){
        file >> *aui;
        while(!file.eof() && !found){
            id = aui->getId();
            const char* c=aui->getAutor();
            if(!strcmp(c,autor.c_str())){
                found=1;
            }
            file >> *aui;
        }
    }
    file.close();
    delete aui;
    if(!found){
        return -1;
    }
    return id;
}

/*unsigned int IndiceAutor::obtenerIdBinary(std::string autor){
return 0;
}*/

int IndiceAutor::guardarOcurrencia(AutorOcurrencia* ocur){
    std::ofstream file;
    file.open(temporalOcurrencias.c_str(),std::fstream::app);
    file << *ocur;
    file.close();
    return 0;
}

int IndiceAutor::pack(){
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Indexando autores...";
        std::cout.flush();
        if(Utils::existeArchivo(temporalOcurrencias)){
            SortExterno<AutorOcurrencia>* sort = new SortExterno<AutorOcurrencia>(this->temporalOcurrencias,4096);
            SortExterno<AutorId>* sort2 = new SortExterno<AutorId>(this->autores,4096);
            sort->ordenar();
            sort2->ordenar();
            delete sort;
            delete sort2;
            std::ifstream file;
            file.open(this->temporalOcurrencias.c_str());
            AutorOcurrencia aid;
            unsigned int autor = 0;
            unsigned int cant = 0;
            unsigned int refLista = 0;
            std::list<unsigned int>* lista;
            arbol->abrir(this->arbolName.c_str());
            file >> aid;
            while(!file.eof()){
                autor = aid.getAutorId();
                cant = 0;
                AutorReferencias* ar = new AutorReferencias(autor);
                refLista = this->getRefLista();
                lista = new std::list<unsigned int>();
                while((aid.getAutorId() == autor) && !file.eof()){
                    switch(cant){
                        case 0: ar->setRef1(aid.getDocumentoId());
                                break;
                        case 1:
                                ar->setRef2(aid.getDocumentoId());
                                break;
                        case 2:
                                ar->setRef3(aid.getDocumentoId());
                                break;
                        case 3:
                                ar->setRef4(aid.getDocumentoId());
                                break;
                        case 4:
                                ar->setRef5(aid.getDocumentoId());
                                break;
                        case 5: ar->setRefLista(refLista);
                        default: lista->push_back(aid.getDocumentoId());
                    }
                    cant++;
                    file >> aid;
                }
                ar->setCant(cant);
                if(cant > 5){
                    this->guardarRefLista(lista);
                }else{
                    ar->setRefLista(UINT_MAX);
                }
                arbol->insertar(ar->getClave(),*ar);
                delete ar;
                delete lista;
            }
            arbol->cerrar();
            remove(this->temporalOcurrencias.c_str());
            std::cout << "OK"<< std::endl;
        }
    }
    return  0;
}

int IndiceAutor::packAppend(){
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Indexando autores...";
        std::cout.flush();
        SortExterno<AutorOcurrencia>* sort = new SortExterno<AutorOcurrencia>(this->temporalOcurrencias,4096);
        SortExterno<AutorId>* sort2 = new SortExterno<AutorId>(this->autores,4096);
        sort->ordenar();
        sort2->ordenar();
        delete sort;
        delete sort2;
        std::ifstream file;
        file.open(this->temporalOcurrencias.c_str());
        AutorOcurrencia aid;
        unsigned int autor=0;
        unsigned int cant = 0;
        unsigned int refLista=0;
        std::list<unsigned int>* lista;
        arbol->abrir(this->arbolName.c_str());
        file >> aid;
        while(!file.eof()){
            autor = aid.getAutorId();
            cant = 0;
            AutorReferencias* ar = new AutorReferencias(autor);
            refLista = this->getRefLista();
            lista = new std::list<unsigned int>();
            bool b =arbol->buscar(autor,*ar);
            if(b){
                cant = ar->getCant();
                if(cant > 5){
                    copylist(ar->getRefLista(),lista);
                }
            }
            while((aid.getAutorId() == autor) && !file.eof()){
                switch(cant){
                    case 0: ar->setRef1(aid.getDocumentoId());
                            break;
                    case 1:
                            ar->setRef2(aid.getDocumentoId());
                            break;
                    case 2:
                            ar->setRef3(aid.getDocumentoId());
                            break;
                    case 3:
                            ar->setRef4(aid.getDocumentoId());
                            break;
                    case 4:
                            ar->setRef5(aid.getDocumentoId());
                            break;
                    case 5: ar->setRefLista(refLista);
                    default: lista->push_back(aid.getDocumentoId());
                }
                cant++;
                file >> aid;
            }
            ar->setCant(cant);
            if(cant > 5){
                this->guardarRefLista(lista);
            }else{
                ar->setRefLista(UINT_MAX);
            }
            if(b){
                arbol->actualizar(ar->getClave(),*ar);
            }else{
                arbol->insertar(ar->getClave(),*ar);
            }
            delete ar;
            delete lista;
        }
        arbol->cerrar();
        remove(this->temporalOcurrencias.c_str());
        std::cout << "OK"<< std::endl;
    }
    return  0;
}

int IndiceAutor::copylist(unsigned int reflista,std::list<unsigned int>*lista){
    std::ifstream file;
    file.open(this->listaRefs.c_str());
    file.seekg(reflista);
    unsigned int cant;
    file.read((char*)&cant,sizeof(unsigned int));
    for(unsigned int i=0;i<cant;i++){
        unsigned int ref;
        file.read((char*)&ref,sizeof(unsigned int));
        lista->push_back(ref);
    }
    return 0;
}

unsigned int IndiceAutor::getRefLista(){
    std::ofstream file;
    file.open(this->listaRefs.c_str(),std::fstream::app);
    unsigned int pos = file.tellp();
    file.close();
    return pos;
}

int IndiceAutor::guardarRefLista(std::list<unsigned int>* lista){
    unsigned int cant = lista->size();
    std::ofstream file;
    file.open(this->listaRefs.c_str(),std::fstream::app);
    file.write((char*)&cant,sizeof(cant));
    for(std::list<unsigned int>::iterator it = lista->begin();it != lista->end();it++){
        file.write((char*)&*it,sizeof(unsigned int));
    }
    file.close();
    return 0;
}

int IndiceAutor::printOcurrencias(){
    std::ifstream file;
    file.open(temporalOcurrencias.c_str(), std::fstream::binary);
    AutorOcurrencia* o = new AutorOcurrencia();
    file >> *o;
    while(!file.eof()){
        std::cout << "(" << o->getAutorId() << "," << o->getDocumentoId() << ")";
        file >> *o;
    }
    delete o;
    file.close();
    return 0;
}


int IndiceAutor::recuperar(std::string autor, std::list<unsigned int> *lista){
    int clave2 = this->buscarId(autor);
    if(clave2 == -1){
        return 0;
    }
    unsigned int clave = (unsigned int) clave2;
    arbol->abrir(this->arbolName.c_str());
    AutorReferencias* ar = new AutorReferencias();
    bool b = arbol->buscar(clave, *ar);
    if(b){
        unsigned int* refs = ar->getRefs();
        unsigned int stop = ar->getCant() > 5 ? 5: ar->getCant();
        for(unsigned int i=0;i<stop;i++){
            lista->push_back(refs[i]);
        }
        if(ar->getCant() > 5){
            std::ifstream file;
            file.open(this->listaRefs.c_str());
            file.seekg(ar->getRefLista());
            unsigned int cant;
            file.read((char*)&cant,sizeof(unsigned int));
            for(unsigned int i=0;i < cant;i++){
                unsigned int ref;
                file.read((char*)&ref,sizeof(unsigned int));
                lista->push_back(ref);
            }
            file.close();
        }
    }
    delete ar;
    arbol->cerrar();
    return 0;
}

int IndiceAutor::eliminarTodo(){
    arbol->abrir(arbolName.c_str());
    arbol->eliminar();
    remove(autores.c_str());
    remove(listaRefs.c_str());
    return 0;
}
