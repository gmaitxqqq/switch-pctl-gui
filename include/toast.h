/**
 * @file toast.h
 * @brief Toast 通知（底部临时消息）
 */

#ifndef SWITCH_PCTL_GUI_TOAST_H_
#define SWITCH_PCTL_GUI_TOAST_H_

#include <borealis.hpp>
#include <string>

class Toast {
public:
    /**
     * @brief 显示 Toast 消息（2秒后自动消失）
     */
    static void show(const std::string& message);

    /**
     * @brief 显示 Toast 消息（自定义时长）
     */
    static void show(const std::string& message, int durationMs);
};

#endif  // SWITCH_PCTL_GUI_TOAST_H_
