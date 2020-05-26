#include "process.h"

using namespace std;

int ReadFromFlash::runProcess()
{
    if(kernel->readingProgram > kernel->programCount)
    {
        destroyResource("InputStream");
    }
    if(run == 0)
    {
        askForResource("InputStream");
        run++;
    }
    else if(run == 1)
    {
        askForResource("Channel1");
        run++;
    }
    else if(run == 2)
    {
        kernel->channel1->readToChannel("flash/flash" + to_string(kernel->readingProgram) + ".txt");
        kernel->readingProgram++;
        askForResource("SupervisorMemory");
        run++;
    }
    else
    {
        kernel->channel1->readToSupervisorMemory(kernel);
        kernel->cpu.ch1 = 1;
        createResource(20 + kernel->launchingProgram, this, false, "MainProc", kernel, kernel->launchingProgram, false);
        releaseResource("MainProc");
        releaseResource("Channel1");
        releaseResource("InputStream");
        run = 0;
    }
    return 0;
}