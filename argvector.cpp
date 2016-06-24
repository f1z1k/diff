#include <cstring>
#include "argvector.h"

bool TArgVector::GetArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-i") == 0) Isi = true;
            else if (strcmp(argv[i], "-E") == 0) IsE = true;
            else if (strcmp(argv[i], "-b") == 0) Isb = true;
            else if (strcmp(argv[i], "-w") == 0) Isw = true;
            else if (strcmp(argv[i], "-B") == 0) IsB = true;
            else if (strcmp(argv[i], "--normal") == 0) IsNormal = true;
            else if (strcmp(argv[i], "-t") == 0) Ist = true;
            continue;
        }
        if (File1 == NULL) {
            File1 = argv[i];
        } else if (File2 == NULL) {
            File2 = argv[i];
        } else {
            cerr << "diff: extra operand \'" << argv[i] << "\'" << endl;
            return false;
        }
    }
    if (File1 == NULL) {
        cerr << "diff: missing operand after 'diff'" << endl;
        return false;
    }
    if (File2 == NULL) {
        cerr << "diff: missing operand after \'" << File1 << "\'" << endl;
        return false;
    }
    return true;
}


