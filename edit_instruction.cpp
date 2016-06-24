#include "edit_instruction.h"
#include <cstring>
#include <iostream>

TEditSet::TEditSet(const TEditSet& editSet):Size(editSet.Size) {
    Set = NULL;
    if (Size > 0) {
        Set = new TEditItem [Size];
        memcpy(Set, editSet.Set, Size * sizeof(TEditItem));
    }
}

TEditSet TEditSet::operator+(const TEditSet& b) {
    int size = Size + b.Size;
    TEditItem* set = NULL;
    if (size > 0) {
        set = new TEditItem [Size + b.Size];
        if (Size > 0) {
            memcpy(set, Set, Size * sizeof(TEditItem));
        }
        if (b.Size > 0) {
            memcpy(&(set[Size]), b.Set, b.Size * sizeof(TEditItem));
        }
    }
    return TEditSet(size, set);
}


TEditInstruction::TEditInstruction
    (TStringVector& s1, TStringVector& s2, TArgVector& args):
    S1(s1, args), S2(s2, args),
    EditSet(getEditSet(0, S1.length(), 0, S2.length())),
    Args(args)
{
}

char TEditInstruction::ToChar(TTypeEdit c) {
    if (c == DELETE) {
        return 'D';
    } else if (c == INSERT) {
        return 'I';
    } else if (c == REPLACE) {
        return 'R';
    }
    return 'M';
}

bool TEditInstruction::Print(TStringVector& s1, TStringVector& s2) {
    bool isEdit = false;
    int firstEdit = 0;
    int cntR = 0, cntD = 0, cntI = 0;
    for (int i = 0; i < EditSet.Size; i++) {
        switch (EditSet[i].TypeEdit) {
            case REPLACE: cntR++; break;
            case INSERT: cntI++; break;
            case DELETE: cntD++; break;
            default:
                if (cntI + cntD + cntR != 0) isEdit = true;
                PrintEdit(s1, s2, firstEdit, cntI, cntD, cntR);
                firstEdit = i + 1;
                cntR = cntI = cntD = 0;
        }
    }
    if (cntI + cntD + cntR != 0) isEdit = true;
    PrintEdit(s1, s2, firstEdit, cntI, cntD, cntR);
    return isEdit;
}

void TEditInstruction::PrintEdit(TStringVector& s1, TStringVector& s2,
        int firstEdit, int cntI, int cntD, int cntR)
{
    if (cntR + cntI + cntD == 0) {
        return;
    }
    if (cntR == 0 && cntD == 0) {
        PrintAdd(s1, s2, firstEdit, cntI);
    } else if (cntR == 0 && cntI == 0) {
        PrintDelete(s1, s2, firstEdit, cntD);
    } else {
        PrintChange(s1, s2, firstEdit, cntI, cntD, cntR);
    }
}

bool TEditInstruction::isBlankHunk(TStringVector& s, int a, int b) {
    bool isAllEmpty = true;
    for (int i = a - 1; i < b; i++) {
        if (s[i].length() != 0) {
            return false;
        }
    }
    return isAllEmpty;
}

void TEditInstruction::PrintAdd(TStringVector& s1, TStringVector& s2,
        int firstAdd, int numAdd)
{
    int first, last;
    int ptrEdit = 0;
    GetRangeAddHunk(first, last, firstAdd, numAdd);
    if (Args.IsB && isBlankHunk(S2, first, last)) {
        return;
    }
    if (firstAdd != 0) {
        ptrEdit = EditSet[firstAdd - 1].Old + 1;
    }

    cout << ptrEdit << "a" << first;
    if (first != last) {
        cout << "," << last;
    }
    cout << endl;
    PrintAddHunk(s1, s2, first, last, Args.Ist);
}

void TEditInstruction::PrintDelete(TStringVector& s1, TStringVector& s2,
        int firstDelete, int numDelete)
{
    int first, last;
    int ptrEdit = 0;
    GetRangeDeleteHunk(first, last, firstDelete, numDelete);
    if (Args.IsB && isBlankHunk(S1, first, last)) {
        return;
    }
    if (firstDelete != 0) {
        ptrEdit = EditSet[firstDelete - 1].New + 1;
    }
    cout << first;
    if (first != last) {
        cout << "," << last;
    }
    cout << "d" << ptrEdit << endl;
    PrintDeleteHunk(s1, s2, first, last, Args.Ist);
}

