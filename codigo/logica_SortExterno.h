#ifndef LOGICA_SORTEXTERNO_H_INCLUDED
#define LOGICA_SORTEXTERNO_H_INCLUDED

/*
 * File:    logica_SortExterno.h
 * Author: damian
 *
 * Created on 12 de mayo de 2013, 13:55
 *
 * Este programa ordena por medio del metodo sort externo un archivo desordenado.
 *
 * PARAMETROS:
 * "partesbytes" = BYTES PARA DETERMINAR EL TAMANIO DE MEMORIA A UTILIZAR PARA ORDENAR.
 * "nombre del archivo desordenado" = no esta la variable creada, está hardcodeada.
 *
 * Nota: el archivo ordenado se graba con el nombre "salida.dat", hay que cambiarlo por
 * el mismo nombre del archivo de entrada. Primero se debe borrar el archivo desordenado
 * y luego renombrar.
 *
 */
 #include "logica_Comparable.h"
 #include <string>
 #include <sstream>
 #include <iostream>
 #include <fstream>

using namespace std;

template<class T>
class SortExterno{
    public:
        SortExterno(std::string file, unsigned int partesBytes);
        ~SortExterno();
        int ordenar();
    private:
        void mergear(unsigned int partes, unsigned int tamanio); // mergea todos los archivos temporales ordenados
        unsigned long dividir (T* array[], unsigned long inicio, unsigned long fin); // utilizado por el sort
        void quicksort(T* array[], unsigned long inicio, unsigned long fin); // metodo de ordenamiento
        void burbuja(T* array[],unsigned int cant);
        std::string file;
        unsigned int partesBytes;
};


template<class T>
SortExterno<T>::SortExterno(std::string file, unsigned int partesBytes){
    this->file = file;
    this->partesBytes= partesBytes;
}

template<class T>
SortExterno<T>::~SortExterno(){
}

template<class T>
int SortExterno<T>::ordenar(){
    std::string cadena = "";
    std::ifstream in;
    in.open((this->file).c_str(), std::ios::binary);
    if(in.fail())
    {
       std::cout << "Error al abrir el archivo desordenado." << std::endl;
       return 1;
    }

    unsigned int tamanyo = sizeof(T);
    //unsigned int tamanyo = 12;
    //unsigned int num_registros
    unsigned int i=0;

    //long posicion;
    //calculamos el número de registros
    in.seekg(0,std::fstream::end); //nos posicionamos al final del fichero

    unsigned int cant_registros;
    //unsigned int totalbytes;
    //número de registros = bytes / tamaño en bytes de un registro

    /*totalbytes = in.tellg(); //cantidad total de registros
    num_registros = totalbytes / tamanyo;
    partes = totalbytes / this->partesBytes;
    if (totalbytes % this->partesBytes) partes++;
    cant_registros = num_registros / partes;
*/
    cant_registros = this->partesBytes / tamanyo;

    //unsigned int registrosCargados
    unsigned int p;
    unsigned int cantACargar;
    //unsigned int cantCargadaParcial;

    //registrosCargados = 0;
    cantACargar = 0;
    //cantCargadaParcial = 0;
    p = 0; // partes

    T *array;
    T** arrayPointer;
    //while (registrosCargados < num_registros)
    in.seekg(0,std::fstream::beg);

    while(!in.eof())
    {
        p = p + 1;
        //posicion = (p - 1) * cant_registros * tamanyo;

        //creamos el array necesario para ordenar los registros del fichero por partes
        array = new T[cant_registros];
        arrayPointer = new T*[cant_registros];
        //data = new char[tamanyo*cant_registros];
        if(array == NULL)
        {
           std::cout << "Error en la asignación de memoria\n";
           return 1;
        }

        //in.seekg(posicion); //nos posicionamos en el archivo

        //Leemos el fichero y pasamos los registros al array
        unsigned int j = 0;
        //in.read(data+(j*tamanyo), tamanyo);
        //array[j]=(T*) (data+(j*tamanyo));
        in >> array[j];
        arrayPointer[j] = array + j;
        while((j < cant_registros-1) and !in.eof())
        {

            /* if ((cantCargadaParcial + j) == num_registros)
             {
                 j--;
                 break;
             }*/
             //in.read((char *) &array[j], tamanyo);
             j++;
             in >> array[j];
             arrayPointer[j] = array + j;
             //in.read(data+(j*tamanyo), tamanyo);
             //array[j]=(T*)(data+(j*tamanyo));
        }
        //cantCargadaParcial = cantCargadaParcial + j + 1;
        if(j==cant_registros-1){
            j++;
        }
        cantACargar = j;

        //ordenar el array
        burbuja(arrayPointer, j);
        //quicksort(arrayPointer, 0, cantACargar-1);

        std::ofstream out;

        //char cnum[6];
        std::stringstream s2;
        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << p))->str();

        //strcpy(cnum, "p");
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");
        s2 << "p" << cadena.data() << ".dat";

        out.open(s2.str().c_str(), std::fstream::app);
        if(out.fail())
        {
           std::cout << "Error al crear el archivo de salida." << std::endl;
           return 1;
        }

        //escribir el array ordenado en el fichero de salida
        for(i = 0; i < cantACargar; i++)
        {
                // PARA DEBUG:
                //cout << "IdPalabra: " << arrayPointer[i]->getPalabraId() << endl;
                //cout << "Codigo: " << array[i].codigo << endl;
                //out.write((char *) array[i], tamanyo);
                out << *arrayPointer[i];
        }
        out.close();
        delete [] array;
        delete [] arrayPointer;
        //delete [] data;

        //registrosCargados = registrosCargados + cant_registros;

    }
    in.close();

    //Merge de x partes
    if (p > 2)
    {
        mergear(p, tamanyo);
    }
    else // no hay que mergear
    {
        rename("p1.dat", (this->file).c_str());
    }
    return 0;

}

