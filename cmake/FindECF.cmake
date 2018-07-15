# - Try to find ECF
# Once done this will define
#  ECF_FOUND - System has ECF
#  ECF_INCLUDE_DIRS - The ECF include directories
#  ECF_LIBRARIES - The libraries needed to use ECF
#  ECF_DEFINITIONS - Compiler switches required for using ECF

#set(ECF_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(ECF_INCLUDE_DIR ecf/ECF.h
  HINTS
    $ENV{ECF_DIR}
    $ENV{FFMPEG_DIR}
  PATHS
    /usr/local
    /usr
  PATH_SUFFIXES
    include
)

find_library(ECF_LIBRARY
  NAMES
    ecf
    libecf
  HINTS
    $ENV{ECF_DIR}
  PATH_SUFFIXES
    lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ECF DEFAULT_MSG
  ECF_INCLUDE_DIR ECF_LIBRARY
)

mark_as_advanced(ECF_INCLUDE_DIR ECF_LIBRARY)

set(ECF_INCLUDE_DIRS ${ECF_INCLUDE_DIR})
set(ECF_LIBRARIES ${ECF_LIBRARY})
