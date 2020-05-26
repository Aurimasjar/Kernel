#include "process.h"

using namespace std;

int MemoryWriter::runProcess()
{
    if(run == 0)
    {
        askForResource("TaskInSupervisorMemory");
        run++;
    }
    else
    {
        int success = putCommandsToUserMemory();

        destroyResource("TaskInSupervisorMemory");
        Resource *r = kernel->getResource("MemoryWritten");
        r->setValue(success);
        releaseResource("MemoryWritten");
        releaseResource("SupervisorMemory");
        run = 0;
    }
    
    return 0;
}

int MemoryWriter::putCommandsToUserMemory()
{
    int i = 0, j = 10 * 16;
    string command;
    bool haltExists = false;
    for(i = 0; i < 80 * 16; i++)
    {
        command = Word::wordToString(kernel->supervisorMemory[i/16].getWord(i%16));
        if(command == "$STR")
        {
            break;
        }
    }
    for(i = i + 1; i < 80 * 16; i++)
    {
        command = Word::wordToString(kernel->supervisorMemory[i/16].getWord(i%16));
        //cout << command << endl;
        if(command == "HALT") haltExists = true;
        else if(command == "$END")
        {
            if(haltExists) return 0;
            else break;
        }
        kernel->memory->setRealData(Word::stringToWord(command), j);
        j++;
    }
    return -1;
}