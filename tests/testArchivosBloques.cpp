//
//  main.cpp
//  FileHandler
//
//  Created by Federico Colangelo on 21/04/13.
//  Copyright (c) 2013 Federico Colangelo. All rights reserved.
//

#include <iostream>
//#include <fstream>
#include "../codigo/fisica_ArchivoBloques.h"

// Cada int es de 4bytes, 4 ints ->  16 bytes
#define BLOCKSIZE 16

// Como compilar :
// Compilar la libreria ppcial, borrar el interfaz_main.o
// g++ ./testArchivosBloques.cpp ../codigo/*.o -o pepe

using namespace std;

int assertEqualsArchivoBloques (int* b1, int*b2)
{
	return (b1[0] == b2[0] && b1[1] == b2[1] && b1[2] == b2[2] && b1[3] == b2[3]);

}

int main(int argc, const char * argv[])
{
	int buf[4] = { 1, 2 , 3, 4};
	int buf2[4] = { 5, 6 , 7, 8};

	//Pruebo la correcta creacion y apertura del archivo
	ArchivoBloques file(BLOCKSIZE,"test.dat");
	file.crearArchivo();
	file.abrirArchivo();
	if (!file.estaAbierto()) {cerr<<"El archivo deberia estar abierto y esta cerrado"<<endl; return -1;};

	//Ahora pruebo que la escritura y lectura funcionen

	file.escribirBloque((void*) buf, 1, BLOCKSIZE);
	file.escribirBloque((void*) buf2, 2, BLOCKSIZE);

	int buftmp[4] = {0};
	file.leerBloque((void*) buftmp, 1);
	if (!assertEqualsArchivoBloques(buf, buftmp)) {
		cerr<<"Los struct no son iguales"<<endl;
		for(int i=0;i<4;i++)
			cerr<< "\t orig: "<< buf[i] << " read: " << buftmp[i] << endl;
		//return -1;
	};

	int buftmp2[4] = {0};
	file.leerBloque((void*)  buftmp2, 2);
	if (!assertEqualsArchivoBloques(buf2, buftmp2)) {
		cerr<<"Los struct no son iguales"<<endl;
		for(int i=0;i<4;i++)
			cerr<< "\t orig: "<< buf2[i] << " read: " << buftmp2[i] << endl;
		return -1;
	};

	//file.borrarArchivo();

	cout<<"Prueba testArchivosBloques EXITOSA"<<endl;
	return 0;
}
