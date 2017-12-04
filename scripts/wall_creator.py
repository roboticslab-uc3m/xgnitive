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

NSquares=4

Ex=0.001
Ey=0.46/NSquares
Ez=0.26/NSquares

#for i in range(0, NSquares)
#    for j in range(0, NSquares)
#x= 0.6
#y= -Ey*(1+2i)
#z= 0.26+(Ez*(1+2*j))


KinBody = etree.Element("KinBody", name="wall")
for i in range(NSquares): #filas
    for j in range(NSquares): #columnas

        #Calculate position of this "pixel"
        x = 0.6
        y = -Ey*(1+2*((NSquares-1)-i)) #i=columna
        z = 0.26+(Ez*(1+2*j))
        Number=j+(i*NSquares)

        #Create pixel
        Body = etree.SubElement(KinBody, "Body", name="square"+str(Number), type="static")
        Geom = etree.SubElement(Body, "Geom", type="box")
        Extents = etree.SubElement(Geom, "Extents").text= "0.001 "+ str(Ey)+" "+str(Ez)
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

text_file= open('wall.xml', 'wb')
text_file.write(str)
text_file.close()



