#include "memory.h"

using namespace std;

Memory::Memory(Kernel *kernel)
{
    this->kernel = kernel;
}

void Memory::initMemory()
{
    for(int i = 0; i < 80; i++)
    {
        // set block words to 0x00000000
    }
}

int Memory::getRealData(int virtualAddress)
{
    int a2 = kernel->cpu.ptr.getByte(2);
    int a3 = kernel->cpu.ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    return Word::wordToInt(data[Word::wordToInt(data[16 * a2 + a3].getWord(x1))].getWord(x2));
}

void Memory::setRealData(int x, int virtualAddress)
{
    int a2 = kernel->cpu.ptr.getByte(2);
    int a3 = kernel->cpu.ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    data[Word::wordToInt(data[16 * a2 + a3].getWord(x1))].setWord(Word::intToWord(x), x2);
}

void Memory::setRealData(Word x, int virtualAddress)
{
    int a2 = kernel->cpu.ptr.getByte(2);
    int a3 = kernel->cpu.ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    data[Word::wordToInt(data[16 * a2 + a3].getWord(x1))].setWord(x, x2);
}