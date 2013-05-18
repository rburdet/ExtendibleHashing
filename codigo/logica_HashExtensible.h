/* ****************************************************************************
 * ****************************************************************************
 * Clase HashExtensible
 * ............................................................................
 * Implementación de la clase que modela un hash extensible.
 *
 * ****************************************************************************
 * ***************************************************************************/

#ifndef HASH_EXTENSIBLE_H
#define HASH_EXTENSIBLE_H

#include <string>
using namespace std;

class HashExtensible {
public:
	//HashExtensible();
	int makeNameANumber(string name);
	static int doHash(int number, int tableSize);
	virtual ~HashExtensible();
};



#endif
 
