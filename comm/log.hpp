#pragma once
#include <iostream>
#include <string>
#include "util.hpp"

namespace ns_log
{
    using namespace ns_util;
    // 日志等级
    enum
    {
        INFO,
        DEBUG,
        WARNING,
        ERROR,
        FATAL
    };

    inline std::ostream& Log(const std::string& level, const std::string& file_name, int line)
    {
        std::string message = "[";
        // 添加日志等级
        message += level;
        message += "]";

        // 添加报错的文件名
        message += "[";
        message += file_name;
        message += "]";

        // 添加报错行
        message += "[";
        message += std::to_string(line);
        message += "]";

        // 添加报错时间戳
        message += "[";
        message += TimeUtil::GetTimeStamp();
        message += "]";

        // cout 本质 内部是包含缓冲区的
        std::cout << message;

        return std::cout;
    }
    // LOG() << "message" << "\n";
    // 开放式日志
    #define LOG(level) Log(#level, __FILE__, __LINE__)
}