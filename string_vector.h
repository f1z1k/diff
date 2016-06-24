#ifndef STRINGVECTOR_H
#define STRINGVECTOR_H
#include <string>
#include <iostream>
#include "argvector.h"
#include <fstream>
using namespace std;

class TStringVector {
public:
    TStringVector(): S(NULL), Length(0) {}
    TStringVector(const TStringVector& src, TArgVector& args);
    ~TStringVector() { if (S) delete[] S; }
    friend istream& operator >> (istream& in, TStringVector& vec) {
        vec.read(in);
        return in;
    }
    string& operator[] (int i) { return S[i]; }
    int length() { return Length; }
    void read(istream& in);
    string* S;
    int Length;
};

#endif

