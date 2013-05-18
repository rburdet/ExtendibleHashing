/*
 * Block.cpp
 *
 *  Created on: May 4, 2013
 *      Author: rburdet
 */
#include "fisica_Block.h"
#include "logica_HashExtensible.h"
#include "logica_Reg.h"
#include "config.h"

using namespace std;

//TODO: hidratar bloque

Block::Block(int dispersionSize,int blockAdress){
	//TODO:revisar que se use all lo que esta aca
	blockCurrentSize=0;
	this->dispersionSize=dispersionSize;
	//this->blockAdress=blockAdress; TODO: Como hago para laburar con el num de bloque?
	maxBlockSize=MAX_BLOCK_SIZE;
	blockNum++;
}

/*
 * en un bloque yo tengo que agregar un registro, y ademas tengo que agregar ese bloque a la lista de bloques
 * */


int Block::Insert(Reg & aReg){
	this->blockCurrentSize+=aReg.getSize();
	this->regsList.push_back(aReg);
	return 0;

}

void Block::changeDispersionSize(int newDispersionSize){
	this->dispersionSize=newDispersionSize;
}
list<Reg> Block::getRegList(){
	return this->regsList;
}

int Block::getBlockNum(){
	return this->blockNum;
}

int Block::duplicateDispersionSize(){
	return (this->dispersionSize*=2);
}

int Block::getDispersionSize(){
	return dispersionSize;
}


// TODO: SE ELIMINA?
int Block::separate(){
	return 0; //hacer
}

int Block::getCurrentSize(){
	return this->blockCurrentSize;
}

/* TODO: SE USA  ? ? ?
int Block::getBlockAdress(){
	return blockAdress;
}*/

bool Block::easyInsert(Reg& aReg){
	return (aReg.getSize()+ blockCurrentSize < MAX_BLOCK_SIZE);
}

void Block::setList(list<Reg> newRegList){
	this->regsList=newRegList;
}

int Block::search(Reg& regToLook){
	list<Reg>::iterator it;
	for (it = regsList.begin(); it != regsList.end(); it++){
		if ((*it).getId()==regToLook.getId()){
			regToLook.setFileAdress((*it).getFileAdress());
			//El size es siempre el mismo
			return ((*it).getFileAdress());
		}
	}
	return 0;
}

//void Block::open(const char* fileName){


//esto que voy a hacer esta mal peor solo para ver si falla por lo de
//fede o por lo mio
/*
void Block::write(const char*fileName){
	fstream out(fileName, ios :: out | ios :: binary) ;
	//char Buf[2*MAX_BLOCK_SIZE];
	//int i=0;
	//list<Reg>::iterator it; Primero ver si lo puedo tirar cabeza
	//for (it = this->r)
	list<Reg>aList;
	aList = this->getRegList();
	out.write((char*)&aList,120); // 120 = 4 * 3 * 10
	out.close();
	ifstream in(fileName);
	in.read((char*)&aList,120);
	list<Reg>::iterator it;
	for (it=aList.begin(); it!=aList.end();it ++)
	{
			cout << "elemento: "<< (*it).getId();
	}

}
*/
/*void Block::read(const char* fileName){
	ifstream in(fileName);
	in.seekg(0,in.end);
	int length = in.tellg();
	length /= sizeof(int);
	in.seekg(0,in.beg);
	list<Reg>aList;
	cout << "el archivo tiene "<< length << " elementos"<< endl;
	in.read((char*)&aList,length);
	this->setList(aList);
	list <Reg>::iterator it;
	for (it=this->getRegList().begin(); it!=this->getRegList().end();it ++)
		cout << "elemento: "<< (*it).getId();
}
*/

void Block::write(const char* fileName){ // TODO: pasar a .h
	ArchivoBloques* archivo = new ArchivoBloques(MAX_BLOCK_SIZE, fileName);
	// Inicializamos el archivo de bloques o lo levantamos si ya existia
	if(archivo->abrirArchivo() == -1){
		// El archivo no existe, lo creamos
		archivo->crearArchivo();
		archivo->abrirArchivo();
	}
	//Si existe pero esta cerrado, lo abrimos
	if (!archivo->estaAbierto()){
		archivo->abrirArchivo();
	}
	char Buf[MAX_BLOCK_SIZE];
	int i=0;
	list<Reg>::iterator it;
	for(it = regsList.begin(); it!= regsList.end(); it++){
		Buf[i++] = it->getId();
		cout << " id metida : " << it->getId() << endl;
		Buf[i++] = it->getFileAdress();
		cout << "file adress metida: "<< it->getFileAdress()<< endl;
	}

	archivo->escribirBloque((void*) Buf, this->getBlockNum(),MAX_BLOCK_SIZE);
	archivo->cerrarArchivo();
}

void Block::read(const char* fileName){
	ArchivoBloques* archivo = new ArchivoBloques(MAX_BLOCK_SIZE, fileName);
	// Inicializamos el archivo de bloques o lo levantamos si ya existia
	if(archivo->abrirArchivo() == -1){
		// El archivo no existe, lo creamos, TODO: EL WRITE LO CREA
	//	//archivo->crearArchivo();
		//archivo->abrirArchivo();
		return;
	}
	//Si existe pero esta cerrado, lo abrimos
	if (!archivo->estaAbierto()){
		archivo->abrirArchivo();
	}
	char buf[MAX_BLOCK_SIZE];
	list<Reg>aList;
	archivo->leerBloque((void*)buf,this->getBlockNum());
	for (int i=0; i < 10 ; i++)
			cout << (int)buf[i]<< " ";
	for (int i=0;buf[i]!=0;i++){
		Reg* aReg= new Reg(buf[i],buf[i++]);
		aList.push_back(*aReg);
		cout << aReg->getId() << " =id, numero : " << i << endl;
	}
	this->setList(aList);
	for (list<Reg>::iterator it = aList.begin();it !=aList.end();it++)
	{
		cout<<"estoy en la lista " << (*it).getId() << endl;
	}
	archivo->cerrarArchivo();

}/**/
/*
ArchivoBloques* Block::getArchivo(){
	return this->archivo;
}*/

	/*
	Block* Block::createNew(int newDispersionSize){
		Block* aNewBlock = new Block(newDispersionSize);
	}
	*/
/*
int Block::redistribute(Block* aNewBlock,int tableSize){
		list<Reg>::iterator it;
		HashExtensible *aHash= new HashExtensible();
		for (it = this->getRegList().begin(); it != this->getRegList().end() ; it++) {
			int pos = aHash->doHash((*it).getId(),tableSize);
			this->Insert(*it);	//SEGUIR CON ESTOOOO
		}
		return 0; //cambiar
}*/ //LA REDISPERSION LA TENDRIA QUE SABER HACER LA TABLA, NO EL BLOQUE

Block::~Block() {

}
