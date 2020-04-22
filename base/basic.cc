#include "basic.h"

namespace base {
/**
NotCopyable **/
NotCopyable::NotCopyable() {}

NotCopyable::NotCopyable(const NotCopyable&) {}

NotCopyable& NotCopyable::operator=(const NotCopyable&) { return *this; }


}  // namespace base