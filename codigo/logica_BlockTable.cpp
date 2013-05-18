/*
 * BlockTable.cpp
 *
 *  Created on: May 11, 2013
 *      Author: rburdet
 */


#include <string.h>
#include "logica_BlockTable.h"
#include "config.h"
using namespace std;

BlockTable::BlockTable() {
	//TODO:Cambiar esto, hay que crear la tabla con los datos de un archivo donde tenga la tabla

	string name = HASH_BLOCK_TABLE_FILE;
	//fstream out(name.c_str(),ios::in|ios::out|ios::binary);
	this->archivo = new fstream(name.c_str(),ios::in|ios::out|ios::binary);
	if (!this->archivo->is_open()){
		this->archivo->open(name.c_str(),ios::out|ios::binary);
		//this->archivo->open(name.c_str(),ios::in|ios::out|ios::binary);
	}
	this->hidratateBlockTable();
	//blockReferences = new int [1];
}

void BlockTable::hidratateBlockTable(){
	if(this->size){
		//TODO: liberar memoria de blockReferences
		this->archivo->seekg(0,this->archivo->end);
		this->size = this->archivo->tellg();
		this->archivo->seekg(0,this->archivo->beg);
		this->size /= sizeof(int);
		this->blockReferences = new int[this->size];
		this->archivo->read((char*)this->blockReferences,this->size);
	}else{
		this->size = 1;
		this->blockReferences = new int[this->size];
	}
}

//Si falla, devuelve 0
int BlockTable::open(const char * fileName){
	this->archivo->open(fileName,ios::in|ios::out|ios::binary);
	if(!this->archivo){
		return 0;
	}
	return 1;
}

void BlockTable::prueba(Block* aBlock){
	cout<< "imprimo algo" << aBlock->getCurrentSize()<< endl;
}

int BlockTable::search(Reg& aReg){
	// TODO: persistencia
	int pos=HashExtensible::doHash(aReg.getId(),this->getSize());
	//Aca ya tengo que tener la tabla en memoria, siempre supongo que entra toda la tabla en memoria
	int blockNumber=this->blockReferences[pos];
	Block *aBlock = new Block(this->getSize(),blockNumber);
	//aBlock->getArchivo()->leerBloque((void*)aBlock,blockNumber);
	aBlock->search(aReg);
	return 0;

}

int BlockTable::insert(Reg & aReg){
	//TODO: ESTO ES UNA NEGRADA ! CAMBIAR -> tenes razon =D
	//TODO: persistencia
	int pos = HashExtensible::doHash(aReg.getId(),this->getSize());
	int tmpBlockNumber = this->blockReferences[pos];
//	cout << "pos "<< pos << " temp " << tmpBlockNumber << endl;
	string name=HASH_BLOCK_FILE;
	Block *tmpBlock = new Block(this->getSize(),tmpBlockNumber);

	tmpBlock->read(name.c_str());



	//this->archivo = new ArchivoBloques(MAX_BLOCK_SIZE,".hashBlock");
	//archivo->abrirArchivo();
	list<Reg> aList;
	//tmpBlock->getArchivo()->leerBloque((void*)&aList,tmpBlockNumber);
	//tmpBlock->setList(aList);
	//A este bloque lo tengo que hidratar,tener en cuenta que en un archivo se van a guardar solo los registros del bloque
	//mientras que en otro archivo se va a guardar una tira con todas las posiciones de bloques

	if (tmpBlock->easyInsert(aReg)){
	//	cout<< "easy" << endl;
		tmpBlock->Insert(aReg);
		cout << "entro al easy insert "<<endl;
		//aList = tmpBlock->getRegList();
		tmpBlock->write(name.c_str());
		//tmpBlock->read(name.c_str());


		//tmpBlock->getArchivo()->escribirBloque((void*) &aList,tmpBlock->getBlockNum(),MAX_BLOCK_SIZE);
	}
	else {
		// la posicion del nuevo bloque va a ser al final del archivo
		int lastBlockNum=0; //TODO: el numero de bloque se guarda en otro archivo


		Block * anotherBlock = new Block(tmpBlock->duplicateDispersionSize(),lastBlockNum+1); //HARDCODEADA CABEZA


		//si no tengo referencias repetidas tengo que duplicar la tabla
		if (! canAddBlock(tmpBlock)){
			duplicateTable();
		}

		insertBlock(pos,lastBlockNum+1,tmpBlock->getDispersionSize());
		redisperse(tmpBlock,anotherBlock);
		aList = tmpBlock->getRegList();
		//tmpBlock->getArchivo()->escribirBloque((void*)&aList,tmpBlock->getBlockNum(),tmpBlock->getCurrentSize());
		aList = anotherBlock->getRegList();
		//anotherBlock->getArchivo()->escribirBloque((void*)&aList,anotherBlock->getBlockNum(),anotherBlock->getCurrentSize());
		this->insert(aReg);
		// Llamar recursivametne this->insert(aReg)
	}
	delete tmpBlock;
	//TODO: ESTO de aca
	//archivo->leerBloque((void*) tmpBlock,blockNumber); No esta implementado
	//archivo->escribirBloque((void*)tmpBlock,blockNumber); No esta implementado

	return 0;
}



int BlockTable::getSize(){
	return size;
}

bool BlockTable::canAddBlock(Block* aBlock){
	return (aBlock->getDispersionSize()!=this->getSize());
}

void BlockTable::insertBlock(int blockPos,int newBlockPos,int td){
	for (int i = blockPos; i < this->getSize(); i+=td) {
		blockReferences[i]=newBlockPos;
	}
	for (int i = blockPos-td; i >= 0; i-=td) {
		blockReferences[i]=newBlockPos;
	}
}

void BlockTable::redisperse(Block* anOldBlock, Block* aNewBlock){
	HashExtensible* aHash= new HashExtensible();
	list<Reg>::iterator it;
	list<Reg> tmpList;

	for (it = anOldBlock->getRegList().begin(); it != anOldBlock->getRegList().end() ; it++) {
		int pos = aHash->doHash((*it).getId(),this->getSize());
		int blockNum = this->blockReferences[pos];
		if( anOldBlock->getBlockNum() == blockNum )
			tmpList.push_back((*it));
		else
			aNewBlock->Insert((*it));
	}

	anOldBlock->setList(tmpList); //cabeza
}



void BlockTable::duplicateTable(){
	int newBlockTableSize = size *2;
	int *tmpBlockReference = new int [newBlockTableSize];
	for (int i = 0; i < size ; i++) {
		tmpBlockReference[i] = blockReferences[i];
		tmpBlockReference[2*i] = blockReferences[i];
	}
	delete blockReferences;
	blockReferences=tmpBlockReference;
}

BlockTable::~BlockTable(){
	//
	this->archivo->write((char*)this->blockReferences,sizeof(int)*this->size);
	//this->archivo->escribirBloque((void*)this,0,100);//Aca tendria que tirar la tabla en un archivo, y nada mas.
													 //el archivo no tiene bloques nada
	this->archivo->close();
	delete []blockReferences;
}
