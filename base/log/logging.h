#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "base/base_export.h"
#include "base/macros.h"
#include "config/build_config.h"

namespace logging
{
    enum class log_level : unsigned int
    {
        TRACE = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        FATAL = 4
    };

    std::map<std::string, log_level> data = {{"INFO", log_level::INFO},
                                             {"WARN", log_level::WARN},
                                             {"ERROR", log_level::ERROR},
                                             {"FATAL", log_level::FATAL}};

    struct log_format
    {
    };

    struct log_dest_option
    {
        bool auto_cut;
        enum class cut_mode
        {
            daily, size
        };
        cut_mode mode;
        bool max_log_size;
        bool stop_logging_if_full_disk;
    };

    struct log_setting
    {
        log_format format;
        log_dest_option dest_option;
        bool sync;
        int min_level;
        std::string log_dir;
    };
    BASE_EXPORT InitLogging(const log_setting& setting);
    BASE_EXPORT ShutdownLogging();
    BASE_EXPORT InstallFailureFunction(std::function& fail_func);

    class Logger 
    {
    public:
        constexpr  int GetLogLevel()
        {
            return level_;
        };
    private:
        log_level  level_;     
    };
}  // namespace logging


#define LOG(LEVEL)  if(logging::log_level::##LEVEL >= GetLogLevel()) /
logger(__FILE,__LINE)
