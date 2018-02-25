cmake_minimum_required(VERSION 3.4)
project(thunderbots_navigator)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(include)

set(SOURCE_FILES main.cpp include/Node.h include/RealNode.h include/RealNode.tpp include/GraphNode.tpp include/GraphNode.tpp include/GraphNode.h)

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
        include/RealNode.tpp
        include/GraphNode.tpp
        )
target_link_libraries(realnode_test ${TESTING_LIBS})

add_executable(graphnode_test
        test/GraphNode_test.cpp
        include/RealNode.h
        include/GraphNode.h
        include/Node.h
        include/RealNode.tpp
        include/GraphNode.tpp
        )
target_link_libraries(graphnode_test ${TESTING_LIBS})

add_executable(graph_factory_test
        test/GraphFactory_test.cpp
        include/RealNode.h
        include/GraphNode.h
        include/Node.h
        include/RealNode.tpp
        include/GraphNode.tpp
        include/GraphFactory.h
        include/GraphFactory.tpp
        include/Area.h
        include/Rectangle.h
        include/Circle.h
        include/Rectangle.tpp
        include/Circle.tpp
        )
target_link_libraries(graph_factory_test ${TESTING_LIBS})

add_executable(rectangle_test
        test/Rectangle_test.cpp
        include/Area.h
        include/Rectangle.h
        include/Rectangle.tpp
        )
target_link_libraries(rectangle_test ${TESTING_LIBS})

add_executable(circle_test
        test/Circle_test.cpp
        include/Area.h
        include/Circle.h
        include/Circle.tpp
        )
target_link_libraries(circle_test ${TESTING_LIBS})

# Misc executables
add_executable(drawing_test
        drawing_test.cpp
        include/RealNode.h
        include/GraphNode.h
        include/Node.h
        include/GraphFactory.h
        include/RealNode.tpp
        include/GraphNode.tpp
        include/GraphFactory.tpp
        include/Area.h
        include/Rectangle.h
        include/Circle.h
        include/Rectangle.tpp
        include/Circle.tpp
        )
target_link_libraries(drawing_test ${OpenCV_LIBS})
