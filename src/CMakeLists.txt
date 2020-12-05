# Copyright 2019 Chris Fontas. All rights reserved.
# Use of this source code is governed by the license that can be
# found in the LICENSE file.

set(LIBRARY_NAME streaming)

set(HEADER_LIST 
    "${Streaming_SOURCE_DIR}/include/streaming/buffer.hpp" 
    "${Streaming_SOURCE_DIR}/include/streaming/file_stream.hpp"
    "${Streaming_SOURCE_DIR}/include/streaming/file_system.hpp"
    "${Streaming_SOURCE_DIR}/include/streaming/memory_stream.hpp"
    "${Streaming_SOURCE_DIR}/include/streaming/stream.hpp"
)

add_library (streaming
    buffer.cpp
    file_stream.cpp
    file_system.cpp
    memory_stream.cpp
    stream.cpp
    ${HEADER_LIST}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(streaming PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# All users of this library will need at least C++11
target_compile_features(streaming PUBLIC cxx_std_11)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
