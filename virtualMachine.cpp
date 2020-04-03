#include "virtualMachine.h"

string VirtualMachine::getCommand()
{
    return command;
}

/*char VirtualMachine::getCommand(int pos)
{
    return command[pos];
}*/

string VirtualMachine::readCommand()
{
    if(!program.eof())
    {
        program >> command;
        return command;
    }
    return "";
}