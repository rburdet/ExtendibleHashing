#include "domain_RTTocurrencia.h"
#include <iostream>

RTTocurrencia::RTTocurrencia(unsigned int palabraId, unsigned int documentoId, unsigned int position){
    this->palabraId=palabraId;
    this->documentoId=documentoId;
    this->position=position;
}

RTTocurrencia::RTTocurrencia(){
}

RTTocurrencia::~RTTocurrencia(){
}

int RTTocurrencia::comparar(Comparable *c) const{
    RTTocurrencia *ocur = (RTTocurrencia*) c;
    if(this->palabraId > ocur->palabraId) return 1;
    if(this->palabraId < ocur->palabraId) return -1;
    if(this->documentoId > ocur->documentoId) return 1;
    if(this->documentoId < ocur->documentoId) return -1;
    if(this->position > ocur->position) return 1;
    if(this->position < ocur->position) return -1;
    return 0;
}

unsigned int RTTocurrencia::getDocumentoId() const{
    return documentoId;
}

unsigned int RTTocurrencia::getPalabraId() const{
    return palabraId;
}

unsigned int RTTocurrencia::getPosition() const{
    return position;
}

std::ostream& operator << (std::ostream &out, RTTocurrencia &ocur){
    unsigned int p = ocur.getPalabraId();
    out.write((char*) &p,sizeof(unsigned int));
    p = ocur.getDocumentoId();
    out.write((char*) &p,sizeof(unsigned int));
    p = ocur.getPosition();
    out.write((char*) &p,sizeof(unsigned int));
    return out;
}

std::istream& operator >> (std::istream &out, RTTocurrencia &ocur){
    unsigned int p;
    out.read((char*) &p,sizeof(unsigned int));
    ocur.palabraId = p;
    out.read((char*) &p,sizeof(unsigned int));
    ocur.documentoId = p;
    out.read((char*) &p,sizeof(unsigned int));
    ocur.position = p;
    return out;
}
