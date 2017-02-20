# Copyright: 2014 UC3M
# Author: Juan G Victores
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

set(appname models)

file(GLOB xml ${CMAKE_CURRENT_SOURCE_DIR}/*.xml)

yarp_install(FILES ${xml} DESTINATION ${XGNITIVE_CONTEXTS_INSTALL_DIR}/${appname})

