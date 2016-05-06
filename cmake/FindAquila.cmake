# - Try to find Aquila
# Once done this will define
#  AQUILA_FOUND - System has Aquila
#  AQUILA_INCLUDE_DIRS - The Aquila include directories
#  AQUILA_LIBRARIES - The libraries needed to use Aquila
#  AQUILA_DEFINITIONS - Compiler switches required for using Aquila

#set(AQUILA_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(AQUILA_INCLUDE_DIR aquila.h
  $ENV{AQUILA_DIR}
  $ENV{FFMPEG_DIR}/aquila
  /usr/local/include/aquila
  /usr/include/aquila
)

find_library(AQUILA_LIBRARY NAMES Aquila libAquila )

mark_as_advanced(AQUILA_INCLUDE_DIR AQUILA_LIBRARY )

set(AQUILA_LIBRARIES ${AQUILA_LIBRARY} )
set(AQUILA_INCLUDE_DIRS ${AQUILA_INCLUDE_DIR} )
