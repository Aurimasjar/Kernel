#include "virtualMachine.h"

string VirtualMachine::getCommand()
{
    return command;
}

/*char VirtualMachine::getCommand(int pos)
{
    return command[pos];
}*/

string VirtualMachine::readCommand(int &ch)
{
    if(!program.eof())
    {
        program >> command;
        return command;
    }
    ch = 1;
    return "";
}