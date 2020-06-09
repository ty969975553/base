#pragma once

#include <windows.h>

#include "base/base_export.h"
#include "base/macros.h"
#include "config/build_config.h"
#include "config/compiler_specific.h"

// TODO(rvargas): remove this with the rest of the verifier.
#if defined(COMPILER_MSVC)
#include <intrin.h>
#define BASE_WIN_GET_CALLER _ReturnAddress()
#elif defined(COMPILER_GCC)
#define BASE_WIN_GET_CALLER \
    __builtin_extract_return_addr(\ __builtin_return_address(0))
#endif

// TODO(crbug.com/566182): DEPRECATED!
// Use DISALLOW_COPY_AND_ASSIGN instead, or if your type will be used in
// Callbacks, use DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND instead.
#define MOVE_ONLY_TYPE_FOR_CPP_03(type) \
    DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND(type)

// A macro to disallow the copy constructor and copy assignment functions.
// This should be used in the private: declarations for a class.
//
// Use this macro instead of DISALLOW_COPY_AND_ASSIGN if you want to pass
// ownership of the type through a base::Callback without heap-allocating it
// into a scoped_ptr.  The class must define a move constructor and move
// assignment operator to make this work.
//
// This version of the macro adds a Pass() function and a cryptic
// MoveOnlyTypeForCPP03 typedef for the base::Callback implementation to use.
// See IsMoveOnlyType template and its usage in base/callback_internal.h
// for more details.
// TODO(crbug.com/566182): Remove this macro and use DISALLOW_COPY_AND_ASSIGN
// everywhere instead.
#if defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_MACOSX)
#define DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND(type) \
private:                                                  \
    type(const type&) = delete;                           \
    void operator=(const type&) = delete;                 \
                                                          \
public:                                                   \
    typedef void MoveOnlyTypeForCPP03;                    \
                                                          \
private:
#else
#define DISALLOW_COPY_AND_ASSIGN_WITH_MOVE_FOR_BIND(type)         \
private:                                                          \
    type(const type&) = delete;                                   \
    void operator=(const type&) = delete;                         \
                                                                  \
public:                                                           \
    type&& Pass() WARN_UNUSED_RESULT { return std::move(*this); } \
    typedef void MoveOnlyTypeForCPP03;                            \
                                                                  \
private:
#endif

namespace base
{
    namespace win
    {
        template <class Traits, class Verifier>
        class GenericScopedHandle
        {
            MOVE_ONLY_TYPE_FOR_CPP_03(GenericScopedHandle)

        public:
            typedef typename Traits::Handle Handle;

            GenericScopedHandle() : handle_(Traits::NullHandle()) {}

            explicit GenericScopedHandle(Handle handle)
                : handle_(Traits::NullHandle())
            {
                Set(handle);
            }

            GenericScopedHandle(GenericScopedHandle&& other)
                : handle_(Traits::NullHandle())
            {
                Set(other.Take());
            }

            ~GenericScopedHandle() { Close(); }

            bool IsValid() const { return Traits::IsHandleValid(handle_); }

            GenericScopedHandle& operator=(GenericScopedHandle&& other)
            {
                Set(other.Take());
                return *this;
            }

            void Set(Handle handle)
            {
                if (handle_ != handle)
                {
                    // Preserve old LastError to avoid bug 528394.
                    auto last_error = ::GetLastError();
                    Close();

                    if (Traits::IsHandleValid(handle))
                    {
                        handle_ = handle;
                        Verifier::StartTracking(handle, this,
                                                BASE_WIN_GET_CALLER,
                                                BASE_WIN_GET_CALLER);
                    }
                    ::SetLastError(last_error);
                }
            }

            Handle Get() const { return handle_; }

            // Transfers ownership away from this object.
            Handle Take()
            {
                Handle temp = handle_;
                handle_ = Traits::NullHandle();
                if (Traits::IsHandleValid(temp))
                {
                    Verifier::StopTracking(temp, this, BASE_WIN_GET_CALLER,
                                           BASE_WIN_GET_CALLER);
                }
                return temp;
            }

            // Explicitly closes the owned handle.
            void Close()
            {
                if (Traits::IsHandleValid(handle_))
                {
                    Verifier::StopTracking(handle_, this, BASE_WIN_GET_CALLER,
                                           BASE_WIN_GET_CALLER);

                    Traits::CloseHandle(handle_);
                    handle_ = Traits::NullHandle();
                }
            }

        private:
            Handle handle_;
        };

        class HandleTraits
        {
        public:
            typedef HANDLE Handle;

            // Closes the handle.
            static bool BASE_EXPORT CloseHandle(HANDLE handle);

            // Returns true if the handle value is valid.
            static bool IsHandleValid(HANDLE handle)
            {
                return handle != NULL && handle != INVALID_HANDLE_VALUE;
            }

            // Returns NULL handle value.
            static HANDLE NullHandle() { return NULL; }

        private:
            DISALLOW_IMPLICIT_CONSTRUCTORS(HandleTraits);
        };

        // Performs actual run-time tracking.
        class BASE_EXPORT VerifierTraits
        {
        public:
            typedef HANDLE Handle;

            static void StartTracking(HANDLE handle, const void* owner,
                                      const void* pc1, const void* pc2);
            static void StopTracking(HANDLE handle, const void* owner,
                                     const void* pc1, const void* pc2);

        private:
            DISALLOW_IMPLICIT_CONSTRUCTORS(VerifierTraits);
        };

        typedef GenericScopedHandle<HandleTraits, VerifierTraits> ScopedHandle;
    }  // namespace win
}  // namespace base