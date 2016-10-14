#!/usr/bin/python3.5
#./TraceAnalyzer <NS2-TRACE-FILE> <NODEQTT> <NODE-PAUSE-TIME> <RANGE> <WIDTH> <LENGTH>
import sys
from analyzer import *

def main(tracepath, n, nodepause, r, width, length):
    initialize(n)
    #print(ini_x)
    print("File: {}\nNumber of nodes: {}\nNode pause time: {}\nRange: {}\nWidth: {} \t Length: {}".format(tracepath, n, nodepause, r, width, length));
    read_trace(tracepath)
    set_data()
    #print_trace()

if __name__ == "__main__":
    if len(sys.argv) == 7:
        main(sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6]);
    else: print("Wrong number of parameters")
