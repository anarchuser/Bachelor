cmake_minimum_required(VERSION 3.24.0)
project(Bachelor)

# Language specs
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)

# Linker & compiler flags
set(CMAKE_REQUIRED_LIBRARIES "-lglog")
set(CMAKE_CXX_FLAGS "-fconcepts")

# List source files and define target
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*)
list(REMOVE_ITEM SRCS ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(Bachelor ${PROJECT_SOURCE_DIR}/src/main.cpp ${SRCS})

# Threads
set(CMAKE_THREAD_LIBS_INIT "-pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(Bachelor Threads::Threads)

# Define dependencies
## Google Logging Library
target_link_libraries(Bachelor glog)

# If Catch2 is installed prepare the `Test` target
find_package(Catch2 3 CONFIG)
IF (Catch2_FOUND)
    file(GLOB_RECURSE TESTS ${PROJECT_SOURCE_DIR}/test/*.c*)
    add_executable(Test ${SRCS} ${TESTS})

    target_link_libraries(Test PRIVATE Catch2::Catch2WithMain)
    include(CTest)
    include(Catch)
    catch_discover_tests(Test)

    target_link_libraries(Test PRIVATE glog)
ENDIF()
