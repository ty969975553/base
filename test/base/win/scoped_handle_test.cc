#include "base/win/scoped_handle.h"

#include <iostream>
#include <string>

#include "base/test/unittest.h"
#include "ui/console/console.h"

using namespace base::win;

namespace Test
{
    TEST_FILE
    {
        TEST_CATEGORY(L"ScopedHandleTest")
        {
            TEST_CASE(L"ScopedHandle")
            {
                const DWORD magic_error = 0x12345678;

                HANDLE handle = ::CreateMutex(nullptr, FALSE, nullptr);
                // Call SetLastError after creating the handle.
                ::SetLastError(magic_error);
                base::win::ScopedHandle handle_holder(handle);
                TEST_ASSERT(magic_error == ::GetLastError());

                // Create a new handle and then set LastError again.
                handle = ::CreateMutex(nullptr, FALSE, nullptr);
                ::SetLastError(magic_error);
                handle_holder.Set(handle);
                TEST_ASSERT(magic_error == ::GetLastError());

                // Create a new handle and then set LastError again.
                handle = ::CreateMutex(nullptr, FALSE, nullptr);
                base::win::ScopedHandle handle_source(handle);
                ::SetLastError(magic_error);
                handle_holder = handle_source.Pass();
                TEST_ASSERT(magic_error == ::GetLastError());
            });
        });
    }
}  // namespace Test