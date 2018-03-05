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

include_directories(goem/multi_res_graph)

set(SOURCE_FILES 
    main.cpp 
    geom/multi_res_graph/Node.h 
    geom/multi_res_graph/RealNode.h 
    geom/multi_res_graph/RealNode.tpp 
    geom/multi_res_graph/GraphNode.tpp 
    geom/multi_res_graph/GraphNode.tpp 
    geom/multi_res_graph/GraphNode.h
    )

# Test Executables
add_executable(realnode_test
        test/RealNode_test.cpp
        geom/multi_res_graph/RealNode.h
        geom/multi_res_graph/GraphNode.h
        geom/multi_res_graph/Node.h
        geom/multi_res_graph/RealNode.tpp
        geom/multi_res_graph/GraphNode.tpp
        )
target_link_libraries(realnode_test ${TESTING_LIBS})

add_executable(graphnode_test
        test/GraphNode_test.cpp
        geom/multi_res_graph/RealNode.h
        geom/multi_res_graph/GraphNode.h
        geom/multi_res_graph/Node.h
        geom/multi_res_graph/RealNode.tpp
        geom/multi_res_graph/GraphNode.tpp
        )
target_link_libraries(graphnode_test ${TESTING_LIBS})

add_executable(graph_factory_test
        test/GraphFactory_test.cpp
        geom/multi_res_graph/RealNode.h
        geom/multi_res_graph/GraphNode.h
        geom/multi_res_graph/Node.h
        geom/multi_res_graph/RealNode.tpp
        geom/multi_res_graph/GraphNode.tpp
        geom/multi_res_graph/GraphFactory.h
        geom/multi_res_graph/GraphFactory.tpp
        geom/multi_res_graph/Area.h
        geom/multi_res_graph/Rectangle.h
        geom/multi_res_graph/Circle.h
        geom/multi_res_graph/Rectangle.tpp
        geom/multi_res_graph/Circle.tpp
        )
target_link_libraries(graph_factory_test ${TESTING_LIBS})

add_executable(rectangle_test
        test/Rectangle_test.cpp
        geom/multi_res_graph/Area.h
        geom/multi_res_graph/Rectangle.h
        geom/multi_res_graph/Rectangle.tpp
        )
target_link_libraries(rectangle_test ${TESTING_LIBS})

add_executable(circle_test
        test/Circle_test.cpp
        geom/multi_res_graph/Area.h
        geom/multi_res_graph/Circle.h
        geom/multi_res_graph/Circle.tpp
        )
target_link_libraries(circle_test ${TESTING_LIBS})

add_executable(WorldGraphFactory_test
        test/WorldGraphFactory_test.cpp
        geom/multi_res_graph/WorldGraphFactory.h
        )
target_link_libraries(WorldGraphFactory_test ${TESTING_LIBS})


# Misc executables
add_executable(drawing_test
        drawing_test.cpp
        geom/multi_res_graph/RealNode.h
        geom/multi_res_graph/GraphNode.h
        geom/multi_res_graph/Node.h
        geom/multi_res_graph/GraphFactory.h
        geom/multi_res_graph/RealNode.tpp
        geom/multi_res_graph/GraphNode.tpp
        geom/multi_res_graph/GraphFactory.tpp
        geom/multi_res_graph/Area.h
        geom/multi_res_graph/Rectangle.h
        geom/multi_res_graph/Circle.h
        geom/multi_res_graph/Rectangle.tpp
        geom/multi_res_graph/Circle.tpp
        )
target_link_libraries(drawing_test ${OpenCV_LIBS})
