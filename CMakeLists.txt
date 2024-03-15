cmake_minimum_required(VERSION 3.28.0)

project(zpk_archive)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_TOOL "Build executable tool" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

set(headers
	include/archive.h
	include/archive_reader.h
	include/archive_writer.h
	include/file_stream.h
	include/utils.h
	include/zpk.h
	include/zpk_config.h)
	
set(sources
	src/archive_writer.cpp
	src/archive_reader.cpp
	src/file_stream.cpp
	src/utils.cpp
	src/archive.cpp)
	
add_library(zpk_archive STATIC ${sources} ${headers})
target_include_directories(zpk_archive PUBLIC include/)

if(BUILD_TOOL)

	set(tool_sources
		tool_src/main.cpp)
	add_executable(zpk_archive_tool ${tool_sources})
	target_link_libraries(zpk_archive_tool PRIVATE zpk_archive)
	
endif()
	
if(BUILD_EXAMPLES)

	set(example_sources
		examples/example_1.cpp
		examples/example_2.cpp)
	add_executable(zpk_archive_examples ${example_sources})
	target_link_libraries(zpk_archive_examples PRIVATE zpk_archive)
	
endif()
