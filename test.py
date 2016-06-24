from random import *
from subprocess import *
import os

maxTest = 2000
maxStrCnt = 5
maxStrLen = 10

def genTest():
    tst1 = open(".tst1", "w")
    tst2 = open(".tst2", "w")
    res = open(".res", "w")
    res1 = open(".res1", "w")
    n = randint(0, maxStrCnt)
    for i in range(n): 
        s = ''
        m = randint(0, maxStrLen)
        for i in range(m):
            s = s + choice(abc)
        case = randint(0, 2)
        if (case == 0):
            tst1.write(s + "\n")
            tst2.write(s + "\n")
            res.write(s + "\n")
            res1.write(s + "\n")
        elif (case == 1):
            tst1.write(s + "\n")
            res.write(s + "\n")
            res1.write(s + "\n")
        else:
            tst2.write(s + "\n")
    
    tst1.close()
    tst2.close()
    res.close()
    res1.close()

def genFlags():
    flags = [".tst1", ".tst2"]
    if (randint(0, 1) == 1):
        flags += ["-i"]
    if (randint(0, 1) == 1):
        flags += ["-E"]
    if (randint(0, 1) == 1):
        flags += ["-b"]
    if (randint(0, 1) == 1):
        flags += ["-w"]
    if (randint(0, 1) == 1):
        flags += ['-B'];
    if (randint(0, 1) == 1):
        flags += ["--normal"]
    if (randint(0, 1) == 1):
        flags += ["-t"]
    return flags

#gen alfabeta    
abc = [chr(i) for i in range(ord('a'), ord('z'))]
abc += [chr(i) for i in range(ord('A'), ord('Z'))]
abc += [str(i) for i in range(10)]
abc += [' ', '\t']

seed()
for t in range(maxTest):
    genTest()
    flags = genFlags()
    print(flags)
    p = open(".p", "w")
    p1 = open(".p1", "w")
    status = call(["./diff"] + flags, stdout = p)
    if (status > 1):
        print("./diff: bad status")
        quit()
    status = call(["patch",  ".res", ".p"], stdout = DEVNULL)
    if (status != 0):
        print("./diff: patch error")
        quit()

    status = call(["diff"] + flags, stdout = p1)
    if (status > 1):
        print("diff: bad status")
        quit()
    status = call(["patch",  ".res1", ".p1"], stdout = DEVNULL)
    if (status != 0):
        print("diff: patch error")
        quit()

    status = call(["cmp",  ".res", ".res1"], stdout = DEVNULL)
    if (status != 0):
        print("cmp != 0")
        quit()
    p.close()
    p1.close()

os.remove(".tst1")
os.remove(".tst2")
os.remove(".res")
os.remove(".p")
os.remove(".p1")
os.remove(".res1")
print("./diff:    OK");
