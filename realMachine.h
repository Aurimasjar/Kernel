#ifndef REALMACHINE_H
#define REALMACHINE_H

#include <iostream>
#include <ctime>
#include <cmath>
#include "virtualMachine.h"
#include "word.h"
#include "block.h"

using namespace std;

class Word;
class Block;

class RealMachine
{
    private:

    public:

    int ic = 160;
    //char ptr[4];
    Word ptr;
    int sptr;
    int ba;
    int bb;
    int bc;
    int sf = 0;
    int s = 0;
    int mode = 0;
    int ti = 30;
    int pi = 0;
    int si = 0;
    int ch1 = 0;
    int ch2 = 0;
    int ch3 = 0;
    //char data[1280][4];
    Block data[80];
    
    //Semaphore sem;

    ofstream printer;

    RealMachine();
    ~RealMachine();

    void runProgram(string filename, bool trace = false);

    void checkTrace(bool trace);

    int test();
    void checkInterrupts(VirtualMachine &vm, string command, bool trace);
    void interruptQuit(int status);
    void freeVirtualMemory();

    void execute(string command);

    void printRegisters();
    void printVirtualRegisters();
    void printData();
    void printVirtualData();
    void printPageTable();
    void printReservedBlocks();

    int getRealData(int virtualAddress);
    void setRealData(int x, int virtualAddress);

    void initializePtr();
    void initializePageTable();
    void prepareCommands(VirtualMachine &vm);

    string getCommand();

    //arithmetic operations
    void add();
    void sub();
    void mul();
    void div();

    //operations for working with data
    void la(int x1, int x2);
    void lb(int x1, int x2);
    void ua(int x1, int x2);
    void ub(int x1, int x2);
    void gd(int x1, int x2);
    void pd(int x1, int x2);
    void loc(int x);
    void unl(int x);
    void sla(int x);
    void sua(int x);

    //comparison operations
    void cmp();

    //control operations
    void jm(int x1, int x2);
    void je(int x1, int x2);
    void ja(int x1, int x2);
    void jb(int x1, int x2);
    void halt();
};

#endif