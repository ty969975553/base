#
# 设置输出目录
#
if (OUTPATH_WITHOUT_TYPE)
    message("---Put All generation in same Path")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
else()
    message("---Put All generation in different Path")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
endif()

#
# 设置多线程编译
# 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /wd4100 /wd4189 /wd4996")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4100 /wd4189 /wd4996")

#
# 设置输入文件的后缀
#
set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_RELEASE_POSTFIX "")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible configurations" FORCE)

#
# 是否为动态编译
#
macro(FIX_DEFAULT_COMPILER_SETTINGS_MT)
    if (MSVC)
        foreach(flag_var
                CMAKE_C_FLAGS 
                CMAKE_C_FLAGS_DEBUG 
                CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL 
                CMAKE_C_FLAGS_RELWITHDEBINFO

                CMAKE_CXX_FLAGS 
                CMAKE_CXX_FLAGS_DEBUG 
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL 
                CMAKE_CXX_FLAGS_RELWITHDEBINFO)
           if(${flag_var} MATCHES "/MD")
              string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MD")
        endforeach(flag_var)
    endif()
endmacro()

#
# 是否启用wchar_t 为内置类型
#
macro(FIX_DEFAULT_COMPILER_SETTINGS_WCHAR)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t-")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zc:wchar_t-")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zc:wchar_t-")
    set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /Zc:wchar_t-")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zc:wchar_t-")    
endmacro()

macro(config_compiler_and_linker)
    #
    # 修正编译参数
    #
    if (NOT SHARED_CRT)
        message("-- Building with MT")
        FIX_DEFAULT_COMPILER_SETTINGS_MT()
        set(BUILD_CONF_MT "1")
    else()
        message("-- Building with MD")
        set(BUILD_CONF_MT "0")
    endif()

    if (NOT WCHAR_AS_DEFAULT)
        FIX_DEFAULT_COMPILER_SETTINGS_WCHAR()
        set(BUILD_CONF_WCHAR "0")
    else()
        set(BUILD_CONF_WCHAR "1")
    endif()

    if(USE_UNICODE)
        add_definitions(-DUNICODE -D_UNICODE)
        set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -DUNICODE -D_UNICODE")
        message("-- Building with Unicode")
        set(BUILD_CONF_UNICODE "1")
    else(USE_UNICODE)
        add_definitions(-D_MBCS)
        message("-- Building with MBCS")
        set(BUILD_CONF_UNICODE "0")
    endif(USE_UNICODE)

    #
    # 支持xp环境
    #
    if (XP_TOOLSET) 
        if (MSVC_VERSION GREATER 1700) # 1700 is Visual Studio 2012.
            add_definitions(-D_USING_V110_SDK71_)
        endif()
        set(BUILD_CONF_XP_TOOLSET "1")
    else(XP_TOOLSET)
        set(BUILD_CONF_XP_TOOLSET "0")
    endif()
endmacro()

#
# 按目录创建filter 
#

macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            # message(sgbd_file)
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
            # message(${sgbd_group_name})
            # message(${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)