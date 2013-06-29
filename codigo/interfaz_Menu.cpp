#include <iostream>
#include "interfaz_Menu.h"
#include "logica_Validator.h"
#include "logica_Indexer.h"
#include "logica_Buscador.h"
#include "logica_Utils.h"
#include "runtimeConfig.h"


using namespace std;

Menu::Menu(){ }

Menu::~Menu(){ }


int Menu::start(){
    string op;
    this->imprimir();
    cin >> op;
    int r = 0;
    int opcion = 0;
    if ((opcion = Validator::validateOpcion(op))){
        r = this->ingresarOpcion(opcion);
    }else{
        cout << "Opcion Invalida" << endl;
        return 1;
    }
    return r;
}


int Menu::imprimir()
{
    cout << "Ingrese una opcion" << endl;
    cout << "1. Indexar canciones from scratch" << endl;
    cout << "2. Indexar canciones en modo append" << endl;
    cout << "3. Buscar canciones por autor" << endl;
    cout << "4. Buscar canciones por titulo" << endl;
    cout << "5. Buscar canciones por frase" << endl;
    cout << "6. Listar todos los temas indexados" << endl;
    cout << "7. Ver estadísticas" << endl;
    cout << "8. Salir" << endl<<endl;
    return 0;
}


int Menu::ingresarOpcion(int opcion){
    string text;
    Indexer *in;
    Buscador *busc;
    switch(opcion){
        case 1:
            in = new Indexer;
            cout << "Indexando canciones desde 0" << endl;
            cout << "Eliminando cualquier registro de canciones previo" << endl;
            in->eliminarTodo();
            in->indexarCancionesDesde(0);
            std::cout << std::endl;
            delete in;
            break;
        case 2:
            in = new Indexer;
            cout << "Indexando canciones en modo append" << endl;
            in->indexarCancionesDesde(1);
            std::cout << std::endl;
            delete in;
            break;
        case 3:
            busc = new Buscador(&this->estadista);
            std::cout << "Ingrese el autor a buscar" << std::endl;
            std::cin.ignore();
            std::getline(std::cin, text);

            // Censamos en el estadista
            this->estadista.censarBusquedaPorAutor(text);

            text = Utils::uniformizarString(text);
            std::cout << std::endl;
            while (text.compare("0")){
                busc->buscarPorAutor(text);
                std::cout << std::endl;
                std::cout << "Ingrese el autor a buscar / Ingrese '0' para volver al menú principal" << std::endl;
                std::getline(std::cin, text);
                text = Utils::uniformizarString(text);
                std::cout << std::endl;
            }
            delete busc;
            break;
        case 4:
            busc = new Buscador(&this->estadista);
            cout << "Ingrese el titulo a buscar" << endl;
            std::cin.ignore();
            std::getline(std::cin, text);

            // Censamos en el estadista
            this->estadista.censarBusquedaPorTitulo(text);

            text = Utils::uniformizarString(text);
            std::cout << std::endl;
            while (text.compare("0")){
                busc->buscarPorTitulo(text);
                std::cout << std::endl;
                std::cout << "Ingrese el titulo a buscar / Ingrese '0' para volver al menú principal" << std::endl;
                std::getline(std::cin, text);
                text = Utils::uniformizarString(text);
                std::cout << std::endl;
            }
            delete busc;
            std::cout << std::endl;
            break;
        case 5:
            busc = new Buscador(&this->estadista);
            cout << "Ingrese la frase a buscar" << endl;
            std::cin.ignore();
            std::getline(std::cin, text);

            // Censamos en el estadista
            this->estadista.censarBusquedaPorFrase(text);

            std::cout << std::endl;
            while (text.compare("0")){
                busc->buscarPorFrase(text);
                std::cout << std::endl;
                std::cout << "Ingrese la frase a buscar / Ingrese '0' para volver al menú principal" << std::endl;
                std::getline(std::cin, text);
                std::cout << std::endl;
            }
            delete busc;
            std::cout << std::endl;
            break;
        case 6:
            busc = new Buscador(&this->estadista);
            busc->listarTodo();
            delete busc;
            break;
        case 7:
            {
                std::cout << std::endl << "ESTADÍSTICAS" << std::endl;
                std::cout << std::string(80, '-') << std::endl;

                // Obtenemos las estadísticas recaudadas hasta el momento
                std::vector< std::string > titulos, autores, frases, temas;
                this->estadista.obtenerTitulosMasBuscados(titulos,
                    estadistaMaximoTitulosMasBuscados());
                this->estadista.obtenerAutoresMasBuscados(autores,
                    estadistaMaximoAutoresMasBuscados());
                this->estadista.obtenerFrasesMasBuscadas(frases,
                    estadistaMaximoFrasesMasBuscadas());
                this->estadista.obtenerTemaConMayorCoincidencias(temas);
                
                // Imprimimos titulos mas buscados
                std::cout << std::endl <<  "TITULOS MAS BUSCADOS:" << std::endl;

                if(titulos.empty())
                    std::cout << "No se han realizado búsquedas por título aún." << std::endl;
                else {
                    for(unsigned int i = 0; i < titulos.size(); i++)
                        std::cout << "- " << titulos[i] << std::endl;
                }

                // Imprimimos autores mas buscados
                std::cout << std::endl << "AUTORES MAS BUSCADOS:" << std::endl;

                if(autores.empty())
                    std::cout << "No se han realizado búsquedas por autor aún."
                        << std::endl;
                else {
                    for(unsigned int i = 0; i < autores.size(); i++)
                        std::cout << "- " << autores[i] << std::endl;
                }

                // Imprimimos frases mas buscadas
                std::cout << std::endl << "FRASES MAS BUSCADAS:" << std::endl;

                if(frases.empty())
                    std::cout << "No se han realizado búsquedas por frase aún."
                        << std::endl;
                else {
                    for(unsigned int i = 0; i < frases.size(); i++)
                        std::cout << "- " << frases[i] << std::endl;
                }

                // Imprimimos tema mas coincidente con las busquedas
                std::cout << std::endl << "TEMA QUE COINCIDIO MAS VECES EN TODAS LAS BUSQUEDAS:" << std::endl;

                if(temas.empty())
                    std::cout << "No se han realizado búsquedas aún." 
                        << std::endl;
                else
                    for(unsigned int i = 0; i < temas.size(); i++)
                        std::cout << "- " << temas[i] << std::endl;

                std::cout << std::endl << std::endl;
                std::cout << "NOTA: Cuando son varios los temas que coincidieron como los mas buscados, se\nmuestran todos estos." << std::endl;

                std::cout << std::endl << std::endl;
                std::cin.ignore();
                while (text.compare("0")) {
                    std::cout << "Ingrese '0' para volver al menú principal" 
                        << std::endl;
                    std::getline(std::cin, text);
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            break;
        case 8:
            return 0;
            break;
    }
    return 1;
}

