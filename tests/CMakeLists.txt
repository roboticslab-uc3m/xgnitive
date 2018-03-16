find_package(GTestSources 1.6.0 QUIET)

if(NOT GTestSources_FOUND AND (NOT DEFINED ENABLE_tests OR ENABLE_tests))
    message(WARNING "GTestSources package not found, disabling tests")
endif()

include(CMakeDependentOption)
cmake_dependent_option(ENABLE_tests "Enable/disable unit tests" ON
                       GTestSources_FOUND OFF)

if(ENABLE_tests)

    find_package(YARP REQUIRED)

    add_definitions(${XGNITIVE_DEFINITIONS})
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${YARP_INCLUDE_DIRS} ${XGNITIVE_INCLUDE_DIRS})
    link_directories(${XGNITIVE_LINK_DIRS})

    add_executable(testCgdaRecognitionLib testCgdaRecognitionLib.cpp)
    target_link_libraries(testCgdaRecognitionLib ${YARP_LIBRARIES} ${XGNITIVE_LIBRARIES} gtest gtest_main)

else()

    set(ENABLE_tests OFF CACHE BOOL "Enable/disable unit tests" FORCE)

endif()
