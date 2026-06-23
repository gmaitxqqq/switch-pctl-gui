#include <borealis.hpp>
#include "main_activity.h"
#include "i18n.h"

// ── Forward declarations for click handlers ──
static bool onToggleRestriction(bool on);
static bool onTimeLimitClick(brls::View* view);
static bool onPinChangeClick(brls::View* view);
static bool onDeleteClick(brls::View* view);

// ── Constructor (minimal — only set defaults) ──
MainActivity::MainActivity() = default;

MainActivity::~MainActivity() = default;

// ── Create the content view (called by pushActivity → setContentView) ──
brls::View* MainActivity::createContentView() {
    // AppletFrame = title bar + content area
    brls::AppletFrame* frame = new brls::AppletFrame();

    // ScrollingFrame for scrollable content
    brls::ScrollingFrame* scroll = new brls::ScrollingFrame();

    // ── Status label (header) ──
    statusLabel = new brls::Label();
    statusLabel->setText("家长控制状态：检测中...");
    statusLabel->setFontSize(20);
    statusLabel->setMargins(20, 20, 10, 20);
    scroll->addView(statusLabel);

    // ── Toggle: enable/disable restriction ──
    toggleCell = new brls::BooleanCell();
    toggleCell->init("家长控制", "开启或关闭家长控制限制");
    toggleCell->setOn(false);  // default OFF
    toggleCell->registerClickAction(onToggleRestriction);
    scroll->addView(toggleCell);

    // ── Time limit setting ──
    timeLimitCell = new brls::DetailCell("游玩时长限制", "点击设置每日游玩时长");
    timeLimitCell->registerClickAction(onTimeLimitClick);
    scroll->addView(timeLimitCell);

    // ── PIN change ──
    pinCell = new brls::DetailCell("更改 PIN 码", "修改家长控制 PIN");
    pinCell->registerClickAction(onPinChangeClick);
    scroll->addView(pinCell);

    // ── Delete restriction ──
    deleteCell = new brls::DetailCell("删除限制", "移除所有家长控制限制");
    deleteCell->registerClickAction(onDeleteClick);
    scroll->addView(deleteCell);

    // ── Hint at bottom ──
    brls::Label* hint = new brls::Label();
    hint->setText("按 B 返回桌面");
    hint->setFontSize(16);
    hint->setMargins(20, 20, 20, 20);
    scroll->addView(hint);

    frame->setContentView(scroll);
    return frame;
}

// ── Lifecycle: content is now available ──
void MainActivity::onContentAvailable() {
    // Safe to access UI pointers here
    refreshStatus();
    brls::Activity::onContentAvailable();
}

void MainActivity::willAppear(bool firstTime) {
    Activity::willAppear(firstTime);
    if (!firstTime) refreshStatus();
}

// ── Refresh status display (NO pctl calls yet — static text) ──
void MainActivity::refreshStatus() {
    if (!statusLabel) return;

    // Static text for now — pctl calls come later
    statusLabel->setText("家长控制状态：未知（pctl 未调用）");

    // Set cells to default values (no pctl yet)
    if (toggleCell) {
        toggleCell->setFocusable(false);  // disable until pctl works
    }
    if (timeLimitCell) {
        timeLimitCell->setValue("未知");
        timeLimitCell->setFocusable(false);
    }
    if (pinCell) {
        pinCell->setValue("未知");
        pinCell->setFocusable(false);
    }
    if (deleteCell) {
        deleteCell->setFocusable(false);
    }
}

// ── Click handlers (stubs — show toast, no pctl yet) ──

bool onToggleRestriction(bool on) {
    (void)on;
    brls::Application::notify("切换家长控制：功能开发中");
    return true;
}

bool onTimeLimitClick(brls::View* view) {
    (void)view;
    brls::Application::notify("游玩时长设置：功能开发中");
    return true;
}

bool onPinChangeClick(brls::View* view) {
    (void)view;
    brls::Application::notify("更改 PIN：功能开发中");
    return true;
}

bool onDeleteClick(brls::View* view) {
    (void)view;
    brls::Application::notify("删除限制：功能开发中");
    return true;
}
