#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <iostream>

class Menu
{
    public:
        Menu();
        ~Menu();
        int imprimir();
        int ingresarOpcion(int opcion);
        int start();
    private:
       std::string from;



};


#endif // MENU_H_INCLUDED
