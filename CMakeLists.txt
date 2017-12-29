cmake_minimum_required(VERSION 3.4)
project(thunderbots_navigator)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

set(SOURCE_FILES main.cpp include/Node.h include/RealNode.h src/RealNode.cpp src/GraphNode.cpp src/GraphNode.cpp include/GraphNode.h)
add_executable(thunderbots_navigator ${SOURCE_FILES})

# Setup stuff to let us use GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} include)
set(TESTING_LIBS ${GTEST_BOTH_LIBRARIES} pthread)
enable_testing()

# Test Executables
add_executable(realnode_test
        test/RealNode_test.cpp
        include/RealNode.h
        include/GraphNode.h
        include/Node.h
        src/RealNode.cpp
        src/GraphNode.cpp
        )
target_link_libraries(realnode_test ${TESTING_LIBS})
