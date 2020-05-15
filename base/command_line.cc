#include "base/command_line.h"

#include <iostream>
#include <utility>

#include "base/macros.h"
#include "base/strings/string_util.h"
#include "ui/console/console.h"

namespace base
{
#if defined(OS_WIN)
    const wchar_t kSwitchTerminator[] = L"--";
    const wchar_t kSwitchValueSeparator[] = L"=";

    const wchar_t* const kSwitchPrefixes[] = {L"--", L"-", L"/"};
#else
    const char kSwitchTerminator[] = ("--");
    const char kSwitchValueSeparator[] = ("=");

    const char* const kSwitchPrefixes[] = {"--", "-", "/"};
#endif

    size_t switch_prefix_count = arraysize(kSwitchPrefixes);

    size_t GetSwitchPrefixLength(const StringView& string)
    {
        for (size_t i = 0; i < switch_prefix_count; ++i)
        {
            StringView prefix(kSwitchPrefixes[i]);
            if (string.compare(0, prefix.length(), prefix) == 0)
                return prefix.length();
        }
        return 0;
    }

    CommandLine::CommandLine(int argc, const CharType* const* argv)
    {
        InitFromArgv(argc, argv);
    }

    void CommandLine::PrintUsage() const
    {
        if (!print_usage_)
        {
            ui::console::WriteLine(L"no parameter description£¡");
        }
        else
        {
            ui::console::WriteLine(L"usage : [" + std::to_wstring(usages_.size()) + L"]");
            for (auto item : usages_)
            {
#if defined(OS_WIN)
                ui::console::Write(L"par : -" + item.para);
                ui::console::WriteLine(L"description : " + item.description);
#else

#endif  // defined(OS_WIN)              
            }
        }
    }

    void CommandLine::SetCommandInfos(const UsageVector& infos) {
        print_usage_ = true;
        usages_ = infos;
        PrintUsage();
    }

    void CommandLine::PrintParseOptions() const
    {
        ui::console::WriteLine(L"------PrintParseOptions begin------");
        for (auto item : options_)
        {   
            ui::console::WriteLine(L"flag: " + std::wstring(item.first));
            ui::console::WriteLine(L"value: " +
                                   std::wstring(item.second.value_or(L"")));
        }
        ui::console::WriteLine(L"------PrintParseOptions end------");
    }

    void CommandLine::InitFromArgv(int argc, const CharType* const* argv)
    {
        agrs_.clear();
        options_.clear();
        for (int i = 0; i < argc; ++i)
        {
            agrs_.push_back(argv[i]);
        }
        InitFromArgv(agrs_);
    }

    void CommandLine::InitFromArgv(StringVector& argv)
    {
        for (auto& item : argv)
        {
            ParseParameter(item);
        }
    }

    bool CommandLine::HasOption(const StringView& flag) const
    {
        return options_.find(flag) != options_.end();
    }

    bool CommandLine::HasOption(const CharType flag[]) const
    {
        return HasOption(StringView(flag));
    }

    bool CommandLine::ParseParameter(StringView arg)
    {
        size_t prefix_length = GetSwitchPrefixLength(arg);
        if (prefix_length == 0 || prefix_length == arg.length()) return false;

        const size_t equals_position = arg.find(kSwitchValueSeparator);

        StringView flag_view =
            arg.substr(prefix_length, equals_position - prefix_length);
        if (equals_position != StringView::npos)
        {
            StringView value_view = arg.substr(equals_position + 1);
            std::optional<StringView> value = value_view;
            if (options_.count(flag_view) != 0)
            {
                options_[flag_view] = value;
            }
            else
            {
                options_.emplace(std::make_pair(flag_view, value));
            }
        }
        else
        {
            options_.emplace(std::make_pair(flag_view, std::nullopt));
        }
        return true;
    }
}  // namespace base