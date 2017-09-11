cmake_minimum_required(VERSION 3.5.1)
project(trace)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
# Proyect headers directory
include_directories(include)

# File(GLOB...) allows for wildcard additions
file(GLOB SOURCES "src/*.cpp")

# Generate shared library from the sources
add_library(trace SHARED ${SOURCES})

# The location were the library will be installed.
# If sudo installation considered, change:
# - location of target: /usr/lib
# - installation procedure "sudo make install"
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/guttaccept/lib )
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/jtracesort/lib )
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/jgutt2swf/lib )
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/jeventmerge/lib )
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/jtracemerge/lib )
install(TARGETS trace LIBRARY DESTINATION /home/ahiralesc/mysrc/cpp/cutu/lib )
# install(TARGETS trace LIBRARY DESTINATION ../../lib )

# Uncomment to enable unit test cases
# Unit test uses boost unit tests framework
# See http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/index.html
enable_testing()

# Find boost test framework
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# The location of the test codes sources
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

# Find test codes
foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
    get_filename_component(testName ${testSrc} NAME_WE)

    # Add compile target
    add_executable(${testName} ${testSrc})

    # Add dependency
    add_dependencies(${testName} trace)

    # link to Boost libraries AND your targets and dependencies
    target_link_libraries(${testName} ${Boost_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR}/build/libtrace.so )

    # Location of the test binaries 
    set_target_properties(${testName} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin)

    # Add the test to the collection of tests 
    # Notice the WORKING_DIRECTORY and COMMAND
    add_test(NAME ${testName} 
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin 
             COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin/${testName} )
endforeach(testSrc)
