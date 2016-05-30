# - Try to find ECF
# Once done this will define
#  ECF_FOUND - System has Aquila
#  ECF_INCLUDE_DIRS - The Aquila include directories
#  ECF_LIBRARIES - The libraries needed to use Aquila
#  ECF_DEFINITIONS - Compiler switches required for using Aquila

#set(AQUILA_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(ECF_INCLUDE_DIR ECF.h
  $ENV{ECF_DIR}
  $ENV{FFMPEG_DIR}/ecf
  /usr/local/include/ecf
  /usr/include/ecf
)

find_library(ECF_LIBRARY NAMES ecf libecf )

mark_as_advanced(ECF_INCLUDE_DIR ECF_LIBRARY )

#set(AQUILA_DEFINITIONS -std=c++11 )
set(ECF_INCLUDE_DIRS ${ECF_INCLUDE_DIR} )
set(ECF_LIBRARIES ${ECF_LIBRARY} )
