"""
 Simple graphics demo
 
 Sample Python/Pygame Programs
 Simpson College Computer Science
 http://programarcadegames.com/
 http://simpson.edu/computer-science/
 
"""
 
# Import a library of functions called 'pygame'
import pygame
import yarp
 
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


# Set the height and width of the screen
size = (0, 0)
screen = pygame.display.set_mode(size)
pygame.FULLSCREEN
 
pygame.display.set_caption("Teo painting Demo")

# Loop until the user clicks the close button.
done = False
clock = pygame.time.Clock()
screenSize=pygame.display.Info()
print(screenSize.current_w, screenSize.current_h)

# Number of screens
scn=2

# Number of rectangles
hrect=4 #Horizontal
vrect=4 #Vertical

# Clear the screen and set the screen background
screen.fill(WHITE)

# YARP Callback
class DataProcessor(yarp.PortReader): 
    #received = yarp.Bottle()
    def read(self,connection):
        print("in DataProcessor.read")
        if not(connection.isValid()):
            print("Connection shutting down")
            return False
        bin = yarp.Bottle()
        bout = yarp.Bottle()
        print("Trying to read from connection")
        ok = bin.read(connection)
	#self.received=bin;
	
	#Lets paint the rectangle
	x=bin.get(0).asInt()
	y=bin.get(1).asInt()
	print("Pos rectangle")
	print x
	print y
	pygame.draw.rect(screen, BLUE, [x*screenSize.current_w/(hrect*scn), y*screenSize.current_h/vrect, screenSize.current_w/(hrect*scn), screenSize.current_h/vrect], 0)
	
        if not(ok):
            print("Failed to read input")
            return False
        print("Received [%s]"%bin.toString())
        bout.addString("Received:")
        bout.append(bin)
        print("Sending [%s]"%bout.toString())
        writer = connection.getWriter()
        if writer==None:
            print("No one to reply to")
            return True
        return bout.write(writer)

p = yarp.Port()
r = DataProcessor()
p.setReader(r)
p.open("/painted_wall");
# Loop as long as done == False
while not done: 
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
       
    # Go ahead and update the screen with what we've drawn.
    # This MUST happen after all the other drawing commands.
    pygame.display.flip()
 
    # This limits the while loop to a max of 60 times per second.
    # Leave this out and we will use all CPU we can.
    clock.tick(60)
 
# Be IDLE friendly
yarp.Network.fini() # disconnect from the YARP network
pygame.quit()
