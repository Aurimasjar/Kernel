#ifndef BLOCK_H
#define BLOCK_H

#include "word.h"

class Block
{
    private:

    int id;
    Word block[16];
    bool reserved;

    public:

    Block() { reserved = false; }
    ~Block() {}

    void setId(int x) { id = x; }
    int getId() { return id; }

    void setReserved(bool x) { reserved = x; }
    bool isReserved() { return reserved; }

    //void setBlock(Word x[16]) { block = x; }
    //void getBlock(Word x[16]) { x = block; }
    void setWord(Word word, int pos) { block[pos] = word; }
    Word getWord(int pos) { return block[pos]; }
};

#endif