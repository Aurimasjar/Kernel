#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>
#include <fstream>

using namespace std;

class VirtualMachine
{
    private:

    int ic;
    int ba;
    int bb;
    int data[256];
    ifstream flash;
    int readData;
    ofstream printer;

    public:

    VirtualMachine()
    {
        ic = 0;
        ba = 0;
        bb = 0;
        flash.open("flash.txt");
        printer.open("printer.txt");
    }

    ~VirtualMachine()
    {
        flash.close();
        printer.close();
    }

    void setIC(int ic) { this->ic = ic; }
    int getIC() { return ic; }

    void setBA(int ba) { this->ba = ba; }
    int getBA() { return ba; }

    void setBB(int bb) { this->bb = bb; }
    int getBB() { return bb; }

    void printData();

    //arithmetic operations
    void add();
    void sub();
    void mul();
    void div();

    //operations for working with data
    void la(int x1, int x2);
    void lb(int x1, int x2);
    void ua(int x1, int x2);
    void ub(int x1, int x2);
    void gd(int x1, int x2);
    void pd(int x1, int x2);
    void loc(int x);
    void uni(int x);
    void sla(int x);
    void sua(int x);

    //comparison operations
    void cmp();

    //control operations
    void jm(int x1, int x2);
    void halt();

};

#endif