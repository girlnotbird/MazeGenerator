cmake_minimum_required(VERSION 3.30)

project(MazeGen VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE MazeGen_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE MazeGen_HEADERS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/*.h)

add_executable(MazeGen
        ${MazeGen_HEADERS}
        ${MazeGen_SOURCES}
)

target_include_directories(MazeGen
        PUBLIC ${CMAKE_SOURCE_DIR}/include
)