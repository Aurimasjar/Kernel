#ifndef WORD_H
#define WORD_H

#include <iostream>

using namespace std;

class Word
{
    private:

    char word[4];

    public:

    Word() { word[0] = 0; word[1] = 0; word[2] = 0; word[3] = 0; }
    ~Word() {}

    void setWord(int x) { word[0] = x / (256*256*256); word[1] = x / (256*256) % 256; word[2] = x / 256 % 256; word[3] = x % 256; }
    void setWord(char x[4]) { for(int i = 0; i < 4; i++) { word[i] = x[i]; }}
    //void setWord(string x) { for(int i = 0; i < 4; i++) { word[i] = x[i]; }}
    void setByte(char x, int pos) { word[pos] = x; }
    
    int getWord() { return wordToIntDec(word); }
    //char *getWord() { return word; }
    char getByte(int pos) { return word[pos]; }

    static void intToWord(char word[4], int x);
    static Word intToWord(int x);
    static Word intToWordDec(int x);
    static int wordToInt(char word[4]);
    static int wordToInt(Word word);
    static int wordToIntDec(char word[4]);
    static int wordToIntDec(Word word);

    void print();
    void printAsSymbol();

    static int hexToInt(char s);
    static char charToHex(char s);
    static char intToHex(int x);
    static Word stringToWord(string str);
    static string wordToString(Word w);
};

#endif