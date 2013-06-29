#ifndef LOGICA_COMPARABLE_H_INCLUDED
#define LOGICA_COMPARABLE_H_INCLUDED

/**
*   Autor: Pablo Rodriguez
*
*   Clase abstracta que permite que dos objetos se comparen entre si
*/
class Comparable{
    public:
    virtual int comparar(Comparable* c) const = 0;
};

#endif // LOGICA_COMPARABLE_H_INCLUDED
