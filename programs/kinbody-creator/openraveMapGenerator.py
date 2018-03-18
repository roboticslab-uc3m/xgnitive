#!/usr/bin/python

#import lxml.etree
#import lxml.builder
from lxml import etree

#E = lxml.builder.ElementMaker()

#KINBODY=E.KinBody
#BODY=E.Body
#GEOM=E.Geom
#EXTENTS=E.Extents
#TRANSLATION=E.Translation
#DIFUSSECOLOR=E.diffuseColor

# User variables
nX = 3
nY = 2
boxHeight = 1.0

resolution = 2.0  # Just to make similar to MATLAB [pixel/meter]
meterPerPixel = 1 / resolution  # [meter/pixel]

# Program
Ez = boxHeight / 2.0  # Box size is actually double the extent

Ex = meterPerPixel / 2.0
Ey = meterPerPixel / 2.0

KinBody = etree.Element("KinBody", name="map")

for iY in range(nY):
    # print "iY:",iY
    for iX in range(nX):
        # print "* iX:",iX
        #-- Add E___ to each to force begin at 0,0,0 (centered by default)
        x = Ex + iX*meterPerPixel
        y = Ey + iY*meterPerPixel
        z = Ez  # Add this to raise to floor level (centered by default)
        Number = iX + (iY * nX)

        #Create pixel
        Body = etree.SubElement(KinBody, "Body", name="square"+str(Number), type="static")
        Geom = etree.SubElement(Body, "Geom", type="box")
        Extents = etree.SubElement(Geom, "Extents").text= str(Ex)+" "+ str(Ey)+" "+str(Ez)
        Translation = etree.SubElement(Geom, "Translation").text= str(x)+" "+str(y)+" "+str(z)
        DifusseColor = etree.SubElement(Geom, "diffuseColor").text= ".5 .5 .5"

'''
the_doc = KINBODY(
            BODY(
                GEOM(
                    EXTENTS("0.001 0.115 0.065"),
                    TRANSLATION("0.6 "+   "-0.8 0.32"),
                    DIFUSSECOLOR(".5 .5 .5"),
                    type="box",
                ),
                name="square"+str(i), type="static"
            ),
            name="wall",
        )
'''

myStr = etree.tostring(KinBody, pretty_print=True)

outFile = open('map.kinbody.xml', 'w')
outFile.write(myStr)
outFile.close()
