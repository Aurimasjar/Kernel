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

    int ic = 0;
    //char ptr[4];
    Word ptr;
    int sptr;
    int ba;
    int bb;
    int bc;
    int sf;
    int s = 0;
    int mode;
    int ti = 30;
    int pi = 0;
    int si = 0;
    int ch1;
    int ch2;
    int ch3;
    //char data[1280][4];
    Block data[80];
    
    //Semaphore sem;

    ofstream printer;

    RealMachine();
    ~RealMachine();

    void runProgram(string filename, bool trace = false);

    void checkTrace(VirtualMachine &vm, bool trace);

    int test();
    void checkInterrupts(VirtualMachine &vm);
    void interruptQuit(int status);
    void freeVirtualMemory();

    void execute(VirtualMachine &vm);

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
    void halt();
};

#endif