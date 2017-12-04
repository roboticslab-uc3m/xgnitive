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
#y= -Ey*(1+2i) #i=columna
#z= 0.26+(Ez*(1+2*j))


KinBody = etree.Element("KinBody", name="wall")

Body = etree.SubElement(KinBody, "Body", name="wall")
Geom = etree.SubElement(Body, "Geom", name="square-"+str(2), type="static")
Extents = etree.SubElement(Geom, "EXTENTS").text= "0.001 0.115 0.065"
Translation = etree.SubElement(Geom, "TRANSLATION").text= "0.6 "+   "-0.8 0.32"
DifusseColor = etree.SubElement(Geom, "DIFUSSECOLOR").text= ".5 .5 .5"





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

print etree.tostring(KinBody, pretty_print=True)
