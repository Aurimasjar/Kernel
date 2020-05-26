#include "process.h"

using namespace std;

int MainProc::runProcess()
{
    if(run == 0)
    {
        askForResource("MainProc");
        run++;
    }
    else
    {
        Resource *r = kernel->getResource("MainProc");
        if(r->getValue() == 0)
        {
            // destroy process JobGovernor
            kernel->removeProcess(r->father);
            kernel->finishedProgram++;

            if(kernel->finishedProgram > kernel->programCount)
            {
                releaseResource("MosEnd");
            }
        }
        else
        {
            Process *pr;
            pr = new JobGovernor(20 + kernel->launchingProgram, this, 2, 50, "JobGovernor" + to_string(kernel->launchingProgram), kernel);
            kernel->launchingProgram++;
            kernel->addProcess(pr);
        }
        destroyResource("MainProc");
        run = 0;
    }
    return 0;
}