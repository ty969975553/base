
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cc)
set(SOURCES ${project_headers} ${project_cpps})
source_group_by_dir(SOURCES)

set(MANIFEST_FILE 
${CMAKE_SOURCE_DIR}/cmake/compatibility.manifest
${CMAKE_SOURCE_DIR}/cmake/as_invoker.manifest
)

add_executable(PEStringScanner ${SOURCES} ${MANIFEST_FILE})

TARGET_LINK_LIBRARIES (PEStringScanner modp_b64 base)