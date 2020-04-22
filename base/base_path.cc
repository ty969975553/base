#pragma once

#include "base/base_path.h"

namespace base {

bool PathProvider(int key, FilePath* result) {
  // NOTE: DIR_CURRENT is a special case in PathService::Get

  switch (key) {
    case DIR_EXE:
      PathService::Get(FILE_EXE, result);
      *result = result->DirName();
      return true;
    case DIR_MODULE:
      PathService::Get(FILE_MODULE, result);
      *result = result->DirName();
      return true;
    case DIR_TEMP:
      if (!GetTempDir(result))
        return false;
      return true;
    case base::DIR_HOME:
      *result = GetHomeDir();
      return true;
    case DIR_TEST_DATA: {
      FilePath test_data_path;
      if (!PathService::Get(DIR_SOURCE_ROOT, &test_data_path))
        return false;
      test_data_path = test_data_path.Append(FILE_PATH_LITERAL("base"));
      test_data_path = test_data_path.Append(FILE_PATH_LITERAL("test"));
      test_data_path = test_data_path.Append(FILE_PATH_LITERAL("data"));
      if (!PathExists(test_data_path))  // We don't want to create this.
        return false;
      *result = test_data_path;
      return true;
    }
    default:
      return false;
  }
}