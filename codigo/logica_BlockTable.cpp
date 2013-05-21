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
	this->archivo = new fstream(name.c_str(),ios::in|ios::out|ios::binary);
	if (!this->archivo->is_open()){
		this->archivo->open(name.c_str(),ios::out|ios::binary);
	}
	this->hidratateBlockTable();
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


//TODO: por que le paso un registro y no un id solo, osae, al crear un registro ya lo creo con un adres especifico, y si estoy buscando, no se ese adres
int BlockTable::search(Reg& aReg){ // TODO: persistencia
	int pos=HashExtensible::doHash(aReg.getId(),this->getSize());
	
	//Aca ya tengo que tener la tabla en memoria, siempre supongo que entra toda la tabla en memoria
	int blockNumber=this->blockReferences[pos];
	Block *aBlock = new Block(this->getSize(),blockNumber);

	string name=HASH_BLOCK_FILE;
	aBlock->read(name.c_str());

	//aBlock->getArchivo()->leerBloque((void*)aBlock,blockNumber);
	return aBlock->search(aReg);
}

int BlockTable::insert(Reg & aReg){
	//TODO: persistencia

	int pos = HashExtensible::doHash(aReg.getId(),this->getSize());

	int tmpBlockNumber = this->blockReferences[pos];
	string name=HASH_BLOCK_FILE;

	cout << "\tpos: "<< pos << " tempBlockNumber: " << tmpBlockNumber << " tamanio tabla:  " << this->getSize() << endl;
	Block *tmpBlock = new Block(this->getSize(),tmpBlockNumber);
	//cout << "ultimo bloque segun fede: "<< tmpBlock->newBlockNum(name.c_str())<< endl;

	tmpBlock->read(name.c_str());

	if (tmpBlock->easyInsert(aReg)){
		cout << "\tentro al easy insert "<<endl;
		tmpBlock->Insert(aReg);
		tmpBlock->write(name.c_str());
	}
	else {
		cout << "\tNo es easyInsert " << endl;
		if (! canAddBlock(tmpBlock)){
			cout << "\t\tDuplico tabla" << endl;
			duplicateTable();
		}
		// la posicion del nuevo bloque va a ser al final del archivo

		//Con esto pasa las pruebas bien
		//int lastBlockNum=0; //TODO: el numero de bloque se guarda en otro archivo
		int lastBlockNum= tmpBlock->newBlockNum(name.c_str());
		cout << "\tdispersion viejo= " << tmpBlock->getDispersionSize() ;
		Block * anotherBlock = new Block(tmpBlock->duplicateDispersionSize(),lastBlockNum); //HARDCODEADA CABEZA
		cout << "dispersion new= " << tmpBlock->getDispersionSize()<<endl ;

		//si no tengo referencias repetidas tengo que duplicar la tabla
		cout << "\tinserto bloque" << endl;
		insertBlock(pos,lastBlockNum,tmpBlock->getDispersionSize());

		//Estos 2 no andan...

		cout << "\tredisperso" << endl;
		redisperse(tmpBlock,anotherBlock);

		delete anotherBlock;
		cout << "\tVuevlo a insertar" << endl;
		this->insert(aReg);
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
	cout << "\t\t disperscionBlock=" << aBlock->getDispersionSize() << "  " << this->getSize() << endl;
	return (aBlock->getDispersionSize()!=this->getSize());
}

void BlockTable::insertBlock(int blockPos,int newBlockNum,int td){
	cout << " estoy insertando bloque: "<<endl;
	cout << " blockPos: "<<blockPos<<endl;
	cout << " newBlockPos: "<<newBlockNum<<endl;

	for (int i = blockPos; i < this->getSize(); i+=td) {
		blockReferences[i]=newBlockNum;
		cout << "en la pos: "<< i<<" de blockReference tengo: "<< blockReferences[i]<<endl;
	}
	for (int i = blockPos-td; i >= 0; i-=td) {
		blockReferences[i]=newBlockNum;
		cout << "en la pos: "<< i<<" de blockReference tengo: "<< blockReferences[i]<<endl;
	}
}

void BlockTable::redisperse(Block* anOldBlock, Block* aNewBlock){
	HashExtensible* aHash= new HashExtensible();
	list<Reg>::iterator it;
	list<Reg> tmpList;
	list<Reg> anOldBlockList = anOldBlock->getRegList();

	for (it = anOldBlockList.begin(); it != anOldBlockList.end() ; it++) {
		int pos = aHash->doHash((*it).getId(),this->getSize());
		int blockNum = this->blockReferences[pos];
		if( anOldBlock->getBlockNum() == blockNum )
			tmpList.push_back((*it));
		else
			aNewBlock->Insert((*it));
	}

	anOldBlock->setList(tmpList); //cabeza
	string name=HASH_BLOCK_FILE;
	anOldBlock->write(name.c_str());
	aNewBlock->write(name.c_str());
}



void BlockTable::duplicateTable(){
	int newBlockTableSize = size *2;
	int *tmpBlockReference = new int [newBlockTableSize];
	for (int i = 0; i <= size ; i++) {
		tmpBlockReference[i] = blockReferences[i];
		tmpBlockReference[2*i] = blockReferences[i];
	}
	delete blockReferences;
	blockReferences=tmpBlockReference;
	this->size = newBlockTableSize;
	cout << "new block size " << this->size << endl;
}

BlockTable::~BlockTable(){
	//
	this->archivo->write((char*)this->blockReferences,sizeof(int)*this->size);
	//this->archivo->escribirBloque((void*)this,0,100);//Aca tendria que tirar la tabla en un archivo, y nada mas.
													 //el archivo no tiene bloques nada
	this->archivo->close();
	delete []blockReferences;
}
