#include <string>
#include <fstream>
#include "logica_RTTgenerator.h"
#include "domain_RTTocurrencia.h"
#include <string.h>
#include "logica_SortExterno.h"
#include "logica_Utils.h"



RTTgenerator::RTTgenerator(std::string path){
    this->temporalOcurrencias=path+".ocurrencias";
    this->palabras=path+".palabras";
    this->nonIndexableWords=path+ ".palabrasNoIndexables";
    this->listasInvertidasDocumentos=path+ ".listaDocumentos";
    this->listasInvertidasPosiciones=path+ ".listaPosiciones";
    this->arbolName = path + ".arbolRTT";
    this->arbol = new ArbolBmas<RTTreferencia>();
}

RTTgenerator::~RTTgenerator(){
    delete arbol;
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

int RTTgenerator::buscarId(std::string palabra){
    std::ifstream file;
    std::string p;
    int found = 0;
    unsigned int id =0;
    file.open(this->palabras.c_str());
    if(!file.good()){
        return -1;
    }
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
        return -1;
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
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Generando RTT...";
        std::cout.flush();
        SortExterno<RTTocurrencia>* sort = new SortExterno<RTTocurrencia>(this->temporalOcurrencias,4096);
        sort->ordenar();
        delete sort;
        std::ifstream file;
        file.open(this->temporalOcurrencias.c_str());
        RTTocurrencia ocur;
        unsigned int idPalabra = 0;
        unsigned int idDoc = 0;
        unsigned int cantDocs = 0;
        unsigned int cantPos = 0;
        unsigned int refListaDocs;
        unsigned int refListaPos;
        std::list<RTTreferencia*>* listaDocs;
        std::list<unsigned int>* listaPos;
        arbol->abrir(this->arbolName.c_str());
        file >> ocur;
        while(!file.eof()){
            idPalabra = ocur.getPalabraId();
            cantDocs = 0;
            RTTreferencia* ar = new RTTreferencia(idPalabra);
            refListaDocs = this->getRefListaDocs();
            ar->setRefLista(refListaDocs);
            listaDocs = new std::list<RTTreferencia*>();

            while((ocur.getPalabraId() == idPalabra) && !file.eof()){
                idDoc = ocur.getDocumentoId();
                cantPos = 0;
                refListaPos = this->getRefListaPos();
                RTTreferencia* refPos = new RTTreferencia(idDoc);
                refPos->setRefLista(refListaPos);
                listaPos = new std::list<unsigned int>;
                while((ocur.getDocumentoId() == idDoc) && (ocur.getPalabraId() == idPalabra) && !file.eof()){
                    listaPos->push_back(ocur.getPosition());
                    cantPos++;
                    file >> ocur;
                }

                this->guardarListaPos(listaPos);
                delete listaPos;
                listaDocs->push_back(refPos);
                cantDocs++;
            }
            this->guardarListaDocs(listaDocs);
            arbol->insertar(ar->getClave(),*ar);
            delete ar;
            delete listaDocs;
        }
        arbol->cerrar();
        remove(this->temporalOcurrencias.c_str());
        std::cout << "OK" << std::endl;
    }
    return  0;
}

int RTTgenerator::packAppend(){
    if(Utils::existeArchivo(temporalOcurrencias)){
        std::cout << "Actualizando RTT...";
        std::cout.flush();
        SortExterno<RTTocurrencia>* sort = new SortExterno<RTTocurrencia>(this->temporalOcurrencias,4096);
        sort->ordenar();
        delete sort;
        std::ifstream file;
        file.open(this->temporalOcurrencias.c_str());
        RTTocurrencia ocur;
        unsigned int idPalabra;
        unsigned int idDoc;
        unsigned int cantDocs = 0;
        unsigned int cantPos = 0;
        unsigned int refListaDocs;
        unsigned int refListaPos;
        std::list<RTTreferencia*>* listaDocs;
        std::list<unsigned int>* listaPos;
        arbol->abrir(this->arbolName.c_str());
        file >> ocur;
        while(!file.eof()){
            idPalabra = ocur.getPalabraId();
            cantDocs = 0;
            RTTreferencia* ar = new RTTreferencia(idPalabra);
            refListaDocs = this->getRefListaDocs();
            listaDocs = new std::list<RTTreferencia*>;

            bool b = arbol->buscar(idPalabra,*ar);
            if(b){
                copyList(ar->getRefLista(),listaDocs);
            }
            ar->setRefLista(refListaDocs);
            while((ocur.getPalabraId() == idPalabra) && !file.eof()){
                idDoc = ocur.getDocumentoId();
                cantPos = 0;
                refListaPos = this->getRefListaPos();
                RTTreferencia* refPos = new RTTreferencia(idDoc);
                refPos->setRefLista(refListaPos);
                listaPos = new std::list<unsigned int>;
                while((ocur.getDocumentoId() == idDoc) && (ocur.getPalabraId() == idPalabra) && !file.eof()){
                    listaPos->push_back(ocur.getPosition());
                    cantPos++;
                    file >> ocur;
                }

                this->guardarListaPos(listaPos);
                delete listaPos;
                listaDocs->push_back(refPos);
                cantDocs++;
            }
            this->guardarListaDocs(listaDocs);
            if(b){
                arbol->actualizar(ar->getClave(),*ar);
            }else{
                arbol->insertar(ar->getClave(),*ar);
            }
            delete ar;
            delete listaDocs;
        }
        arbol->cerrar();
        remove(this->temporalOcurrencias.c_str());
        std::cout << "OK" << std::endl;
    }
    return  0;
}



