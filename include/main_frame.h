/**
 * @file main_frame.h
 * @brief Main application frame - 使用 brls::List 实现可点击菜单
 */

#ifndef SWITCH_PCTL_GUI_MAIN_FRAME_H_
#define SWITCH_PCTL_GUI_MAIN_FRAME_H_

#include <borealis.hpp>

class MainFrame : public brls::Frame {
public:
    MainFrame();
    ~MainFrame() override;

    void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;
    void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height,
              brls::Style* style, brls::FrameContext* ctx) override;

private:
    // 刷新状态显示
    void refreshStatus();

    // 菜单动作
    void onTimeLimitClicked();
    void onToggleRestrictionClicked();
    void onChangePinClicked();
    void onDeleteConfigClicked();
    void onUnlinkAppClicked();
    void onHelpClicked();
};

#endif  // SWITCH_PCTL_GUI_MAIN_FRAME_H_
