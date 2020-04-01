#include <iostream>
#include <cstdlib>
#include "realMachine.h"

using namespace std;

int main(int argc, char *argv[])
{
    RealMachine rm;
    if(argc > 0 && atoi(argv[1]) == 1)
    {
        rm.runProgram("program2.txt", true);
    }
    else
    {
        rm.runProgram("program2.txt");
    }
    
    //rm.printData();
}