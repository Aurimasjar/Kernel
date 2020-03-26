#include "realMachine.h"

RealMachine::RealMachine()
{

}

void RealMachine::runProgram(string filename, bool trace)
{
    VirtualMachine vm(filename);
    initializePtr();
    initializePageTable();

    //printPageTable();
    //printReservedBlocks();

    vm.readCommand(ch1);
    while(true)
    {
        vm.readCommand(ch1);

        checkTrace(vm, trace);

        execute(vm);        
        cout << vm.getCommand() << endl;
        checkInterrupts(vm);
    }
}

void RealMachine::checkTrace(VirtualMachine &vm, bool trace)
{
    string waitCommand;
    if(trace == true)
        {
            while(true)
            {
                cout << "Press any symbol and enter to continue or 'nextcom', 'rmdata', 'vmdata', 'pagetable', 'rmreg', 'vmreg' commands to print data" << endl;
                cin >> waitCommand;
                if(waitCommand == "nextcom")
                {
                    cout << "Next command is " << vm.getCommand() << endl;
                }
                else if(waitCommand == "rmdata")
                {
                    printData();
                }
                else if(waitCommand == "vmdata")
                {
                    printVirtualData();
                }
                else if(waitCommand == "pagetable")
                {
                    printPageTable();
                }
                else if(waitCommand == "rmreg")
                {
                    printRegisters();
                }
                else if(waitCommand == "vmreg")
                {
                    printVirtualRegisters(vm);
                }
                else
                {
                    break;
                }
                
            }
            waitCommand = "";
        }
}

int RealMachine::test()
{
    if((pi + si) > 0 || ti == 0)
    {
        return 1;
    }
    return 0;
}

void RealMachine::checkInterrupts(VirtualMachine &vm)
{
    if(test())
    {
        mode = 1;

        if(pi > 0)
        {
            interruptQuit(1, vm);
        }
        else if(ti <= 0)
        {
            interruptQuit(1, vm);
        }
        else if(si == 1) //GD
        {
            int x1 = vm.getCommand(2) - '0';
            int x2 = vm.getCommand(3) - '0';
            int x;
            vm.externalData >> x;
            if(vm.externalData) ch3 = 1;
            else ch3 = 0;
            setRealData(x, 16*x1 + x2);
            //flash >> data[16*x1 + x2];
        }
        else if(si == 2) //PD
        {
            int x1 = vm.getCommand(2) - '0';
            int x2 = vm.getCommand(3) - '0';
            vm.printer << getRealData(16*x1 + x2);
            if(vm.printer) ch2 = 1;
            else ch2 = 0;
        }
        else if(si == 3) //HALT
        {
            interruptQuit(0, vm);
        }
        else if(si == 4) //SLA
        {
            int x = vm.getCommand(3) - '0';
            vm.setBA(Word::wordToIntDec(data[sptr].getWord(x)));
        }
        else if(si == 5) //SUA
        {
            int x = vm.getCommand(3) - '0';
            data[sptr].setWord(Word::intToWordDec(vm.getBA()), x);
        }

        si = 0;
        pi = 0;

        mode = 0;
    }
}

void RealMachine::interruptQuit(int status, VirtualMachine &vm)
{
    cout << endl;
    printRegisters();
    printVirtualRegisters(vm);
    printData();
    printVirtualData();
    printPageTable();
    //free memory?
    exit(status);
}

