#ifndef INDICEPRIMARIO_H_INCLUDED
#define INDICEPRIMARIO_H_INCLUDED

#include <iostream>

class IndicePrimario{
    public:
        IndicePrimario();
        ~IndicePrimario();
        int agregar(std::string claveTema, int posArch);
        int recuperar(std::string claveTema);
        int eliminarTodo();
    private:
        //UN HASH


};

#endif // INDICEPRIMARIO_H_INCLUDED
