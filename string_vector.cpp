#include "string_vector.h"
TStringVector::TStringVector(const TStringVector& src, TArgVector& args)
        : Length(0)
{
    S = new string [src.Length];
    for (int i = 0; i < src.Length; i++) {
        bool isFirstSpace = true;
        string str = "";
        int lastChar = -1;
        for (int j = 0; j < src.S[i].length(); j++) {
            char c = src.S[i][j];
            string res = "";
            res += args.Isi ? tolower(c) : c;
            if (!isblank(c)) {
                lastChar = str.length();
            }
            if (args.IsE && c == '\t') res = "        ";
            
            if (args.Isb) {
                if (isblank(c)) {
                    if (!isFirstSpace)
                        res = "";
                } else {
                    isFirstSpace = true;
                }
            }
            
            if (args.Isw && isblank(c)) res = "";
            str += res;
        }
        if (args.Isb) {
            string zStr = "";
            for (int i = 0; i <= lastChar; i++) {
                zStr += str[i];
            }
            str = zStr;
        }
        S[Length] = str;
        Length++; 
    }
}

void TStringVector::read(istream& in) {
    string s;
    getline(in, s);
    while (!in.eof()) {
        string* newS = new string [Length + 1];
        if (S != NULL) {
            for (int i = 0; i < Length; i++) {
                newS[i] = S[i];
            }
            delete[] S;
        }
        S = newS;
        S[Length] = s;
        Length++;
        getline(in, s);
    }
}
