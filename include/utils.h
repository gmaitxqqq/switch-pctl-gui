/**
 * @file utils.h
 * @brief Utility functions for the Switch parental control GUI
 */

#ifndef SWITCH_PCTL_GUI_UTILS_H_
#define SWITCH_PCTL_GUI_UTILS_H_

#include <string>
#include <cstdint>

/**
 * @namespace Utils
 * @brief Utility functions namespace
 */
namespace Utils {

    /**
     * @brief Format time from nanoseconds to human-readable string
     * @param nanoseconds Time in nanoseconds
     * @return Formatted string (e.g., "2小时30分钟", "45分钟", "0分钟")
     */
    std::string formatTime(int64_t nanoseconds);

    /**
     * @brief Format PIN length to human-readable string
     * @param length PIN length in bits (e.g., 4, 6, 8)
     * @return Formatted string (e.g., "4位", "6位")
     */
    std::string formatPinLength(int length);

    /**
     * @brief Convert time limit raw value to nanoseconds
     * @param rawValue Raw value from PlayTimerSettings.raw[i]
     * @return Time in nanoseconds
     * @note The actual encoding format may vary
     */
    int64_t rawToNanoseconds(uint16_t rawValue);

    /**
     * @brief Convert nanoseconds to raw value for PlayTimerSettings
     * @param nanoseconds Time in nanoseconds
     * @return Raw value for PlayTimerSettings.raw[i]
     */
    uint16_t nanosecondsToRaw(int64_t nanoseconds);

}  // namespace Utils

#endif  // SWITCH_PCTL_GUI_UTILS_H_
