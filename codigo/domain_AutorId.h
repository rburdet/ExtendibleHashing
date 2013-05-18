#ifndef DOMAIN_AUTORID_H_INCLUDED
#define DOMAIN_AUTORID_H_INCLUDED

#include "logica_Comparable.h"

class AutorId : public Comparable {
    public:
        AutorId(char* autor, unsigned int id);
        ~AutorId();
        virtual int comparar(Comparable *c) const;
        unsigned int getId() const;
        const char* getAutor() const;
    private:
        char autor[100];
        unsigned int id;
};

#endif // DOMAIN_AUTORID_H_INCLUDED