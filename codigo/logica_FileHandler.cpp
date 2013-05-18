/*
 * fileHandler.cpp
 *
 *  Created on: Apr 26, 2013
 *      Author: rburdet
 */

#include "logica_FileHandler.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

FileHandler::FileHandler() {
}

FileHandler::~FileHandler() {
}

string FileHandler::getFirstLine(string fileName){
	string line;
	ifstream inFile;
	inFile.open(fileName.c_str());
	getline(inFile,line);
	inFile.close();
	return line;
}

string FileHandler::getAuthor(string line){
	int pos = line.find_first_of("-");
	line.erase(pos,line.size());
	return line;
}

string FileHandler::getSongName(string line) {
	string tmp=line;
	int pos1 = line.find_first_of("-");
	line.erase(0,pos1+1);
	int pos2 = line.find_first_of("-");
	line.erase(pos2,line.length()-1);
	if (atoi(line.c_str())){
		tmp.erase(0,pos1+1+pos2+1);
		tmp.erase(tmp.find_first_of("-"),tmp.length());
		return tmp;
	}
	else{
		tmp.erase(0,tmp.find_first_of("-")+1);
		tmp.erase(tmp.find_first_of("-"),tmp.size());
		return tmp;
	}


}
