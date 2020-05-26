#include "process.h"

using namespace std;

int GetLine::runProcess()
{
    if(run == 0)
    {
        askForResource("GetWord");
        run++;
    }
    else if(run == 1)
    {
        askForResource("Channel3");
        run++;
    }
    else
    {
        Resource *r = kernel->getResource("GetWord");
        int jgid = r->father->getId();
        //read word

        //int x1 = Word::hexToInt(command[2]);
        //int x2 = Word::hexToInt(command[3]);

        int x1 = r->getAddress()/16;
        int x2 = r->getAddress()%16;
        int x = kernel->channel3->readWordToChannel();
        if(x != -1)
        {
            kernel->memory->setRealData(x, 16*x1 + x2);
            kernel->cpu.ch3 = 1;
        }

        releaseResource("Channel3");
        createResource(jgid + 50, this, false, "FromGetLine", kernel, 0, false);
        releaseResource("FromGetLine");
        destroyResource("GetWord");
        run = 0;
    }
    return 0;
}