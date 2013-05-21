#include <iostream>
#include "../interfaz_Menu.h"
#include "../logica_BlockTable.h"
#include "../logica_Reg.h"

#define TEST_FILE "./test_file_TESTBLOCKFILE"
#define TEST_FILE_TABLE "./test_file_TESTTABLEFILE"
#define DATA_LENGTH 7
#define BLOCK_SIZE 24

using namespace std;

int DATA[DATA_LENGTH][2];

void fillData(){
	for(int i=0;i<DATA_LENGTH;i++){
		DATA[i][0] = i+1;
		DATA[i][1] = i+1;
	}
}

int main()
{
	BlockTable* aBlockTable = new BlockTable(TEST_FILE_TABLE, TEST_FILE, BLOCK_SIZE);
	int error=0;

	//Inserto
	fillData();
	for(int i=0;i<DATA_LENGTH;i++){
		cout << "** Agregando registro "<< i  << endl;
		Reg *aReg= new Reg(DATA[i][0],DATA[i][0]);
		aBlockTable->insert(*aReg);
		delete aReg;
	}

	//Busco
	for(int i=0;i<DATA_LENGTH;i++){
		Reg* myReg = new Reg(DATA[i][0], 1);
		int ad = aBlockTable->search(*myReg);
		if( ad != DATA[i][1]){
			cout << " \t ** Error i=" << i << " id=" << DATA[i][0] << " ad=" << DATA[i][1] << " ret_val=" << ad << endl;
			error++;
		}
	}


	cout << "fin " ;
	if(error)
		cout << "ERROR";
	cout << endl;


	delete aBlockTable;

    return 0;
}
