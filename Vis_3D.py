from vpython import *
from time import *
import numpy as np
import math

import serial
import time

STM32L4 = serial.Serial('com8', 115200)
time.sleep(1)

scene_3d = canvas(background=color.white, width=1200, height=300)
scene_3d.axis = vector(0,0.8,-1)

x = 0
start_time = time.time()

# Two coordinate systems, Body-Fixed Frame 1, Inertial Frame 2
axisX1 = arrow(length=0.5, pos=vector(0, 0, -0.5), axis=vector(1, 0, 0), color=color.red, shaftwidth=0.05)
axisY1 = arrow(length=0.5, pos=vector(0, 0,  -0.5), axis=vector(0, 1, 0), color=color.green, shaftwidth=0.05)
axisZ1 = arrow(length=0.5, pos=vector(0, 0,  -0.5), axis=vector(0, 0, 1), color=color.blue, shaftwidth=0.05)

axisX2 = arrow(length=1.5, pos=vector(0, 0, -0.5), axis=vector(1, 0, 0), color=color.cyan, shaftwidth=0.05)
axisY2 = arrow(length=1.5, pos=vector(0, 0, -0.5), axis=vector(0.5, 1, 0), color=color.magenta, shaftwidth=0.05)
axisZ2 = arrow(length=1.5, opacity = 0.5,pos=vector(0, 0, -0.5), axis=vector(0, 0, 1), color=color.yellow, shaftwidth=0.05)


# Pseudo drone model 
corpus = box(pos=vector(0, 0, 0), size=vector(1, 0.5, 0.1), color=color.black)

prostopadloscian1 = box(pos=corpus.pos, size=vector(2, 0.1, 0.1),
                        axis=vector(1, 1, 0), color=color.black)
prostopadloscian2 = box(pos=corpus.pos, size=vector(2, 0.1, 0.1),
                        axis=vector(1, -1, 0), color=color.black)
walec1 = cylinder(pos=prostopadloscian1.pos + vector(0.7, 0.7, 0), axis=vector(0, 0, 0.1),
                  radius=0.2, color=color.black)
walec2 = cylinder(pos=prostopadloscian2.pos + vector(-0.7, -0.7, 0), axis=vector(0, 0, 0.1),
                  radius=0.2, color=color.black)
walec3 = cylinder(pos=prostopadloscian1.pos + vector(-0.7, 0.7, 0), axis=vector(0, 0, 0.1),
                  radius=0.2, color=color.black)
walec4 = cylinder(pos=prostopadloscian2.pos + vector(0.7, -0.7, 0), axis=vector(0, 0, 0.1),
                  radius=0.2, color=color.black)

Dron = compound([corpus, prostopadloscian1, prostopadloscian2, walec1, walec2, walec3, walec4])
Dron.pos = vector(0, 0, -0.5)
Dron.opacity = 0.5

# labels 
label(pos=axisX2.axis + vector(0, 0, 0), text='X', height=10, box=False)
label(pos=axisY2.axis + vector(0, 0.1, 0), text='Y', height=10, box=False)
label(pos=axisZ2.axis + vector(0, 0, 0.1), text='Z', height=10, box=False)

# text labels 
roll_pitch_label = label(pos=vector(3.5, -1, 0) , height=15, box=False)


graph_height = 200
graph_spacing = 20
graph_width = 295

# Roll
graph1 = graph(title='Roll (deg)', align='left', width=graph_width, height=graph_height)
plot1 = gcurve(graph=graph1, color=color.red)

# Pitch
graph2 = graph(title='Pitch (deg)', align='left', width=graph_width, height=graph_height)
plot2 = gcurve(graph=graph2, color=color.blue)

# Yaw
graph3 = graph(title='Yaw (deg)', align='left', width=graph_width, height=graph_height)
plot3 = gcurve(graph=graph3, color=color.green)

#  Altitude
graph4 = graph(title='Altitude (m)', align='left', width=graph_width, height=graph_height)
plot4 = gcurve(graph=graph4, color=color.black)




while True:
    while STM32L4.inWaiting() == 0:
        pass

    DataPacket  = STM32L4.readline()
    DataPacket  = str(DataPacket, 'utf-8')
    splitPacket = DataPacket.split(',')

    Roll              = float(splitPacket[0]) 
    Pitch             = float(splitPacket[1]) * (-1)
    Yaw               = float(splitPacket[2])
    Altitude          = float(splitPacket[3]) 
    IS_MAG_ON         = float(splitPacket[4])
    DataAnlizeOnOff   = float(splitPacket[5])
    


    # interial frame ZYX
    axisX1.axis = vector(
        cos(Pitch) * cos(Yaw),
        cos(Pitch) * sin(Yaw),
        -sin(Pitch)
    )
    axisX1.length = 1

    axisY1.axis = vector(
        cos(Yaw)*sin(Roll)*sin(Pitch)-cos(Roll)*sin(Yaw),
        cos(Roll)*cos(Yaw) + sin(Roll)*sin(Yaw)*sin(Pitch),
        cos(Pitch)*sin(Roll)
    
    )
    axisZ1.length = 1

    axisZ1.axis = vector(
        sin(Roll)*sin(Yaw)+cos(Roll)*cos(Yaw)*sin(Pitch),
        cos(Roll)*sin(Yaw)*sin(Pitch)-cos(Yaw)*sin(Roll),
        cos(Roll)*cos(Pitch)
         
    )
    
            
    Dron.axis  = axisX1.axis
    Dron.up    = axisY1.axis

   

    elapsed_time = time.time() - start_time
    x = elapsed_time

    axisX1.pos = vector(0,0,Altitude)
    axisY1.pos = vector(0,0,Altitude)
    axisZ1.pos = vector(0,0,Altitude)
    Dron.pos   = vector(0,0,Altitude)
    
    
    rate(100)
    
    if DataAnlizeOnOff == 1:
        plot4.plot(x, Altitude)
        plot1.plot(x, Roll * (180/np.pi))
        plot2.plot(x, Pitch * (180/np.pi))
        plot3.plot(x, Yaw * (180/np.pi))
        plot4.plot(x, Altitude)
        roll_pitch_label.text = f'Roll: {Roll * (180/np.pi):.2f}, Pitch: {Pitch * (180/np.pi):.2f}, Yaw: {Yaw * (180/np.pi):.2f}, Altitude: {Altitude*100:.2f} cm, \nData Analize on: {DataAnlizeOnOff}, Mag on: {IS_MAG_ON}'
