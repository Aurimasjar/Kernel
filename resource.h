#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>
#include <deque>
#include <iostream>
#include <iomanip>
#include "kernel.h"
#include "process.h"

using namespace std;

class Resource
{
    public:

    int id;
    bool inUse;
    bool available;
    int value;
    int address;
    Process *father;
    Process *user;
    Kernel *kernel;
    string externalName;
    deque<Process*> queue;

    Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available);
    Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, bool available);
    Resource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, int value, int address, bool available);
    ~Resource();

    void setUser(Process *user) { this->user = user; }
    Kernel *getUser() { return kernel; }

    void setAvailable(bool available) { this->available = available; }
    bool isAvailable() { return available; }
    void setValue(int value) { this->value = value; }
    int getValue() { return value; }
    void setAddress(int address) { this->address = address; }
    int getAddress() { return address; }

    string getName() { return externalName; }

    virtual void printResourceInfo();
};

class ListResource : public Resource
{
    public:

    vector<bool> avList;
    vector<Process*> userList;

    ListResource(int id, Process *father, bool inUse, string externalName, Kernel *kernel, bool available, int n) :
    Resource(id, father, inUse, externalName, kernel, n, available)
    {
        avList.resize(n, available);
        userList.resize(n, nullptr);
    }

    void printResourceInfo();
};

#endif