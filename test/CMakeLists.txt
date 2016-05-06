# Testing things #########################################################################################
add_subdirectory(gtest-1.7.0)
enable_testing()

find_package(YARP REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${YARP_INCLUDE_DIRS} ${XGNITIVE_INCLUDE_DIRS})
link_directories(${XGNITIVE_LINK_DIRS})

add_definitions(-std=c++11)  # Required for aquila-dsp used by DtwCdgaRecognition
add_executable(testCdgaRecognitionLib testCdgaRecognitionLib.cpp)
target_link_libraries(testCdgaRecognitionLib ${YARP_LIBRARIES} ${XGNITIVE_LIBRARIES} gtest gtest_main)

