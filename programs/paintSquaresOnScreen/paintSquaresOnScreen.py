#!/usr/bin/env python

'''
paintSquaresOnScreen.py program

Authors: Raul Fernandez-Fernandez; Juan G. Victores

Universidad Carlos III de Madrid 
'''

## @ingroup xgnitive_applications
 # \defgroup paintSquaresOnScreen
 #
 # @brief Application used to paint squares on screen, receives a [X,Y] coordinate via YARP.
 #
 # @section paintSquaresOnScreen_legal Legal
 #
 # Copyright: 2016 (C) Universidad Carlos III de Madrid
 #
 # Author: raulfdzbis, jgvictores
 #
 # CopyPolicy: This program is free software; you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License 3.0 or later
 #
 # <hr>
 #
 # This file can be edited at paintSquaresOnScreen
 
# Import a library of functions called 'pygame'
import pygame
import yarp
import numpy as np

 
# Initialize the game engine
pygame.init()

# Initialize YARP
yarp.Network.init()

# Checking
if yarp.Network.checkNetwork() != True:  # let's see if there was actually a reachable YARP network
    print '[error] Please try running yarp server'  # tell the user to start one with 'yarp server' if there isn't any
    quit()

# Define some colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
CYAN = (0, 255, 255)
YELLOW = (255, 255, 0)
MAGENTA = (255, 0, 104)




# Set the height and width of the screen
size = (960, 540)
#size = (0, 0)
screen = pygame.display.set_mode(size)
#pygame.FULLSCREEN
 
pygame.display.set_caption("Teo painting Demo")

# Loop until the user clicks the close button.
done = False
clock = pygame.time.Clock()
screenSize=pygame.display.Info()
print(screenSize.current_w, screenSize.current_h)

#Since using 2 screens are presented we can not auto detect the size of screen
#screenW=1920;
#screenH=1080;
screenW=960;
screenH=540;

# Number of screens
#scn=2

#Where is the kinect 1 above, 0 below
kinect=0

# Number of rectangles
hrect=16 #Horizontal
vrect=16 #Vertical

# Clear the screen and set the screen background
screen.fill(WHITE)

# YARP Callback
class DataProcessor(yarp.PortReader): 

    def myInit(self):
        #self.myMem = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
        self.myMem = np.zeros(hrect*vrect)
        self.brushColour=1 #By default blue

    def read(self,connection):
        print("in DataProcessor.read")
        if not(connection.isValid()):
            print("Connection shutting down")
            return False
        bin = yarp.Bottle()
        bout = yarp.Bottle()
        print("Trying to read from connection")
        ok = bin.read(connection)
        if not(ok):
            print("Failed to read input")
            return False
        #self.received=bin;

        #Read the rectangle from bottle
        self.x=bin.get(0).asInt()
        self.y=bin.get(1).asInt()
        print("what i have received is [",self.x,",",self.y,"]")

        #Paint conditions
        if self.y < vrect: #We are talking about painting voxels if this is true
            if self.brushColour == 1:
                self.paintBlue()

            elif self.brushColour == 2:
                self.paintYellow()

            elif self.brushColour == 3:
                self.paintMagenta()
        else: #We are talking about utilityVoxels
            self.utilityVoxels()


    def paintBlue(self):

        #kinect below
        if kinect==0:
            pygame.draw.rect(screen,BLUE,[self.x*screenW/hrect,(vrect-self.y)*screenH/vrect,screenW/hrect,screenH/vrect],0)
        elif kinect==1:
            pygame.draw.rect(screen, BLUE, [self.x*screenW/(hrect), self.y*screenH/vrect, screenW/(hrect), screenH/vrect], 0)

        place =self.x*hrect+self.y*vrect #Number of pixel.
        print 'place', place
        print 'self.myMem', self.myMem
        print 'size', len(self.myMem)
        self.myMem[place] = 1

        #print("Received [%s]" % bin.toString())
        #bout.addString("Received:")
        #bout.append(bin)
        print("Sending [%s]" % bout.toString())
        writer = connection.getWriter()
        if writer == None:
            print("No one to reply to")
            return True
        return bout.write(writer)

    def paintYellow(self):

        #kinect below
        if kinect==0:
            pygame.draw.rect(screen,YELLOW,[self.x*screenW/hrect,(vrect-self.y)*screenH/vrect,screenW/hrect,screenH/vrect],0)
        elif kinect==1:
            pygame.draw.rect(screen,YELLOW, [self.x*screenW/(hrect), self.y*screenH/vrect, screenW/(hrect), screenH/vrect], 0)

        place =self.x*hrect+self.y*vrect #Number of pixel.
        print 'place', place
        print 'self.myMem', self.myMem
        print 'size', len(self.myMem)
        self.myMem[place] = 2

        #print("Received [%s]" % bin.toString())
        #bout.addString("Received:")
        #bout.append(bin)
        print("Sending [%s]" % bout.toString())
        writer = connection.getWriter()
        if writer == None:
            print("No one to reply to")
            return True
        return bout.write(writer)

    def paintMagenta(self):

        #kinect below
        if kinect==0:
            pygame.draw.rect(screen,MAGENTA,[self.x*screenW/hrect,(vrect-self.y)*screenH/vrect,screenW/hrect,screenH/vrect],0)
        elif kinect==1:
            pygame.draw.rect(screen,MAGENTA, [self.x*screenW/(hrect), self.y*screenH/vrect, screenW/(hrect), screenH/vrect], 0)

        place =self.x*hrect+self.y*vrect #Number of pixel.
        print 'place', place
        print 'self.myMem', self.myMem
        print 'size', len(self.myMem)
        self.myMem[place] = 3

        #print("Received [%s]" % bin.toString())
        #bout.addString("Received:")
        #bout.append(bin)
        print("Sending [%s]" % bout.toString())
        writer = connection.getWriter()
        if writer == None:
            print("No one to reply to")
            return True
        return bout.write(writer)

    def utilityVoxels(self):
        if self.x == 0:
            self.brushColour = 1

        elif self.x == 1:
            self.brushColour = 2

        elif self.x == 2:
            self.brushColour = 3

        elif self.x == 3:
            pass #right now doing nothing




p = yarp.Port()
r = DataProcessor()
r.myInit()
p.setReader(r)
pout = yarp.Port()

p.open("/voxelOccupancyDetection/state:i")
pout.open("/paintSquaresOnScreen/state:o")
# Loop as long as done == False
while not done: 
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
       
    # Go ahead and update the screen with what we've drawn.
    # This MUST happen after all the other drawing commands.
    pygame.display.flip() 

    #Calculate percentage
    percent = 0
    for elem in r.myMem:
        if elem!=0: #For each element not painted.
            percent += 1
    percent = 100.0 * percent / float(len(r.myMem))
    out = yarp.Bottle()
    out.addDouble(percent)
    pout.write(out)

    # This limits the while loop to a max of 60 times per second.
    # Leave this out and we will use all CPU we can.
    clock.tick(60)
 
# Be IDLE friendly
p.close()
pout.close()
yarp.Network.fini() # disconnect from the YARP network
pygame.quit()
