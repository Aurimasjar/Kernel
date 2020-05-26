#include "process.h"

using namespace std;

Process::Process(int id, Process *father, int initialState, int priority, string externalName, Kernel *kernel)
{
    running = false;
    ready = false;
    blocked = false;
    stopped = false;

    this->id = id;
    this->father = father;
    this->kernel = kernel;
    
    if(initialState == 1) running = true;                               // running process
    else if(initialState == 2) ready = true;                            // ready process
    else if(initialState == 3) blocked = true;                           // blocked process
    else if(initialState == 4) { ready = true;  stopped = true; }        // ready stopped process
    else if(initialState == 5) { blocked = true;  stopped = true; }      // blocked stopped process
    else cout << "Process " << externalName << " failed to get initial state";

    this->priority = priority;
    this->externalName = externalName;
}

Process::~Process()
{

}

void Process::createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available)
{
    Resource *r;
    r = new Resource(id, father, inUse, externalName, kernel, available);
    kernel->resources.push_back(r);
}

void Process::createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, bool available)
{
    Resource *r;
    r = new Resource(id, father, inUse, externalName, kernel, value, available);
    kernel->resources.push_back(r);
}

void Process::createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, int address, bool available)
{
    Resource *r;
    r = new Resource(id, father, inUse, externalName, kernel, value, address, available);
    kernel->resources.push_back(r);
}

void Process::createResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available, int n)
{
    Resource *r;
    r = new ListResource(id, father, inUse, externalName, kernel, available, n);
    kernel->resources.push_back(r);
}

void Process::destroyResource(string externalName)
{
    for(int i = 0; i < kernel->resources.size(); i++)
    {
        if(kernel->resources[i]->getName() == externalName)
        {
            for(int j = 0; j < kernel->resources[i]->queue.size(); j++)
            {
                // unblock all processes who were waiting for this resource
                //cout << "Waiter that is being unblocked: " << kernel->resources[i]->queue[j] << endl;
                kernel->resources[i]->queue[j]->setReady();
            }
            delete kernel->resources[i];
            kernel->resources.erase(kernel->resources.begin() + i);
            break;
        }
    }
    //cout << "Error: Resource destruction failed: Cannot find a resource called " << externalName << endl; 
}

void Process::askForResource(string externalName)
{
    for(int i = 0; i < kernel->resources.size(); i++)
    {
        if(kernel->resources[i]->getName() == externalName)
        {
            // register request
            //kernel->blockedProcesses.push_back(this);
            kernel->resources[i]->queue.push_back(this);
            //cout << "New queue member: " << kernel->resources[i]->queue.back()->getName() << endl;
            break;
        }
        if(i == kernel->resources.size() - 1)
        {
            //cout << getName() << " waits for resource that does not exist yet: " << externalName << endl;
            setRequiredResource(externalName);
        }
    }
    setBlocked();
    //cout << "Resource request: " << getName() << " requests for " << externalName << endl;
    //cout << "Error: Resource request failed: Cannot find a resource called " << externalName << endl;
    // call resource distributor
    kernel->resourceDistributor(this, externalName);
}

void Process::releaseResource(string externalName)
{
    for(int i = 0; i < kernel->resources.size(); i++)
    {
        if(kernel->resources[i]->getName() == externalName)
        {
            if(externalName == "UserMemory")
            {
                // free blocks that this process was using
                ListResource *um = dynamic_cast<ListResource*>(kernel->resources[i]);
                for(int i = 0; i < 79; i++)
                {
                    if(um->userList[i] == this)
                    {
                        um->userList[i] = nullptr;
                        um->avList[i] = true;
                    }
                }
            }
            // free resource
            kernel->getResource(externalName)->setAvailable(true);
            break;
        }
    }
    //cout << "Resource release: " << getName() << " releases resource " << externalName << endl;
    //cout << "Error: Resource release failed: Cannot find a resource called " << externalName << endl; 
    // call resource distributor
    kernel->resourceDistributor(this, externalName);
}

void Process::printProcessInfo()
{
    //cout << "Id externalName fatherID priority state{running,ready,blocked,stopped}" << endl;
    int fatherId = father == nullptr ? -1 : father->getId();
    cout << "PROCESS INFO [" << id << " " << externalName << " " << fatherId << " " << priority << " ";
    cout << running << ready << blocked << stopped << "]" << endl;
}