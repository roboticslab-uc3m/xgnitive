# - Try to find Aquila
# Once done this will define
#  AQUILA_FOUND - System has Aquila
#  AQUILA_INCLUDE_DIRS - The Aquila include directories
#  AQUILA_LIBRARIES - The libraries needed to use Aquila
#  AQUILA_DEFINITIONS - Compiler switches required for using Aquila

#set(AQUILA_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(AQUILA_INCLUDE_DIR aquila/aquila.h
  HINTS
    $ENV{AQUILA_DIR}
    $ENV{FFMPEG_DIR}
  PATHS
    /usr/local
    /usr
  PATH_SUFFIXES
    include
)

find_library(AQUILA_LIBRARY
  NAMES
    Aquila
    libAquila
  HINTS
    $ENV{AQUILA_DIR}
  PATH_SUFFIXES
    lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Aquila DEFAULT_MSG
  AQUILA_INCLUDE_DIR AQUILA_LIBRARY
)

mark_as_advanced(AQUILA_INCLUDE_DIR AQUILA_LIBRARY)

set(AQUILA_DEFINITIONS -std=c++11)
set(AQUILA_INCLUDE_DIRS ${AQUILA_INCLUDE_DIR})
set(AQUILA_LIBRARIES ${AQUILA_LIBRARY})
