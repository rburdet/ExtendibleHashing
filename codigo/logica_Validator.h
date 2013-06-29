#ifndef VALIDATOR_H_INCLUDED
#define VALIDATOR_H_INCLUDED

#include <iostream>

/**
*   Autor: Pablo Rodriguez
*
*   Se encarga de validar diversas cosas
*/
class Validator{
    public:
        static int validateOpcion(std::string op);
        static int validateHeader(std::string header);

};



#endif // VALIDATOR_H_INCLUDED
