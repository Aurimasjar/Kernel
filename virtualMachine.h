#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>
#include <fstream>
//#include "realMachine.h"

using namespace std;

class VirtualMachine
{
    private:

    //int ic = 0;
    //int ba;
    //int bb;
    //char data[256];
    ifstream program;

    string command;

    public:

    ifstream externalData;

    VirtualMachine(string filename)
    {
        /*ic = 0;
        ba = 0;
        bb = 0;*/
        program.open(filename);
        externalData.open("data.txt");
        //printer << "Atidarem" << endl;
    }

    ~VirtualMachine()
    {
        //printer << "Uzdarem" << endl;
        program.close();
        externalData.close();
    }

    /*void setIC(int ic) { this->ic = ic; }
    int getIC() { return ic; }

    void setBA(int ba) { this->ba = ba; }
    int getBA() { return ba; }

    void setBB(int bb) { this->bb = bb; }
    int getBB() { return bb; }*/

    string getCommand();
    //char getCommand(int pos);

    string readCommand();
};

#endif