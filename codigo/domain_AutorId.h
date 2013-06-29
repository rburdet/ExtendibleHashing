#ifndef DOMAIN_AUTORID_H_INCLUDED
#define DOMAIN_AUTORID_H_INCLUDED

#include "logica_Comparable.h"
#include <iostream>

/**
*   Autor: Pablo Rodriguez
*
*   AutorId representa un par de (idAutor, Autor)
*/
class AutorId : public Comparable {
    public:
        AutorId(const char* autor, unsigned int id);
        AutorId();
        virtual ~AutorId();
        virtual int comparar(Comparable *c) const;
        unsigned int getId() const;
        const char* getAutor() const;
        friend std::istream& operator >> (std::istream &out, AutorId &ocur);
        friend std::ostream& operator << (std::ostream &out, AutorId &ocur);
    private:
        char autor[100];
        unsigned int id;
};

#endif // DOMAIN_AUTORID_H_INCLUDED
