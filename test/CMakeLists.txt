# Testing things #########################################################################################
add_subdirectory(gtest-1.7.0)
enable_testing()

find_package(YARP REQUIRED)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${YARP_INCLUDE_DIRS} ${XGNITIVE_INCLUDE_DIRS} ${COLOR_DEBUG_INCLUDE_DIRS})
link_directories(${XGNITIVE_LINK_DIRS})

add_executable(testActionRecognitionLib testActionRecognitionLib.cpp)
target_link_libraries(testActionRecognitionLib ${YARP_LIBRARIES} ${XGNITIVE_LIBRARIES} gtest gtest_main)

