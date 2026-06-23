/**
 * @file confirm_dialog.h
 * @brief 确认对话框 - 封装 brls::Dialog
 */

#ifndef SWITCH_PCTL_GUI_CONFIRM_DIALOG_H_
#define SWITCH_PCTL_GUI_CONFIRM_DIALOG_H_

#include <borealis.hpp>
#include <functional>
#include <string>

class ConfirmDialog {
public:
    ConfirmDialog(const std::string& title, const std::string& content, bool dangerMode = false);
    ~ConfirmDialog();

    void setConfirmCallback(std::function<void()> callback);
    void setCancelCallback(std::function<void()> callback);
    void show();

private:
    brls::Dialog* m_dialog;
    std::function<void()> m_confirmCallback;
    std::function<void()> m_cancelCallback;
};

#endif  // SWITCH_PCTL_GUI_CONFIRM_DIALOG_H_