void RealMachine::execute(VirtualMachine &vm)
{
    if(vm.getCommand() == "ADD0")
    {
       add(vm);
    }
    else if(vm.getCommand() == "SUB0")
    {
       sub(vm);
    }
    else if(vm.getCommand() == "MUL0")
    {
       mul(vm);
    }
    else if(vm.getCommand() == "DIV0")
    {
       div(vm);
    }
    else if(vm.getCommand(0) == 'L' && vm.getCommand(1) == 'A')
    {
        la(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'L' && vm.getCommand(1) == 'B')
    {
        lb(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'U' && vm.getCommand(1) == 'A')
    {
        ua(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'U' && vm.getCommand(1) == 'B')
    {
        ub(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'G' && vm.getCommand(1) == 'D')
    {
        gd(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'P' && vm.getCommand(1) == 'D')
    {
        pd(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'L' && vm.getCommand(1) == 'O' && vm.getCommand(0) == 'C')
    {
        loc(vm, vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'U' && vm.getCommand(1) == 'N' && vm.getCommand(0) == 'L')
    {
        unl(vm, vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'S' && vm.getCommand(1) == 'L' && vm.getCommand(0) == 'A')
    {
        sla(vm, vm.getCommand(3) - '0');
    }
    else if(vm.getCommand(0) == 'S' && vm.getCommand(1) == 'U' && vm.getCommand(0) == 'A')
    {
        sua(vm, vm.getCommand(3) - '0');
    }
    else if(vm.getCommand() == "CMP ")
    {
        cmp(vm);
    }
    else if(vm.getCommand(0) == 'J' && vm.getCommand(1) == 'M')
    {
        jm(vm, vm.getCommand(2) - '0', vm.getCommand(3) - '0');
    }
    else if(vm.getCommand() == "HALT")
    {
        halt(vm);
    }
    else
    {
        pi = 2;
    }
        
}

int RealMachine::getRealData(int virtualAddress) // apsaugoti semaforu?
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    sptr = Word::wordToIntDec(data[10 * a2 + a3].getWord(x1));
    return Word::wordToInt(data[sptr].getWord(x2));
}

void RealMachine::setRealData(int x, int virtualAddress) // apsaugoti semaforu?
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    sptr = Word::wordToIntDec(data[10 * a2 + a3].getWord(x1));
    data[sptr].setWord(Word::intToWord(x), x2);
}

void RealMachine::initializePtr()
{
    srand(time(NULL));
    int pageTableBlockIndex = rand() % 80;
    int a0 = 0;
    int a1 = 0;
    int a2 = pageTableBlockIndex / 10;
    int a3 = pageTableBlockIndex % 10;
    //cout << "Ptr: " << pageTableBlockIndex << endl;

    ptr.setByte((char) a0, 0);
    ptr.setByte((char) a1, 1);
    ptr.setByte((char) a2, 2);
    ptr.setByte((char) a3, 3);

    sptr = pageTableBlockIndex;
    data[sptr].setReserved(true); // pakeisti S reiksme
    s = pageTableBlockIndex;
}

void RealMachine::initializePageTable()
{
    srand(time(NULL));

    int randomBlockIndex;
    for(int i = 0; i < 16; i++)
    {
        randomBlockIndex = -1;
        while(true)
        {
            randomBlockIndex = rand() % 80;
            if(!data[randomBlockIndex].isReserved())
            {
                break;
            }
        }
        data[randomBlockIndex].setReserved(true);
        sptr = 10 * ptr.getByte(2) + ptr.getByte(3);
        data[sptr].setWord(Word::intToWordDec(randomBlockIndex), i);
    }
}

void RealMachine::printVirtualRegisters(VirtualMachine &vm)
{
    cout << "Virtual machine registers:" << endl;
    cout << "Ic = " << vm.getIC() << endl;
    cout << "Ba = " << vm.getBA() << endl;
    cout << "Bb = " << vm.getBB() << endl;
    cout << endl;
}

void RealMachine::printRegisters()
{
    cout << "Real machine registers:" << endl;
    cout << "Ic = " << ic << endl;
    cout << "Ptr = " << ptr.getWord() << endl;
    cout << "Sptr = " << sptr << endl;
    cout << "Ba = " << ba << endl;
    cout << "Bb = " << ba << endl;
    cout << "Bc = " << ba << endl;
    cout << "Sf = " << sf << endl;
    cout << "S = " << s << endl;
    cout << "Mode = " << mode << endl;
    cout << "Ti = " << ti << endl;
    cout << "Pi = " << pi << endl;
    cout << "Si = " << si << endl;
    cout << "Ch1 = " << ch1 << endl;
    cout << "Ch2 = " << ch2 << endl;
    cout << "Ch3 = " << ch3 << endl;
    cout << endl;
}

void RealMachine::printData()
{
    cout << "Real machine memory:" << endl;
    for(int i = 0; i < 80; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < 16; j++)
        {
            data[i].getWord(j).print();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void RealMachine::printVirtualData()
{
    cout << "Virtual machine memory:" << endl;
    for(int i = 0; i < 16; i++)
    {
        cout << i << ": ";
        for(int j = 0; j < 16; j++)
        {
            Word::intToWord(getRealData(16*i+j)).print();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void RealMachine::printPageTable()
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    cout << "VM  |  RM" << endl;
    for(int i = 0; i < 16; i++)
    {
        cout << i << "  |  " << data[10*a2 + a3].getWord(i).getWord() << endl;
    }
}

void RealMachine::printReservedBlocks()
{
    cout << "Reserved blocks:" << endl;
    for(int i = 0; i < 80; i++)
    {
        cout << data[i].isReserved();
    }
    cout << endl;
}

void RealMachine::add(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    vm.setBA(vm.getBA() + vm.getBB());
}

void RealMachine::sub(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    vm.setBA(vm.getBA() - vm.getBB());
}

void RealMachine::mul(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    vm.setBA(vm.getBA() * vm.getBB());
}

void RealMachine::div(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    int r = vm.getBA() % vm.getBB();
    vm.setBA(vm.getBA() / vm.getBB());
    vm.setBB(r);
}


void RealMachine::la(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        vm.setBA(getRealData(16*x1 + x2));
    }
    else
    {
        pi = 1; // wrong address
    }
    
    //vm.ba = wordToInt(data[16*x1 + x2]);
}

void RealMachine::lb(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        vm.setBB(getRealData(16*x1 + x2));
    }
    else
    {
        pi = 1; // wrong address
    }
    //vm.bb = data[16*x1 + x2];
}

void RealMachine::ua(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        setRealData(vm.getBA(), 16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    //data[16*x1 + x2] = vm.ba;
}

void RealMachine::ub(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        setRealData(vm.getBB(), 16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    //data[16*x1 + x2] = vm.bb;
}

void RealMachine::gd(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti -= 3;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        //mode = 1;
        si = 1;
        /*int x;
        vm.externalData >> x;
        if(vm.externalData) ch3 = 1;
        else ch3 = 0;
        setRealData(x, 16*x1 + x2);
        //flash >> data[16*x1 + x2];*/
        //mode = 0;
    }
    else
    {
        pi = 1; // wrong address
    }
}

void RealMachine::pd(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti -= 3;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        //mode = 1;
        si = 2;
        /*vm.printer << getRealData(16*x1 + x2);
        if(vm.printer) ch2 = 1;
        else ch2 = 0;
        //printer << data[16*x1 + x2];*/
        //mode = 0;
    }
    else
    {
        pi = 1; // wrong address
    }
}

void RealMachine::loc(VirtualMachine &vm, int x)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x >= 0 && x <= 9)
    {
        //word [16*sptr + x] locked
        s = x; //ar s=16*sptr+x?
    }
    else
    {
        pi = 1;
    }
    
}

void RealMachine::unl(VirtualMachine &vm, int x)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x >= 0 && x <= 9)
    {
        //word [16*sptr + x] unlocked
        s = x; //ar s=16*sptr+x?
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::sla(VirtualMachine &vm, int x)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x >= 0 && x <= 9)
    {
        //mode = 1;
        si = 4;
        //vm.setBA(Word::wordToIntDec(data[sptr].getWord(x)));
        //mode = 0;
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::sua(VirtualMachine &vm, int x)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x >= 0 && x <= 9)
    {
        //mode = 1;
        si = 5;
        //data[sptr].setWord(Word::intToWordDec(vm.getBA()), x);
        //mode = 0;
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::cmp(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(vm.getBA() == vm.getBB()) sf = 0;
    else if(vm.getBA() > vm.getBB()) sf = 1;
    else sf = 2;
}

void RealMachine::jm(VirtualMachine &vm, int x1, int x2)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    if(x1 >= 0 && x1 <= 9 && x2 >= 0 && x2 <= 9)
    {
        //perduoti valdyma komandai adresu 16*x1 + x2
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::halt(VirtualMachine &vm)
{
    ic++;
    vm.setIC(vm.getIC() + 1);
    ti--;
    //mode = 1;
    si = 3;
    //mode = 0;
}
