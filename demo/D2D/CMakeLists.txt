
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cc)
set(SOURCES ${project_headers} ${project_cpps})
source_group_by_dir(SOURCES)

set(MANIFEST_FILE 
${CMAKE_SOURCE_DIR}/cmake/compatibility.manifest
${CMAKE_SOURCE_DIR}/cmake/as_invoker.manifest
)

add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(D2DDemo ${SOURCES} ${MANIFEST_FILE})

TARGET_LINK_LIBRARIES (D2DDemo modp_b64 base)

# include(cmake/msvcpch.cmake)

# if (CMAKE_SYSTEM_NAME MATCHES "Windows")
# 	# msvcpch.cmake的路径
# 	set(MSVCCMAKE_PCH ${CMAKE_SOURCE_DIR}/build/)
# 	# 预编译头文件和源文件的路径
# 	set(STDAFX_PCH_H ${CMAKE_SOURCE_DIR}/pch.h)
# 	set(STDAFX_PCH_C ${CMAKE_SOURCE_DIR}/pch.cpp)	
	
# 	# 添加预编译cmake的路径
# 	LIST(APPEND CMAKE_MODULE_PATH ${MSVCCMAKE_PCH})
# 	# 包含msvcpch.cmake
# 	INCLUDE(msvcpch)
# endif ()
 
 
# if (CMAKE_SYSTEM_NAME MATCHES "Windows")
# 	# 添加源文件，生成可执行文件
# 	add_executable(Proxy ${proxy_src} ${STDAFX_PCH_C})
# 	# 添加链接库
# 	target_link_libraries(Proxy Comm)	
# 	# 此处是关键，添加我们在msvcpch.cmake定义的宏
# 	USE_MSVC_PCH(Proxy stdafx.h ${STDAFX_PCH_C})
# endif ()