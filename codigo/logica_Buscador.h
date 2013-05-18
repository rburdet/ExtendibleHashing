#ifndef BUSCADOR_H_INCLUDED
#define BUSCADOR_H_INCLUDED

class Buscador{
    public:
        Buscador();
        ~Buscador();
        int buscarPorAutor(std::string autor);
        int buscarPorTitulo(std::string titulo);
        int buscarPorFrase(std::string frase);
    private:

};


#endif // BUSCADOR_H_INCLUDED
