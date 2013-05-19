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



Block::Block(int dispersionSize, int blockNum){
	blockCurrentSize=0;
	this->dispersionSize=dispersionSize;
	//this->blockAdress=blockAdress; TODO: Como hago para laburar con el num de bloque?
	//								 [rburdet]: Con la posicion del archivo de bloques (.HashBlock)
	maxBlockSize=MAX_BLOCK_SIZE;
	this->blockNum = blockNum;
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

int Block::getCurrentSize(){
	return this->blockCurrentSize;
}

bool Block::easyInsert(Reg& aReg){
	cout << " REG_SIZE " << aReg.getSize() << " blockCurre " << this->blockCurrentSize << " MAX_BLOCK " << MAX_BLOCK_SIZE <<  endl;
	//return (aReg.getSize()+ blockCurrentSize < MAX_BLOCK_SIZE);
	return (aReg.getSize()+ this->blockCurrentSize < MAX_BLOCK_SIZE);
}

void Block::setList(list<Reg> newRegList){
	this->regsList=newRegList;
}

//  Devuelve el addres si lo encuentra, sino -1, no ponog 0 por qe puede qe el addres sea 0, no?
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

void Block::write(const char* fileName){
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
	//cout << " Max Block Size " << MAX_BLOCK_SIZE << endl;
	int Buf[MAX_BLOCK_SIZE / sizeof(int)] = {0};
	int i=0;
	list<Reg>::iterator it;
	for(it = regsList.begin(); it!= regsList.end(); it++){
		//cout << "Voy a guardar esto i=" << i << " id: " << it->getId()  << " add: " << it->getFileAdress() << endl;
		Buf[i++] = it->getId();
		Buf[i++] = it->getFileAdress();
	}

	//Imprimo para debuggear
	//for(unsigned int i=0; i < (MAX_BLOCK_SIZE / sizeof(int) ) ; i+=2)
	//	printf("i= %d id= %d ad=%d \n", i, Buf[i],  Buf[i+1]);
	//--

	//cout << "num block " << this->getBlockNum() << endl;
	archivo->escribirBloque((void*) Buf, this->getBlockNum(), MAX_BLOCK_SIZE);
	archivo->cerrarArchivo();
}

void Block::read(const char* fileName){
	ArchivoBloques* archivo = new ArchivoBloques(MAX_BLOCK_SIZE, fileName);
	// Inicializamos el archivo de bloques o lo levantamos si ya existia
	if(archivo->abrirArchivo() == -1){
		// El archivo no existe, lo creamos
	//	//archivo->crearArchivo();
		//archivo->abrirArchivo();
		return;
	}
	//Si existe pero esta cerrado, lo abrimos -> al pedo
	if (!archivo->estaAbierto()){
		archivo->abrirArchivo();
	}
	int buf[MAX_BLOCK_SIZE/sizeof(int)] = {0};

	archivo->leerBloque((void*)buf,this->getBlockNum());
	for (int i=0; ((unsigned int) i)<MAX_BLOCK_SIZE/sizeof(int) && buf[i]!=0; i++){ //Cuando viene un id =0 significa qe ya no hay mas info TODO: controlar qe no se pase del buffer
		Reg* aReg= new Reg(buf[i],buf[i++]);
		this->Insert(*aReg);
		//cout << i << ") id= " << aReg->getId() << " add=" << aReg->getFileAdress() << endl;

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
