#include "process.h"

using namespace std;

int StartStop::runProcess()
{
    if(run == 0)
    {
        initResources();
        initProcesses();
        askForResource("MosEnd");
        run++;
    }
    else
    {
        destroyProcesses();
        destroyResources();
    }
    return 0;
}

void StartStop::initResources()
{
    createResource(0, this, false, "MosEnd", kernel, false);
    createResource(1, this, false, "InputStream", kernel, true);
    createResource(2, this, false, "Channel1", kernel, true);
    createResource(7, this, false, "Channel2", kernel, true);
    createResource(7, this, false, "Channel3", kernel, true);
    createResource(3, this, false, "SupervisorMemory", kernel, true);
    createResource(4, this, false, "MemoryWritten", kernel, false);
    //createResource(5, this, false, "FromInterrupt", kernel, false);
    //createResource(6, this, false, "PrintWord", kernel, false);
    //createResource(8, this, false, "FromPrintLine", kernel, false);
    //createResource(9, this, false, "ReadWord", kernel, false);
    //createResource(10, this, false, "FromGetLine", kernel, false);
    createResource(11, this, false, "Semaphore", kernel, false);
    //createResource(12, this, false, "FromMemoryGovernor", kernel, false);
    createResource(13, this, false, "UserMemory", kernel, true, 80);
    for(int i = 0; i < 16; i++) createResource(140 + i, this, false, "SharedMemoryBlock" + to_string(i), kernel, true);
}

void StartStop::initProcesses()
{
    // init all processes except jobGovernor and virtualMachine and memoryGovernor
    Process *pr;
    pr = new ReadFromFlash(1, this, 2, 90, "ReadFromFlash", kernel);
    kernel->addProcess(pr);
    pr = new MainProc(2, this, 2, 70, "MainProc", kernel);
    kernel->addProcess(pr);
    pr = new Interrupt(3, this, 2, 60, "Interrupt", kernel);
    kernel->addProcess(pr);
    pr = new PrintLine(4, this, 2, 60, "PrintLine", kernel);
    kernel->addProcess(pr);
    pr = new GetLine(5, this, 2, 60, "GetLine", kernel);
    kernel->addProcess(pr);
    pr = new MemoryWriter(6, this, 2, 60, "MemoryWriter", kernel);
    kernel->addProcess(pr);
}

void StartStop::destroyProcesses()
{
    // delete all processes except startStop
    for(int i = 1; i < kernel->processes.size(); i++)
    {
        delete kernel->processes[i];
    }
    kernel->processes.clear();
    delete this;
}

void StartStop::destroyResources()
{
    for(int i = 0; i < kernel->resources.size(); i++)
    {
        delete kernel->resources[i];
    }
    kernel->resources.clear();
}