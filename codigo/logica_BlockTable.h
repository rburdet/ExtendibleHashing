/*
 * BlockTable.h
 *
 *  Created on: May 4, 2013
 *      Author: rburdet
 */

/*
 *   Esta clase es la encargada de manejar las referencias a bloques.
 *	Al agregar un registro, se le aplica una funcion de dispersion, el valor que retorne
 *	esa funcion de dispersion sera la posicion en la tabla.
 *	En cada posicion de la tabla se encuentra una referencia a un bloque en un archivo
 *	de bloques.
 *
 * */

#ifndef BLOCKTABLE_H_
#define BLOCKTABLE_H_

#include "fisica_Block.h"
#include "fisica_ArchivoBloques.h"
#include "logica_HashExtensible.h"
#include <string.h>


using namespace std;

template <class T>
class BlockTable{
	public:
		//Path -> archivo de la tabla, blockPath -> archivo de bloques
		BlockTable(string path, string blockPath, int blockSize);	// Cuando se crea la tabla de bloques, se crea a partir del archivo.
		~BlockTable();	// Cierro los archivos, libero la memoria pedida por blockReferences.
		int insert(T*); // A su vez llama al insertar del bloque
		void insertBlock(int blockPos,int newBlockReference,int td); //En la posicion blockPos de la tabla pongo la nueva ref.
		int countNumberOfReferences(int blockNumber);

		//Creo que no se usa
		int saveBlock(Block<T> * aBlock); //Cuando agrego algo a un bloque lo tengo que guardar
		void duplicateTable();
		bool search(T*&); // Busca el registro atraves de la funcion hash

		void write(); //Escribe tabla a disco
		void read(); //Lee tabla de disco

		int getSize(); //Devuelve el tamanio de la tabla
		bool canAddBlock(Block<T>* aBlock); //Tengo referencias libres? true significa que no hace falta duplicar
		void redisperse(Block<T>* anOldBlock,Block<T>* aNewBlock); //redisperso los registros del bloque viejo al bloque nuevo

		void actualizar(T & newReg);

		void prueba(Block<T>*);
	protected:
		int size;
		int *blockReferences;
		char *filePath;
		char *blockPath;
		int blockSize;
		bool firstTime;
		unsigned int numberOfDuplications;
};

template <class T>
BlockTable<T>::BlockTable(string path, string blockPath, int blockSize) {
	this->filePath= new char[strlen(path.c_str())+1]();
	strcpy(this->filePath, path.c_str());
	this->blockPath = new char[strlen(blockPath.c_str())+1]();
	strcpy(this->blockPath, blockPath.c_str());
	this->blockReferences = NULL;
	this->blockSize = blockSize;
	this->firstTime = true;
	this->read();
	this->numberOfDuplications= 0;
}

template <class T>
void BlockTable<T>::read(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);

	if (!archivo.is_open())
		archivo.open(this->filePath,ios::out|ios::binary);

	archivo.seekg(0,archivo.end);
	this->size = archivo.tellg() / sizeof(int);

	if(this->blockReferences)
		delete [] this->blockReferences;

	if(this->size > 0){
		archivo.seekg(0,archivo.beg);
		this->blockReferences= new int[this->size]();
		archivo.read((char*)this->blockReferences, this->size * sizeof(int));
	}else if(this->size < 0){
		cerr << "*** No se puede crear archivo, error del sistema:" << this->size << " path: "<< this->filePath << endl;
	}else{
		this->size = 1;
		this->blockReferences = new int[this->size]();
	}
	archivo.close();
}

template <class T>
void BlockTable<T>::write(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);
	archivo.write((char*)this->blockReferences,sizeof(int)*this->size);
	archivo.close();
}
template <class T>
bool BlockTable<T>::search(T*& aReg){
	int pos=HashExtensible::doHash(aReg->getClave(),this->getSize());

	int blockNumber=this->blockReferences[pos];

	Block<T> aBlock(this->getSize(), blockNumber, this->blockPath, this->blockSize);
	aBlock.read();

	return aBlock.search(aReg);
}

template <class T>
int BlockTable<T>::countNumberOfReferences(int blockNumber){
	int count = 0;
	for (int i=0; i< this->getSize(); i++){
		if (this->blockReferences[i]==blockNumber)
			count +=1;
	}
	return count;
}

