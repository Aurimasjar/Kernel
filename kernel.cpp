#include "kernel.h"

using namespace std;

Kernel::Kernel()
{
    channel1 = new ChannelDevice(1);
    channel2 = new ChannelDevice(2, "printer/printer.txt");
    channel3 = new ChannelDevice(1, "hardDisk/hardDisk.txt");

    memory = new Memory(this);

    srand(time(NULL));
}

Kernel::~Kernel()
{
    delete channel1;
    delete channel2;
    delete channel3;
    
    delete memory;
}

int Kernel::runSystem(int debug)
{
    cout << "MOS launched" << endl;
    Process *currentProcess = new StartStop(0, nullptr, 2, 100, "StartStop", this);
    this->addProcess(currentProcess);
    planner(&currentProcess);
    checkDebug(debug, currentProcess);
    while(true)
    {
        if(debug == 2) cout << "Current process: " << currentProcess->getName() << endl;
        currentProcess->runProcess();
        if(processes.empty()) break;
        planner(&currentProcess);
        checkDebug(debug, currentProcess);
    }
    cout << "MOS finished" << endl;
    return 0;
}

void Kernel::checkDebug(int debug, Process *currentProcess)
{
    string waitCommand;
    if(debug == 1)
    {
        printProcesses();
        cout << "Next process: " << currentProcess->getName() << endl;
        while(true)
        {
            cout << "Press enter to continue or type 'nextcom'*, 'rmdata', 'rmdatas', 'vmdata'*, 'pagetable'*, 'rmreg', 'vmreg'*, "
            << "'proc', 'res', 'rq' commands to print data" << endl;
            getline(cin, waitCommand);
            if(waitCommand == "nextcom")
            {
                //cout << "Next command is " << getCommand() << endl;
            }
            else if(waitCommand == "rmdata")
            {
                printMemory();
            }
            else if(waitCommand == "rmdatas")
            {
                printMemoryAsSymbols();
            }
            else if(waitCommand == "vmdata")
            {
                //printVirtualData();
            }
            else if(waitCommand == "pagetable")
            {
                //printPageTable();
            }
            else if(waitCommand == "rmreg")
            {
                printRegisters();
            }
            else if(waitCommand == "vmreg")
            {
                //printVirtualRegisters();
            }
            else if(waitCommand == "proc")
            {
                printProcesses();
            }
            else if(waitCommand == "res")
            {
                printResources();
            }
            else if(waitCommand == "rq")
            {
                cout << "RESOURCES STATUS AND THEIR QUEUES" << endl;
                for(int i = 0; i < resources.size(); i++)
                {
                    cout << resources[i]->isAvailable() << " " << resources[i]->getName() << ": ";
                    for(int j = 0; j <resources[i]->queue.size(); j++)
                    {
                        cout << resources[i]->queue[j]->getName() << " ";
                    } cout << endl;
                } cout << endl;
            }
            else
            {
                break;
            }
            
        }
        waitCommand = "";
    }
}

void Kernel::printProcesses()
{
    cout << "             Id  processName  fatherID  priority  run  state" << endl;
    for(int i = 0; i < processes.size(); i++)
    {
        this->processes[i]->printProcessInfo();
    }
}

void Kernel::printResources()
{
    cout << "Id  fatherID  available  resourceName" << endl;
    for(int i = 0; i < resources.size(); i++)
    {
        this->resources[i]->printResourceInfo();
    }
}

void Kernel::printRegisters()
{
    cout << "Real machine registers:" << endl;
    cout << "Ic = " << Word::intToHex(cpu.ic/16) << Word::intToHex(cpu.ic%16) << endl;
    cout << "Ptr = " << Word::intToHex(cpu.ptr.getWord()/16) << Word::intToHex(cpu.ptr.getWord()%16) << endl;
    cout << "Sptr = " << Word::intToHex(cpu.sptr/16) << Word::intToHex(cpu.sptr%16) << endl;
    cout << "Ba = " << Word::intToHex(cpu.ba/16) << Word::intToHex(cpu.ba%16) << endl;
    cout << "Bb = " << Word::intToHex(cpu.bb/16) << Word::intToHex(cpu.bb%16) << endl;
    cout << "Bc = " << Word::intToHex(cpu.bc/16) << Word::intToHex(cpu.bc%16) << endl;
    cout << "Sf = " << Word::intToHex(cpu.sf/16) << Word::intToHex(cpu.sf%16) << endl; //should be 1 byte
    cout << "S = " << Word::intToHex(cpu.s/16) << Word::intToHex(cpu.s%16) << endl; //should be 1 byte
    cout << "Mode = " << Word::intToHex(cpu.mode/16) << Word::intToHex(cpu.mode%16) << endl; //should be 1 byte
    cout << "Ti = " << Word::intToHex(cpu.ti/16) << Word::intToHex(cpu.ti%16) << endl;
    cout << "Pi = " << Word::intToHex(cpu.pi/16) << Word::intToHex(cpu.pi%16) << endl;
    cout << "Si = " << Word::intToHex(cpu.si/16) << Word::intToHex(cpu.si%16) << endl;
    cout << "Ch1 = " << Word::intToHex(cpu.ch1/16) << Word::intToHex(cpu.ch1%16) << endl; //should be 1 byte
    cout << "Ch2 = " << Word::intToHex(cpu.ch2/16) << Word::intToHex(cpu.ch2%16) << endl; //should be 1 byte
    cout << "Ch3 = " << Word::intToHex(cpu.ch3/16) << Word::intToHex(cpu.ch3%16) << endl; //should be 1 byte
    cout << endl;
}

