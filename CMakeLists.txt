#1 make verson
cmake_minimum_required(VERSION 3.2)

#2 project
project(pp)

include(cmake/windows.cmake)

# set(CMAKE_CXX_FLAGS )
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_UNICODE "Use Unicode" ON)

option(SHARED_CRT "Use shared crt runtime library." OFF)

option(WCHAR_AS_DEFAULT "Use wchar_t as internal type" ON)

option(XP_TOOLSET "" ON)

config_compiler_and_linker()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(config/config.h.in config/config.h)

# add definitions
add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

#3 head file path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#4 source directory

# add_subdirectory()

add_subdirectory(third_party)
add_subdirectory(base)
add_subdirectory(ui)
add_subdirectory(test)


# add_subdirectory(base)
# add_subdirectory(ui)


#5 environment variable

#6 add executable file ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})


