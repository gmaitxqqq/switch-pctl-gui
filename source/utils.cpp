/**
 * @file utils.cpp
 * @brief 工具函数实现
 */

#include "utils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace Utils {

    std::string formatTime(int64_t nanoseconds) {
        if (nanoseconds <= 0) {
            return "0分钟";
        }

        int64_t totalSeconds = nanoseconds / 1000000000LL;
        int hours = static_cast<int>(totalSeconds / 3600);
        int minutes = static_cast<int>((totalSeconds % 3600) / 60);

        std::ostringstream oss;

        if (hours > 0) {
            oss << hours << "小时";
        }

        if (minutes > 0 || hours == 0) {
            if (hours > 0) {
                oss << " ";
            }
            oss << minutes << "分钟";
        }

        // 小于1分钟但大于0秒
        if (hours == 0 && minutes == 0 && totalSeconds > 0) {
            oss << "<1分钟";
        }

        return oss.str();
    }

    std::string formatPinLength(int length) {
        if (length <= 0) {
            return "未知";
        }
        return std::to_string(length) + "位";
    }

}  // namespace Utils
