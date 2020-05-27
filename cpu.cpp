#include "cpu.h"

using namespace std;

/*void Cpu::initCpu()
{
    // initialize register's values
    ic = 160;
    ptr.setWord(0);
    sptr = 79;
    ba = 0;
    bb = 0;
    bc = 0;
    sf = 0;
    s = 0;
    mode = 0;
    ti = 30;
    pi = 0;
    si = 0;
    ch1 = 0;
    ch2 = 0;
    ch3 = 0;
}*/

Cpu::Cpu(const Cpu &c)
{
    // copy register's values
    ic = c.ic;
    ptr = c.ptr;
    sptr = c.sptr;
    ba = c.ba;
    bb = c.bb;
    bc = c.bc;
    sf = c.sf;
    s = c.s;
    mode = c.mode;
    ti = c.ti;
    pi = 0;
    si = 0;
    ch1 = c.ch1;
    ch2 = c.ch2;
    ch3 = c.ch3;
}