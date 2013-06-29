#include "logica_IndiceTitulo.h"
#include "domain_AutorId.h"
#include "logica_SortExterno.h"
#include "domain_TituloReferencias.h"
#include "logica_Utils.h"
#include <fstream>
#include <string.h>
#include <climits>

IndiceTitulo::IndiceTitulo(std::string path){
    this->temporalOcurrencias = path + ".ocurrenciasTitulo";
    this->titulos = path + ".titulos";
    this->listaRefs = path + ".titulosRef";
    this->tableName =path+".hashTable";
    this->blocksName=path+".hashTitulos";
    this->hash = new BlockTable<TituloReferencias>(tableName,blocksName,hashBlockSize());
}

IndiceTitulo::IndiceTitulo(){
}

IndiceTitulo::~IndiceTitulo(){
    delete hash;
}


int IndiceTitulo::agregar(unsigned int songPosition, std::string titulo){
    unsigned int tituloId = this->obtenerId(titulo);
    AutorOcurrencia* ocur = new AutorOcurrencia(tituloId, songPosition);
    this->guardarOcurrencia(ocur);
    delete ocur;
    return 0;
}

unsigned int IndiceTitulo::obtenerId(std::string titulo){
    std::ifstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(this->titulos.c_str(), std::fstream::in | std::fstream::out);
    AutorId* aui = new AutorId();
    if(file.good()){
        file >> *aui;
        while(!file.eof() && !found){
            id = aui->getId();
            const char* c=aui->getAutor();
            if(!strcmp(c,titulo.c_str())){
                found=1;
            }
            file >> *aui;
        }
    }
    file.close();
    delete aui;
    if(!found){
        std::ofstream file2;
        file2.open(titulos.c_str(), std::fstream::app);
        id = file2.tellp();
        AutorId* aid = new AutorId(titulo.c_str(),id);
        file2 << *aid;
        file2.close();
        delete aid;
    }
    return id;
}

