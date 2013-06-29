//
//  VarBuffer.h
//  Clase encargada del manejo de lecto/escritura desde disco
//  Esta clase no se encarga del manejo de registros sino que simplemente lee de disco a memoria o
//  escribe de memoria a disco
//  Esta disenada con el fin de trabajar un esquema de registros de longitud variable, donde cada
//  registro esta encabezado por un campo (2 bytes) donde se delimita la longitud del registro
//
//  Created by Federico Colangelo on 21/04/13.
//  Copyright (c) 2013 Federico Colangelo. All rights reserved.
//

#ifndef fisica_VarBuffer_h
#define fisica_VarBuffer_h

#include <iostream>
#include <fstream>

using namespace std;

class VarBuffer
{
    
public:
    VarBuffer(const unsigned int maxSize);
    ~VarBuffer();
    void clear();
    int read(fstream &file);
    int write(fstream &file);
    int pack(const void* object, unsigned short int size);
    int unpack(void* object);
    unsigned short int getBuffSize();
    char* getBuffer();
    
protected:
    unsigned short int bufferSize;
    unsigned short int maxSize;
    char* buffer;
};


#endif
