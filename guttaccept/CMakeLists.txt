cmake_minimum_required(VERSION 3.5.1)
project(guttaccept)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header location
include_directories( ../csvrw/include )
include_directories( ../trace/include )
include_directories( include )

# Library location
set(PROJECT_LINK_LIBS libcsv.so libtrace.so)
link_directories( lib ) 

# Source location
file(GLOB SOURCES "src/*.cpp")

# Executable target
add_executable(guttaccept ${SOURCES})

# Link libraries to executable target
target_link_libraries(guttaccept ${PROJECT_LINK_LIBS})

# The location were the binary will be install
install(TARGETS guttaccept DESTINATION /home/ahiralesc/mysrc/cpp/cutu/bin)
