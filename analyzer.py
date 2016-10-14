from decimal import *
from misc import *

class Point:
    def __init__(self):
        self.x = 0; self.y = 0; self.time = 0

    def set(self, x, y, time):
        self.x = x; self.y = y; self.time = time

    def __repr__(self):
        return "x: {}\ty: {}\ttime: {}".format(self.x, self.y, self.time)

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

NODEQTT = None
MAX_NUMBER_WAYPOINTS = 50 #Couldn't find why 50, but doesn't seem to change
TIME_SLOT = 900

#Alloc dynamic structures
def initialize(n): #node quantity
    global ini_x, ini_y, trace, NODEQTT
    ini_x = [None] * n
    ini_y = [None] * n
    trace = [{}] * n
    NODEQTT = n

def read_trace(tracepath):
    global trace, ini_x, ini_y
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
        #print ("ACTION: " + str(action))
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
                #print("x0: " + str(ini_x[id]))

            elif axis == "Y_":
                ini_y[id] = getNextDec(tracefile) #y0 = node's position on y axis
                #trace[id][t].y = y0
                #print("y0: " + str(ini_y[id]))
        # $ns_ at 30.000000234323 "$node_(1) setdest 534.67642310 435.43899348 43.367834743"
	    #  or $ns_ at 344.442322520850 "$god_ set-dist 0 1 7215"
        elif action == 'ns':
            tracefile.read(4)
            time = getNextWord(tracefile)
            action = tracefile.read(6)
            if action == "\"$node":
                id = getNextInt(tracefile)
                getNextWord(tracefile)
                x = getNextDec(tracefile) #prox parada, que o nó atingirá após t segundos, não necessáriamente próximo x
                y = getNextDec(tracefile)
                speed = getNextDec(tracefile)
                trace[id][time] = Data()
                trace[id][time].set(id, x, y, time, speed)

                if time == '0.0':
                    trace[id][time].x = ini_x[id]
                    trace[id][time].y = ini_y[id]
                    trace[id][time].angle = get_angle(trace[id][time].x, trace[id][time].y, trace[id][time].nx, trace[id][time].ny)
                    trace[id][time].timetodestiny = dist(trace[id][time].x, trace[id][time].y, trace[id][time].nx, trace[id][time].ny) / trace[id][time].speed;
            tracefile.readline() #Reads \n at the end of the line
        else: #$god_ set-dist 0 1 1677215 ?? Why?
            tracefile.readline()
    tracefile.close()

def  set_data():
    ttd = 0 #??
    #waypoints: a bidimensional array that stores the visiting points of a node
    waypoints = [[Point() for x in range(NODEQTT)] for y in range(NODEQTT)]
    for id in range(NODEQTT):
        waypoints[id][0].set(trace[id]['0.0'].x, trace[id]['0.0'].y, 0) #Stores time as string, so 0 == 0.0
        t = 0
        while node_is_stationary(id, t) and t < TIME_SLOT:
            update_stationary_state(id, t)
            t += 1

        pindex = 0 #pause time array index
        aindex = 0 #velocity angle array index
        sindex = 0 #speed array index
        tripindex = 0 #trip length array index
        windex = 1 #waypoint index

        #t > 0 implies that node has been paused for t seconds
        #if t > 0:
        # nodePauseTimes[id][pindex++] = t; T is not a integer. Using string/Decimal to store better precision


    #print(waypoints)


def print_trace():
    for i in range(0, NODEQTT):
        for t in trace[i]:
            print(trace[i][t])
