cmake_minimum_required(VERSION 3.4)
project(multi_resolution_graph)

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

# GTest Setup
include(gtest.cmake)
include_directories(${GTEST_INCLUDE_DIRS})

# GMock Setup
pkg_check_modules(GMOCK REQUIRED)
include_directories(${GMOCK_INCLUDE_DIRS})

# General Testing Setup
set(TESTING_LIBS ${GTEST_BOTH_LIBRARIES} pthread)
enable_testing()

# Include headers (and templated files, since this is a primarily templated project)
include_directories(./include)

# Find Test Files
FILE(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

# Create the test executable
add_executable(all_tests
        test/multi_resolution_graph/main.cpp
    ${TEST_FILES}
    ${HEADER_FILES}
    ${SRC_FILES}
)
target_link_libraries(all_tests ${TESTING_LIBS})

# Demo Executable - Only build if we can find OpenCV
find_package(OpenCV)
if(OPENCV_FOUND)
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    add_executable(drawing_test
            drawing_test.cpp
            ${HEADER_FILES}
            ${SRC_FILES}
            )
    target_link_libraries(drawing_test ${OpenCV_LIBS})
endif(OPENCV_FOUND)
