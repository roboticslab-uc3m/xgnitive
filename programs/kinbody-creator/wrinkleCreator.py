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

#The number of sqaures per side
NSquares=4

offsetEx=0
offsetEy=-0.1
offsetEz=0

#A table is like a wall but with the x and y swapped: zt=xw, yt=yw, xt=zw
Ez=0.001+offsetEx
Ey=(offsetEy+0.46)/NSquares
Ex=(offsetEz+0.26)/NSquares

#for i in range(0, NSquares)
#    for j in range(0, NSquares)
#x= 0.6
#y= -Ey*(1+2i)
#z= 0.26+(Ez*(1+2*j))

offsetX= -0.1
offsetY= 0.2
offsetZ= -0.38


KinBody = etree.Element("KinBody", name="iron-wrinkle")
for i in range(NSquares): #filas
    for j in range(1): #columnas, For a wrinkle we only want one column.

        #Calculate position of this "pixel"
        x = offsetX+0.6
        y = offsetY-Ey*(1+2*((NSquares-1)-i)) #i=columna
        z = offsetZ+0.26+(Ez*(1+2*j))
        Number=i+(j*NSquares)

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

str = etree.tostring(KinBody, pretty_print=True)

text_file= open('../../share/models/iron-wrinkle.kinbody.xml', 'wb') #Right now it is saved automatically in models, better if is saved HERE for final version.
text_file.write(str)
text_file.close()



