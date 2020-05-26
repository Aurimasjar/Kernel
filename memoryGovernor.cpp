#include "process.h"

using namespace std;

int MemoryGovernor::runProcess()
{
    if(run == 0)
    {
        askForResource("Semaphore" + to_string(father->getId() - 20));
        run++;
    }
    else if(run == 1)
    {
        Resource *r = kernel->getResource("Semaphore" + to_string(father->getId() - 20));
        command = r->getValue();
        wordAddress = r->address % 16;
        askForResource("SharedMemoryBlock" + to_string(wordAddress));
        run++;
    }
    else
    {
        //do stuff
        if(command == 4)    // SLA
        {
            kernel->cpu.ba = Word::wordToInt(kernel->memory->data[kernel->cpu.sptr].getWord(wordAddress));
        }
        else                // SUA
        {
            kernel->memory->data[kernel->cpu.sptr].setWord(Word::intToWord(kernel->cpu.ba), wordAddress);
        }
        

        Process::createResource(father->getId() + 100, this, false, "FromMemoryGovernor" + to_string(father->getId() - 20),
        kernel, father->getId(), false);
        releaseResource("FromMemoryGovernor" + to_string(father->getId() - 20));
        run = 0;
    }
    return 0;
}