unsigned int RTTgenerator::getRefListaDocs(){
    std::ofstream file;
    file.open(this->listasInvertidasDocumentos.c_str(),std::fstream::app);
    unsigned int pos = file.tellp();
    file.close();
    return pos;
}


unsigned int RTTgenerator::getRefListaPos(){
    std::ofstream file;
    file.open(this->listasInvertidasPosiciones.c_str(),std::fstream::app);
    unsigned int pos = file.tellp();
    file.close();
    return pos;
}

int RTTgenerator::copyList(unsigned int refLista,std::list<RTTreferencia*> *lista){
    std::ifstream file;
    file.open(this->listasInvertidasDocumentos.c_str());
    file.seekg(refLista);
    unsigned int cant;
    file.read((char*)&cant,sizeof(unsigned int));
    for(unsigned int i=0;i<cant;i++){
        RTTreferencia *oc = new RTTreferencia();
        unsigned int p;
        file.read((char*)&p,sizeof(p));
        oc->setClave(p);
        file.read((char*)&p,sizeof(p));
        oc->setRefLista(p);
        lista->push_back(oc);
    }
    return 0;
}

int RTTgenerator::guardarListaDocs(std::list<RTTreferencia*> *lista){
    unsigned int cant = lista->size();
    std::ofstream file;
    file.open(this->listasInvertidasDocumentos.c_str(),std::fstream::app);
    file.write((char*)&cant,sizeof(cant));
    for(std::list<RTTreferencia*>::iterator it = lista->begin();it != lista->end();it++){
        unsigned int p = (*it)->getClave();
        file.write((char*)&p,sizeof(unsigned int));
        p = (*it)->getRefLista();
        file.write((char*)&p,sizeof(unsigned int));
        delete *it;
    }
    file.close();
    return 0;
}

int RTTgenerator::guardarListaPos(std::list<unsigned int> *lista){
    unsigned int cant = lista->size();
    std::ofstream file;
    file.open(this->listasInvertidasPosiciones.c_str(),std::fstream::app);
    file.write((char*)&cant,sizeof(cant));
    for(std::list<unsigned int>::iterator it = lista->begin();it != lista->end();it++){
        file.write((char*)&*it,sizeof(unsigned int));
    }
    file.close();
    return 0;
}

int RTTgenerator::printOcurrencias(){
    std::ifstream file;
    file.open(temporalOcurrencias.c_str(), std::fstream::binary);
    RTTocurrencia* o = new RTTocurrencia;
    file >> *o;
    while(!file.eof()){
        std::cout << "(" << o->getPalabraId() << "," << o->getDocumentoId() << "," << o->getPosition() << ")";
        file >> *o;
    }
    delete o;
    file.close();
    return 0;
}

int RTTgenerator::eliminarTodo(){
    remove(palabras.c_str());
    remove(listasInvertidasDocumentos.c_str());
    remove(listasInvertidasPosiciones.c_str());
    arbol->abrir(arbolName.c_str());
    arbol->eliminar();
    return 0;
}

