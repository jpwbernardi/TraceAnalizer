from decimal import *
from math import sqrt, atan, floor

PI = 3.14159265359

#Calculate the distance between two nodes
def dist(a0, b0, a1, b1):
    dx = a0 - a1;
    dy = b0 - b1;
    return Decimal(sqrt(dx * dx + dy * dy))

#Calculate the angle between two nodes
#Not sure if does what should do, same code as old
def get_angle(a0, b0, a1, b1):
    angle = 0
    dx = a0 - a1
    dy = b0 - b1
    # print ("dx:  " + str(dx))
    # print ("dy:  " + str(dy))
    # print ("----------")
    if dy == 0 and dx == 0:
        angle = 0
    elif dx == 0 and dy > 0:
        angle = PI / 2
    elif dx == 0 and dy < 0:
        angle = -PI / 2
    elif dy >= 0 and dx > 0:
        angle = atan(dy / dx)
    elif dy <= 0 and dx > 0:
        angle = atan(dy / dx)
    elif dy >= 0 and dx < 0:
        angle = atan(dy / dx) + PI
    elif dy <= 0 and dx < 0:
        angle = atan(dy / dx) - PI
    angle = floor(angle * 100 +  0.5) / 100; #to avoid returning values like 1.72E-12 Python has this problem? Yep
    return angle;
