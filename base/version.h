#pragma once
#include <string>
#include <vector>

#include "base/base_export.h"

namespace base
{
    class BASE_EXPORT Version
    {
    public:
        Version(/* args */);
        ~Version();

        // Initializes from a decimal dotted version number, like "0.1.1".
        // Each component is limited to a uint16_t. Call IsValid() to learn
        // the outcome.
        explicit Version(const std::string& version_str);

        // Returns true if the object contains a valid version number.
        bool IsValid() const;

        // Returns true if the version wildcard string is valid. The version
        // wildcard string may end with ".*" (e.g. 1.2.*, 1.*). Any other
        // arrangement with "*" is invalid (e.g. 1.*.3 or 1.2.3*). This
        // functions defaults to standard Version behavior (IsValid) if no
        // wildcard is present.
        static bool IsValidWildcardString(const std::string& wildcard_string);

        // Commonly used pattern. Given a valid version object, compare if a
        // |version_str| results in a newer version. Returns true if the
        // string represents valid version and if the version is greater than
        // than the version of this object.
        bool IsOlderThan(const std::string& version_str) const;

        bool Equals(const Version& other) const;

        // Returns -1, 0, 1 for <, ==, >.
        int CompareTo(const Version& other) const;

        // Given a valid version object, compare if a |wildcard_string| results
        // in a newer version. This function will default to CompareTo if the
        // string does not end in wildcard sequence ".*".
        // IsValidWildcard(wildcard_string) must be true before using this
        // function.
        int CompareToWildcardString(const std::string& wildcard_string) const;

        // Return the string representation of this version.
        const std::string GetString() const;

        const std::vector<uint32_t>& components() const { return components_; }

    private:
        std::vector<uint32_t> components_;
    };
}  // namespace base