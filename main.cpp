#include <iostream>
#include <cstdlib>
#include <vector>
#include "kernel.h"

using namespace std;

int main(int argc, char *argv[])
{
    Kernel *kernel = new Kernel();

    if(argc > 0 && atoi(argv[1]) == 1)
    {
        kernel->runSystem(true);
    }
    else
    {
        kernel->runSystem(false);
    }
    
    delete kernel;
}