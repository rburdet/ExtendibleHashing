#ifndef INDICEAUTOR_H_INCLUDED
#define INDICEAUTOR_H_INCLUDED

#include <iostream>

class IndiceAutor{
    public:
        IndiceAutor();
        ~IndiceAutor();
        int agregar(unsigned int posArch, std::string autor);
        int recuperar(std::string autor, unsigned int* posArch);
        int eliminarTodo();
    private:
        //UN ARBOL


};



#endif // INDICEAUTOR_H_INCLUDED
