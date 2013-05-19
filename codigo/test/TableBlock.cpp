#include <iostream>
#include "../interfaz_Menu.h"
#include "../logica_BlockTable.h"
#include "../logica_Reg.h"

#define TEST_FILE "./test_file_TESTBLOCKFILE"
using namespace std;

int DATA[5][2] = {
	{1, 1},
	{2, 2},
	{3, 3},
	{4, 4},
	{5, 5}
};

int main()
{
	int dispersion = 1, blockN = 0;
	Reg* regs[5];
	Block *tmpBlock = new Block(dispersion, blockN);

	for (int i=0;i<5;i++){
		regs[i] = new Reg(DATA[i][0],DATA[i][1]);
		tmpBlock->Insert(*regs[i]);
	}

	tmpBlock->write(TEST_FILE);
	//tmpBlock->read(TEST_FILE);

	delete tmpBlock;
	for(int i=0;i<5;i++)
		delete regs[i];

	tmpBlock = new Block(dispersion, blockN);
	tmpBlock->read(TEST_FILE);

	list<Reg> regsList = tmpBlock->getRegList();

	list<Reg>::iterator it;
	int i = 0;
	for(it = regsList.begin(); it!= regsList.end(); it++){
		int id = it->getId(), ad = it->getFileAdress();
		if(id != 0 && (DATA[i][0] != id || DATA[i++][1] != ad))
			cout << "ERROR i=" << i << " id=" << id << " add=" << ad << endl;
	}

	cout << "No error" << endl;
    return 0;
}
