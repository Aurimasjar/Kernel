#ifndef CHANNELDEVICE_H
#define CHANNELDEVICE_H

#include <fstream>
#include <string.h>
#include "block.h"
#include "word.h"
#include "kernel.h"

using namespace std;

class Kernel;

class ChannelDevice
{
    public:

    Block channelMemory[16];
    int word;
    int type;
    ifstream iFile;
    ofstream oFile;

    ChannelDevice(int type);
    ChannelDevice(int type, string file);
    ~ChannelDevice();
    void readToChannel(string file);
    int readWordToChannel();
    void readToSupervisorMemory(Kernel *kernel);
    void writeFromSupervisorMemory(Kernel *kernel);
    void writeFromChannel(string file);
    void writeWordFromChannel(int word);

    void printChannelMemory();
};

#endif