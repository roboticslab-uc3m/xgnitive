# Generated by "yarp cmake"

cmake_minimum_required(VERSION 2.6)

IF (ENABLE_cgdaExecutionIETYarp)

set(KEYWORD "cgdaExecutionIETYarp")

# Find YARP.
find_package(YARP REQUIRED)
find_package(Boost COMPONENTS iostreams python thread system)
find_package(ECF REQUIRED)

# Find KDL
FIND_PACKAGE(orocos_kdl REQUIRED)

# Search for source code.
file(GLOB folder_source *.cpp *.cc *.c)
file(GLOB folder_header *.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

# Automatically add include directories if needed.
foreach(header_file ${folder_header})
  get_filename_component(p ${header_file} PATH)
  include_directories(${p})
endforeach(header_file ${folder_header})

#if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
#  add_definitions("-fncgdao-strict-aliasing -Wall")
#endif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )

add_definitions(-std=c++11)

include_directories(${orocos_kdl_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS} ${ECF_INCLUDE_DIRS} ${XGNITIVE_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

if( Boost_INCLUDE_DIRS )
  include_directories(${Boost_INCLUDE_DIRS})
endif()

link_directories(${Boost_LIBRARY_DIRS} ${XGNITIVE_LINK_DIRS})

# Set up our main executable.
if (folder_source)
  add_executable(${KEYWORD} ${folder_source} ${folder_header})
  #add_executable(evMono main.cpp CgdaExecutionIET.cpp CgdaFitnessFunction.cpp)
  target_link_libraries(${KEYWORD} ${YARP_LIBRARIES} ${orocos_kdl_LIBRARIES} ${XGNITIVE_LIBRARIES} ${OpenCV_LIBS} ${OpenRAVE_LIBRARIES} ${OpenRAVE_CORE_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${ECF_LIBRARIES})
  install(TARGETS ${KEYWORD} DESTINATION bin)
else (folder_source)
  message(FATAL_ERROR "No source code files found. Please add something")
endif (folder_source)

  # Exporting dependencies for XGNITIVEConfig.cmake quite manually for now...
  #  set(XGNITIVE_DEFINITIONS ${XGNITIVE_DEFINITIONS} -std=c++ CACHE INTERNAL "appended definitions")


ENDIF (ENABLE_cgdaExecutionIETYarp)

