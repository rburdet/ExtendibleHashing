/*
*	Block.h
*
*	Esta clase es la mas importante dentro del hash. Incluye informacion vital para el funcionamiento del mismo.
Esta informacion incluye:
	Posicion del bloque en el archivo
	Tamanio de dispersion del bloque
	Los registros que contiene cada bloque
*
*
*/

#ifndef BLOCK_H_
#define BLOCK_H_


#include <list>
#include <string.h>
#include <stdlib.h>
#include "fisica_ArchivoBloques.h"
#include "logica_HashExtensible.h"
#include "domain_RegistroGenerico.h"
#include "runtimeConfig.h"
#include "fisica_SerialBuffer.h"
using namespace std;



template <class T>
class Block {
	public:
		Block(int DispersionSize,int blockAdress, char* filePath, int blocksize); //Creo un bloque en una tabla de bloques. Inicialmente el tamanio es 0

		//  Agrego un registro a la cubeta, los resultados posibles son: 0 sino se agrego, 1 si se agrego, 2 si se duplico el tamanio.
		int Insert(T & aReg);

		// Si el bloque queda en overflow tengo que crear otro para luego redistribuir los registros*/
		Block* createNew(int newDispersionSize);

		//crea un nuevo bloque, redispersando y cambiando los tamanios de dispersion de ambos bloques
		int separate();

		// Cuando se produce un overflow, tengo que redispersar los registros */
		int redistribute(Block * aNewBlock,int tableSize);

		// Cuando se duplica la tabla, o un bloque queda en overflow pero con mas de 1 referencia en la tabla entonces es necesario cambiar el tamanio de dispersion del bloque
		// Devuelve el nuevo tamanio de dispersion
		void changeDispersionSize(int);

		// Cuando un bloque queda en overflow, necesito buscar el proximo bloque en donde pueda redispersar,  */
		int getBlockInTable();
		
		void actualizar(T& newReg);

		//Si entra el registro devuelve verdadero, si no falso
		bool easyInsert(T& aReg);

		//Devuelve el tamanio de dispersion del bloque
		int getDispersionSize();

		//Cuando se agrega un nuevo bloque se tiene que duplicar el tamanio de dispersion del bloque
		int duplicateDispersionSize();

		//Devuelve el numero de bloque
		int getBlockNum();

		//Devuelve la lista de registros
		list<T> getRegList();

		//Devuelve la posicion del bloque en el archivo de bloques
		int getBlockAdress();

		//Devuelve el espacio ocupado dentro del bloque
		short int getNumberOfRegs();


		//Una vez que obtengo el bloque, tengo que buscar en la lista el registro que yo quiero
		//Devuelvo el fileAdress de ese registro que es donde estara guardado el dato
		//SI NO SE ENCUENTRA EL REGISTRO SE DEVUELVE 0
		bool search(T*&);

		void read();
		void write();

		//Obtengo el numero del ultimo bloque
		unsigned int newBlockNum();

		~Block();

	protected:
		short int numberOfRegs;
		int maxBlockSize;
		int dispersionSize;
		list< T > regsList;
		int blockNum;
		char *filePath;

	private:
		void getCurrentBuffer(SerialBuffer * );
};

template <class T>
Block<T>::Block(int dispersionSize, int blockNum, char* filePath, int blocksize){
	this->numberOfRegs=0;
	this->dispersionSize=dispersionSize;
	this->maxBlockSize=blocksize;
	this->blockNum = blockNum;
	this->filePath=new char[strlen(filePath)+1]();
	strcpy(this->filePath, filePath);
}


template <class T>
unsigned int Block<T>::newBlockNum(){
	ArchivoBloques archivo(this->maxBlockSize, this->filePath);
	return  archivo.ultimoBloque();
}

template <class T>
int Block<T>::Insert(T & aReg){
	this->numberOfRegs+=1;
	this->regsList.push_back(aReg);
	return 0;

}


template <class T>
void Block<T>::changeDispersionSize(int newDispersionSize){
	this->dispersionSize=newDispersionSize;
}


template <class T>
list<T> Block<T>::getRegList(){
	return this->regsList;
}


template <class T>
int Block<T>::getBlockNum(){
	return this->blockNum;
}


template <class T>
int Block<T>::duplicateDispersionSize(){
	return (this->dispersionSize*=2);
}


template <class T>
int Block<T>::getDispersionSize(){
	return dispersionSize;
}


template <class T>
short int Block<T>::getNumberOfRegs(){
	return this->numberOfRegs;
}


template <class T>
bool Block<T>::easyInsert(T& aReg){
	SerialBuffer aSerialBuffer(maxBlockSize);
	this->getCurrentBuffer(&aSerialBuffer);
	bool a = aReg.serializar(&aSerialBuffer);
	return a;
}

template <class T>
bool Block<T>::search(T*& regToLook){
	typename list<T>::iterator it;
	for (it = regsList.begin(); it != regsList.end(); it++){
		if ((it)->getClave()==regToLook->getClave()){
			memcpy(regToLook, &*it, sizeof(T));
			return true;
		}
	}
	return false;
}

template <class T>
void Block<T>::getCurrentBuffer(SerialBuffer* aSerialBuffer){
	aSerialBuffer->pack(&(this->numberOfRegs), sizeof(this->numberOfRegs));
	typename list<T>::iterator it;
	for(it = regsList.begin(); it!= regsList.end(); it++){
		it->serializar(aSerialBuffer);
	}
}

template <class T>
void Block<T>::write(){
	ArchivoBloques archivo(maxBlockSize,this->filePath);
	SerialBuffer aSerialBuffer(maxBlockSize);
	this->getCurrentBuffer(&aSerialBuffer);
	archivo.escribirBloque(aSerialBuffer.getBuffer(),this->getBlockNum());
}



template <class T>
void Block<T>::read(){
	ArchivoBloques archivo(maxBlockSize,this->filePath);
	if(!archivo.existe())
		return;

	SerialBuffer aSerialBuffer(maxBlockSize);
	archivo.leerBloque(aSerialBuffer.getBuffer(),this->getBlockNum());
	if(aSerialBuffer.unpack(&(this->numberOfRegs)))
	regsList.clear();
	for (int i=0; i < this->numberOfRegs;i++){
		T reg;
		reg.deserializar(&aSerialBuffer);
		regsList.push_back(reg);
	}
}

template <class T>
Block<T>::~Block() {
	regsList.clear();
	delete [] this->filePath;
}


template <class T>
void Block<T>::actualizar(T& newReg){
	typename list<T>::iterator it;
	for (it = regsList.begin(); it != regsList.end(); it++){
		if (it->getClave() == newReg.getClave()){;
			*it = newReg;
		}
	}
}

#endif
