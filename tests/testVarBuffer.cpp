//
//  main.cpp
//  FileHandler
//
//  Created by Federico Colangelo on 21/04/13.
//  Copyright (c) 2013 Federico Colangelo. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "fisica_VarBuffer.h"

#define BUFFSIZE 128
#define TESTCASES 10

using namespace std;

struct testStructVarBuffer {
    int unEntero;
    char* unString;
    bool unBool;
};

int assertEqualsVarBuffer (testStructVarBuffer &unStruct, testStructVarBuffer &otroStruct)
{
    return (unStruct.unEntero==otroStruct.unEntero && unStruct.unString==otroStruct.unString && unStruct.unBool==otroStruct.unBool);
    
}


int main(int argc, const char * argv[])
{     
    fstream outFile("testVarBuffer.dat",ios::out|ios::binary);
    
    cout<<outFile.tellp()<<endl;
    
    testStructVarBuffer unTestStruct[TESTCASES];
    
    VarBuffer unBuffer(BUFFSIZE);
    
    srand(time(NULL));
    
    for (int i=0;i<TESTCASES;i++) {
        unTestStruct[i].unEntero=rand()%100;
        unTestStruct[i].unString=(char*)(rand()%10000);
        unBuffer.pack(&unTestStruct[i], sizeof(unTestStruct[i]));
        unBuffer.write(outFile);
        cout<<outFile.tellp()<<endl;
        
    }
    outFile.close();
    
    fstream inFile("testVarBuffer.dat",ios::in|ios::binary); 
    testStructVarBuffer otroTestStruct;
    
    for (int i=0; i<TESTCASES; i++) {
        unBuffer.read(inFile);
        cout<<inFile.tellg()<<endl;
        unBuffer.unpack(&otroTestStruct);
        if(!assertEqualsVarBuffer(unTestStruct[i], otroTestStruct)) {cerr<<"Las estructuras son diferentes"<<endl; return -1;};
        };
    inFile.close();
    
    cout<<"Prueba testVarBuffer EXITOSA"<<endl;
    return 0;
}