template <class T>
int BlockTable<T>::insert(T *aReg){

	int pos = HashExtensible::doHash(aReg->getClave(),this->getSize());
	int tmpBlockNumber = this->blockReferences[pos];
	int dispersionSize = this->getSize() / this->countNumberOfReferences(tmpBlockNumber);
	Block<T> tmpBlock(dispersionSize, tmpBlockNumber, this->blockPath, this->blockSize);
	tmpBlock.read();

	//Puede ser el caso de que el ID ya exista en el hash, en ese caso, no se agrega

	T *searchReg = new T();
	searchReg->setClave(aReg->getClave());
	if(! tmpBlock.search(searchReg)){
		if (tmpBlock.easyInsert(*aReg)){
			this->firstTime = false;
			tmpBlock.Insert(*aReg);
			tmpBlock.write();
		} else {
			if(this->firstTime){
				cerr<< "el tamanio de bloque que se eligio es muy chico" <<endl;
				exit(EXIT_FAILURE);
				return -1;
			}
			if (! this->canAddBlock(&tmpBlock)){

				if (this->numberOfDuplications<maxDedupTimes()){
					this->duplicateTable();	
					this->numberOfDuplications++;
				}else{
					cerr<<"ERROR AGREGANDO REGISTRO, CAMBIE EL TAMANIO DEL BLOQUE"<<endl;
					exit(EXIT_FAILURE);
				}
			}
			this->numberOfDuplications=0;
			int lastBlockNum = tmpBlock.newBlockNum();
			Block<T> anotherBlock(tmpBlock.duplicateDispersionSize(), lastBlockNum, this->blockPath, this->blockSize);
			this->insertBlock(pos, lastBlockNum, tmpBlock.getDispersionSize());
			this->redisperse(&tmpBlock, &anotherBlock);
			this->insert(aReg);
		}
	}

	delete searchReg;
	return 0;
}

template <class T>
int BlockTable<T>::getSize(){
	return size;
}

template <class T>
bool BlockTable<T>::canAddBlock(Block<T>* aBlock){
	return (aBlock->getDispersionSize()!=this->getSize());
}

template <class T>
void BlockTable<T>::insertBlock(int blockPos, int newBlockNum, int td){
	for (int i = blockPos; i < this->getSize(); i+=td) {
		this->blockReferences[i] = newBlockNum;
	}

	for (int i = blockPos-td; i >= 0; i-=td) {
		this->blockReferences[i] = newBlockNum;
	}
}

template <class T>
void BlockTable<T>::redisperse(Block<T>* anOldBlock, Block<T>* aNewBlock){
	typename list<T>::iterator it;
	list<T> anOldBlockList = anOldBlock->getRegList();
	Block<T> newAnOldBlock(anOldBlock->getDispersionSize(), anOldBlock->getBlockNum(), this->blockPath, this->blockSize);

	for (it = anOldBlockList.begin(); it != anOldBlockList.end() ; it++) {
		int pos = HashExtensible::doHash((*it).getClave(),this->getSize());
		int blockNum = this->blockReferences[pos];
		if( anOldBlock->getBlockNum() == blockNum )
			newAnOldBlock.Insert(*it);
		else
			aNewBlock->Insert((*it));
	}

	newAnOldBlock.write();
	aNewBlock->write();
}


template <class T>
void BlockTable<T>::actualizar(T & newReg){
	this->read();
	int pos = HashExtensible::doHash(newReg.getClave(),this->getSize());
	int tmpBlockNumber = this->blockReferences[pos];
	// No me importa el tamanio de dispersion
	Block<T> tmpBlock(0,tmpBlockNumber,this->blockPath,this->blockSize);
	tmpBlock.read();
	tmpBlock.actualizar(newReg);
	tmpBlock.write();
}

template <class T>
void BlockTable<T>::duplicateTable(){
	int newBlockTableSize = this->size *2;
	int *tmpBlockReference = new int[newBlockTableSize];
	for (int i = 0; i < this->size ; i++) {
		tmpBlockReference[i] = this->blockReferences[i];
		tmpBlockReference[this->size+i] = this->blockReferences[i];
	}
	delete [] this->blockReferences;
	this->blockReferences = tmpBlockReference;
	this->size = newBlockTableSize;
}

template <class T>
BlockTable<T>::~BlockTable(){
	this->write();
	delete [] this->blockReferences;
	delete [] this->blockPath;
	delete [] this->filePath;
}

#endif
