#include "process.h"

using namespace std;

int PrintLine::runProcess()
{
    if(run == 0)
    {
        askForResource("PrintWord");
        run++;
    }
    else if(run == 1)
    {
        askForResource("Channel2");
        run++;
    }
    else
    {
        Resource *r = kernel->getResource("PrintWord");
        int jgid = r->father->getId();
        //print word

        //int x1 = Word::hexToInt(command[2]);
        //int x2 = Word::hexToInt(command[3]);
        int x1 = r->getAddress()/16;
        int x2 = r->getAddress()%16;
        int x = kernel->memory->getRealData(16*x1 + x2);
        kernel->channel2->writeWordFromChannel(x);
        kernel->cpu.ch3 = 1;

        releaseResource("Channel2");
        createResource(jgid + 60, this, false, "FromPrintLine", kernel, 0, false);
        releaseResource("FromPrintLine");
        destroyResource("PrintWord");
        run = 0;
    }
    return 0;
}
