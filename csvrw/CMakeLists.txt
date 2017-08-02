cmake_minimum_required(VERSION 3.5.1)

project(csvrw)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
#Include the headers *.h from the include dir.
include_directories(include)

#Wildcard addition for sources
file(GLOB SOURCES "src/*.cpp")

#Generate the shared library from the sources
add_library(csv SHARED ${SOURCES})

#Set the location for library installation -- i.e., /usr/lib in this case
# not really necessary in this example. Use "sudo make install" to apply
install(TARGETS csv DESTINATION /home/ahiralesc/mysrc/cpp/cutu/guttaccept/lib)
install(TARGETS csv DESTINATION /home/ahiralesc/mysrc/cpp/cutu/jtracesort/lib)
install(TARGETS csv DESTINATION /home/ahiralesc/mysrc/cpp/cutu/lib)
