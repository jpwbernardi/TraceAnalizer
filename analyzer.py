import collections
from decimal import *
from misc import *
from file import File
from collections import OrderedDict


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
        return "id: {}\ttime: {}\tx: {}\ty: {}\tspeed: {}".format(self.id, self.time, self.nx, self.ny, self.speed)


ini_x = None #List that stores node's original x position
ini_y = None #List that stores node's original y position
trace = None #List that stores trace data
nodePauseTimes = None #Keep the values of pause duration of all nodes

NODEQTT = None
NODE_PAUSE_TIME_SLOT = 50 #Couldn't find why 50, but doesn't seem to change
MAX_NUMBER_WAYPOINTS = 50 #Couldn't find why 50, but doesn't seem to change
TIME_SLOT = 900 #In the trace file, the time range is 0 and 900

#Alloc dynamic structures
def initialize(n): #node quantity
    global ini_x, ini_y, trace, NODEQTT
    ini_x = [None] * n
    ini_y = [None] * n
    trace = [None] * n
    NODEQTT = n
    nodePauseTimes = [[None for x in range(NODEQTT)] for y in range(NODE_PAUSE_TIME_SLOT)]

def read_trace(tracepath):
    global trace, ini_x, ini_y
    tracefile = File(tracepath)
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
            id = tracefile.getNextInt()
            #print (num)
            tracefile.getNextWord();
            axis = tracefile.getNextWord()
            if axis == "X_":
                ini_x[id] = tracefile.getNextDec() #x0 = node's position on x axis
                #trace[id][t].x = x0
                #print("x0: " + str(ini_x[id]))

            elif axis == "Y_":
                ini_y[id] = tracefile.getNextDec() #y0 = node's position on y axis
                #trace[id][t].y = y0
                #print("y0: " + str(ini_y[id]))
        # $ns_ at 30.000000234323 "$node_(1) setdest 534.67642310 435.43899348 43.367834743"
	    #  or $ns_ at 344.442322520850 "$god_ set-dist 0 1 7215"
        elif action == 'ns':
            tracefile.read(4)
            time = tracefile.getNextDec() #Comparation between string is easier :P
            time = int(time)
            action = tracefile.read(6)
            if action == "\"$node":
                id = tracefile.getNextInt()
                tracefile.getNextWord()
                x = tracefile.getNextDec() #prox parada, que o nó atingirá após t segundos, não necessáriamente próximo x
                y = tracefile.getNextDec()
                speed = tracefile.getNextDec()
                if trace[id] == None:
                    trace[id] = OrderedDict()
                trace[id][time] = Data()
                trace[id][time].set(id, x, y, time, speed)

                if time == 0:
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
        waypoints[id][0].set(trace[id][0].x, trace[id][0].y, 0)

        while node_is_stationary(id, t) and t < TIME_SLOT:
            update_stationary_state(id, t)
            t += 1


        pindex = 0 #pause time array index
        aindex = 0 #velocity angle array index
        sindex = 0 #speed array index
        tripindex = 0 #trip length array index
        windex = 1 #waypoint index
        paux = 0

        #t > 0 implies that node has been paused for t seconds
        #if t > 0:
            #nodePauseTimes[id][pindex++] = t;

        # while (t < TIME_SLOT):
        #     ttd = start_new_movement(id, t);
        # if (aindex == 0):
        #     nodeAngles[id][aindex++] = getPositiveAngle(trace[id][t].angle);
        # elif (t > 0 and not equal_or_almost_equal(trace[id][t].angle, trace[id][t - 1].angle))

    #print(waypoints)


def print_trace():
    # for i, data in enumerate(data for data in trace):
    #     print ("OOOOI: " + str(i) + "\n" + str(data) + "\n\n")
    for i in range(0, NODEQTT):
        #print ("ID CERTO: " + str(i))
        for t in trace[i]:
            print(trace[i][t])
