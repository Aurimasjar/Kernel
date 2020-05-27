#include "process.h"

using namespace std;

int Interrupt::runProcess()
{
    if(run == 0)
    {
        askForResource("Interrupt");
        run++;
    }
    else
    {
        //identify interrupt type
        int interruptType = identifyInterrupt();

        //identify jobGovernor
        Resource *r = kernel->getResource("Interrupt");
        int jgid = r->father->father->getId() - 20;
        createResource(jgid + 50, this, false, "FromInterrupt" + to_string(jgid), kernel, interruptType, r->getAddress(), false);
        releaseResource("FromInterrupt" + to_string(jgid));
        destroyResource("Interrupt");
        run = 0;
    }
    return 0;
}

int Interrupt::identifyInterrupt()
{
    int type;
    if(kernel->cpu.si == 1) //GD
    {
        type = 1;
    }
    else if(kernel->cpu.si == 2) //PD
    {
        type = 2;
    }
    else if(kernel->cpu.si == 3) //HALT
    {
        type = 3;
    }
    else if(kernel->cpu.si == 4) //SLA
    {
        type = 4;
    }
    else if(kernel->cpu.si == 5) //SUA
    {
        type = 5;
    }
    if(kernel->cpu.ti <= 0)
    {
        type += 20;
    }
    if(kernel->cpu.pi > 0)
    {
        type = -1;
    }
    kernel->cpu.si = 0;
    kernel->cpu.pi = 0;

    return type;
}