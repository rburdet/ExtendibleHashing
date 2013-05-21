/*
 * BlockTable.cpp
 *
 *  Created on: May 11, 2013
 *      Author: rburdet
 */
#include "logica_BlockTable.h"

#define PRINT_REF(X) cout << X << "blockReferences" ; for(int i=0;i<this->getSize();i++) cout << " '" << this->blockReferences[i] << "'"; cout << endl
using namespace std;

BlockTable::BlockTable(string path, string blockPath, int blockSize) {
	// TODO: exepcion si path == NULL
	this->filePath = (char*) calloc(strlen(path.c_str()) +1, sizeof(char));
	strcpy(this->filePath, path.c_str());
	this->blockPath = (char*) calloc(strlen(blockPath.c_str()) +1, sizeof(char));
	strcpy(this->blockPath, blockPath.c_str());
	this->blockReferences = NULL;
	this->blockSize = blockSize;
	this->read();
}

void BlockTable::read(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);

	if (!archivo.is_open())
		archivo.open(this->filePath,ios::out|ios::binary);

	archivo.seekg(0,archivo.end); //TODO: revisar que esto ande bien!
	this->size = archivo.tellg() / sizeof(int);

	if(this->blockReferences)
		free(this->blockReferences);

	if(this->size){ //TODO: revisar que esto ande bien!
		archivo.seekg(0,archivo.beg);
		this->blockReferences = (int*) calloc(this->size, sizeof(int));
		archivo.read((char*)this->blockReferences, this->size * sizeof(int));
	}else{
		this->size = 1;
		this->blockReferences = (int*) calloc(this->size, sizeof(int));;
	}

	archivo.close();
}

void BlockTable::write(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);
	archivo.write((char*)this->blockReferences,sizeof(int)*this->size);
	archivo.close();
}

//TODO: por que le paso un registro y no un id solo, osae, al crear un registro ya lo creo con un adres especifico, y si estoy buscando, no se ese adress!
int BlockTable::search(Reg& aReg){ // TODO: persistencia
	int pos=HashExtensible::doHash(aReg.getId(),this->getSize());
	
	int blockNumber=this->blockReferences[pos];

	cout << "\tSearch doHash(regId=" << aReg.getId() << ", size=" << this->getSize() <<") = "<< pos << endl; 
	PRINT_REF("\t\t (" << blockNumber << ")");

	Block aBlock(this->getSize(), blockNumber, this->blockPath, this->blockSize);
	cout << "\t\t\t Read";
	aBlock.read();

	return aBlock.search(aReg);
}

int BlockTable::insert(Reg & aReg){
	int pos = HashExtensible::doHash(aReg.getId(),this->getSize());

	int tmpBlockNumber = this->blockReferences[pos];

	cout << "\tInserto doHash(regId=" << aReg.getId() << ", size=" << this->getSize() <<") = "<< pos << endl; 
	PRINT_REF("\t\t (" << tmpBlockNumber << ")");

	Block tmpBlock(this->getSize(), tmpBlockNumber, this->blockPath, this->blockSize);
	cout << "\t\t\t tmpBlock";
	tmpBlock.read();

	if (tmpBlock.easyInsert(aReg)){
		tmpBlock.Insert(aReg);
		cout << "\t\t\t\teasyInsert ";
		tmpBlock.write();
	} else {
		if (! this->canAddBlock(&tmpBlock)){
			this->duplicateTable();
			PRINT_REF("\t\t afterDuplicate:");
		}
		int lastBlockNum = tmpBlock.newBlockNum();
		Block anotherBlock(tmpBlock.duplicateDispersionSize(), lastBlockNum, this->blockPath, this->blockSize);
		cout << "\t\t anotherBlock(TD="<< anotherBlock.getDispersionSize() << ", lastBlockNum=" << lastBlockNum << ", path=" << this->blockPath << ", size="<< this->blockSize << ")" << endl;

		this->insertBlock(pos, lastBlockNum, tmpBlock.getDispersionSize());
		cout << "\t\tinsertBlock(pos=" << pos << ", lastBlockNum=" << lastBlockNum << ", TD=" << tmpBlock.getDispersionSize() << ")" << endl;
		PRINT_REF("\t\t after insertBlock:");

		this->redisperse(&tmpBlock, &anotherBlock);

		this->insert(aReg);
	}
	return 0;
}

int BlockTable::getSize(){
	return size;
}

bool BlockTable::canAddBlock(Block* aBlock){
	cout << "\t\t TT:" << this->getSize() << " TD:" << aBlock->getDispersionSize() << endl;
	return (aBlock->getDispersionSize()!=this->getSize());
}

void BlockTable::insertBlock(int blockPos, int newBlockNum, int td){
	for (int i = blockPos; i < this->getSize(); i+=td) {
		this->blockReferences[i] = newBlockNum;
	}

	for (int i = blockPos-td; i >= 0; i-=td) {
		this->blockReferences[i] = newBlockNum;
	}
}

void BlockTable::redisperse(Block* anOldBlock, Block* aNewBlock){
	list<Reg>::iterator it;
	list<Reg> anOldBlockList = anOldBlock->getRegList();
	Block newAnOldBlock(anOldBlock->getDispersionSize(), anOldBlock->getBlockNum(), this->blockPath, this->blockSize);

	for (it = anOldBlockList.begin(); it != anOldBlockList.end() ; it++) {
		int pos = HashExtensible::doHash((*it).getId(),this->getSize());
		int blockNum = this->blockReferences[pos];
		if( anOldBlock->getBlockNum() == blockNum )
			newAnOldBlock.Insert(*it);
		else
			aNewBlock->Insert((*it));
	}

	cout << "\t\t\t\t(" << newAnOldBlock.getBlockNum() <<", TD:" <<  newAnOldBlock.getDispersionSize() << ") newAnOldBlock.write() ";
	newAnOldBlock.write();
	cout << "\t\t\t\t(" << aNewBlock->getBlockNum() <<", TD:" <<  aNewBlock->getDispersionSize() << ") aNewBlock.write() ";
	aNewBlock->write();
}



void BlockTable::duplicateTable(){
	int newBlockTableSize = this->size *2;
	int *tmpBlockReference = (int*) calloc(newBlockTableSize, sizeof(int));
	for (int i = 0; i < this->size ; i++) {
		tmpBlockReference[i] = this->blockReferences[i];
		tmpBlockReference[this->size+i] = this->blockReferences[i];
	}
	delete this->blockReferences;
	this->blockReferences = tmpBlockReference;
	this->size = newBlockTableSize;
}

BlockTable::~BlockTable(){
	this->write();
	free(this->blockReferences);
	free(this->filePath);
	free(this->blockPath);
}
