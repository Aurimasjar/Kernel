#include "interpreter.h"

void Interpreter::runProgram()
{
    fd >> command;
    while(true)
    {
        fd >> command;
        if(command == "$END")
        {
            break;
        }
        execute();
        cout << command << " " <<vm.getBA() << " " << vm.getBB() << endl;
        
        if(trace == true)
        {
            cout << "Press any key to continue. ..."; // add print, ...
            cin >> waitCommand;
        }
    }
}

void Interpreter::execute()
{
    if(command == "ADD0")
    {
       vm.add();
    }
    else if(command == "SUB0")
    {
       vm.sub();
    }
    else if(command == "MUL0")
    {
       vm.mul();
    }
    else if(command == "DIV0")
    {
       vm.div();
    }
    else if(command[0] == 'L' && command[1] == 'A')
    {
        vm.la((int) command[2], (int) command[3]);
    }
    else if(command[0] == 'L' && command[1] == 'B')
    {
        vm.lb((int) command[2], (int) command[3]);
    }
    else if(command[0] == 'U' && command[1] == 'A')
    {
        vm.ua((int) command[2], (int) command[3]);
    }
    else if(command[0] == 'U' && command[1] == 'B')
    {
        vm.ub((int) command[2], (int) command[3]);
    }
    else if(command[0] == 'G' && command[1] == 'D')
    {
        vm.gd((int) command[2], (int) command[3]);
    }
    else if(command[0] == 'P' && command[1] == 'D')
    {
        vm.pd((int) command[2], (int) command[3]);
    }
             
}