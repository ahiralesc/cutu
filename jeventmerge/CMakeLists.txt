cmake_minimum_required(VERSION 3.5.1)
project(jeventmerge)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system thread REQUIRED)
  
# Header location
include_directories(../trace/include include ${Boost_INCLUDE_DIRS})

# Library location
set(PROJECT_LINK_LIBS libtrace.so pthread)
link_directories(lib ${Boost_LIBRARY_DIRS})

# Source location
file(GLOB SOURCES "src/*.cpp")

# Executable target
add_executable(jeventmerge ${SOURCES})

# Link libraries to executable target
target_link_libraries(jeventmerge ${PROJECT_LINK_LIBS} ${Boost_LIBRARIES})

# The location were the binary executable will be install
# install(TARGETS jeventmerge DESTINATION /home/ahiralesc/mysrc/cpp/guct/bin)


# Uncomment to enable unit test cases
# Unit test uses boost unit tests framework
# See http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/index.html
#enable_testing()

# Find boost test framework
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS} include ../trace/include)

# The location of the test codes sources
#file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

# Find test codes
#foreach(testSrc ${TEST_SRCS})
    #Extract the filename without an extension (NAME_WE)
#    get_filename_component(testName ${testSrc} NAME_WE)

    # Add compile target
    # add_executable(${testName} ${testSrc} ${DEPENDENCY})
#    add_executable(${testName} ${testSrc})

    # link to Boost libraries AND your targets and dependencies
#    target_link_libraries(${testName} ${Boost_LIBRARIES} ${PROJECT_LINK_LIBS} )

    # Location of the test binaries
#    set_target_properties(${testName} PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin)

    # Add the test to the collection of tests
    # Notice the WORKING_DIRECTORY and COMMAND
#    add_test(NAME ${testName}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin
#            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build/testBin/${testName} )
#endforeach(testSrc)
