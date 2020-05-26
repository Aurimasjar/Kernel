#include "resource.h"

using namespace std;

Resource::Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available)
{
    this->id = id;
    this->father = father;
    this->user = nullptr;
    this->inUse = inUse;
    this->externalName = externalName;
    this->kernel = kernel;
    this->available = available;
    this->value = 1;

    //cout << "NEW RESOURCE: " << externalName << endl;
    for(int i = 0; i < kernel->processes.size(); i++)
    {
        if(kernel->processes[i]->getRequiredResource() == externalName)
        {
            //cout << kernel->processes[i]->getName() << " finally got into queue of resource " << externalName << endl;
            queue.push_back(kernel->processes[i]);
            kernel->processes[i]->setRequiredResource("");
        }
    }
}

Resource::Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, bool available)
{
    this->id = id;
    this->father = father;
    this->user = nullptr;
    this->inUse = inUse;
    this->externalName = externalName;
    this->kernel = kernel;
    this->available = available;
    this->value = value;
    this->address = -1;

    //cout << "NEW RESOURCE: " << externalName << endl;
    for(int i = 0; i < kernel->processes.size(); i++)
    {
        if(kernel->processes[i]->getRequiredResource() == externalName)
        {
            //cout << kernel->processes[i]->getName() << " finally got into queue of resource " << externalName << endl;
            queue.push_back(kernel->processes[i]);
            kernel->processes[i]->setRequiredResource("");
        }
    }
}

Resource::Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, int address, bool available)
{
    this->id = id;
    this->father = father;
    this->user = nullptr;
    this->inUse = inUse;
    this->externalName = externalName;
    this->kernel = kernel;
    this->available = available;
    this->value = value;
    this->address = address;

    //cout << "NEW RESOURCE: " << externalName << endl;
    for(int i = 0; i < kernel->processes.size(); i++)
    {
        if(kernel->processes[i]->getRequiredResource() == externalName)
        {
            //cout << kernel->processes[i]->getName() << " finally got into queue of resource " << externalName << endl;
            queue.push_back(kernel->processes[i]);
            kernel->processes[i]->setRequiredResource("");
        }
    }
}


Resource::~Resource()
{

}

void Resource::printResourceInfo()
{
    //cout << "Id externalName fatherID userID inUse" << endl;
    int userId = user == nullptr ? -1 : user->getId();
    cout << "RESOURCE INFO [" << id << " " << externalName << " " << father->getId() << " " << userId << " " << available << "]" << endl;
}

void ListResource::printResourceInfo()
{
    cout << "RESOURCE INFO [" << id << " " << getName() << " " << father->getId() << "]:" << endl;
    cout << "Units: ";
    for(int i = 0; i < avList.size(); i++)
    {
        cout << avList[i] << " ";
    } cout << endl;
    cout << "Users: ";
    for(int i = 0; i < avList.size(); i++)
    {
        if(userList[i] == nullptr) cout << -1 << " ";
        else cout << userList[i]->getId() << " ";
    } cout << endl;
}