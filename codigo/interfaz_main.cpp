#include <iostream>
#include "interfaz_Menu.h"
#include "logica_ArbolBmas.h"
#include "logica_BlockTable.h"
#include "logica_Reg.h"

using namespace std;

int main()
{
	BlockTable * aBlockTable = new BlockTable();
	for (int i = 1 ; i<= 130 ; i++)
	{
		Reg * aReg = new Reg(i,10);
		aBlockTable->insert(*aReg);
	}
    // Menu *m = new Menu;
    // while(m->start());
    // delete m;
    return 0;
}
