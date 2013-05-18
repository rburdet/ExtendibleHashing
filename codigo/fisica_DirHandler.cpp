/*
 * dirHandler.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: rburdet
 */

#include "fisica_DirHandler.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

DirHandler::DirHandler(const char* path) {
	this->fileList= new list<string>;
	this->initialPathName=path;
}

DirHandler::~DirHandler() {
	delete this->fileList;
}

list<string>* DirHandler::getFileList() {
	return this->fileList;
}

void DirHandler::setPath(const char* path){
	this->initialPathName=path;
}

/*
 *  Metodo encargado de recorrer recursivamente el directorio, haciendo uso de la libreria de C dirent.h
 * 	En un atributo protegido guarda una lista de los nombres de los temas del directorio elegido, y de todos
 * 	los temas en los subdirectorios. El nombre ademas incluye ademas el pathname, por ejemplo para el archivo
 * 	"Yesterday.txt" guardara "home/Usuario/music/lyrics/The_Beatles/Yesterday.txt".
 *
 */
void DirHandler::getFiles(const char* dir){
	DIR *dirStream;
	struct dirent *dirEntry  = 0;
	dirStream = opendir(dir);

	if (! dirStream)
		return ;
	while ((dirEntry = readdir(dirStream))){
		if (dirEntry->d_name[0]=='.')
			continue;
		if (dirEntry->d_type == DT_DIR){
			char *tmp=new char[strlen(dir)+strlen(dirEntry->d_name)+1];
			sprintf(tmp,"%s/%s", dir, dirEntry->d_name);
			this->setPath(tmp);
			this->getFiles(tmp);
		}
		else{
			string actualPath = dir;
			actualPath += "/";
			actualPath += dirEntry->d_name;
			this->fileList->push_back(actualPath);
		}
	}
	(void) closedir(dirStream);
}
