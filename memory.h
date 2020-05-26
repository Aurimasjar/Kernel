#ifndef MEMORY_H
#define MEMORY_H

#include "kernel.h"
#include "block.h"
#include "word.h"

using namespace std;

class Kernel;

class Memory
{
    public:

    Block data[80];
    Kernel *kernel;

    Memory(Kernel *kernel);
    ~Memory() {}

    void initMemory();
    int getRealData(int virtualAddress);
    void setRealData(int x, int virtualAddress);
    void setRealData(Word x, int virtualAddress);
};

#endif