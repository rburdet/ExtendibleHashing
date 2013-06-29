#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include "logica_Utils.h"

int Utils::splitString(std::string s, char del, std::list<std::string> *ptr){
    std::string s2;
    std::istringstream is(s);
    while (std::getline(is,s2,del)){
        ptr->push_back(s2);
    }
    return 0;
}


std::string Utils::getClaveFromHeader(std::string header){
    std::list<std::string> *ptr = new std::list<std::string>;
    Utils::splitString(header,'-',ptr);
    std::ostringstream s;
    s << Utils::uniformizarString(ptr->front()) << "-";
    ptr->pop_front();
    if(ptr->size() == 3){
        ptr->pop_front();
    }
    s << Utils::uniformizarString(ptr->front()) << "-";
    ptr->pop_front();
    s << Utils::uniformizarString(ptr->front());
    delete ptr;
    return s.str();
}

int Utils::getAutoresFromHeader(std::string header, std::list<std::string>* lista){
    std::list<std::string> *ptr = new std::list<std::string>;
    Utils::splitString(header,'-',ptr);
    std::string s = ptr->front();
    delete ptr;
    std::string s2;
    std::istringstream is(s);
    while (std::getline(is,s2,';')){
        lista->push_back(Utils::uniformizarString(s2));
    }
    return 0;
}

std::string Utils::getTituloFromHeader(std::string header){
    std::list<std::string> *ptr = new std::list<std::string>;
    Utils::splitString(header,'-',ptr);
    ptr->pop_front();
    if(ptr->size() == 3){
        ptr->pop_front();
    }
    std::string titulo = Utils::uniformizarString(ptr->front());
    delete ptr;
    return titulo;
}

std::string Utils::uniformizarString(std::string source){
    std::ostringstream dest;
    char c;
    for(unsigned int i=0; i < source.size();i++){
        c = source.at(i);
        if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
            dest << c;
        }else if(c >= 'A' && c <= 'Z'){
            dest << (char)tolower(c);
        }else{
            if (c == '\xc3') {
                i++;
                c = source.at(i);
                switch (c) {
                    case '\xa1':
                        dest << 'a';
                        break;

                    case '\x81':
                        dest << 'a';
                        break;

                    case '\xa9':
                        dest << 'e';
                        break;

                    case '\x89':
                        dest << 'e';
                        break;

                    case '\xad':
                        dest << 'i';
                        break;

                    case '\x8d':
                        dest << 'i';
                        break;

                    case '\xb3':
                        dest << 'o';
                        break;

                    case '\x93':
                        dest << 'o';
                        break;

                    case '\xba':
                        dest << 'u';
                        break;

                    case '\x9a':
                        dest << 'u';
                        break;

                    case '\xb1':
                        dest << 'n';
                        dest << 'i';
                        break;

                    case '\x91':
                        dest << 'n';
                        dest << 'i';
                        break;

                    default:
                        break;
                }
            }
        }
    }
    return dest.str();
}

int Utils::existeArchivo(std::string archivo){
    std::ifstream file;
    file.open(archivo.c_str());
    bool existe = file.good();
    file.close();
    return existe;
}
