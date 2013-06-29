#include <iostream>
#include "interfaz_Menu.h"

int main()
{
	Menu *m = new Menu;
	while(m->start());
	delete m;
	return 0;
}
