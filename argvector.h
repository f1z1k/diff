#ifndef ARGVECTOR_H
#define ARGVECTOR_H
#include <iostream>
using namespace std;
class TArgVector {
public:
    TArgVector(): File1(NULL), File2(NULL), 
        Isi(false), IsE(false), Isb(false), Isw(false), IsB(false),
        IsNormal(true), Ist(false) {}
    bool GetArgs(int argc, char* argv[]);
    char* File1;
    char* File2;
    bool Isi;
    bool IsE;
    bool Isb;
    bool Isw;
    bool IsB;
    bool IsNormal;
    bool Ist;
};

#endif
