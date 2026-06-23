/**
 * @file status_view.cpp
 * @brief 状态卡片实现 - 使用 brls::Label 显示状态
 */

#include "status_view.h"
#include "pctl_manager.h"
#include "utils.h"
#include "i18n.h"

StatusView::StatusView()
    : brls::Box(brls::Box::VERTICAL)
    , m_pinLabel(nullptr)
    , m_restrictionLabel(nullptr)
    , m_timeLimitLabel(nullptr)
    , m_remainingLabel(nullptr) {

    setSpacing(8);
    setMargins(12, 16, 12, 16);

    // 卡片背景会在 draw() 里画，或者依赖 borealis 默认样式
    // 这里只用 Label 显示文字

    m_pinLabel = new brls::Label(brls::LabelStyle::SMALL, "", false);
    addView(m_pinLabel);

    m_restrictionLabel = new brls::Label(brls::LabelStyle::SMALL, "", false);
    addView(m_restrictionLabel);

    m_timeLimitLabel = new brls::Label(brls::LabelStyle::SMALL, "", false);
    addView(m_timeLimitLabel);

    m_remainingLabel = new brls::Label(brls::LabelStyle::SMALL, "", false);
    addView(m_remainingLabel);

    updateStatus();
}

StatusView::~StatusView() {
}

void StatusView::updateStatus() {
    PctlManager& mgr = PctlManager::getInstance();

    if (!mgr.isInitialized()) {
        m_pinLabel->setText("PIN: 需要在真实 Switch 上运行");
        m_restrictionLabel->setText("家长控制: 未知");
        m_timeLimitLabel->setText("每日限制: 未知");
        m_remainingLabel->setText("");
        return;
    }

    // 1. PIN 长度
    m_pinLabel->setText("PIN 长度: " + Utils::formatPinLength(mgr.getPinLength()));

    // 2. 家长控制开关
    bool enabled = mgr.getRestrictionEnabled();
    if (enabled) {
        m_restrictionLabel->setText("家长控制: 已启用");
        m_restrictionLabel->setTextColor(nvgRGB(100, 255, 100));
    } else {
        m_restrictionLabel->setText("家长控制: 已禁用");
        m_restrictionLabel->setTextColor(nvgRGB(255, 100, 100));
    }

    // 3. 时长限制
    if (mgr.hasTimeLimit()) {
        m_timeLimitLabel->setText("每日限制: " + Utils::formatTime(mgr.getTimeLimit()));
    } else {
        m_timeLimitLabel->setText("每日限制: 未设置");
    }

    // 4. 剩余时间
    int64_t remaining = mgr.getRemainingPlayTime();
    if (remaining > 0) {
        m_remainingLabel->setText("剩余时间: " + Utils::formatTime(remaining));
    } else {
        m_remainingLabel->setText("剩余时间: --");
    }
}