template <class T>
void SortExterno<T>::mergear(unsigned int partes, unsigned int tamanio)
{
    unsigned int i = 1, sal_par = 1;
    std::string cadena = "";

    T *r1, *r2;
    r1 =new T;
    r2 = new T;
    std::ifstream t1, t2;
    std::ofstream salida;

    //char cnum[6];


    while (sal_par < partes )
    {
        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << sal_par))->str();
        std::stringstream ss;

        //strcpy(cnum, "s");
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");
        ss << "s" << cadena.data() << ".dat";

        salida.open(ss.str().c_str(), std::ios::binary);

        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();

        std::stringstream ss1;
        if (sal_par == 1) // el primer archivo t1 es p1
        {
            //strcpy(cnum, "p");
            ss1 << "p1";
        }
        else
        {
            //strcpy(cnum, "s"); // los siguientes archivos a mergear son s1, s2, etc
            cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << sal_par - 1))->str();
            ss1 << "s" << cadena.data();
        }
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");
        ss1 << ".dat";
        t1.open(ss1.str().c_str(), std::fstream::binary);

        i++;
        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();

        std::stringstream ss2;
        ss2 << "p" << cadena.data() << ".dat";
        //strcpy(cnum, "p");
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");

        t2.open(ss2.str().c_str(), std::fstream::binary);

        t1.seekg(0);
        t2.seekg(0);


        t1 >> *r1;
        t2 >> *r2;

        while(!t1.eof() && !t2.eof())
        {
            if((r2->comparar(r1))==1)
            {
                 //salida.write((char *) r1, tamanio);
                 salida << *r1;
                 t1 >> *r1;
            }
            else
            {
                 //salida.write((char *) r2, tamanio);
                 salida << *r2;
                 t2 >> *r2;
            }
        }

      while(!t1.eof())
      {
        //salida.write((char *) r1, tamanio);
        salida << *r1;
        t1 >> *r1;
      }
      while(!t2.eof())
      {
        //salida.write((char *) &r2, tamanio);
        salida << *r2;
        t2 >> *r2;
      }

        t1.close();
        t2.close();
        salida.close();
        sal_par++;
    }

    // borrar archivos temporales ordenados
    i = 1;
    for (i = 1; i <= partes; i++)
    {
        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();

        //strcpy(cnum, "p");
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");
        std::stringstream ss3;
        ss3 << "p" << cadena.data() << ".dat";
        remove(ss3.str().c_str());
    }

    // renombrar archivo final ordenado
    cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << sal_par-1))->str();
    std::stringstream ss3;
    //strcpy(cnum, "s");
    //strcat(cnum, cadena.data());
    //strcat(cnum, ".dat");
    ss3 << "s" << cadena.data() << ".dat";
    rename(ss3.str().c_str(), this->file.c_str());

    // borrar archivos temporales del merge
    i = 1;
    for (i = 1; i < sal_par; i++)
    {
        cadena = static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
        std::stringstream ss4;

        ss4 << "s" << cadena.data() << ".dat";
        //strcpy(cnum, "s");
        //strcat(cnum, cadena.data());
        //strcat(cnum, ".dat");
        remove(ss4.str().c_str());
    }
    delete r1;
    delete r2;
}

//funcion para dividir el array y hacer los intercambios del sort
template <class T>
unsigned long SortExterno<T>::dividir (T* array[], unsigned long inicio, unsigned long fin)
{
  unsigned int izq;
  unsigned int der;
  T* pivote;
  T* temp;

  pivote = array[inicio];
  izq = inicio;
  der = fin;

  //Mientras no se cruzen los índices
  while (izq < der)
  {
    while ((array[der]->comparar(pivote))==1) // array[der] > pivote
    {
      der--;
    }

    while ((izq < der) && ((pivote->comparar(array[izq]))==1))
    {
      izq++;
    }

    // Si todavia no se cruzan los indices seguimos intercambiando
    if (izq < der)
    {
      temp = array[izq];
      array[izq] = array[der];
      array[der] = temp;
    }
  }

  //Los indices ya se han cruzado, ponemos el pivote en el lugar que le corresponde
  temp = array[der];
  array[der] = array[inicio];
  array[inicio] = temp;

  //La nueva posición del pivote
  return der;
}

//funcion recursiva para hacer el ordenamiento
template <class T>
void SortExterno<T>::quicksort(T* array[], unsigned long inicio, unsigned long fin)
{
  unsigned long pivote;
  if (inicio < fin)
  {
    pivote = dividir(array, inicio, fin);
    if (pivote > 0)
    {
        quicksort(array, inicio, pivote - 1); //ordeno la lista de los menores
    }
    else
    {
        quicksort(array, inicio, pivote); //ordeno la lista de los menores
    }
    quicksort(array, pivote + 1, fin); //ordeno la lista de los mayores
  }
}

template<class T>
void SortExterno<T>::burbuja(T* array[],unsigned int cant){
    for(unsigned int i = 0; i < cant; i++){
        for(unsigned int j=0; j< (cant-i)-1;j++){
            if((array[j]->comparar(array[j+1]))==1){
                T* temp = array[j];
                array[j] = array[j+1];
                array[j+1] =temp;
            }
        }
    }
}


#endif // LOGICA_SORTEXTERNO_H_INCLUDED
