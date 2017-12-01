#!/usr/bin/python
import lxml.etree
import lxml.builder

E = lxml.builder.ElementMaker()

KINBODY=E.KinBody
BODY=E.Body
GEOM=E.Geom
EXTENTS=E.Extents
TRANSLATION=E.Translation
DIFUSSECOLOR=E.diffuseColor

NSquares=4

Ex=0.001
Ey=0.46/NSquares
Ez=0.26/NSquares

#for i in range(0, NSquares)
#    for j in range(0, NSquares)
#x= 0.6
#y= -Ey*(1+2i) #i=columna
#z= 0.26+(Ez*(1+2*j))


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

print lxml.etree.tostring(the_doc, pretty_print=True)
