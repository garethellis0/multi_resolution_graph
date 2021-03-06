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

# General Testing Setup
set(TESTING_LIBS gtest pthread)
enable_testing()

# Include headers (and templated files, since this is a primarily templated project)
include_directories(./include)

# Find Header Files
FILE(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.tpp")

# Find Test Files
FILE(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

# Create the test executable
add_executable(all_tests
        test/multi_resolution_graph/main.cpp
    ${TEST_FILES}
    ${HEADER_FILES}
)
target_link_libraries(all_tests ${TESTING_LIBS})

# Export library
add_library(multi_resolution_graph SHARED
        ${HEADER_FILES}
        )
set_target_properties(multi_resolution_graph PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(multi_resolution_graph PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

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
