
import pylab as pl
import numpy as np

name=0
for i in range(4):
    for j in range(4):

        print "<Body name=",'"square'+str(name)+'"'," type=",'"static"',">";
        print "<Geom type=", '"box"',">";
        print "<Extents>0.001 0.1 0.1</Extents>";
        print "<Translation>", str(0.6) +' '+ str (-0.8+0.2*i) +' '+ str(0.32+0.2*j), "</Translation>";
        print " <diffuseColor>.5 .5 .5</diffuseColor>";
        print "</Geom>";
        print "</Body>";
        name += 1

