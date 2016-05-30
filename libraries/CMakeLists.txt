# Copyright: Universidad Carlos III de Madrid (C) 2016
# Authors: jgvictores, raulfdzbis, smorante

add_subdirectory(ColorDebug)  # ColorDebug sets COLOR_DEBUG_INCLUDE_DIRS
set(XGNITIVE_INCLUDE_DIRS ${XGNITIVE_INCLUDE_DIRS} ${COLOR_DEBUG_INCLUDE_DIRS} CACHE INTERNAL "appended headers")

add_subdirectory(CgdaRecognitionLib)

add_subdirectory(EcfComponents)
