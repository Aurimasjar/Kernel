#ifndef KERNEL_H
#define KERNEL_H

#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>

#include "memory.h"
#include "block.h"
#include "word.h"
#include "resource.h"
#include "process.h"
#include "cpu.h"
#include "channelDevice.h"

using namespace std;

class Resource;
class Process;
class ChannelDevice;
class Memory;

class Kernel
{
    private:

    public:

    Memory *memory;
    Block supervisorMemory[16];
    Cpu cpu;

    int programCount = 3;
    int readingProgram = 1;
    int launchingProgram = readingProgram;
    int runningProgram = 1;
    int finishedProgram = 1;

    vector<Resource*> resources;
    //vector<Resource> usedResources;

    vector<Process*> processes;
    //vector<Process*> blockedProcesses;

    ChannelDevice *channel1, *channel2, *channel3;

    Kernel();
    ~Kernel();

    int runSystem(int debug);

    void checkDebug(int debug, Process *currentProcess);

    void printProcesses();
    void printResources();
    void printRegisters();
    void printMemory();
    void printMemoryAsSymbols();

    void reserveUserMemoryBlocks(Resource *userMemory, Process *process);
    void resourceDistributor(Process *process, string resourceName);
    void planner(Process **currentProcess);

    void sortProcesses();

    void addProcess(Process *pr);
    void removeProcess(string name);
    void removeProcess(Process *pr);
    void stopProcess(Process *pr);
    void stopProcess(string name);
    void releaseProcess(Process *pr, Cpu &savedCpu);
    void releaseProcess(string name);

    Resource *getResource(string externalName);
    Process *getProcess(string externalName);
};

//Memory Kernel::memory;

#endif