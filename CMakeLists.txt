cmake_minimum_required(VERSION 3.4)
project(thunderbots_navigator)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)

# OpenMP Setup
include(FindOpenMP)
if (OPENMP_FOUND)
    # Enable OpenMP if the compiler supports it
    message("Found OpenMP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else(OPENMP_FOUND)
    # TODO: Should we error and fail here?
    message("EROR: OpenMP not found - may have reduced performance")
endif(OPENMP_FOUND)

# OpenCV Setup
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

include_directories(include)

set(SOURCE_FILES main.cpp include/Node.h include/RealNode.h include/RealNode.tpp include/GraphNode.tpp include/GraphNode.tpp include/GraphNode.h)

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

add_executable(WorldGraphFactory_test
        test/WorldGraphFactory_test.cpp
        include/WorldGraphFactory.h
        )
target_link_libraries(WorldGraphFactory_test ${TESTING_LIBS})


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
