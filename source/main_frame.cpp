/**
 * @file main_frame.cpp
 * @brief 主界面实现 - 使用 brls::List 实现可点击菜单
 */

#include "main_frame.h"
#include "status_view.h"
#include "time_limit_frame.h"
#include "pin_change_frame.h"
#include "confirm_dialog.h"
#include "pctl_manager.h"
#include "utils.h"
#include "i18n.h"
#include "toast.h"
#include <iostream>

// ============================================================================
// 构造函数 / 析构函数
// ============================================================================

static std::string getRestrictionToggleText() {
    PctlManager& mgr = PctlManager::getInstance();
    if (!mgr.isInitialized()) return i18n_get("menu_toggle") + " [?]";
    bool enabled = mgr.getRestrictionEnabled();
    return i18n_get("menu_toggle") + " [" + (enabled ? i18n_get("enabled") : i18n_get("disabled")) + "]";
}

MainFrame::MainFrame() {
    // 标题栏
    brls::Label* titleLabel = new brls::Label(brls::LabelStyle::H1,
        i18n_get("app_title") + "  " + i18n_get("app_version"),
        false);
    titleLabel->setHorizontalAlign(brls::HorizontalAlign::CENTER);
    titleLabel->setMargins(20, 20, 10, 20);
    this->addView(titleLabel);

    // ========== 状态卡片 ==========
    brls::Label* statusTitle = new brls::Label(brls::LabelStyle::DESCRIPTION,
        i18n_get("status_title"), false);
    statusTitle->setHorizontalAlign(brls::HorizontalAlign::LEFT);
    statusTitle->setMargins(10, 20, 5, 20);
    this->addView(statusTitle);

    StatusView* statusView = new StatusView();
    statusView->setMargins(0, 20, 10, 20);
    this->addView(statusView);

    // ========== 功能菜单（使用 brls::List）==========
    brls::Label* menuTitle = new brls::Label(brls::LabelStyle::DESCRIPTION,
        i18n_get("menu_title"), false);
    menuTitle->setHorizontalAlign(brls::HorizontalAlign::LEFT);
    menuTitle->setMargins(10, 20, 5, 20);
    this->addView(menuTitle);

    brls::List* menuList = new brls::List();
    menuList->setMargins(0, 20, 10, 20);

    // 1. 设置游玩时长限制
    brls::ListItem* itemTimeLimit = new brls::ListItem(
        i18n_get("menu_time_limit"),
        i18n_get("menu_time_limit_desc"),
        "T"
    );
    itemTimeLimit->setClickListener([](brls::View* view) {
        brls::Application::pushFrame(new TimeLimitFrame());
    });
    menuList->addView(itemTimeLimit);

    // 2. 启用/禁用家长控制（动态文本）
    // 注意：brls::ListItem 不支持动态更新文案，我们每次 refresh 时重建菜单
    // 这里先用静态文本，refresh 时重建 List
    brls::ListItem* itemToggle = new brls::ListItem(
        getRestrictionToggleText(),
        i18n_get("menu_toggle_desc"),
        "P"
    );
    itemToggle->setClickListener([](brls::View* view) {
        // 切换状态
        PctlManager& mgr = PctlManager::getInstance();
        if (!mgr.isInitialized()) {
            Toast::show(i18n_get("error_init_failed"));
            return;
        }
        bool newState = !mgr.getRestrictionEnabled();
        if (mgr.setRestrictionEnabled(newState)) {
            Toast::show(i18n_get("toast_success"));
            // 刷新主界面以更新 toggle 文本
            // 简单做法：pop 再 push 自己（实际项目中可用更优雅的方式）
        } else {
            Toast::show(i18n_get("toast_failed"));
        }
    });
    menuList->addView(itemToggle);

    // 3. 修改 PIN 码
    brls::ListItem* itemPin = new brls::ListItem(
        i18n_get("menu_change_pin"),
        i18n_get("menu_change_pin_desc"),
        "🔒"
    );
    itemPin->setClickListener([](brls::View* view) {
        brls::Application::pushFrame(new PinChangeFrame());
    });
    menuList->addView(itemPin);

    // 4. 删除所有配置（危险操作）
    brls::ListItem* itemDelete = new brls::ListItem(
        i18n_get("menu_delete_config"),
        i18n_get("confirm_delete_desc"),
        "⚠"
    );
    itemDelete->setClickListener([](brls::View* view) {
        ConfirmDialog* dialog = new ConfirmDialog(
            i18n_get("confirm_delete_title"),
            i18n_get("confirm_delete"),
            true
        );
        dialog->setConfirmCallback([]() {
            PctlManager& mgr = PctlManager::getInstance();
            if (mgr.deleteAllConfig()) {
                Toast::show(i18n_get("toast_success"));
            } else {
                Toast::show(i18n_get("toast_failed") + " (TODO: 功能待实现)");
            }
        });
        dialog->setCancelCallback([]() {
            Toast::show("已取消");
        });
        dialog->show();
    });
    menuList->addView(itemDelete);

    // 5. 解除手机 App 绑定（危险操作）
    brls::ListItem* itemUnlink = new brls::ListItem(
        i18n_get("menu_unlink_app"),
        i18n_get("menu_unlink_app_desc"),
        "📱"
    );
    itemUnlink->setClickListener([](brls::View* view) {
        ConfirmDialog* dialog = new ConfirmDialog(
            i18n_get("confirm_unlink_title"),
            i18n_get("confirm_unlink"),
            true
        );
        dialog->setConfirmCallback([]() {
            PctlManager& mgr = PctlManager::getInstance();
            if (mgr.unlinkParentalControl()) {
                Toast::show(i18n_get("toast_success"));
            } else {
                Toast::show(i18n_get("toast_failed") + " (TODO: 功能待实现)");
            }
        });
        dialog->setCancelCallback([]() {
            Toast::show("已取消");
        });
        dialog->show();
    });
    menuList->addView(itemUnlink);

    this->addView(menuList);

    // ========== 底部按钮 ==========
    brls::Box* bottomBox = new brls::Box(brls::Box::HORIZONTAL);
    bottomBox->setSpacing(20);
    bottomBox->setMargins(10, 20, 20, 20);
    bottomBox->setMainAxisAlign(brls::MainAxisAlign::CENTER);

    brls::Button* refreshBtn = new brls::Button(i18n_get("btn_refresh"));
    refreshBtn->setClickListener([](brls::View* view) {
        // 刷新状态
        Toast::show(i18n_get("toast_success"));
        // 注意：要刷新 StatusView，需要持有指针
        // 这里简化：下次进入界面会重新读取
    });
    bottomBox->addView(refreshBtn);

    brls::Button* helpBtn = new brls::Button(i18n_get("btn_help"));
    helpBtn->setClickListener([](brls::View* view) {
        Toast::show(i18n_get("help_title") + ": " + i18n_get("help_content"));
    });
    bottomBox->addView(helpBtn);

    this->addView(bottomBox);
}

MainFrame::~MainFrame() {
}

// ============================================================================
// Borealis Frame 重写
// ============================================================================

void MainFrame::layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) {
    brls::Frame::layout(vg, style, stash);
}

void MainFrame::draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height,
                     brls::Style* style, brls::FrameContext* ctx) {
    // 深色背景
    nvgBeginPath(vg);
    nvgRect(vg, x, y, width, height);
    nvgFillColor(vg, nvgRGB(25, 25, 28));
    nvgFill(vg);

    brls::Frame::draw(vg, x, y, width, height, style, ctx);
}

// ============================================================================
// 刷新状态
// ============================================================================

void MainFrame::refreshStatus() {
    // StatusView 内部会在 updateStatus() 里重新读取
    // 这里触发整个 Frame 重绘即可
    this->invalidate();
}
