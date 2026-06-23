/**
 * @file time_limit_frame.cpp
 * @brief 时长设置界面 - 用列表选择预设值
 */

#include "time_limit_frame.h"
#include "pctl_manager.h"
#include "confirm_dialog.h"
#include "toast.h"
#include "i18n.h"
#include "utils.h"

// 预设时长（分钟）
static const int PRESET_MINUTES[] = {5, 15, 20, 30, 45, 60, 90, 120, 180};
static const char* PRESET_LABELS[] = {
    "5 分钟", "15 分钟", "20 分钟", "30 分钟",
    "45 分钟", "1 小时", "1.5 小时", "2 小时", "3 小时"
};
static const int NUM_PRESETS = sizeof(PRESET_MINUTES) / sizeof(PRESET_MINUTES[0]);

TimeLimitFrame::TimeLimitFrame() {
    setTitle("设置游玩时长限制");

    brls::Label* desc = new brls::Label(brls::LabelStyle::DESCRIPTION,
        "选择每日游玩时长限制（所有天相同）", false);
    desc->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    desc->setMargins(20, 20, 10, 20);
    addView(desc);

    // 当前设置
    PctlManager& mgr = PctlManager::getInstance();
    int64_t currentLimit = mgr.getTimeLimit();
    int currentMinutes = static_cast<int>(currentLimit / 60000000000LL);  // ns -> minutes

    brls::Label* currentLabel = new brls::Label(brls::LabelStyle::SMALL,
        "当前设置: " + (mgr.hasTimeLimit() ? Utils::formatTime(currentLimit) : "未设置"), false);
    currentLabel->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    currentLabel->setMargins(0, 20, 15, 20);
    addView(currentLabel);

    // 预设列表
    brls::List* presetList = new brls::List();
    presetList->setMargins(0, 20, 10, 20);

    for (int i = 0; i < NUM_PRESETS; i++) {
        int minutes = PRESET_MINUTES[i];
        brls::ListItem* item = new brls::ListItem(
            PRESET_LABELS[i],
            "",
            nullptr
        );
        int idx = i;  // capture by value
        item->setClickListener([minutes, idx](brls::View* view) {
            ConfirmDialog* dialog = new ConfirmDialog(
                "确认应用",
                "设置每日游玩时长为 " + std::string(PRESET_LABELS[idx]) + "？",
                false
            );
            dialog->setConfirmCallback([minutes]() {
                PctlManager& mgr = PctlManager::getInstance();
                int h = minutes / 60;
                int m = minutes % 60;
                if (mgr.setDailyTimeLimit(h, m, true)) {
                    Toast::show("设置成功");
                    brls::Application::popFrame();
                } else {
                    Toast::show("设置失败");
                }
            });
            dialog->setCancelCallback(nullptr);
            dialog->show();
        });
        presetList->addView(item);
    }

    // 清除限制
    brls::ListItem* clearItem = new brls::ListItem(
        "⚠ 清除时长限制",
        "关闭每日游玩时长限制",
        nullptr
    );
    clearItem->setClickListener([](brls::View* view) {
        ConfirmDialog* dialog = new ConfirmDialog(
            "确认清除",
            "确定要清除每日游玩时长限制吗？",
            true
        );
        dialog->setConfirmCallback([]() {
            PctlManager& mgr = PctlManager::getInstance();
            if (mgr.clearTimeLimit()) {
                Toast::show("已清除");
                brls::Application::popFrame();
            } else {
                Toast::show("操作失败");
            }
        });
        dialog->setCancelCallback(nullptr);
        dialog->show();
    });
    presetList->addView(clearItem);

    addView(presetList);
}

TimeLimitFrame::~TimeLimitFrame() {
}
