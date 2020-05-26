#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
#include <iostream>
#include "cpu.h"
#include "resource.h"
#include "kernel.h"
#include "block.h"
#include "word.h"

using namespace std;

class Kernel;

class Process
{
    protected:

    int run = 0;

    public:

    bool running;
    bool ready;
    bool blocked;
    bool stopped;

    int priority; // 1-100

    Process *father;
    //vector<Resource*> processResources;
    string externalName;
    int id;
    Kernel *kernel;
    string requiredResource = "";

    Process(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel);
    ~Process();

    int getId() { return id; }
    string getName() { return externalName; }
    int getPriority() { return priority; }
    void setRequiredResource(string resource) { this->requiredResource = resource; }
    string getRequiredResource() { return requiredResource; }

    void setRunning() { running = true; ready = false; }
    void setStopped() { stopped = true; }
    void setReleased() { stopped = false; }
    void setBlocked() { running = false; ready = false; blocked = true; }
    void setReady() { running = false; ready = true; blocked = false; }
    bool isRunning() { return running; }
    bool isReady() { return ready && !stopped; }
    bool isBlocked() { return blocked && !stopped; }
    bool isReadyStopped() { return ready && stopped; }
    bool isBlockedStopped() { return blocked && stopped; }

    void createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available);
    void createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available, int n);
    void createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, bool available);
    void createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, int address, bool available);
    void destroyResource(string externalName);
    void askForResource(string externalName);
    void releaseResource(string externalName);

    void checkBlockage(string resName);
    void printProcessInfo();

    virtual int runProcess() = 0;
};


class StartStop : public Process
{
    public:
    StartStop(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
    void initResources();
    void initProcesses();
    void destroyResources();
    void destroyProcesses();
};


class ReadFromFlash : public Process
{
    public:
    ReadFromFlash(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
};


class MainProc : public Process
{
    public:
    MainProc(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
};


class JobGovernor : public Process
{
    public:
    JobGovernor(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
    void initializePageTable();
};


class VirtualMachine : public Process
{
    public:
    Cpu savedCpu;
    bool firstRun = true;
    VirtualMachine(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel, Word ptr) : 
    Process(id, father, initialState, priority, externalName, kernel) { savedCpu.ptr = ptr; }
    int runProcess();
    int test();
    string runProgram();
    string getCommand();
    void execute(string command);

    //arithmetic operations
    void add();
    void sub();
    void mul();
    void div();

    //operations for working with data
    void la(int x1, int x2);
    void lb(int x1, int x2);
    void ua(int x1, int x2);
    void ub(int x1, int x2);
    void gd(int x1, int x2);
    void pd(int x1, int x2);
    void loc(int x);
    void unl(int x);
    void sla(int x);
    void sua(int x);

    //comparison operations
    void cmp();

    //control operations
    void jm(int x1, int x2);
    void je(int x1, int x2);
    void ja(int x1, int x2);
    void jb(int x1, int x2);
    void halt();
};


class Interrupt : public Process
{
    public:
    Interrupt(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
    int identifyInterrupt();
};


class PrintLine : public Process
{
    public:
    PrintLine(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
};


class GetLine : public Process
{
    public:
    GetLine(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
};


class MemoryWriter : public Process
{
    public:
    MemoryWriter(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
    int putCommandsToUserMemory();
};


class MemoryGovernor : public Process
{
    public:
    int command;
    int wordAddress;
    MemoryGovernor(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel) : 
    Process(id, father, initialState, priority, externalName, kernel) {}
    int runProcess();
};


#endif