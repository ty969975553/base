#include "base/win/resource_util.h"

namespace base
{
    namespace win
    {
        bool GetResourceFromModuleByName(HMODULE module, const char* res_name,
                                         const char* res_type, void** data,
                                         size_t* length)
        {
            if (!module) module = ::GetModuleHandle(NULL);

            if (!module) return false;

            HRSRC hres_info = ::FindResourceA(module, res_name, res_type);
            if (!hres_info) return false;

            DWORD data_size = SizeofResource(module, hres_info);
            HGLOBAL hres = ::LoadResource(module, hres_info);
            if (!hres) return false;

            void* resource = LockResource(hres);
            if (!resource) return false;

            *data = resource;
            *length = static_cast<size_t>(data_size);

            return true;
        }

        bool GetResourceFromModuleById(HMODULE module, int resource_id,
                                       LPCTSTR resource_type, void** data,
                                       size_t* length)
        {
            if (!module) module = ::GetModuleHandle(NULL);

            if (!module) return false;

            if (!IS_INTRESOURCE(resource_id))
            {
                return false;
            }

            HRSRC hres_info = FindResource(module, MAKEINTRESOURCE(resource_id),
                                           resource_type);
            if (NULL == hres_info) return false;

            DWORD data_size = SizeofResource(module, hres_info);
            HGLOBAL hres = LoadResource(module, hres_info);
            if (!hres) return false;

            void* resource = LockResource(hres);
            if (!resource) return false;

            *data = resource;
            *length = static_cast<size_t>(data_size);
            return true;
        }

        bool BASE_EXPORT GetDataResourceFromModuleById(HMODULE module,
                                                       int resource_id,
                                                       void** data,
                                                       size_t* length)
        {
            return GetResourceFromModuleById(module, resource_id, L"BINDATA",
                                             data, length);
        }

        bool LoadStringResource(HMODULE module, unsigned int id,
                                std::string* str)
        {
            if (!module) module = ::GetModuleHandle(NULL);

            if (!module) return false;

            // if (str) str->empty();

            // TODO LoadStringA
            // int nCount = sizeof(szTemp) / sizeof(szTemp[0]);
            // int nLen = ::(module, id, szTemp, nCount);
            // if()
            return false;
        }
    }  // namespace win
}  // namespace base