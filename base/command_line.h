#pragma once

#include <array>
#include <functional>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "base/base_export.h"
#include "config/build_config.h"

namespace base
{
#if defined(OS_WIN)
    typedef std::wstring StringType;
    typedef wchar_t CharType;
    typedef std::wstring_view StringView;
    constexpr std::array<const CharType*, 2> falsities{{L"0", L"false"}};
    typedef std::wistringstream StringStream;
#else
    typedef std::string StringType;
    typedef char CharType;
    typedef std::string_view StringView;
    constexpr std::array<const CharType*, 2> falsities{{"0", "false"}};
    typedef std::istringstream StringStream;
#endif
    typedef std::vector<StringType> StringVector;


    using option_map =
        std::unordered_map<StringView, std::optional<StringView>>;
        
    struct BASE_EXPORT command_line_info
    {
        StringType para;         // para
        StringType description;  //参数说明
        bool excute_return;      //执行后是否退出
        std::function<void(const StringType&)> callback;  // para 存在执行函数
        /* data */
    };

    typedef std::vector<command_line_info> UsageVector;

    class BASE_EXPORT CommandLine
    {
    public:
        CommandLine(int argc, const CharType* const* argv);

        void PrintUsage() const;

        void SetCommandInfos(const UsageVector& infos); 

        void PrintParseOptions() const;

        // Initialize from an argv vector.
        void InitFromArgv(int argc, const CharType* const* argv);
        void InitFromArgv(StringVector& argv);

        bool HasOption(const StringView& flag) const;
        bool HasOption(const CharType flag[]) const;

        std::optional<StringView> CommandLine::GetOption(const StringView& flag)
        {
            if (const auto it = options_.find(flag); it != options_.end())
            {
                return it->second;
            }
            return std::nullopt;
        }

        template <class T>
        std::optional<T> get(const StringView& flag)
        {
            if (const auto view = GetOption(flag))
            {
                if (T value; StringStream(StringType(*view)) >> value)
                    return value;
            }
            return std::nullopt;
        }

        template <>
        std::optional<bool> get(const StringView& flag)
        {
            if (const auto value = GetOption(flag))
            {
                return std::none_of(
                    falsities.begin(), falsities.end(),
                    [&value](auto falsity) { return *value == falsity; });
            }
            if (options_.find(flag) != options_.end()) return true;
            return std::nullopt;
        }

    private:
        bool ParseParameter(StringView arg);

    private:
        //是否打印命令行说明
        bool print_usage_ = false;
        option_map options_;
        StringVector agrs_;
        UsageVector usages_;
    };
}  // namespace base