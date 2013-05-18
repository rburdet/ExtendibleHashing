/*
 * fileHandler.h
 *
 *	Esta clase es la encargada de manejar los archivos de texto, dando informacion acerca del autor, nombre del tema
 *	y otra informacion que pueda ser util a la hora de generar los indices
 *
 *  Created on: Apr 26, 2013
 *      Author: rburdet
 */

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <string>

using namespace std;

class FileHandler {
public:
	FileHandler();
	virtual ~FileHandler();
	string getFirstLine(string fileName);
	string getAuthor(string line);
	string getSongName(string line);
};

#endif /* FILEHANDLER_H_ */
