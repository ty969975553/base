#include "base/log/logging.h"

#include "base/strings/string_util.h"

namespace logging
{
    const char* LevelHelper::convertToString(Level level)
    {
        if (level == Level::Global) return "GLOBAL";
        if (level == Level::Debug) return "DEBUG";
        if (level == Level::Info) return "INFO";
        if (level == Level::Warning) return "WARNING";
        if (level == Level::Error) return "ERROR";
        if (level == Level::Fatal) return "FATAL";
        if (level == Level::Verbose) return "VERBOSE";
        if (level == Level::Trace) return "TRACE";
        return "UNKNOWN";
    }

    struct StringToLevelItem
    {
        const char* levelString;
        Level level;
    };

    static struct StringToLevelItem stringToLevelMap[] = {
        {"global", Level::Global},   {"debug", Level::Debug},
        {"info", Level::Info},       {"warning", Level::Warning},
        {"error", Level::Error},     {"fatal", Level::Fatal},
        {"verbose", Level::Verbose}, {"trace", Level::Trace}};

    Level LevelHelper::convertFromString(const char* levelStr)
    {
        for (auto& item : stringToLevelMap)
        {
            if (StringCompareNoCase(levelStr, item.levelString))
            {
                return item.level;
            }
        }
        return Level::Unknown;
    }

    void LevelHelper::forEachLevel(unsigned int* startIndex,
                                   const std::function<bool(void)>& fn)
    {
        unsigned int lIndexMax = LevelHelper::kMaxValid;
        do
        {
            if (fn())
            {
                break;
            }
            *startIndex = static_cast<unsigned int>(*startIndex << 1);
        } while (*startIndex <= lIndexMax);
    }

    const char* ConfigurationTypeHelper::convertToString(
        ConfigurationType configurationType)
    {
        if (configurationType == ConfigurationType::Enabled) return "ENABLED";
        if (configurationType == ConfigurationType::Filename) return "FILENAME";
        if (configurationType == ConfigurationType::Format) return "FORMAT";
        if (configurationType == ConfigurationType::ToFile) return "TO_FILE";
        if (configurationType == ConfigurationType::ToStandardOutput)
            return "TO_STANDARD_OUTPUT";
        if (configurationType == ConfigurationType::SubsecondPrecision)
            return "SUBSECOND_PRECISION";
        if (configurationType == ConfigurationType::PerformanceTracking)
            return "PERFORMANCE_TRACKING";
        if (configurationType == ConfigurationType::MaxLogFileSize)
            return "MAX_LOG_FILE_SIZE";
        if (configurationType == ConfigurationType::LogFlushThreshold)
            return "LOG_FLUSH_THRESHOLD";
        return "UNKNOWN";
    }

    struct ConfigurationStringToTypeItem
    {
        const char* configString;
        ConfigurationType configType;
    };

    static struct ConfigurationStringToTypeItem configStringToTypeMap[] = {
        {"enabled", ConfigurationType::Enabled},
        {"to_file", ConfigurationType::ToFile},
        {"to_standard_output", ConfigurationType::ToStandardOutput},
        {"format", ConfigurationType::Format},
        {"filename", ConfigurationType::Filename},
        {"subsecond_precision", ConfigurationType::SubsecondPrecision},
        {"milliseconds_width", ConfigurationType::MillisecondsWidth},
        {"performance_tracking", ConfigurationType::PerformanceTracking},
        {"max_log_file_size", ConfigurationType::MaxLogFileSize},
        {"log_flush_threshold", ConfigurationType::LogFlushThreshold},
    };

    ConfigurationType ConfigurationTypeHelper::convertFromString(
        const char* configStr)
    {
        for (auto& item : configStringToTypeMap)
        {
            if (StringCompareNoCase(configStr, item.configString))
            {
                return item.configType;
            }
        }
        return ConfigurationType::Unknown;
    }

    inline void ConfigurationTypeHelper::forEachConfigType(
        unsigned int* startIndex, const std::function<bool(void)>& fn)
    {
        unsigned int cIndexMax = ConfigurationTypeHelper::kMaxValid;
        do
        {
            if (fn())
            {
                break;
            }
            *startIndex = static_cast<unsigned int>(*startIndex << 1);
        } while (*startIndex <= cIndexMax);
    }
}  // namespace logging