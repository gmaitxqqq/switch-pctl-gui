/**
 * @file pin_change_frame.cpp
 * @brief PIN 修改界面 - 启动系统 applet
 */

#include "pin_change_frame.h"
#include "pctl_manager.h"
#include "toast.h"
#include "i18n.h"

PinChangeFrame::PinChangeFrame() {
    setTitle(i18n_get("menu_change_pin"));

    brls::Label* desc = new brls::Label(brls::LabelStyle::DESCRIPTION,
        "将启动系统 PIN 修改界面。\n请按照屏幕提示操作。", false);
    desc->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    desc->setMargins(50, 20, 30, 20);
    addView(desc);

    brls::Button* startBtn = new brls::Button("启动 PIN 修改");
    startBtn->setMarginTop(20);
    startBtn->setClickListener([](brls::View* view) {
        PctlManager& mgr = PctlManager::getInstance();
        if (mgr.launchPinChangeApplet()) {
            Toast::show("已启动，请查看 Switch 屏幕");
        } else {
            Toast::show("启动失败");
        }
        brls::Application::popFrame();
    });
    addView(startBtn);

    brls::Button* backBtn = new brls::Button("返回");
    backBtn->setMarginTop(10);
    backBtn->setClickListener([](brls::View* view) {
        brls::Application::popFrame();
    });
    addView(backBtn);
}

PinChangeFrame::~PinChangeFrame() {
}
