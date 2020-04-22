#pragma once
#include <iterator>

namespace base
{
    template <typename T>
    class StringT
    {
    public:
        typedef size_t size_type;
        typedef typename T::value_type value_type;
        typedef const value_type* poniter;
        typedef const value_type& reference;
        typedef const value_type& const_reference;
        typedef ptrdiff_t difference_type;
        typedef const value_type* const_iterator;
        typedef const std::reverse_iterator<const_iterator>
            const_reverse_iterator;

        static const size_type npos;
    protected:
        const value_type* ptr;
        size_type length_;
    public:
        StringT():ptr_(NULL),length_(0){}
        StringT(const value_type* str):ptr_(str),length_(str ? 0 : T::Traits_type)

    }
}  // namespace base