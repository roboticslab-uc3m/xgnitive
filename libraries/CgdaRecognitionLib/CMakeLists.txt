# Generated by "yarp cmake"

cmake_minimum_required(VERSION 2.6)

IF (ENABLE_CgdaRecognitionLib)

set(KEYWORD "CgdaRecognitionLib")

# Find YARP.
find_package(YARP REQUIRED)

# Find Aquila.
find_package(Aquila REQUIRED)

# Search for source code.
file(GLOB folder_source *.cpp *.cc *.c)
file(GLOB folder_header *.h *.hpp)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

# Automatically add include directories if needed.
foreach(header_file ${folder_header})
  get_filename_component(p ${header_file} PATH)
  include_directories(${p})
endforeach(header_file ${folder_header})

add_definitions(${AQUILA_DEFINITIONS})

include_directories(${YARP_INCLUDE_DIRS} ${XGNITIVE_INCLUDE_DIRS} ${AQUILA_INCLUDE_DIRS})

link_directories(${XGNITIVE_LINK_DIRS})

# Set up our main executable.
if (folder_source)
  add_library(${KEYWORD} ${folder_source} ${folder_header})
  target_link_libraries(${KEYWORD} ${YARP_LIBRARIES} ${AQUILA_LIBRARIES})
  install(TARGETS ${KEYWORD} DESTINATION bin)
else (folder_source)
  message(FATAL_ERROR "No source code files found. Please add something")
endif (folder_source)

  # Exporting dependencies for XGNITIVEConfig.cmake quite manually for now...
  set(XGNITIVE_DEFINITIONS ${XGNITIVE_DEFINITIONS} ${AQUILA_DEFINITIONS} CACHE INTERNAL "appended definitions")
  set(XGNITIVE_INCLUDE_DIRS ${XGNITIVE_INCLUDE_DIRS} ${AQUILA_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "appended headers")
  set(XGNITIVE_LIBRARIES ${XGNITIVE_LIBRARIES} ${AQUILA_LIBRARIES} CgdaRecognitionLib CACHE INTERNAL "appended libraries")

ENDIF (ENABLE_CgdaRecognitionLib)

