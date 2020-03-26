#ifndef REALMACHINE_H
#define REALMACHINE_H

#include <iostream>
#include <ctime>
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
    int s;
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

    RealMachine();

    void runProgram(string filename, bool trace = false);

    void checkTrace(VirtualMachine &vm, bool trace);

    int test();
    void checkInterrupts(VirtualMachine &vm);
    void interruptQuit(int status, VirtualMachine &vm);
    void freeVirtualMemory(VirtualMachine &vm);

    void execute(VirtualMachine &vm);

    void printRegisters();
    void printVirtualRegisters(VirtualMachine &vm);
    void printData();
    void printVirtualData();
    void printPageTable();
    void printReservedBlocks();

    int getRealData(int virtualAddress);
    void setRealData(int x, int virtualAddress);

    void initializePtr();

    void initializePageTable();

    //arithmetic operations
    void add(VirtualMachine &vm);
    void sub(VirtualMachine &vm);
    void mul(VirtualMachine &vm);
    void div(VirtualMachine &vm);

    //operations for working with data
    void la(VirtualMachine &vm, int x1, int x2);
    void lb(VirtualMachine &vm, int x1, int x2);
    void ua(VirtualMachine &vm, int x1, int x2);
    void ub(VirtualMachine &vm, int x1, int x2);
    void gd(VirtualMachine &vm, int x1, int x2);
    void pd(VirtualMachine &vm, int x1, int x2);
    void loc(VirtualMachine &vm, int x);
    void unl(VirtualMachine &vm, int x);
    void sla(VirtualMachine &vm, int x);
    void sua(VirtualMachine &vm, int x);

    //comparison operations
    void cmp(VirtualMachine &vm);

    //control operations
    void jm(VirtualMachine &vm, int x1, int x2);
    void halt(VirtualMachine &vm);
};

#endif