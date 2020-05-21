#pragma once

#include <objbase.h>

#include "base/macros.h"

namespace base
{
    namespace win
    {
        template <typename T>
        class ScopedCoMem
        {
        public:
            ScopedCoMem() : mem_ptr_(NULL) {}
            ~ScopedCoMem() { Reset(NULL); }

            T** operator&()
            {  // NOLINT
                return &mem_ptr_;
            }

            operator T*() { return mem_ptr_; }

            T* operator->() { return mem_ptr_; }

            const T* operator->() const { return mem_ptr_; }

            void Reset(T* ptr)
            {
                if (mem_ptr_) CoTaskMemFree(mem_ptr_);
                mem_ptr_ = ptr;
            }

            T* get() const { return mem_ptr_; }

        private:
            T* mem_ptr_;

            DISALLOW_COPY_AND_ASSIGN(ScopedCoMem);
        };

    }  // namespace win
}  // namespace base
