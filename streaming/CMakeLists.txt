add_library (streaming
	buffer.cpp
	file_stream.cpp
	file_system.cpp
	memory_stream.cpp
	stream.cpp
)

target_link_libraries(streaming core)
target_include_directories(streaming PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
