#include "realMachine.h"

RealMachine::RealMachine()
{
    //printer.open("printer.txt");
    /*for(int i = 72; i < 80; i++)
    {
        data[i].setReserved(true);
    }*/
    sptr = 79;
    data[sptr].setReserved(true);
}

RealMachine::~RealMachine()
{
    //printer.close();
}

void RealMachine::runProgram(string filename, bool trace)
{
    VirtualMachine vm(filename);
    initializePtr();
    initializePageTable();
    prepareCommands(vm);

    //printPageTable();
    //printReservedBlocks();

    string command = getCommand();
    while(true)
    {
        command = getCommand();
        checkTrace(trace);
        execute(command);        
        cout << command << endl;
        checkInterrupts(vm, command, trace);
    }
}

string RealMachine::getCommand()
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    int x1 = ic / 16;
    int x2 = ic % 16;
    return Word::wordToString(data[Word::wordToIntDec(data[10 * a2 + a3].getWord(x1))].getWord(x2));
}

void RealMachine::checkTrace(bool trace)
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
                    cout << "Next command is " << getCommand() << endl;
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
                    printVirtualRegisters();
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

void RealMachine::checkInterrupts(VirtualMachine &vm, string command, bool trace)
{
    if(test())
    {
        mode = 1;

        if(pi > 0)
        {
            interruptQuit(1);
        }
        else if(ti <= 0)
        {
            interruptQuit(1);
        }
        else if(si == 1) //GD
        {
            int x1 = Word::hexToInt(command[2]);
            int x2 = Word::hexToInt(command[3]);
            int x;
            vm.externalData >> x;
            if(vm.externalData) ch3 = 1;
            else ch3 = 0;
            setRealData(x, 16*x1 + x2);
            checkTrace(trace);
        }
        else if(si == 2) //PD
        {
            int x1 = Word::hexToInt(command[2]);
            int x2 = Word::hexToInt(command[3]);
            printer.open("printer.txt"); //open with append?
            printer << getRealData(16*x1 + x2);
            printer.close();
            if(printer) ch2 = 1;
            else ch2 = 0;
            checkTrace(trace);
        }
        else if(si == 3) //HALT
        {
            interruptQuit(0);
        }
        else if(si == 4) //SLA
        {
            int x = Word::hexToInt(command[3]);
            if((s & (1 << x)) != 0)
            {
                ba = Word::wordToIntDec(data[sptr].getWord(x));
                checkTrace(trace);
            }
            else 
            {
                pi = 5;
            }
        }
        else if(si == 5) //SUA
        {
            
            int x = Word::hexToInt(command[3]);
            if((s & (1 << x)) != 0)
            {
                data[sptr].setWord(Word::intToWordDec(ba), x);
                checkTrace(trace);
            }
            else
            {
                pi = 5;
            }
        }

        if(pi > 0)
        {
            interruptQuit(0);
        }

        si = 0;
        pi = 0;

        mode = 0;
    }
}

void RealMachine::interruptQuit(int status)
{
    cout << endl;
    printRegisters();
    printVirtualRegisters();
    printData();
    printVirtualData();
    printPageTable();
    freeVirtualMemory();
    cout << "Exit status " << status;
    //cout << " " << si << " " << pi << endl;
    exit(status);
}

void RealMachine::freeVirtualMemory()
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    for(int i = 0; i < 16; i++)
    {
        data[Word::wordToIntDec(data[10 * a2 + a3].getWord(i))].setReserved(false);
    }
    data[10 * a2 + a3].setReserved(false);
}

void RealMachine::execute(string command)
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
        pi = 2;
    }
        
}

int RealMachine::getRealData(int virtualAddress)
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    return Word::wordToInt(data[Word::wordToIntDec(data[10 * a2 + a3].getWord(x1))].getWord(x2));
}

void RealMachine::setRealData(int x, int virtualAddress)
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    int x1 = virtualAddress / 16;
    int x2 = virtualAddress % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    data[Word::wordToIntDec(data[10 * a2 + a3].getWord(x1))].setWord(Word::intToWord(x), x2);
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

    data[pageTableBlockIndex].setReserved(true);
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
        data[10 * ptr.getByte(2) + ptr.getByte(3)].setWord(Word::intToWordDec(randomBlockIndex), i);
    }
}

