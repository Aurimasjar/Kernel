#include "channelDevice.h"

using namespace std;

ChannelDevice::ChannelDevice(int type, string file)
{
    this->type = type;
    if(type == 1)
    {
        iFile.open(file);
    }
    else
    {
        oFile.open(file, std::ios_base::app);
    }
}

ChannelDevice::ChannelDevice(int type)
{
    this->type = type;
}

ChannelDevice::~ChannelDevice()
{
    if(type == 1)
    {
        iFile.close();
    }
    else
    {
        oFile.close();
    }
}

void ChannelDevice::readToChannel(string file)
{
    if(file != "")
    {
        iFile.open(file);
    }
    string text;
    for(int i = 0; !iFile.eof(); i++)
    {
        iFile >> text;
        channelMemory[i/16].setWord(Word::stringToWord(text), i % 16);
    }
    //printChannelMemory();
    if(file != "")
    {
        iFile.close();
    }
}

int ChannelDevice::readWordToChannel()
{
    if(!iFile.eof())
    {
        iFile >> word;
        return word;
    }
    return -1;
}

void ChannelDevice::readToSupervisorMemory(Kernel *kernel)
{
    // if other program in channel1 overwrites supervisor memory make a copy constructor for Word and Block classes
    for(int i = 0; i < 16; i++)
    {
        kernel->supervisorMemory[i] = channelMemory[i];
    }
}

void ChannelDevice::writeFromSupervisorMemory(Kernel *kernel)
{

}

void ChannelDevice::writeFromChannel(string file)
{

}

void ChannelDevice::writeWordFromChannel(int word)
{
    this->word = word;
    oFile << this->word << " ";
}

void ChannelDevice::printChannelMemory()
{
    cout << "printChannelMemory()" << endl;
    for(int i = 0; i < 16; i++)
    {
        cout << Word::intToHex(i/16) << Word::intToHex(i%16) << ": ";
        for(int j = 0; j < 16; j++)
        {
            channelMemory[i].getWord(j).print();
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}