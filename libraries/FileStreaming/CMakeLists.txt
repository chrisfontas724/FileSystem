cmake_minimum_required(VERSION 3.15)

set(namespace "cxl")
project("FileStreaming"
    VERSION 0.9.1
    DESCRIPTION "File streaming"
)

# Check to see if we are in the top level cmake file or not. The top level call
# to "project()" sets CMAKE_PROJECT_NAME which persists across the levels but
# PROJECT_NAME can be changed.
if (NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "This project has a top-level one called [${CMAKE_PROJECT_NAME}]")
else()
    message(STATUS "This project is a top-level one")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/buffer.cpp 
        src/file_stream.cpp 
        src/file_system.cpp 
        src/memory_stream.cpp 
        src/stream.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# without it public headers won't get installed
set(public_headers
    include/buffer.hpp
    include/file_stream.hpp
    include/file_system.hpp 
    include/memory_stream.hpp 
    include/stream.hpp
)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Installing)