#include "process.h"

using namespace std;

int VirtualMachine::runProcess()
{
    if(run == 0)
    {
        askForResource("Continue" + to_string(father->getId() - 20));
        run++;
    }
    else
    {
        if(firstRun)
        {
            kernel->cpu = savedCpu;
            kernel->cpu.pi = 0;
            kernel->cpu.si = 0;
        }
        else
        {
            firstRun = false;
            //kernel->cpu.ptr = ?
        }

        string outWord = runProgram();

        savedCpu = kernel->cpu;

        destroyResource("Continue" + to_string(father->getId() - 20));
        int x1 = Word::hexToInt(outWord[2]);
        int x2 = Word::hexToInt(outWord[3]);
        createResource(father->getId() + 60, this, false, "Interrupt", kernel, father->getId() - 20, 16 * x1 + x2,  false);
        releaseResource("Interrupt");
        run = 0;
    }
    return 0;
}

int VirtualMachine::test()
{
    if((kernel->cpu.pi + kernel->cpu.si) > 0 || kernel->cpu.ti == 0)
    {
        return 1;
    }
    return 0;
}

string VirtualMachine::runProgram()
{
    kernel->cpu.mode = 0;
    string command;
    while(test() == 0)
    {
        command = getCommand();
        execute(command);        
        cout << getId() << " " << command << endl;
    }
    kernel->cpu.mode = 1;
    return command;
}

string VirtualMachine::getCommand()
{
    int a2 = kernel->cpu.ptr.getByte(2);
    int a3 = kernel->cpu.ptr.getByte(3);
    int x1 = kernel->cpu.ic / 16;
    int x2 = kernel->cpu.ic % 16;
    return Word::wordToString(kernel->memory->data[Word::wordToInt(kernel->memory->data[10 * a2 + a3].getWord(x1))].getWord(x2));
}

void VirtualMachine::execute(string command)
{
    if(command == "ADD0")
    {
       add();
    }
    else if(command == "SUB0")
    {
       sub();
    }
    else if(command == "MUL0")
    {
       mul();
    }
    else if(command == "DIV0")
    {
       div();
    }
    else if(command[0] == 'L' && command[1] == 'A')
    {
        la(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'L' && command[1] == 'B')
    {
        lb(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'U' && command[1] == 'A')
    {
        ua(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'U' && command[1] == 'B')
    {
        ub(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'G' && command[1] == 'D')
    {
        gd(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'P' && command[1] == 'D')
    {
        pd(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'L' && command[1] == 'O' && command[2] == 'C')
    {
        loc(Word::hexToInt(command[3]));
    }
    else if(command[0] == 'U' && command[1] == 'N' && command[2] == 'L')
    {
        unl(Word::hexToInt(command[3]));
    }
    else if(command[0] == 'S' && command[1] == 'L' && command[2] == 'A')
    {
        sla(Word::hexToInt(command[3]));
    }
    else if(command[0] == 'S' && command[1] == 'U' && command[2] == 'A')
    {
        sua(Word::hexToInt(command[3]));
    }
    else if(getCommand() == "CMP$")
    {
        cmp();
    }
    else if(command[0] == 'J' && command[1] == 'M')
    {
        jm(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'J' && command[1] == 'E')
    {
        je(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'J' && command[1] == 'A')
    {
        ja(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(command[0] == 'J' && command[1] == 'B')
    {
        jb(Word::hexToInt(command[2]), Word::hexToInt(command[3]));
    }
    else if(getCommand() == "HALT")
    {
        halt();
    }
    else
    {
        kernel->cpu.pi = 2;
    }
        
}

void VirtualMachine::add()
{
    kernel->cpu.ic++;
    kernel->cpu.ba += kernel->cpu.bb;
}

void VirtualMachine::sub()
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    kernel->cpu.ba -= kernel->cpu.bb;
}

void VirtualMachine::mul()
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    kernel->cpu.ba *= kernel->cpu.bb;
}

void VirtualMachine::div()
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    int r = kernel->cpu.ba % kernel->cpu.bb;
    kernel->cpu.ba /= kernel->cpu.bb;
    kernel->cpu.bb = r;
}


void VirtualMachine::la(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
       kernel->cpu.ba = kernel->memory->getRealData(16*x1 + x2);
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
    
}

void VirtualMachine::lb(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->cpu.bb = kernel->memory->getRealData(16*x1 + x2);
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
}

void VirtualMachine::ua(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->memory->setRealData(kernel->cpu.ba, 16*x1 + x2);
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
}

void VirtualMachine::ub(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->memory->setRealData(kernel->cpu.bb, 16*x1 + x2);
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
}

void VirtualMachine::gd(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti -= 3;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->cpu.si = 1;
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
}

void VirtualMachine::pd(int x1, int x2)
{
    kernel->cpu.ic++;
    kernel->cpu.ti -= 3;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->cpu.si = 2;
    }
    else
    {
        kernel->cpu.pi = 1; // wrong address
    }
}

void VirtualMachine::loc(int x)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    Resource *r = kernel->getResource("SharedMemoryBlock" + to_string(x));
    if(x >= 0 && x <= 15)
    {
        //if((kernel->cpu.s & (1 << x)) == 0)
        if(r->isAvailable())
        {
            kernel->cpu.s += pow(2, x);
            r->setAvailable(false);
            r->setValue(father->getId());
        }
        else
        {
            kernel->cpu.pi = 5;
        }
        
    }
    else
    {
        kernel->cpu.pi = 1;
    }
    
}

void VirtualMachine::unl(int x)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    Resource *r = kernel->getResource("SharedMemoryBlock" + to_string(x));
    if(x >= 0 && x <= 15)
    {
        //if((kernel->cpu.s & (1 << x)) != 0)
        if(!r->isAvailable())
        {
            kernel->cpu.s -= pow(2, x);
            r->setAvailable(true);
        }
        else
        {
            kernel->cpu.pi = 5;
        }
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::sla(int x)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x >= 0 && x <= 15)
    {
        kernel->cpu.si = 4;
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::sua(int x)
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(x >= 0 && x <= 15)
    {
        kernel->cpu.si = 5;
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::cmp()
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    if(kernel->cpu.ba == kernel->cpu.bb) kernel->cpu.sf = 0;
    else if(kernel->cpu.ba > kernel->cpu.bb) kernel->cpu.sf = 1;
    else kernel->cpu.sf = 2;
}

void VirtualMachine::jm(int x1, int x2)
{
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        kernel->cpu.ic = 16*x1 + x2;
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::je(int x1, int x2)
{
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(kernel->cpu.sf == 0)
        {
            kernel->cpu.ic = 16*x1 + x2;
        }
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::ja(int x1, int x2)
{
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(kernel->cpu.sf == 1)
        {
            kernel->cpu.ic = 16*x1 + x2;
        }
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::jb(int x1, int x2)
{
    kernel->cpu.ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(kernel->cpu.sf == 2)
        {
            kernel->cpu.ic = 16*x1 + x2;
        }
    }
    else
    {
        kernel->cpu.pi = 1;
    }
}

void VirtualMachine::halt()
{
    kernel->cpu.ic++;
    kernel->cpu.ti--;
    kernel->cpu.si = 3;
}