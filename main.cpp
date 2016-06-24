#include <iostream>
#include <cstring>

#include "argvector.h"
#include "string_vector.h"
#include "edit_instruction.h"
using namespace std;
int main(int argc, char* argv[]) {
    TArgVector args;
    if (!args.GetArgs(argc, argv)) {
        return -1;
    }
    ifstream in1(args.File1);
    ifstream in2(args.File2);
    TStringVector s1;
    TStringVector s2;
    in1 >> s1;
    in2 >> s2;
    TEditInstruction editInstruction(s1, s2, args);
    if (editInstruction.Print(s1, s2)) {
        return 1;
    }
    return 0;
}

