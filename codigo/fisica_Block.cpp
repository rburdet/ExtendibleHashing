/*
 * Block.cpp
 *
 *  Created on: May 4, 2013
 *      Author: rburdet
 */
#include "fisica_Block.h"

using namespace std;

//TODO: hidratar bloque
//TODO: deshardcodear!!!
#define REG_SIZE (sizeof(int)*2)

Block::Block(int dispersionSize, int blockNum, char* filePath, int blockSize){
	//TODO:revisar que se use all lo que esta aca
	this->blockCurrentSize=0;
	this->dispersionSize=dispersionSize;

	this->maxBlockSize=blockSize;

	this->blockNum = blockNum;

	this->filePath = (char*) calloc(strlen(filePath)+1, sizeof(char));
	strcpy(this->filePath, filePath);
}

/*
 * en un bloque yo tengo que agregar un registro, y ademas tengo que agregar ese bloque a la lista de bloques
 * */

//TODO: emprolijar
int Block::newBlockNum(){
	ArchivoBloques archivo(this->maxBlockSize, this->filePath);
	if (!archivo.estaAbierto())
		archivo.abrirArchivo();
	int ultimoBloque = archivo.ultimoBloque();
	archivo.cerrarArchivo();
	return ultimoBloque;

}

int Block::Insert(Reg & aReg){
	//TODO: aReg.getSize() -> devuelve cualqiercosa, el size del reg es siempre REG_SIZE. DESHARDCODEAR
	//this->blockCurrentSize+=aReg.getSize();
	this->blockCurrentSize+=REG_SIZE;
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


// TODO: SE ELIMINA? -> Si
int Block::separate(){
	return 0; //hacer
}

int Block::getCurrentSize(){
	return this->blockCurrentSize;
}

/* TODO: SE USA  ? ? ? -> no, borrar
int Block::getBlockAdress(){
	return blockAdress;
}*/

bool Block::easyInsert(Reg& aReg){
	//TODO: tiene qeu ser menor o menor e igual?
	return (aReg.getSize()+ this->blockCurrentSize <= this->maxBlockSize);
}

//  Devuelve el addres si lo encuentra, sino -1, no ponog 0 por qe puede qe el addres sea 0, no? TODO: Checkear si esto es verdad
//  TODO: Andar anda, pero no se por qe le pasamos un registro, se tendria qe pasar solo el ID
int Block::search(Reg& regToLook){
	list<Reg>::iterator it;
	for (it = regsList.begin(); it != regsList.end(); it++){
		if ((*it).getId()==regToLook.getId()){
			regToLook.setFileAdress((*it).getFileAdress());
			//El size es siempre el mismo
			return ((*it).getFileAdress());
		}
	}
	return -1;
}

// Implementacion con clase de archivo -> No anda
//void Block::write(){
//	ArchivoBloques archivo(this->maxBlockSize, this->filePath);
//	// Inicializamos el archivo de bloques o lo levantamos si ya existia
//	if(archivo.abrirArchivo() == -1){
//		// El archivo no existe, lo creamos
//		archivo.crearArchivo();
//		archivo.abrirArchivo();
//	}
//	//Si existe pero esta cerrado, lo abrimos
//	if (!archivo.estaAbierto()){
//		archivo.abrirArchivo();
//	}
//
//	int *Buf = (int*) calloc(this->maxBlockSize, 1);
//	int i=0;
//
//	list<Reg>::iterator it;
//	cout << " Write -> " ;
//	for(it = regsList.begin(); it!= regsList.end(); it++){
//		Buf[i++] = it->getId();
//		Buf[i++] = it->getFileAdress();
//		cout << " '" << it->getId() << "' '" << it->getFileAdress() << "'";
//	}
//	cout << endl;
//
//	archivo.escribirBloque((void*) Buf, this->getBlockNum(), this->maxBlockSize);
//	archivo.cerrarArchivo();
//	free(Buf);
//}

// Implementacion abriendo archivo de manera cabeza -> Anda =D
void Block::write(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);
	if (!archivo.is_open())
		archivo.open(this->filePath,ios::out|ios::binary);

	int *Buf = (int*) calloc(this->maxBlockSize, 1);
	int i=0;

	list<Reg>::iterator it;
	cout << " Write -> " ;
	for(it = regsList.begin(); it!= regsList.end(); it++){
		Buf[i++] = it->getId();
		Buf[i++] = it->getFileAdress();
		cout << " '" << it->getId() << "' '" << it->getFileAdress() << "'";
	}
	cout << endl;

	archivo.seekg(0,archivo.beg);
	archivo.seekg(this->maxBlockSize*this->getBlockNum());
	archivo.write((char*)Buf,this->maxBlockSize);
	archivo.close();
	free(Buf);
}

// Implementacion con clase de archivo -> No anda
//void Block::read(){
//	ArchivoBloques archivo(this->maxBlockSize, this->filePath);
//	// Inicializamos el archivo de bloques o lo levantamos si ya existia
//	if(archivo.abrirArchivo() == -1){
//		//TODO: Error
//		cout << "Error Abrir archivo!!!!! ***** " << endl;
//		return;
//	}
//	//Si existe pero esta cerrado, lo abrimos -> al pedo
//	if (!archivo.estaAbierto()){
//		archivo.abrirArchivo();
//	}
//	int *buf = (int*) calloc(this->maxBlockSize, 1);
//
//	archivo.leerBloque( (void*)buf, this->getBlockNum());
//	cout << "Read ->" ;
//	//for (int i=0; ((unsigned int) i)<this->maxBlockSize/sizeof(int) && buf[i]!=0; i++){ //Cuando viene un id =0 significa qe ya no hay mas info TODO: controlar qe no se pase del buffer
//	for (int i=0; ((unsigned int) i)<this->maxBlockSize/sizeof(int); i+=2){
//		//TODO: donde libera los registros?
//		cout << " '" << buf[i] << "' '" << buf[i+1] << "'";
//		if(buf[i]!=0){
//			Reg* aReg= new Reg(buf[i],buf[i+1]);
//			this->Insert(*aReg);
//		}
//	}
//	cout << endl;
//	archivo.cerrarArchivo();
//	free(buf);
//	
//}

// Implementacion abriendo archivo de manera cabeza -> Anda =D
void Block::read(){
	fstream archivo(this->filePath,ios::in|ios::out|ios::binary);
	if (!archivo.is_open()){
		cout << "Error Abrir archivo!!!!! ***** " << endl;
		return;
		archivo.open(this->filePath,ios::out|ios::binary);
	}

	int *buf = (int*) calloc(this->maxBlockSize, 1);
	archivo.seekg(0,archivo.beg);
	archivo.seekg(this->maxBlockSize*this->getBlockNum());
	archivo.read((char*)buf, this->maxBlockSize);

	cout << "Read ->" ;
	//for (int i=0; ((unsigned int) i)<this->maxBlockSize/sizeof(int) && buf[i]!=0; i++){ //Cuando viene un id =0 significa qe ya no hay mas info TODO: controlar qe no se pase del buffer
	for (int i=0; ((unsigned int) i)<this->maxBlockSize/sizeof(int); i+=2){
		//TODO: donde libera los registros?
		cout << " '" << buf[i] << "' '" << buf[i+1] << "'";
		if(buf[i]!=0){
			Reg* aReg= new Reg(buf[i],buf[i+1]);
			this->Insert(*aReg);
		}
	}
	cout << endl;
	archivo.close();
	free(buf);
}

/**/
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
	free(this->filePath);
}
