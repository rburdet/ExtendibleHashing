#include <iostream>
#include "interfaz_Menu.h"
#include "logica_BlockTable.h"
#include "logica_Reg.h"

using namespace std;

int main()
{
	BlockTable* aBlockTable = new BlockTable();
//	for(int i=0;i<10;i++){
		Reg *aReg= new Reg(10,10);
		aBlockTable->insert(*aReg);
//	}
   /* Menu *m = new Menu;
    while(m->start());JEjeje
    delete m;*/

	delete aBlockTable;

    return 0;
}
