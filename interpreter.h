#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <fstream>
#include "virtualMachine.h"
#include "realMachine.h"

using namespace std;

class Interpreter
{
    private:

    string command;
    string waitCommand;
    ifstream fd;
    bool trace = false;
    VirtualMachine vm;

    public:

    Interpreter(string filename) { fd.open(filename); runProgram(); }
    Interpreter(string filename, bool trace) { this->trace = trace; fd.open(filename); runProgram(); }
    ~Interpreter() { fd.close(); }

    void runProgram();

    void execute();
};

#endif