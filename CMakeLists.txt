cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(MazeGen VERSION 2024.0.0.1)

set(CMAKE_SOURCE_DIR ${MazeGen_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${MazeGen_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set( MazeGen_INCLUDE ${MazeGen_SOURCE_DIR}/include)
set( MazeGen_SRC ${MazeGen_SOURCE_DIR}/src)

set( main ${MazeGen_SRC}/main.cpp)
set( jm_mazegen ${MazeGen_SRC}/jm_mazegen.cpp)
set( headers ${MazeGen_INCLUDE}/jm_mazegen.h)

set( SOURCES ${main} ${jm_mazegen} ${headers})

add_executable(MazeGen ${SOURCES})
target_include_directories(MazeGen PUBLIC ${MazeGen_INCLUDE}/)
target_include_directories(MazeGen PRIVATE ${MazeGen_SRC}/)


#####################################################################
# DEPENDENCIES
#####################################################################

# include(GoogleTest)

#####################################################################
# LIBRARY
#####################################################################

# add_library(${PROJECT_NAME}
#   src/mazegen/implementation.cpp)

# add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# set_target_properties(${PROJECT_NAME} PROPERTIES
#   VERSION ${PROJECT_VERSION})

# target_include_directories(${PROJECT_NAME}
#   PUBLIC include
#   PRIVATE src)

# target_link_libraries(${PROJECT_NAME}
#   PRIVATE GoogleTest::GoogleTest)

# target_compile_options(${PROJECT_NAME}
#   PRIVATE -Wall -Wextra -pedantic -Werror)

# target_compile_features(${PROJECT_NAME}
#   PRIVATE cxx_std_17)

# install(TARGETS ${PROJECT_NAME}
#   EXPORT MazeGenTargets
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#####################################################################
# UNIT TESTS
#####################################################################