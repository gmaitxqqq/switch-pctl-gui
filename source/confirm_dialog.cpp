/**
 * @file confirm_dialog.cpp
 * @brief 确认对话框实现
 */

#include "confirm_dialog.h"

ConfirmDialog::ConfirmDialog(const std::string& title, const std::string& content, bool dangerMode)
    : m_confirmCallback(nullptr), m_cancelCallback(nullptr) {
    m_dialog = new brls::Dialog(content);

    // 取消按钮（B 键）
    m_dialog->addButton(i18n_get("btn_cancel"), [this](brls::Dialog* dialog) {
        dialog->close();
        if (m_cancelCallback) {
            m_cancelCallback();
        }
    });

    // 确认按钮（A 键）- 危险模式用红色
    std::string confirmText = i18n_get("btn_confirm");
    if (dangerMode) {
        // borealis 支持按钮样式，这里用文本标识
        confirmText = "⚠ " + confirmText;
    }
    m_dialog->addButton(confirmText, [this](brls::Dialog* dialog) {
        dialog->close();
        if (m_confirmCallback) {
            m_confirmCallback();
        }
    });
}

ConfirmDialog::~ConfirmDialog() {
    // dialog 由 borealis 内存管理
}

void ConfirmDialog::setConfirmCallback(std::function<void()> callback) {
    m_confirmCallback = callback;
}

void ConfirmDialog::setCancelCallback(std::function<void()> callback) {
    m_cancelCallback = callback;
}

void ConfirmDialog::show() {
    m_dialog->open();
}