void RealMachine::prepareCommands(VirtualMachine &vm)
{
    int a2 = ptr.getByte(2);
    int a3 = ptr.getByte(3);
    string command;
    for(int i = 0;; i++)
    {
        command = vm.readCommand(ch1);
        if(command == "")
        {
            break;
        }
    int x1 = (i + ic) / 16;
    int x2 = (i + ic) % 16;
    //return 16 * wordToInt(rm->data[16 * (10 * a2 + a3) + x1] + x2);
    data[Word::wordToIntDec(data[10 * a2 + a3].getWord(x1))].setWord(Word::stringToWord(command), x2);
    }
    ic++;
    if(ch1 == 0)
    {
        interruptQuit(1);
    }
}

void RealMachine::printVirtualRegisters()
{
    cout << "Virtual machine registers:" << endl;
    cout << "Ic = " << ic << endl;
    cout << "Ba = " << ba << endl;
    cout << "Bb = " << bb << endl;
    cout << endl;
}

void RealMachine::printRegisters()
{
    cout << "Real machine registers:" << endl;
    cout << "Ic = " << ic << endl;
    cout << "Ptr = " << ptr.getWord() << endl;
    cout << "Sptr = " << sptr << endl;
    cout << "Ba = " << ba << endl;
    cout << "Bb = " << bb << endl;
    cout << "Bc = " << bc << endl;
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

void RealMachine::add()
{
    ic++;
    ba += bb;
}

void RealMachine::sub()
{
    ic++;
    ti--;
    ba -= bb;
}

void RealMachine::mul()
{
    ic++;
    ti--;
    ba *= bb;
}

void RealMachine::div()
{
    ic++;
    ti--;
    int r = ba % bb;
    ba /= bb;
    bb = r;
}


void RealMachine::la(int x1, int x2)
{
    ic++;
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
       ba = getRealData(16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    
    //vm.ba = wordToInt(data[16*x1 + x2]);
}

void RealMachine::lb(int x1, int x2)
{
    ic++;
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        bb = getRealData(16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    //vm.bb = data[16*x1 + x2];
}

void RealMachine::ua(int x1, int x2)
{
    ic++;
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        setRealData(ba, 16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    //data[16*x1 + x2] = vm.ba;
}

void RealMachine::ub(int x1, int x2)
{
    ic++;
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        setRealData(bb, 16*x1 + x2);
    }
    else
    {
        pi = 1; // wrong address
    }
    //data[16*x1 + x2] = vm.bb;
}

void RealMachine::gd(int x1, int x2)
{
    ic++;
    ti -= 3;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
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

void RealMachine::pd(int x1, int x2)
{
    ic++;
    ti -= 3;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
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

void RealMachine::loc(int x)
{
    ic++;
    ti--;
    if(x >= 0 && x <= 15)
    {
        if((s & (1 << x)) == 0)
        {
            s += pow(2, x);
        }
        else
        {
            pi = 5;
        }
        
    }
    else
    {
        pi = 1;
    }
    
}

void RealMachine::unl(int x)
{
    ic++;
    ti--;
    if(x >= 0 && x <= 15)
    {
        if((s & (1 << x)) != 0)
        {
            s -= pow(2, x);
        }
        else
        {
            pi = 5;
        }
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::sla(int x)
{
    ic++;
    ti--;
    if(x >= 0 && x <= 15)
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

void RealMachine::sua(int x)
{
    ic++;
    ti--;
    if(x >= 0 && x <= 15)
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

void RealMachine::cmp()
{
    ic++;
    ti--;
    if(ba == bb) sf = 0;
    else if(ba > bb) sf = 1;
    else sf = 2;
}

void RealMachine::jm(int x1, int x2)
{
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        ic = 16*x1 + x2;
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::je(int x1, int x2)
{
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(sf == 0)
        {
            ic = 16*x1 + x2;
        }
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::ja(int x1, int x2)
{
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(sf == 1)
        {
            ic = 16*x1 + x2;
        }
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::jb(int x1, int x2)
{
    ti--;
    if(x1 >= 0 && x1 <= 15 && x2 >= 0 && x2 <= 15)
    {
        if(sf == 2)
        {
            ic = 16*x1 + x2;
        }
    }
    else
    {
        pi = 1;
    }
}

void RealMachine::halt()
{
    ic++;
    ti--;
    //mode = 1;
    si = 3;
    //mode = 0;
}