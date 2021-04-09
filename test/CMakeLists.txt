
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cc)
set(SOURCES ${project_headers} ${project_cpps})
source_group_by_dir(SOURCES)

set(MANIFEST_FILE 
${CMAKE_SOURCE_DIR}/cmake/compatibility.manifest
${CMAKE_SOURCE_DIR}/cmake/as_invoker.manifest
)

# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable(test ${SOURCES} ${MANIFEST_FILE})

TARGET_LINK_LIBRARIES (test modp_b64 base)