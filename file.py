import string
from decimal import *

class File:
    def __init__(self, path):
        self.f = open(path, 'r')

    def read(self, n):
        return self.f.read(n)

    def readline(self):
        return self.f.readline()

    def close(self):
        self.f.close()

    #Returns nextInt in the file
    def getNextInt(self):
        positive = True; num = self.f.read(1)

        while not num.isnumeric(): #Searches the first digit
            if num == '-':
                positive = False
            else:
                positive = True
            num = self.f.read(1)

        tmp = self.f.read(1)
        while tmp.isnumeric(): #Reads the number
            num += tmp
            tmp = self.f.read(1)

        num = int(num)
        if positive == False: #Correct the sinal (if any)
            num *= -1

        return int(num)

    def getNextWord(self):
        word = self.f.read(1)
        while word in string.whitespace: #Ignores white spaces (tab, space, \n) and get the first char
            word = self.f.read(1)
        tmp = self.f.read(1)
        while tmp not in string.whitespace: #Reads the word
            word += tmp
            tmp = self.f.read(1)
        return word

    def getNextDec(self): #decimal (return str)
        positive = True; num = self.f.read(1)
        while not num.isnumeric(): #Gets first digit
            if num == '-':
                positive = False
            else:
                positive = True
            num = self.f.read(1)

        tmp = self.f.read(1); dot = False
        while tmp.isnumeric() or (dot == False and tmp == '.'): #Reads the number
            if (tmp == '.'): dot = True
            num += tmp
            tmp = self.f.read(1)
        num = Decimal(num)

        if positive == False: #Correct the sinal
            num *= -1

        return num