int RTTgenerator::recuperar(std::string frase, std::list<unsigned int> *lista){
    std::ifstream file;
    file.open(arbolName.c_str());
    if(!file.good()){
        return 0;
    }
    std::list<std::string> *palabras = new std::list<std::string>;
    Utils::splitString(frase,' ',palabras);
    std::string palabra;
    unsigned int idPalabra;
    int id = 0;
    RTTreferencia *rttRef = new RTTreferencia;
    bool b;
    unsigned int cantDocs;
    unsigned int refListaDocs;
    arbol->abrir(this->arbolName.c_str());
    int flag = 0;
    std::list<std::list<RTTreferencia*>*>* listaMadre = new std::list<std::list<RTTreferencia*>*>;
    for(std::list<std::string>::iterator it = palabras->begin(); it != palabras->end() && !flag;it++){
        palabra = Utils::uniformizarString(*it);
        id = this->buscarId(palabra);
        if(id == -1){
            flag =1;
            return 0;
        }
        idPalabra = (unsigned int) id;
        b = arbol->buscar(idPalabra, *rttRef);
        if(b){
            refListaDocs = rttRef->getRefLista();
            std::ifstream file;
            file.open(this->listasInvertidasDocumentos.c_str());
            file.seekg(refListaDocs);
            file.read((char*)&cantDocs,sizeof(cantDocs));
            std::list<RTTreferencia*>* listaDocs = new std::list<RTTreferencia*>;
            for(unsigned int i=0;i<cantDocs;i++){
                RTTreferencia *oc = new RTTreferencia();
                unsigned int p;
                file.read((char*)&p,sizeof(p));
                oc->setClave(p);
                file.read((char*)&p,sizeof(p));
                oc->setRefLista(p);
                listaDocs->push_back(oc);
            }
            listaMadre->push_back(listaDocs);

        }else{
            return 1;
        }
    }
    delete rttRef;
    arbol->cerrar();
    if(!flag){
        cantDocs = this->intersecarListas(listaMadre);


        std::list<std::list<RTTreferencia*>*>::iterator itMadre;
        itMadre=listaMadre->begin();
        std::list<RTTreferencia*> *listaDocs = *itMadre;
        std::list<RTTreferencia*>::iterator itDocs;
        unsigned int j = 0;
        if(listaMadre->size() > 1){
            for(itDocs = listaDocs->begin(); itDocs != listaDocs->end(); itDocs++){
                RTTreferencia* ocur = *itDocs;
                unsigned int refPos = ocur->getRefLista();
                unsigned int cantPos;
                std::ifstream file;
                file.open(this->listasInvertidasPosiciones.c_str());
                file.seekg(refPos);
                file.read((char*)&cantPos,sizeof(cantPos));
                unsigned posOk = 0;
                for(unsigned int i=0; i<cantPos; i++){
                    unsigned int posInicial;
                    file.read((char*)&posInicial,sizeof(posInicial));
                    itMadre = listaMadre->begin();
                    unsigned int posRelativa=1;
                    itMadre++;
                    while(itMadre != listaMadre->end() && !posOk){
                        std::list<RTTreferencia*> *listaDocs2 = *itMadre;
                        std::list<RTTreferencia*>::iterator itDocs2 = listaDocs2->begin();
                        for (unsigned int k=0;k<j;k++){
                            itDocs2++;
                        }

                        RTTreferencia* ocur2 = *itDocs2;
                        unsigned int refPos2 = ocur2->getRefLista();
                        unsigned int cantPos2;
                        std::ifstream file2;
                        file2.open(this->listasInvertidasPosiciones.c_str());
                        file2.seekg(refPos2);
                        file2.read((char*)&cantPos2,sizeof(cantPos2));
                        for(unsigned int k=0; k<cantPos2;k++){
                            unsigned int pos2;
                            file2.read((char*)&pos2,sizeof(pos2));
                            if(pos2 == posInicial + posRelativa ){
                                posOk = 1;
                            }
                        }
                        file2.close();
                        itMadre++;
                        posRelativa++;
                    }
                }
                file.close();
                if(posOk){
                    lista->push_back(ocur->getClave());
                }
            }
        }else{
            for(itDocs = listaDocs->begin(); itDocs != listaDocs->end(); itDocs++){
                RTTreferencia* ocur = *itDocs;
                lista->push_back(ocur->getClave());
            }
        }
    }

    //DELETES DE TODO LO QUE TIENE LISTA MADRE
    std::list<std::list<RTTreferencia*>*>::iterator itMadre;
    std::list<RTTreferencia*> *listaDocs;
    std::list<RTTreferencia*>::iterator itDocs;
    itMadre = listaMadre->begin();
    while(itMadre != listaMadre->end()){
        listaDocs = *itMadre;
        itDocs= listaDocs->begin();
        while(itDocs != listaDocs->end()){
            delete *itDocs;
            itDocs++;
        }
        delete *itMadre;
        itMadre++;
    }
    delete listaMadre;

    return 0;
}

unsigned int RTTgenerator::intersecarListas(std::list<std::list<RTTreferencia*>*>* listaMadre){
    int finished = 0;
    unsigned int candidato=0;
    unsigned int cant=0;
    int ok = 1;
    RTTreferencia* ref;
    while(!finished){
        ok=1;
        std::list<std::list<RTTreferencia*>*>::iterator itMadre = listaMadre->begin();
        while(itMadre != listaMadre->end() && ok && !finished){
            std::list<RTTreferencia*>* lista = *itMadre;
            std::list<RTTreferencia*>::iterator itLista = lista->begin();
            for (unsigned int i=0;i<cant;i++){
                itLista++;
            }
            int found = 0;
            while(itLista != lista->end() && !found && ok){
                ref = *itLista;
                if(candidato > ref->getClave()){
                    delete ref;
                    itLista = lista->erase(itLista);
                }else if(candidato == ref->getClave()){
                    found = 1;
                }else{
                    candidato = ref->getClave();
                    ok=0;
                }
            }
            if(itLista == lista->end()){
                finished =1;
            }
            itMadre++;
        }
        if(ok && !finished){
            cant++;
        }
    }

    return cant;
}
