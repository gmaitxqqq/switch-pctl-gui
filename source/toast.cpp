/**
 * @file toast.cpp
 * @brief Toast 通知 - 简化版（Dialog + OK 按钮）
 */

#include "toast.h"

void Toast::show(const std::string& message) {
    Toast::show(message, 2000);
}

void Toast::show(const std::string& message, int durationMs) {
    // 简化实现：用 brls::Dialog 显示消息，用户按 OK 关闭
    // 后续可改为真正的自动消失 Toast
    brls::Dialog* dialog = new brls::Dialog(message);
    dialog->addButton("OK", [](brls::Dialog* dialog) {
        dialog->close();
    });
    dialog->open();
}
