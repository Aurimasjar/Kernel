#include "process.h"

using namespace std;

int JobGovernor::runProcess()
{
    if(run == 0)
    {
        askForResource("UserMemory");
        run++;
    }
    else if(run == 1)
    {
        initializePageTable();
        createResource(40 + getId(), this, false, "TaskInSupervisorMemory", kernel, getId(), false);
        releaseResource("TaskInSupervisorMemory");
        askForResource("MemoryWritten");
        run++;
    }
    else if(run == 2)
    {
        Resource *r = kernel->getResource("MemoryWritten");
        if(r->getValue() < 0)
        {
            run += 10;
        }
        else
        {
            //kernel->printMemory();
            //kernel->printMemoryAsSymbols();
            Process *pr;
            pr = new MemoryGovernor(getId() + 10, this, 2, 60, "MemoryGovernor" + to_string(getId() - 20), kernel);
            kernel->addProcess(pr);
            pr = new VirtualMachine(getId() + 20, this, 2, 30, "VirtualMachine" + to_string(getId() - 20), kernel, kernel->cpu.ptr);
            kernel->addProcess(pr);
            run++;
        }
    }
    else if(run == 3)
    {
        Process *vmpr = kernel->getProcess("VirtualMachine" + to_string(getId() - 20));
        if(vmpr->isStopped())
        {
            kernel->releaseProcess(vmpr, dynamic_cast<VirtualMachine*>(vmpr)->savedCpu);
        }
        createResource(getId() + 40, this, false, "Continue" + to_string(getId() - 20), kernel, 0, false);
        releaseResource("Continue" + to_string(getId() - 20));
        askForResource("FromInterrupt" + to_string(getId() - 20));
        run++;
    }
    else if(run == 4)
    {
        //do stuff depending on interrupt type
        Resource *r = kernel->getResource("FromInterrupt"  + to_string(getId() - 20));
        int interruptType = r->getValue();
        int address = r->getAddress();
        if(interruptType >= 20)   // Timer interrupt
        {
            kernel->stopProcess("VirtualMachine" + to_string(getId() - 20));
            run = 3;    // or add another state (for example run=8?)
        }
        if(interruptType%20 >= 1 && interruptType%20 <= 2)    // IO interrupt
        {
            if(interruptType%20 == 1)
            {
                createResource(getId() + 70, this, false, "GetWord", kernel, 0, address, false);
                releaseResource("GetWord");
                askForResource("FromGetLine");
                run = 5;
            }
            else
            {
                createResource(getId() + 80, this, false, "PrintWord", kernel, 0, address, false);
                releaseResource("PrintWord");
                askForResource("FromPrintLine");
                run = 6;
            }  
        }
        else if(interruptType%20 >= 4 && interruptType%20 <= 5)   // Semaphore interrupt
        {
            createResource(getId() + 90, this, false, "Semaphore" + to_string(getId() - 20), kernel, interruptType, address, false);
            releaseResource("Semaphore" + to_string(getId() - 20));
            askForResource("FromMemoryGovernor" + to_string(getId() - 20));
            run = 7;
        }
        else if(interruptType < 20)
        {
            run += 10;
        }
        destroyResource("FromInterrupt" + to_string(getId() - 20));
    }
    else if(run == 5)
    {
        destroyResource("FromGetLine");
        run = 3;
    }
    else if(run == 6)
    {
        destroyResource("FromPrintLine");
        run = 3;
    }
    else if(run == 7)
    {
        destroyResource("FromMemoryGovernor" + to_string(getId() - 20));
        run = 3;
    }
    else
    {
        kernel->removeProcess("VirtualMachine" + to_string(getId() - 20));
        kernel->removeProcess("MemoryGovernor" + to_string(getId() - 20));
        releaseResource("UserMemory");
        createResource(getId() + 30, this, false, "MainProc", kernel, 0, false);
        releaseResource("MainProc");
        askForResource("NonExistentResource");
        run = 0;
    }
    return 0;
}

void JobGovernor::initializePageTable()
{
    ListResource *um = dynamic_cast<ListResource*>(kernel->getResource("UserMemory"));
    int i = 0, j = 0, ptr = 0;
    for(i = 0; i < 80; i++)
    {
        if(um->userList[i] == this)
        {
            ptr = i;
            break;
        }
    }
    for(i = i + 1; i < 80; i++)
    {
        if(um->userList[i] == this)
        {
            kernel->memory->data[ptr].setWord(Word::intToWord(i), j);
            j++;
        }
    }
    //kernel->cpu.ptr.setWord(i);
    kernel->cpu.ptr.setByte((char) ptr/16, 2);
    kernel->cpu.ptr.setByte((char) ptr%16, 3);
}