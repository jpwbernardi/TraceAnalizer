#!/usr/bin/python3.5

#./TraceAnalyzer <NS2-TRACE-FILE> <NODEQTT> <NODE-PAUSE-TIME> <RANGE> <WIDTH> <LENGTH>

import sys
from decimal import *
from misc import *


class Data:
    def __init__(self):
        self.id = 0
        self.start = 0.0; self.time = 0.0
        self.x = 0.0; self.y = 0.0
        self.nx = 0.0; self.ny = 0.0
        self.angle = 0.0; self.speed = 0.0
        self.timetodestiny = 0

    def set(self, id, x, y, time, speed):
        self.id = id
        self.nx = x; self.time = time
        self.ny = y; self.speed = speed

    def __repr__(self):
        return "id: {}\ttime: {}\tx: {}\ty: {}\tspeed: {}".format(self.id, self.time, self.x, self.y, self.speed)


ini_x = None #List that stores node's original x position
ini_y = None #List that stores node's original y position
trace = None #List that stores trace data


def main(tracepath, n, nodepause, r, width, length):
    initialize(n)
    #print(ini_x)
    print("File: {}\nNumber of nodes: {}\nNode pause time: {}\nRange: {}\nWidth: {} \t Length: {}".format(tracepath, n, nodepause, r, width, length));
    read_trace(tracepath)
    #print(ini_x)
    #print(trace)
    for i in range(0, n):
        for t in trace[i]:
            print(trace[i][t])

def initialize(n): #node quantity
    global ini_x, ini_y, trace
    ini_x = [None] * n
    ini_y = [None] * n
    trace = [{}] * n

def read_trace(tracepath):
    global trace
    tracefile = open(tracepath, 'r')
    while True:
        char = tracefile.read(1) #Reads 1 character
        if char == '': #EOF
            break
        if char == '#': #If the comment is '#', skip it
            tracefile.readline()
            continue
        #char == '$'
        action = tracefile.read(2)
        if action == 'no':
            # $node_(i) set X_ 26.523097872900
            tracefile.read(4) #de_(
            id = getNextInt(tracefile)
            #print (num)
            getNextWord(tracefile);
            axis = getNextWord(tracefile)
            if axis == "X_":
                ini_x[id] = getNextDec(tracefile) #x0 = node's position on x axis
                #trace[id][t].x = x0
                print("x0: " + str(ini_x[id]))
            elif axis == "Y_":
                ini_y[id] = getNextDec(tracefile) #y0 = node's position on y axis
                #trace[id][t].y = y0
                print("y0: " + str(ini_y[id]))
            else: getNextDec(tracefile) #Will it ever happen?
        # $god_ set-dist 0 1 1677215
        elif action == 'go':
            tracefile.readline()
            #?? Really?
        # $ns_ at 30.000000234323 "$node_(1) setdest 534.67642310 435.43899348 43.367834743"
	     #  or $ns_ at 344.442322520850 "$god_ set-dist 0 1 7215"
        elif action == 'ns':
            #_ at
            tracefile.read(4)
            time = getNextWord(tracefile)
            action = tracefile.read(6)
            print("!!!!!!" + action)
            if action == "\"$node":
                id = getNextInt(tracefile)
                getNextWord(tracefile)
                x = getNextDec(tracefile)
                y = getNextDec(tracefile)
                speed = getNextDec(tracefile)
                trace[id][time] = Data()
                trace[id][time].set(id, x, y, time, speed)

                if time == '0.0':
                    trace[id][time].x = ini_x[id]
                    

            #tracefile.readline()

if __name__ == "__main__":
    if len(sys.argv) == 7:
        main(sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6]);
    else: print("Wrong number of parameters")
