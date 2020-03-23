# Copyright 2019 Chris Fontas. All rights reserved.
# Use of this source code is governed by the license that can be
# found in the LICENSE file.

cmake_minimum_required (VERSION 3.7)

add_library (streaming
    ${CMAKE_CURRENT_SOURCE_DIR}/streaming/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streaming/file_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streaming/file_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streaming/memory_stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streaming/stream.cpp
)


target_include_directories(streaming PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

