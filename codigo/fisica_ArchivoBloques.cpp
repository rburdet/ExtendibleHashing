//
//  fisica_ArchivosBloques.cpp
//  FileHandler
//
//  Created by Federico Colangelo on 12/05/13.
//  Copyright (c) 2013 Federico Colangelo. All rights reserved.
//

#include "fisica_ArchivoBloques.h"

ArchivoBloques::ArchivoBloques(unsigned int blockSize,const char* filename) : IOBuffer(blockSize)
{
    this->filename=filename;
    this->blockSize=blockSize;
}


int ArchivoBloques::crearArchivo()
{
    this->file.open(this->filename, ios::in|ios::binary);
    
    if (this->file.good()) {
        this->file.close();
        this->file.open(this->filename,ios::trunc|ios::binary);
        this->file.close();
        return 1;
    } else {
        this->file.close();
        this->file.open(this->filename, ios::out|ios::binary);
        this->file.close();
        return 0;
    }
    
}

void ArchivoBloques::borrarArchivo()
{
    if (this->estaAbierto()) {
        file.close();
    }
    remove(filename);
}


int ArchivoBloques::abrirArchivo()
{
    this->file.open(this->filename, ios::in|ios::binary);
    
    if (this->file.good()) {
        this->file.close();
        this->file.open(this->filename,ios::in|ios::out|ios::binary);
        return 0;
    } else return -1;
}


void ArchivoBloques::cerrarArchivo()
{
    this->file.close();
}

bool ArchivoBloques::estaAbierto()
{
    return this->file.is_open();
}



int ArchivoBloques::escribirBloque(const void *registro, unsigned int numeroBloque, unsigned int size)
{
    
    unsigned short int datosEscritos=0;
    
    if (!this->estaAbierto()) {
        return -1;
    }
    
    file.seekp(numeroBloque*this->blockSize);
    IOBuffer.pack(registro,size);
    IOBuffer.write(file);
    datosEscritos=IOBuffer.getBuffSize();
    IOBuffer.clear();
    
    return datosEscritos;
}

int ArchivoBloques::leerBloque(void *registro, unsigned int numeroBloque)
{
    unsigned short int datosLeidos=0;
    
    if (!this->estaAbierto()) {
        return -1;
    }
    
    file.seekg(numeroBloque*this->blockSize);
    IOBuffer.read(file);
    IOBuffer.unpack(registro);
    datosLeidos=IOBuffer.getBuffSize();
    IOBuffer.clear();
    
    return datosLeidos;
}

