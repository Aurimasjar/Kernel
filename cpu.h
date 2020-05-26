#ifndef CPU_H
#define CPU_H

using namespace std;

#include "word.h"

class Cpu
{
    public:

    // all registers
    int ic = 160;
    Word ptr;
    int sptr = 79;
    int ba = 0;
    int bb = 0;
    int bc = 0;
    int sf = 0;
    int s = 0;
    int mode;
    int ti = 30;
    int pi = 0;
    int si = 0;
    int ch1 = 0;
    int ch2 = 0;
    int ch3 = 0;

    Cpu() {}
    Cpu(const Cpu &c);
    ~Cpu() {}

    void initCpu();
};

#endif