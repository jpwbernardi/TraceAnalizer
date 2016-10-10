import string
from decimal import *

def getNextInt(f):
    num = f.read(1)
    while not num.isnumeric():
        num = f.read(1)
    tmp = f.read(1)
    while tmp.isnumeric():
        num += tmp
        tmp = f.read(1)
    return int(num)

def getNextWord(f):
    word = f.read(1)
    while word in string.whitespace:
        word = f.read(1)
    tmp = f.read(1)
    while tmp not in string.whitespace:
        word += tmp
        tmp = f.read(1)
    return word

def getNextDec(f): #decimal (return str)
    num = f.read(1)
    while not num.isnumeric():
        num = f.read(1)
    tmp = f.read(1)
    dot = False
    while tmp.isnumeric() or (dot == False and tmp == '.'):
        if (tmp == '.'): dot = True
        num += tmp
        tmp = f.read(1)
    return num
