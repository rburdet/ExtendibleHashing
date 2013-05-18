#include "domain_AutorId.h"
#include <string.h>

AutorId::AutorId(char* autor, unsigned int id){
    memcpy(this->autor,autor,100);
    this->id =id;
}

AutorId::~AutorId(){
}

int AutorId::comparar(Comparable *obj) const{
    return 1;
}

unsigned int AutorId::getId() const{
    return this->id;
}

const char* AutorId::getAutor() const{
    return this->autor;
}