int IndiceTitulo::buscarId(std::string titulo){
    std::ifstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(this->titulos.c_str());
    if(!file.good()){
        return -1;
    }
    AutorId* aui = new AutorId();
    if(file.good()){
        file >> *aui;
        while(!file.eof() && !found){
            id = aui->getId();
            const char* c=aui->getAutor();
            if(!strcmp(c,titulo.c_str())){
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

int IndiceTitulo::guardarOcurrencia(AutorOcurrencia* ocur){
    std::ofstream file;
    file.open(temporalOcurrencias.c_str(),std::fstream::app);
    file << *ocur;
    file.close();
    return 0;
}

int IndiceTitulo::pack(){
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Indexando titulos...";
        std::cout.flush();
        SortExterno<AutorOcurrencia>* sort = new SortExterno<AutorOcurrencia>(this->temporalOcurrencias,4096);
        sort->ordenar();
        delete sort;
        std::ifstream file;
        file.open(this->temporalOcurrencias.c_str());
        AutorOcurrencia aid;
        unsigned int titulo = 0;
        unsigned int cant = 0;
        unsigned int refLista = 0;
        std::list<unsigned int>* lista;
        file >> aid;
        while(!file.eof()){
            titulo = aid.getAutorId();
            cant = 0;
            TituloReferencias* ar = new TituloReferencias(titulo);
            refLista = this->getRefLista();
            lista = new std::list<unsigned int>();
            while((aid.getAutorId() == titulo) && !file.eof()){
                switch(cant){
                    case 0: ar->setRef1(aid.getDocumentoId());
                            break;
                    case 1:
                            ar->setRef2(aid.getDocumentoId());
                            break;
                    case 2:
                            ar->setRef3(aid.getDocumentoId());
                            break;
                    case 3: ar->setRefLista(refLista);
                    default: lista->push_back(aid.getDocumentoId());
                }
                cant++;
                file >> aid;
            }
            ar->setCant(cant);
            if(cant > 3){
                this->guardarRefLista(lista);
            }else{
                ar->setRefLista(UINT_MAX);
            }
            hash->insert(ar);
            delete ar;
            delete lista;
        }
        cout << "OK" <<  endl;
        remove(this->temporalOcurrencias.c_str());
    }
    return  0;
}

int IndiceTitulo::packAppend(){
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Indexando titulos...";
        std::cout.flush();
        SortExterno<AutorOcurrencia>* sort = new SortExterno<AutorOcurrencia>(this->temporalOcurrencias,4096);
        sort->ordenar();
        delete sort;
        std::ifstream file;
        file.open(this->temporalOcurrencias.c_str());
        AutorOcurrencia aid;
        unsigned int titulo=0;
        unsigned int cant = 0;
        unsigned int refLista=0;
        std::list<unsigned int>* lista;
        file >> aid;
        while(!file.eof()){
            titulo = aid.getAutorId();
            cant = 0;
            TituloReferencias* ar = new TituloReferencias(titulo);
            refLista = this->getRefLista();
            lista = new std::list<unsigned int>();
            bool b = hash->search(ar);
            if(b){
                cant = ar->getCant();
                copylist(refLista,lista);
            }
            while((aid.getAutorId() == titulo) && !file.eof()){
                switch(cant){
                    case 0: ar->setRef1(aid.getDocumentoId());
                            break;
                    case 1:
                            ar->setRef2(aid.getDocumentoId());
                            break;
                    case 2:
                            ar->setRef3(aid.getDocumentoId());
                            break;
                    case 3: ar->setRefLista(refLista);
                    default: lista->push_back(aid.getDocumentoId());
                }
                cant++;
                file >> aid;
            }
            ar->setCant(cant);
            if(cant > 3){
                this->guardarRefLista(lista);
            }else{
                ar->setRefLista(UINT_MAX);
            }
			if (b){
				hash->actualizar(*ar);		
			}
            hash->insert(ar);
            delete ar;
            delete lista;
        }
        cout << "OK" <<  endl;
        remove(this->temporalOcurrencias.c_str());
    }
    return  0;
}

int IndiceTitulo::copylist(unsigned int reflista,std::list<unsigned int>*lista){
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

unsigned int IndiceTitulo::getRefLista(){
    std::ofstream file;
    file.open(this->listaRefs.c_str(),std::fstream::app);
    unsigned int pos = file.tellp();
    file.close();
    return pos;
}

int IndiceTitulo::guardarRefLista(std::list<unsigned int>* lista){
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

int IndiceTitulo::printOcurrencias(){
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

int IndiceTitulo::recuperarPlus(std::string titulo,std::list<unsigned int> *lista){
    this->recuperar(titulo,lista);
    unsigned int id = this->obtenerId(titulo);
    std::ifstream file;
    if(Utils::existeArchivo(temporalOcurrencias)){
        file.open(temporalOcurrencias.c_str(), std::fstream::binary);
        AutorOcurrencia* o = new AutorOcurrencia();
        file >> *o;
        while(!file.eof()){
            if(o->getAutorId() == id){
                lista->push_back(o->getDocumentoId());
            }
            file >> *o;
        }
        file.close();
        delete o;
    }
    return 0;
}

int IndiceTitulo::recuperar(std::string titulo, std::list<unsigned int> *lista){
    int clave2 = this->buscarId(titulo);
    if(clave2 == -1){
        return 0;
    }
    unsigned int clave = (unsigned int) clave2;
    TituloReferencias* ar = new TituloReferencias();
    ar->setClave(clave);
    bool b = hash->search(ar);
    if(b){
        unsigned int* refs = ar->getRefs();
        unsigned int stop = ar->getCant() > 3 ? 3: ar->getCant();
        for(unsigned int i=0;i<stop;i++){
            lista->push_back(refs[i]);
        }
        if(ar->getCant() > 3){
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
    return 0;
}

int IndiceTitulo::eliminarTodo(){
    remove(listaRefs.c_str());
    remove(titulos.c_str());
    remove(tableName.c_str());
    remove(blocksName.c_str());
    delete this->hash;
    this->hash = new BlockTable<TituloReferencias>(tableName,blocksName,hashBlockSize());

    return 0;
}
