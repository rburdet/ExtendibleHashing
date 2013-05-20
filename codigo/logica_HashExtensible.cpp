/*
 * logica_HashExtensible.cpp
 *
 *  Created on: May 11, 2013
 *      Author: rburdet
 */

#include "logica_HashExtensible.h"


int HashExtensible::makeNameANumber(string name) {
	int acum = 0;
	for (unsigned int i = 0; i < name.length(); i++){
		acum = acum + int(name[i]);
	}
	return acum;
}
 int HashExtensible::doHash(int x,int tableSize){
	return (x % tableSize);
}

HashExtensible::~HashExtensible() {
	// TODO Auto-generated destructor stub
}



