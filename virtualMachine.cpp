#include "virtualMachine.h"

string VirtualMachine::getCommand()
{
    return command;
}

char VirtualMachine::getCommand(int pos)
{
    return command[pos];
}

string VirtualMachine::readCommand(int &ch)
{
    //mode = 1;
    program >> command;
    if(program) ch = 1;
    else ch = 0;
    //mode = 0;
    return command;
}