/**
 * @file status_view.h
 * @brief 状态卡片 - 显示家长控制当前状态
 */

#ifndef SWITCH_PCTL_GUI_STATUS_VIEW_H_
#define SWITCH_PCTL_GUI_STATUS_VIEW_H_

#include <borealis.hpp>

class StatusView : public brls::Box {
public:
    StatusView();
    ~StatusView() override;

    void updateStatus();

private:
    brls::Label* m_pinLabel;
    brls::Label* m_restrictionLabel;
    brls::Label* m_timeLimitLabel;
    brls::Label* m_remainingLabel;
};

#endif  // SWITCH_PCTL_GUI_STATUS_VIEW_H_
