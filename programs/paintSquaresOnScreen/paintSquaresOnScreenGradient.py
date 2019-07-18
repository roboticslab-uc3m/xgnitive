#!/usr/bin/env python

'''
paintSquaresOnScreenGradient.py program

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
import time

 
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
#Setup1
size = (960, 540)
screenW=960;
screenH=540;

#Setup2
#size = (1512, 1080)
#screenW=1512;
#screenH=1080;


screen = pygame.display.set_mode(size)
#pygame.FULLSCREEN
 
pygame.display.set_caption("Teo painting Demo")

# Loop until the user clicks the close button.
done = False
clock = pygame.time.Clock()
screenSize=pygame.display.Info()
print(screenSize.current_w, screenSize.current_h)


# Number of screens
#scn=2

#Where is the kinect 1 above, 0 below
kinect=1

# Number of rectangles
hrect=16 #Horizontal
vrect=16 #Vertical

# Clear the screen and set the screen background
screen.fill(BLUE)

# YARP Callback
class DataProcessor(yarp.PortReader): 

    def myInit(self):
        #self.myMem = [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
        self.myMem = np.zeros(hrect*vrect)
        self.strokeMem = np.zeros(hrect * vrect)
        self.brushColour=1 #By default blue
        self.xold=0
        self.yold=0
        self.oldColour=1
        self.memplace=-1
        self.numMaxStrokes=0

	
	return True

    def read(self,connection):

        #time.sleep(0.1)  # delays for 5 seconds. You can Also Use Float Value
        print("in DataProcessor.read")
        if not(connection.isValid()):
            print("Connection shutting down")
            return False
        bin = yarp.Bottle()
        #bout = yarp.Bottle()
        print("Trying to read from connection")
        ok = bin.read(connection)
        if not(ok):
            print("Failed to read input")
            return False

        #self.received=bin;

        #Sending procedure
        #bout.append(bin)
        #print("Sending [%s]" % bout.toString())
        #writer = connection.getWriter()
        #if writer == None:
        #    print("No one to reply to")
        #    return True
        #return bout.write(writer)

        if bin.size()==0:
            print("Empty bottle, discarded")

        else:
            #Read the rectangle from bottle
            self.x=bin.get(0).asInt()
            self.y=bin.get(1).asInt()
            print("what i have received is [",self.x,self.y,"]")

            #Paint conditions
            if self.y < vrect: #We are talking about painting voxels if this is true
                if self.brushColour == 1:
                    print("Painting now")
                    self.paint()

                elif self.brushColour == 0:
                    print("ERASING")
                    self.paintErase()

            else: #We are talking about utilityVoxels
                print("Utility pixels")
                self.utilityVoxels()

        return True

    def findColour(self, place):
        #print("Starting finding colour")
        inc = self.numMaxStrokes/5.0
        if inc==0:
            inc=1
        #print("El incremento es:", inc)
        #print("El place es:", place)
        #print("El numero de strokes", self.strokeMem[place])

        if self.strokeMem[place] <= 2 * inc:
            colour = (0, 255*(self.strokeMem[place])/(2*inc), 255) #0-1
        elif self.strokeMem[place] <= 3 * inc:
            colour = (0, 255, 255-(self.strokeMem[place]-2*inc)*255/inc)
        elif self.strokeMem[place] <= 4 * inc:
            colour = (255*(self.strokeMem[place])/(4*inc), 255, 0)
        elif self.strokeMem[place] <= 5 * inc:
            colour = (255, 255-(self.strokeMem[place])*255/(5*inc), 0)

        print(colour)

        return colour

    def paint(self):
        place = self.x + self.y * vrect  # Number of pixel.

        if self.memplace != place:
            self.strokeMem[place] += 1
            self.numMaxStrokes = int(np.amax(self.strokeMem)) #Update the maximum number of strokes

            for i in range(hrect):
                for j in range(vrect):
                    aux_place = i + j * vrect  # Number of pixel.
                    colour = self.findColour(aux_place)
                    #kinect below
                    if kinect==0:
                        #print("Painting now for real")
                        pygame.draw.rect(screen,colour,[i*screenW/hrect,(vrect-(j+1))*screenH/vrect,screenW/hrect,screenH/vrect],0)
                    elif kinect==1:
                        #print("Painting now for real")
                        pygame.draw.rect(screen, colour, [i*screenW/(hrect), j*screenH/vrect, screenW/(hrect), screenH/vrect], 0)

        self.drawCursor()
        #print 'place', place
        #print 'self.strokeMem[place]', self.strokeMem[place]
        #print 'size', len(self.myMem)
        self.myMem[place] = 1

        self.memplace=place
	
	return True

    def paintErase(self):
        #print("Start Erase")
        place =self.x+self.y*vrect #Number of pixel.

        #kinect below
        if kinect==0:
            pygame.draw.rect(screen,WHITE,[self.x*screenW/hrect,(vrect-(self.y+1))*screenH/vrect,screenW/hrect,screenH/vrect],0)
            self.drawCursor()
        elif kinect==1:
            pygame.draw.rect(screen,WHITE, [self.x*screenW/(hrect), self.y*screenH/vrect, screenW/(hrect), screenH/vrect], 0)
            self.drawCursor()

        #print 'place', place
        #print 'self.myMem', self.myMem
        #print 'size', len(self.myMem)
        self.myMem[place] = 3
        self.strokeMem[place] = 0

	return True	

    def drawCursor(self):
        #print("Start cursor")
        place = self.xold+self.yold*vrect
        colour= self.findColour(place)

        if kinect==0:
            #Delete old cursor
            if self.oldColour == 0:
                pygame.draw.circle(screen, WHITE, [(self.xold * screenW / hrect) + screenW / (2 * hrect),(vrect - (self.yold + 1)) * screenH / vrect + screenH / (2 * vrect)],10, 0)

            elif self.oldColour == 1:
                pygame.draw.circle(screen, colour, [(self.xold * screenW / hrect) + screenW / (2 * hrect), (vrect - (self.yold + 1)) * screenH / vrect + screenH / (2 * vrect)],10, 0)

            #DrawCursor
            pygame.draw.circle(screen,BLACK,[(self.x*screenW/hrect)+screenW/(2*hrect),(vrect-(self.y+1))*screenH/vrect+screenH/(2*vrect)],10,0)

        elif kinect==1:
            #Delete old cursor
            if self.oldColour == 0:
                pygame.draw.circle(screen, WHITE, [self.xold * screenW / (hrect) + screenW / (2 * hrect), self.yold * screenH / vrect + screenH / (2 * vrect)], 10, 0)

            elif self.oldColour == 1:
                pygame.draw.circle(screen, colour, [self.xold*screenW/(hrect)+screenW/(2*hrect),self.yold*screenH/vrect+screenH/(2*vrect)], 10, 0)

            #DrawCursor
            pygame.draw.circle(screen,BLACK,[self.x*screenW/(hrect)+screenW/(2*hrect),self.y*screenH/vrect+screenH/(2*vrect)],10,0)

	    # Update cursor position and colour
	    self.xold = self.x
	    self.yold = self.y
	    self.oldColour = self.brushColour

	    return True


    def utilityVoxels(self):
        if self.x == 0:
            self.brushColour = 1
            print("PAINTING")

        elif self.x == 1:
            self.brushColour = 1
            print("PAINTING")

        elif self.x == 2:
            self.brushColour = 0 #Rubber
            print("WARNING: USING ERASER")

        elif self.x == 3:
            self.brushColour = 0 #Rubber
            print("WARNING: USING ERASER")
            pass #right now doing nothing

	return True	


p = yarp.Port()
r = DataProcessor()
r.myInit()
p.setReader(r)
pout = yarp.Port()

p.open("/paintSquaresOnScreen/voxelOccupancyDetection/state:i")
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
