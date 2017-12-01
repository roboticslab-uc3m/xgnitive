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
