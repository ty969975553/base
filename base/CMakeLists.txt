set (BUSINESS_SRC
  business/validate.h
  business/validate.cc
)

set (CHECKSUM_SRC
  checksum/crc.h
  checksum/crc.cc
)

set (ENCRYPT_SRC
  encrypt/md5.h
  encrypt/md5.cc
  encrypt/base64.h
  encrypt/base64.cc
)

set (FILE_SRC
  files/file.h
  files/file.cc
  files/file_util.h
  files/file_util.cc
  files/file_util_win.cc
  files/file_path.h
  files/file_path_win.h
  files/file_path_win.cc
  files/path_service.h
  files/path_service.cc
)


set(LOG_SRC
  log/logging.h
  log/logging.cc 
)

set (STRINGS_SRC
  strings/string_conversion.h
  strings/string_conversion.cc
  strings/string_util.h
  strings/string_split.h
  strings/string_split.cc
)

set (TEST_SRC
  test/unittest.h
  test/unittest.cc
)

set (TIME_SRC
  time/time_util.h
  time/time_util.cc
)

set (WIN_SRC
  win/registry.h
  win/registry.cc
  win/resource_util.h
  win/resource_util.cc
  win/scoped_handle.h
  win/scoped_handle.cc
  win/security_util.h
  win/security_util.cc
  win/sysinfo.h
  win/sysinfo.cc
  win/windows_util.h
  win/windows_util.cc
  win/scoped_co_mem.h
  win/windows_version.h
  win/windows_version.cc
  win/nt_function_util.h
)




set (SOURCES
  ${BUSINESS_SRC}
  ${CHECKSUM_SRC}
  ${ENCRYPT_SRC}
  ${LOG_SRC}
  ${STRINGS_SRC}
  ${TEST_SRC}
  ${TIME_SRC}
  ${WIN_SRC}
  ${FILE_SRC}
  base_export.h
  command_line.h
  command_line.cc
  environment.h
  environment.cc
  file_version_info.h
  file_version_info_win.h
  file_version_info_win.cc
  macros.h
  version.h
  version.cc
)

#
# 一次性添加h cpp文件
#
# file(GLOB_RECURSE project_headers *.h)
# file(GLOB_RECURSE project_cpps *.cpp)
# set(all_files ${project_headers} ${project_cpps})
# source_group_by_dir(all_files)

source_group_by_dir(SOURCES)

link_libraries(console)
add_library(base STATIC ${SOURCES} )