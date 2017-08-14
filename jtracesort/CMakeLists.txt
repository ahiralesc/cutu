cmake_minimum_required(VERSION 3.5.1)
project(jtracesort)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# location of shared library
set(PROJECT_LINK_LIBS libtrace.so libcsv.so)
link_directories(lib) 

# Reference to dynamic library
include_directories(include)
include_directories(../trace/include)
include_directories(../csvrw/include)

add_executable(jtraceextract src/jtraceextract.cpp)
add_executable(jtracesort src/jtracesort.cpp)

target_link_libraries(jtraceextract ${PROJECT_LINK_LIBS} )
target_link_libraries(jtracesort ${PROJECT_LINK_LIBS} ) 

# The location were the binary executable will be install
install(TARGETS jtraceextract DESTINATION /home/ahiralesc/mysrc/cpp/cutu/bin)
install(TARGETS jtracesort DESTINATION /home/ahiralesc/mysrc/cpp/cutu/bin)
