#1 make verson
cmake_minimum_required(VERSION 3.2)

#2 project
PROJECT(pp)

SET(CMAKE_BUILD_TYPE Debug)
IF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE Release)
ENDIF()

MESSAGE("Build type: " ${CMAKE_BUILD_TYPE})

#3 head file path
INCLUDE_DIRECTORIES(
    base
)

#4 source directory
AUX_SOURCE_DIRECTORY(
    .
)

#5 environment variable

#6 add executable file ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})
ADD_EXECUTABLE()

#7 add link library  
TARGET_LINK_LIBRARIES()