#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include <list>
#include <string>

class Utils{
    public:
        static int splitString(std::string s, char delimitador, std::list<std::string> *ptr);
        static std::string getClaveFromHeader(std::string header);
        static int getAutoresFromHeader(std::string s, std::list<std::string>*);
        static std::string getTituloFromHeader(std::string header);
        static std::string uniformizarString(std::string source);
};

#endif // UTILS_H_INCLUDED
