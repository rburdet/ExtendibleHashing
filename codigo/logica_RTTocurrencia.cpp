#include "logica_RTTocurrencia.h"


RTTocurrencia::RTTocurrencia(unsigned int palabraId, unsigned int documentoId, unsigned int position){
    this->palabraId=palabraId;
    this->documentoId=documentoId;
    this->position=position;
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
