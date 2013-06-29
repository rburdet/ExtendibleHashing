#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <iostream>
#include "logica_Estadista.h"


/**
*   Autor: Pablo Rodriguez
*
*   Es la encargada de imprimir el menu y actuar en funcion de la opcion elegida
*/
class Menu
{
    public:
        Menu();
        ~Menu();
        int imprimir();
        int ingresarOpcion(int opcion);
        int start();
    private:
    	Estadista estadista;
		std::string from;



};


#endif // MENU_H_INCLUDED
