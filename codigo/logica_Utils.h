#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include <list>
#include <string>

/**
*   Autor: Pablo Rodriguez
*
*   Contiene metodos staticos que son de utilidad para el manejo del header y strings
*/
class Utils{
    public:
        static int splitString(std::string s, char delimitador, std::list<std::string> *ptr);
        static std::string getClaveFromHeader(std::string header);
        static int getAutoresFromHeader(std::string s, std::list<std::string>*);
        static std::string getTituloFromHeader(std::string header);
        static std::string uniformizarString(std::string source);
        static int existeArchivo(std::string archivo);
};

#endif // UTILS_H_INCLUDED
