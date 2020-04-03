#include "word.h"

void Word::intToWord(char word[4], int x)
{
    word[0] = static_cast<char>(x / (256*256*256));
    word[1] = static_cast<char>(x / (256*256) % 256);
    word[2] = static_cast<char>(x / 256 % 256);
    word[3] = static_cast<char>(x % 256);
}

Word Word::intToWord(int x)
{
    Word w;
    w.word[0] = x / (256*256*256);
    w.word[1] = x / (256*256) % 256;
    w.word[2] = x / 256 % 256;
    w.word[3] = x % 256;
    return w;
}

Word Word::intToWordDec(int x)
{
    Word w;
    w.word[0] = x / 1000;
    w.word[1] = x / 100 % 10;
    w.word[2] = x / 10 % 10;
    w.word[3] = x % 10;
    return w;
}

int Word::wordToInt(char word[4])
{
    int x = 0;
    x += (int) word[0] * 256*256*256;
    x += (int) word[1] * 256*256;
    x += (int) word[2] * 256;
    x += (int) word[3];
    return x;
}

int Word::wordToInt(Word w)
{
    int x = 0;
    x += (int) w.word[0] * 256*256*256;
    x += (int) w.word[1] * 256*256;
    x += (int) w.word[2] * 256;
    x += (int) w.word[3];
    return x;
}

int Word::wordToIntDec(Word w)
{
    int x = 0;
    x += (int) w.word[0] * 1000;
    x += (int) w.word[1] * 100;
    x += (int) w.word[2] * 10;
    x += (int) w.word[3];
    return x;
}

int Word::wordToIntDec(char w[4])
{
    int x = 0;
    x += (int) w[0] * 1000;
    x += (int) w[1] * 100;
    x += (int) w[2] * 10;
    x += (int) w[3];
    return x;
}

void Word::print()
{
    //cout << (int)word[0] <<  (int)word[1] <<  (int)word[2] <<  (int)word[3];
    for(int i = 0; i < 4; i++)
    {
        //cout << word[i];
        cout << charToHex(word[i] / 16) << charToHex(word[i] % 16);
    }
}

char Word::intToHex(int x)
{
    if(x >= 0 && x <= 9)
    {
        return x + 48;
    }
    return x + 55;
}

char Word::charToHex(char s)
{
    if(s >= 0 && s <= 9)
    {
        return s + 48;
    }
    return s + 55;
}


int Word::hexToInt(char s)
{
    if(s >= 'A' && s <= 'F')
    {
        return s - 55;
    }
    return s - 48;
}

Word Word::stringToWord(string str)
{
    Word w;
    if(str.length() < 4)
    {
        for(int i = 0; i < str.length(); i++)
        {
            w.word[i] = str[i];
        }
        for(int i = str.length(); i < 4; i++)
        {
            w.word[i] = '$';
        }
        return w;
    }  
    for(int i = 0; i < 4; i++)
    {
        w.word[i] = str[i];
    }
    return w;
}

string Word::wordToString(Word w)
{
    string str = "";
    for(int i = 0; i < 4; i++)
    {
        str += w.word[i];
    }
    return str;
}
