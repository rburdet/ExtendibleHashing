//
//  main.cpp
//  FileHandler
//
//  Created by Federico Colangelo on 21/04/13.
//  Copyright (c) 2013 Federico Colangelo. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "fisica_ArchivoBloques.h"

#define BLOCKSIZE 512

using namespace std;

struct testStructArchivoBloques {
    int unEntero;
    char* unString;
    int miCasa;
    int otraCasa;
};

int assertEqualsArchivoBloques (testStructArchivoBloques &unStruct, testStructArchivoBloques &otroStruct)
{
    return (unStruct.unEntero==otroStruct.unEntero && unStruct.unString==otroStruct.unString);
    
}

int main(int argc, const char * argv[])
{
    testStructArchivoBloques testStruct1;
    testStruct1.unEntero=15;
    testStruct1.unString="fjlagjfldsgfd";
    
    testStructArchivoBloques testStruct2;
    testStruct2.unEntero=131;
    testStruct2.unString="fdasfdasfds";
        
    testStructArchivoBloques testStructTemp;
    
    //Pruebo la correcta creacion y apertura del archivo
    ArchivoBloques file(BLOCKSIZE,"test.dat");
    file.crearArchivo();
    file.abrirArchivo();
    if (!file.estaAbierto()) {cerr<<"El archivo deberia estar abierto y esta cerrado"<<endl; return -1;};
    
    //Ahora pruebo que la escritura y lectura funcionen

    file.escribirBloque(&testStruct1, 2, sizeof(testStruct1));
    file.escribirBloque(&testStruct2, 7, sizeof(testStruct2));
    
    file.leerBloque(&testStructTemp, 2);
    if (!assertEqualsArchivoBloques(testStruct1, testStructTemp)) {cerr<<"Los struct no son iguales"<<endl; return -1;};
    file.leerBloque(&testStructTemp, 7);
    if (!assertEqualsArchivoBloques(testStruct2, testStructTemp)) {cerr<<"Los struct no son iguales"<<endl; return -1;};
    
    file.borrarArchivo();    
    
    cout<<"Prueba testArchivosBloques EXITOSA"<<endl;
    return 0;
}