void TEditInstruction::PrintChange(TStringVector& s1, TStringVector& s2,
        int firstEdit, int cntI, int cntD, int cntR)
{
    int firstOld, lastOld, firstNew, lastNew;
    int numEdit = cntI + cntD + cntR;
    GetRangeDeleteHunk(firstOld, lastOld, firstEdit, numEdit);
    GetRangeAddHunk(firstNew, lastNew, firstEdit, numEdit);
    if (Args.IsB && isBlankHunk(S1, firstOld, lastOld) &&
            isBlankHunk(S2, firstNew, lastNew)) {
        return;
    }
    cout << firstOld;
    if (firstOld != lastOld) {
        cout << "," << lastOld;
    }
    cout << "c" << firstNew;
    if (firstNew != lastNew) {
        cout << "," << lastNew;
    }
    cout << endl;
    PrintDeleteHunk(s1, s2, firstOld, lastOld, Args.Ist);
    cout << "---" << endl;
    PrintAddHunk(s1, s2, firstNew, lastNew, Args.Ist);
}

void TEditInstruction::PrintDeleteHunk(TStringVector& s1, TStringVector& s2,
        int a, int b, bool isTab)
{
    for (int i = a - 1; i < b; i++) {
        cout << "< ";
        if (isTab) {
            for (int j = 0; j < s1[i].length(); j++) {
                if (s1[i][j] == '\t') {
                    cout << "        ";
                } else {
                    cout << s1[i][j];
                }
            }        
        } else {
            cout << s1[i];
        }
        cout << endl;
    }
}

void TEditInstruction::PrintAddHunk(TStringVector& s1, TStringVector& s2,
        int a, int b, bool isTab)
{
    for (int i = a - 1; i < b; i++) {
        cout << "> ";
        if (isTab) {
            for (int j = 0; j < s2[i].length(); j++) {
                if (s2[i][j] == '\t') {
                    cout << "        ";
                } else {
                    cout << s2[i][j];
                }
            }        
        } else {
            cout << s2[i];
        }
        cout << endl;
    }
}

void TEditInstruction::GetRangeDeleteHunk(int& a, int& b, int firstEdit, int numEdit) {
    int lastEdit = firstEdit + numEdit - 1;
    a = EditSet[firstEdit].Old;
    while (a == -1) {
        a = EditSet[firstEdit++].Old;
    }
    b = EditSet[lastEdit].Old;
    while (b == -1) {
        b = EditSet[lastEdit--].Old;
    }
    a++;
    b++;
}

void TEditInstruction::GetRangeAddHunk(int& a, int& b, int firstEdit, int numEdit) {
    int lastEdit = firstEdit + numEdit - 1;
    a = EditSet[firstEdit].New;
    while (a == -1) {
        a = EditSet[firstEdit++].New;
    }
    b = EditSet[lastEdit].New;
    while (b == -1) {
        b = EditSet[lastEdit--].New;
    }
    a++;
    b++;
}

int TEditInstruction::minK(int begin1L, int end1L, int begin1R, int end1R,
            int begin2, int end2, bool isSwap)
{
    int N = end2 - begin2;
    int *d = getLastD(begin1L, end1L, begin2, end2, isSwap);
    int *e = getLastE(begin1R, end1R, begin2, end2, isSwap);
    int k = 0;
    for (int i = 1; i <= N; i++) {
        if (d[i] + e[N - i] < d[k] + e[N - k]) {
            k = i;
        }
    }
    delete[] d;
    delete[] e;
    return k;
}

void _break() {}

TEditSet
TEditInstruction::getEditSet(int begin1, int end1, int begin2, int end2) {
    _break();
    int M = end1 - begin1;
    int N = end2 - begin2;
    if (M <= 1 || N <= 1) {
        return getEditSet1(begin1, end1, begin2, end2);
    }
    int begin1L = begin1, end1L = begin1 + M / 2;
    int begin1R = end1L, end1R = end1;
    int begin2L = begin2, end2L = begin2 + N / 2;
    int begin2R = end2L, end2R = end2;
    int k = 0;
    if (N < M) {
        k = minK(begin1L, end1L, begin1R, end1R, begin2, end2, false);
        begin2L = begin2;
        end2L = begin2 + k;
        begin2R = end2L;
        end2R = end2;
    } else {
        k = minK(begin2L, end2L, begin2R, end2R, begin1, end1, true);
        begin1L = begin1;
        end1L = begin1 + k;
        begin1R = end1L;
        end1R = end1;
    }
    TEditSet set1 = getEditSet(begin1L, end1L, begin2L, end2L);
    TEditSet set2 = getEditSet(begin1R, end1R, begin2R, end2R);
    return set1 + set2;
}

