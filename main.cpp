#include <iostream>
#include <cstdlib>
#include <vector>
#include "kernel.h"

using namespace std;

int main(int argc, char *argv[])
{
    Kernel *kernel = new Kernel();

    if(argc > 0)
    {
        kernel->runSystem(atoi(argv[1]));
    }
    else
    {
        kernel->runSystem(0);
    }
    
    delete kernel;
}