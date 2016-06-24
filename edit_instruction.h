#include "argvector.h"
#include "string_vector.h"
using namespace std;
const int NOPE = -1;
enum  TTypeEdit {MATCH, DELETE, INSERT, REPLACE};
class TEditItem {
public:
    TEditItem() {}
    TEditItem(TTypeEdit typeEdit, int old, int new_) :
        TypeEdit(typeEdit), Old(old), New(new_) {}
    TTypeEdit TypeEdit;
    int Old;
    int New;
};

class TEditSet {
public:
    TEditSet(int size, TEditItem* set): Size(size), Set(set) {}
    TEditSet(const TEditSet& src);
    ~TEditSet() { if(Set) delete[] Set; }
    TEditSet operator+(const TEditSet& b);
    TEditItem& operator[] (int i) {return Set[i];}
    int Size;
    TEditItem* Set;
};

class TEditInstruction {
public:
    TEditInstruction(TStringVector& s1, TStringVector& s2, TArgVector& args);
    bool Print(TStringVector& s1, TStringVector& s2);
private:
    void PrintEdit(TStringVector& s1, TStringVector& s2,
                        int firstEdit, int cntI, int cntD, int cntR);
    void PrintAdd(TStringVector& s1, TStringVector& s2,
                        int firstEdit, int numEdit);
    void PrintDelete(TStringVector& s1, TStringVector& s2,
                        int firstEdit, int numEdit);
    void PrintChange(TStringVector& s1, TStringVector& s2,
                        int firstEdit, int cntI, int cntD, int cntR);
    void PrintDeleteHunk(TStringVector& s1, TStringVector& s2,
        int firstEdit, int numEdit, bool isTab);
    void PrintAddHunk(TStringVector& s1, TStringVector& s2,
        int firstEdit, int numEdit, bool isTab);

    bool isBlankHunk(TStringVector& s, int a, int b);
    void GetRangeDeleteHunk(int& a, int& b, int firstEdit, int numDelete);
    void GetRangeAddHunk(int& a, int& b, int firstEdit, int numAdd);
    TEditSet getEditSet(int begin1, int end1, int begin2, int end2);    
    TEditSet getEditSet1(int begin1, int end1, int begin2, int end2);
    int* getLastD(int begin1, int end1, int begin2, int end2, bool isSwap);
    int* getLastE(int begin1, int end1, int begin2, int end2, bool isSwap);
    int minK(int begin1L, int end1L, int begin1R, int end1R, int begin2, int end2, bool isSwap);
    TEditSet OnlyDelete(int begin, int n);
    TEditSet OnlyInsert(int begin, int n);
    TEditSet OneChar(int c, int begin, int n);
    TEditSet LastOneChar(int c, int begin, int n);
    int min(int a, int b, int c);
    char ToChar(TTypeEdit c);

    TStringVector S1;
    TStringVector S2;
    TEditSet EditSet;
    TArgVector Args;
};