void Kernel::printMemory()
{
    cout << "Memory:" << endl;
    for(int i = 0; i < 80; i++)
    {
        cout << Word::intToHex(i/16) << Word::intToHex(i%16) << ": ";
        for(int j = 0; j < 16; j++)
        {
            memory->data[i].getWord(j).print();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Kernel::printMemoryAsSymbols()
{
    cout << "Memory as symbols:" << endl;
    for(int i = 0; i < 80; i++)
    {
        cout << Word::intToHex(i/16) << Word::intToHex(i%16) << ": ";
        for(int j = 0; j < 16; j++)
        {
            memory->data[i].getWord(j).printAsSymbol();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Kernel::reserveUserMemoryBlocks(Resource *userMemory, Process *process)
{
    ListResource *um = dynamic_cast<ListResource*>(userMemory);
    int randomBlockIndex;
    for(int i = 0; i < 17; i++)
    {
        randomBlockIndex = -1;
        while(true)
        {
            randomBlockIndex = rand() % 79;
            if(um->avList[randomBlockIndex])
            {
                um->avList[randomBlockIndex] = false;
                um->userList[randomBlockIndex] = process;
                um->setValue(um->getValue() - 1);
                break;
            }
        }
    }
    if(userMemory->getValue() < 20)
    {
        userMemory->setAvailable(false);
    }
}

void Kernel::resourceDistributor(Process *process, string resourceName)
{
    for(int i = 0; i < resources.size(); i++)
    {
        if(resources[i]->getName() == resourceName && resources[i]->isAvailable() && !resources[i]->queue.empty())
        {
            if(resourceName == "UserMemory")
            {
                resources[i]->queue[0]->setReady();
                resources[i]->queue.pop_front();

                // reserve blocks
                reserveUserMemoryBlocks(resources[i], process);
                break;
            }
            else
            {
                resources[i]->queue[0]->setReady();
                resources[i]->queue.pop_front();
                //blockedProcesses[0]->setReady();
                //blockedProcesses.erase(blockedProcesses.begin());
                resources[i]->setAvailable(false);
                break;
            }
        }
        else if(resources[i]->getName() == resourceName && !resources[i]->queue.empty())
        {
            if(resourceName.rfind("SharedMemoryBlock", 0) == 0)
            {
                if(resources[i]->getValue() == process->father->getId())
                {
                    resources[i]->queue[0]->setReady(); // could be not only first element
                    resources[i]->queue.pop_front();
                }
            }
        }
    }
}

void Kernel::planner(Process **currentProcess)
{
    /*if(currentProcess->isBlocked())
    {
        blockedProcesses.push_back(currentProcess);
    }*/
    if((*currentProcess)->isRunning())
    {
        (*currentProcess)->setReady();
    }
    sortProcesses();
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i]->isReady())
        {
            if((processes[i]->getName() == "GetLine" || processes[i]->getName() == "PrintLine") &&
            (processes[i]->run > 0 && processes[i]->run < 3))
            {
                processes[i]->run++;
            }
            else
            {
                // control of CPU is given to currentProcess
                *currentProcess = processes[i];
                (*currentProcess)->setRunning();
                break;
            }
        }
    }
}

void Kernel::sortProcesses()
{
    Process *tempPr;
    for(int i = 0; i < processes.size(); i++)
    {
        for(int j = i; j < processes.size(); j++)
        {
            if(processes[i]->getPriority() < processes[j]->getPriority())
            {
                tempPr = processes[i];
                processes[i] = processes[j];
                processes[j] = tempPr;
            }
        }
    }
}

void Kernel::addProcess(Process *pr)
{
    this->processes.push_back(pr);
}

void Kernel::removeProcess(Process *pr)
{
    // destroy processes and resources created by this process?
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i] == pr)
        {
            delete processes[i];
            processes.erase(processes.begin() + i);
        }
    }
    //processes.remove(pr);
}

void Kernel::removeProcess(string name)
{
    // destroy processes and resources created by this process?
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i]->getName() == name)
        {
            delete processes[i];
            processes.erase(processes.begin() + i);
        }
    }
    //processes.remove(pr);
}

void Kernel::stopProcess(Process *pr)
{
    pr->setStopped();
}

void Kernel::stopProcess(string name)
{
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i]->getName() == name)
        {
            processes[i]->setStopped();
            processes[i]->priority--;
            break;
        }
    }
}

void Kernel::releaseProcess(Process *pr, Cpu &savedCpu)
{
    savedCpu.ti = 10;
    pr->setReleased();
}

void Kernel::releaseProcess(string name)
{
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i]->getName() == name)
        {
            processes[i]->setReleased();
            break;
        }
    }
}



Resource *Kernel::getResource(string externalName)
{
    for(int i = 0; i < resources.size(); i++)
    {
        if(resources[i]->getName() == externalName)
        {
            return resources[i];
        }
    }
    cout << "getResource() error: There is no such a resource called " << externalName << endl;
    return nullptr;
}

Process *Kernel::getProcess(string externalName)
{
    for(int i = 0; i < processes.size(); i++)
    {
        if(processes[i]->getName() == externalName)
        {
            return processes[i];
        }
    }
    cout << "getProcess() error: There is no such a process called " << externalName << endl;
    return nullptr;
}