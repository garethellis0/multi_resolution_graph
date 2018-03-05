cmake_minimum_required(VERSION 3.4)
project(thunderbots_navigator)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

# GTest Setup
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} include)

# GMock Setup
pkg_check_modules(GMOCK REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

# General Testing Setup
set(TESTING_LIBS ${GTEST_BOTH_LIBRARIES} pthread)
enable_testing()

# Include headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find Header Files
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/geom/*.h")

# Find Source Files
FILE(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/geom/*.tpp")

# Find Test Files
FILE(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

# Create the test executable
add_executable(all_tests
    test/unit-tests/main.cpp
    ${TEST_FILES}
    ${HEADER_FILES}
    ${SRC_FILES}
)
target_link_libraries(all_tests ${TESTING_LIBS})

# Misc executables
add_executable(drawing_test
        drawing_test.cpp
        ${HEADER_FILES}
        ${SRC_FILES}
        )
target_link_libraries(drawing_test ${OpenCV_LIBS})
