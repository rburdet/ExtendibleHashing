#include <iostream>
#include "interfaz_Menu.h"
#include "logica_Validator.h"
#include "logica_Indexer.h"
#include "logica_Buscador.h"

using namespace std;

Menu::Menu(){
    //CARGAR DESDE CONFIG LAS RUTAS Y DEMAS PARAMETROS
}

Menu::~Menu(){
}

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
    cout << "6. Salir" << endl;
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
            from = "/home/pablo/tpDatos/7506-tp-grupo07/temas";
            cout << "Indexando canciones desde el directorio" << from << endl;
            in->indexarCancionesDesde(0);
            delete in;
            break;
        case 2:
            in = new Indexer;
            cout << "Indexando canciones en modo append" << endl;
            cout << "Indexando canciones desde el directorio" << from << endl;
            in->indexarCancionesDesde(1);
            delete in;
            break;
        case 3:
            busc = new Buscador;
            cout << "Ingrese el autor a buscar" << endl;
            cin >> text;
            busc->buscarPorAutor(text);
            delete busc;
            break;
        case 4:
            busc = new Buscador;
            cout << "Ingrese el titulo a buscar" << endl;
            cin >> text;
            busc->buscarPorTitulo(text);
            delete busc;
            break;
        case 5:
            busc = new Buscador;
            cout << "Ingrese la frase a buscar" << endl;
            cin >> text;
            busc->buscarPorFrase(text);
            delete busc;
            break;
        case 6:
            return 0;
            break;
    }
    return 1;
}

