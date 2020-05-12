#pragma once
#include <stddef.h>
#include <windows.h>

#include <string>
#include <vector>

#include "base/base_export.h"

namespace base
{
    namespace win
    {
        bool BASE_EXPORT GetResourceFromModuleByName(HMODULE module,
                                                     const char* res_name,
                                                     const char* res_type,
                                                     void** data,
                                                     size_t* length);

        bool BASE_EXPORT GetResourceFromModuleById(HMODULE module,
                                                   int resource_id,
                                                   LPCTSTR resource_type,
                                                   void** data, size_t* length);

        bool BASE_EXPORT GetDataResourceFromModuleById(HMODULE module,
                                                       int resource_id,
                                                       void** data,
                                                       size_t* length);

        bool BASE_EXPORT LoadStringResource(HMODULE module, unsigned int id,
                                            std::string* str);
    }  // namespace win
}  // namespace base