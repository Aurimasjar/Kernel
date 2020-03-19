#include "virtualMachine.h"

void VirtualMachine::printData()
{
    for(int i = 0; i < 256; i++)
    {
        cout << data[i] << " ";
    }
    cout << endl;
}

void VirtualMachine::add()
{
    ba += bb;
}

void VirtualMachine::sub()
{
    ba -= bb;
}

void VirtualMachine::mul()
{
    ba *= bb;
}

void VirtualMachine::div()
{
    int r = ba % bb;
    ba /= bb;
    r = bb;
}


void VirtualMachine::la(int x1, int x2)
{
    ic++;
    ba = data[16*x1 + x2];
}

void VirtualMachine::lb(int x1, int x2)
{
    ic++;
    bb = data[16*x1 + x2];
}

void VirtualMachine::ua(int x1, int x2)
{
    ic++;
    data[16*x1 + x2] = ba;
}

void VirtualMachine::ub(int x1, int x2)
{
    ic++;
    data[16*x1 + x2] = bb;
}

void VirtualMachine::gd(int x1, int x2)
{
    ic++;
    flash >> data[16*x1 + x2];
}

void VirtualMachine::pd(int x1, int x2)
{
    ic++;
    printer << data[16*x1 + x2];
}

void VirtualMachine::loc(int x)
{

}

void VirtualMachine::uni(int x)
{

}

void VirtualMachine::sla(int x)
{

}

void VirtualMachine::sua(int x)
{

}

void VirtualMachine::cmp()
{

}

void VirtualMachine::jm(int x1, int x2)
{

}

void VirtualMachine::halt()
{

}