TEditSet
TEditInstruction::getEditSet1(int begin1, int end1, int begin2, int end2) {
    int M = end1 - begin1;
    int N = end2 - begin2;
    if (M == 0 && N == 0) {
        return TEditSet(0, NULL);
    } else if (M == 0) {
        return OnlyInsert(begin2, N);
    } else if (N == 0) {
        return OnlyDelete(begin1, M);
    } else if (M == 1) {
        return OneChar(begin1, begin2, N);
    } else if (N == 1) {
        return LastOneChar(begin2, begin1, M);
    }
    return TEditSet(0, NULL);
}

TEditSet TEditInstruction::OnlyInsert(int begin, int n) {
    TEditItem* set = new TEditItem[n];
    for (int i = 0; i < n; i++) {
        set[i] = TEditItem(INSERT, NOPE, begin + i);
    }
    return TEditSet(n, set);
}

TEditSet TEditInstruction::OnlyDelete(int begin, int n) {
    TEditItem* set = new TEditItem[n];
    for (int i = 0; i < n; i++) {
        set[i] = TEditItem(DELETE, begin + i, NOPE);
    }
    return TEditSet(n, set);
}

TEditSet TEditInstruction::OneChar(int c, int begin, int n) {
    bool used = false;
    TEditItem* set = new TEditItem[n];
    for (int i = begin; i < begin + n; i++) {
        if (!used && S1[c] == S2[i]) {
            set[i - begin] = TEditItem(MATCH, c, i);
            used = true;
        } else {
            if (i == begin + n - 1 && !used) {
                set[i - begin] = TEditItem(REPLACE, c, i);
            } else {
                set[i - begin] = TEditItem(INSERT, NOPE, i);
            }
        }
    }
    return TEditSet(n, set);
}

TEditSet TEditInstruction::LastOneChar(int c, int begin, int n) {
    bool used = false;
    TEditItem* set = new TEditItem [n];
    for (int i = begin; i < begin + n; i++) {
        if (!used && S2[c] == S1[i]) {
            set[i - begin] = TEditItem(MATCH, i, c);
            used = true;
        } else {
            if (i == begin + n - 1 && !used) {
                set[i - begin] = TEditItem(REPLACE, i, c);
            } else {
                set[i - begin] = TEditItem(DELETE, i, NOPE);
            }
        }
    }
    return TEditSet(n, set);
}

int* TEditInstruction::getLastD(int begin1, int end1, int begin2, int end2,
                                bool isSwap)
{
    int M = end1 - begin1;
    int N = end2 - begin2;
    int* prevD = new int [N + 1];
    int* newD = new int [N + 1];
    for (int j = 0; j <= N; j++) {
        newD[j] = j;
    }
    for (int i = 1; i <= M; i++) {
        swap(prevD, newD);
        newD[0] = i;
        for (int j = 1; j <= N; j++) {
            int m = 0;
            if (!isSwap) {
                m = (S1[begin1 + i - 1] != S2[begin2 + j - 1]) ? 1 : 0;
            } else {
                m = (S2[begin1 + i - 1] != S1[begin2 + j - 1]) ? 1 : 0;
            }
            newD[j] = min(prevD[j] + 1, newD[j - 1] + 1, prevD[j - 1] + m);
        }
    }
    delete[] prevD;
    return newD;
}

int* TEditInstruction::getLastE(int begin1, int end1, int begin2, int end2,
                                bool isSwap)
{
    int M = end1 - begin1;
    int N = end2 - begin2;
    int* prevD = new int [N + 1];
    int* newD = new int [N + 1];
    for (int j = 0; j <= N; j++) {
        newD[j] = j;
    }
    for (int i = 1; i <= M; i++) {
        swap(prevD, newD);
        newD[0] = i;
        for (int j = 1; j <= N; j++) {
            int m = 0;
            if (!isSwap) {
                m = (S1[end1 - i] != S2[end2 - j]) ? 1 : 0;
            } else {
                m = (S2[end1 - i] != S1[end2 - j]) ? 1 : 0;
            }
            newD[j] = min(prevD[j] + 1, newD[j - 1] + 1, prevD[j - 1] + m);
        }
    }
    delete[] prevD;
    return newD;
}

int TEditInstruction::min(int a, int b, int c) {
    if (a > b) {
        a = b;
    }
    if (a > c) {
        a = c;
    }
    return a;
}


