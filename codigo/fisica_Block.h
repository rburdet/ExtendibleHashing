/*
*	Block.h
*
*	Esta clase es la mas importante dentro del hash. Incluye informacion vital para el funcionamiento del mismo.
Esta informacion incluye:
	Posicion del bloque en el archivo
	Tamanio de dispersion del bloque
	Los registros que contiene cada bloque
*
*
*/

#ifndef BLOCK_H_
#define BLOCK_H_


#include "logica_Reg.h"
#include <list>
#include "fisica_ArchivoBloques.h"

using namespace std;
/*
 * El tamanio de registros en un bloque debe poder ser configurable desde afuera
 * ver bien !!!
 * */

class Block {
	public:
		Block(int DispersionSize,int blockAdress); //Creo un bloque en una tabla de bloques. Inicialmente el tamanio es 0

		//  Agrego un registro a la cubeta, los resultados posibles son: 0 sino se agrego, 1 si se agrego, 2 si se duplico el tamanio.
		int Insert(Reg & aReg);

		// Si el bloque queda en overflow tengo que crear otro para luego redistribuir los registros*/
		Block* createNew(int newDispersionSize);

		//crea un nuevo bloque, redispersando y cambiando los tamanios de dispersion de ambos bloques
		int separate();

		// Cuando se produce un overflow, tengo que redispersar los registros */
		int redistribute(Block * aNewBlock,int tableSize);

		// Cuando se duplica la tabla, o un bloque queda en overflow pero con mas de 1 referencia en la tabla entonces es necesario cambiar el tamanio de dispersion del bloque
		// Devuelve el nuevo tamanio de dispersion
		void changeDispersionSize(int);

		// Cuando un bloque queda en overflow, necesito buscar el proximo bloque en donde pueda redispersar,  */
		int getBlockInTable();
		
		//Si entra el registro devuelve verdadero, si no falso
		bool easyInsert(Reg& aReg);

		//Devuelve el tamanio de dispersion del bloque
		int getDispersionSize();

		//Cuando se agrega un nuevo bloque se tiene que duplicar el tamanio de dispersion del bloque
		int duplicateDispersionSize();

		//Devuelve el numero de bloque
		int getBlockNum();

		//Devuelve la lista de registros
		list<Reg> getRegList();

		//Devuelve la posicion del bloque en el archivo de bloques
		int getBlockAdress();

		//Devuelve el espacio ocupado dentro del bloque
		int getCurrentSize();

		void setList(list<Reg> newRegList);

		//Una vez que obtengo el bloque, tengo que buscar en la lista el registro que yo quiero
		//Devuelvo el fileAdress de ese registro que es donde estara guardado el dato
		//SI NO SE ENCUENTRA EL REGISTRO SE DEVUELVE 0
		int search(Reg& aReg);

		void read(const char* fileName);
		void write(const char* fileName);
		//TODO: solo el cierra
		void open(const char * fileName);
		void close();
		ArchivoBloques* getArchivo();

		~Block();

	protected:
		int blockCurrentSize;
		int maxBlockSize; //Privado? constante?

		//int blockAdress; TODO: SE USA ???
		int dispersionSize;
		list<Reg> regsList;
		//ArchivoBloques* archivo; //privado? constante?
		int blockNum; //lo puedo llegar a necesitar

	private:
		struct Metadata{

		};

};
#endif
