set (HOST_SRC
    host/graphic_host.h
    host/graphic_host.cc
)

set (SOURCES
    ${HOST_SRC}
)

source_group_by_dir(SOURCES)

message(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# link_libraries(console)
# link_libraries(zlibstatic)
# include_directories(${CMAKE_BINARY_DIR}/third_party/zlib)
# message(STATUS "Include_directories ${CMAKE_BINARY_DIR}/third_party/zlib")

add_library(gui STATIC ${SOURCES})

target_precompile_headers(gui PRIVATE [["precompile_header.h"]])