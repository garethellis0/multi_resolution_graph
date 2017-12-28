cmake_minimum_required(VERSION 3.8)
project(thunderbots_navigator)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES main.cpp include/Node.h include/RealNode.h src/RealNode.cpp src/GraphNode.cpp src/GraphNode.cpp include/GraphNode.h)
add_executable(thunderbots_navigator ${SOURCE_FILES})