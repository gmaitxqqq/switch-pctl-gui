#include "main_activity.h"
#include "time_limit_activity.h"
#include "pctl_manager.h"
#include "i18n.h"

#include <borealis.hpp>
#include <fmt/format.h>

MainActivity::MainActivity() {
    // Create the main AppletFrame (Horizon-style frame with header/footer)
    brls::AppletFrame* frame = new brls::AppletFrame();
    frame->setTitle(i18n_get("app_title"));

    // Create scrollable content container
    brls::ScrollingFrame* scroll = new brls::ScrollingFrame();
    brls::Box* content = new brls::Box(brls::Axis::COLUMN);
    content->setPadding(20, 20, 20, 20);

    PctlManager& pctl = PctlManager::getInstance();

    // ── Status Section ──────────────────────────────────────────────
    // Restriction enabled status
    {
        brls::Box* row = new brls::Box(brls::Axis::ROW);
        row->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
        row->setAlignItems(brls::AlignItems::CENTER);
        row->setHeight(40);
        row->setGrow(1);

        brls::Label* titleLbl = new brls::Label();
        titleLbl->setText(i18n_get("status_restriction"));
        titleLbl->setFontSize(16);

        restrictionLabel = new brls::Label();
        restrictionLabel->setFontSize(16);
        restrictionLabel->setHorizontalAlign(brls::HorizontalAlign::RIGHT);
        restrictionLabel->setGrow(1);

        row->addView(titleLbl);
        row->addView(restrictionLabel);
        content->addView(row);
    }

    // Rating age
    {
        brls::Box* row = new brls::Box(brls::Axis::ROW);
        row->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
        row->setAlignItems(brls::AlignItems::CENTER);
        row->setHeight(40);
        row->setGrow(1);

        brls::Label* titleLbl = new brls::Label();
        titleLbl->setText("分级年龄");
        titleLbl->setFontSize(16);

        timeLabel = new brls::Label();
        timeLabel->setFontSize(16);
        timeLabel->setHorizontalAlign(brls::HorizontalAlign::RIGHT);
        timeLabel->setGrow(1);

        row->addView(titleLbl);
        row->addView(timeLabel);
        content->addView(row);
    }

    // SNS post restriction
    {
        brls::Box* row = new brls::Box(brls::Axis::ROW);
        row->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
        row->setAlignItems(brls::AlignItems::CENTER);
        row->setHeight(40);
        row->setGrow(1);

        brls::Label* titleLbl = new brls::Label();
        titleLbl->setText("社交限制");
        titleLbl->setFontSize(16);

        remainingLabel = new brls::Label();
        remainingLabel->setFontSize(16);
        remainingLabel->setHorizontalAlign(brls::HorizontalAlign::RIGHT);
        remainingLabel->setGrow(1);

        row->addView(titleLbl);
        row->addView(remainingLabel);
        content->addView(row);
    }

    // Separator
    brls::Rectangle* sep = new brls::Rectangle(nvgRGBA(128, 128, 128, 255));
    sep->setHeight(1);
    content->addView(sep);

    // ── Toggle Restriction ──────────────────────────────────────────
    bool isRestricted = pctl.isRestrictionEnabled();
    toggleCell = new brls::BooleanCell();
    toggleCell->init(i18n_get("menu_toggle"), isRestricted, [this](bool on) {
        bool success = PctlManager::getInstance().setRestrictionEnabled(on);
        if (success) {
            brls::Application::notify(i18n_get("success"));
        } else {
            brls::Application::notify(i18n_get("error_generic"));
        }
        this->refreshStatus();
    });
    content->addView(toggleCell);

    // ── Menu Items ──────────────────────────────────────────────────
    // Time limit setting (opens TimeLimitActivity)
    timeLimitCell = new brls::DetailCell();
    timeLimitCell->setText(i18n_get("menu_time_limit"));
    timeLimitCell->setDetailText("—");
    timeLimitCell->registerClickAction([this](brls::View* view) -> bool {
        brls::Application::pushActivity(new TimeLimitActivity());
        return true;
    });
    content->addView(timeLimitCell);

    // PIN display
    pinCell = new brls::DetailCell();
    pinCell->setText(i18n_get("menu_pin_change"));
    pinCell->setDetailText("");
    pinCell->registerClickAction([](brls::View* view) -> bool {
        bool success = PctlManager::getInstance().showPin();
        if (!success) {
            brls::Application::notify(i18n_get("error_generic"));
        }
        return true;
    });
    content->addView(pinCell);

    // Delete configuration (with confirmation dialog)
    deleteCell = new brls::DetailCell();
    deleteCell->setText(i18n_get("menu_delete"));
    deleteCell->setDetailText("");
    deleteCell->registerClickAction([](brls::View* view) -> bool {
        brls::Dialog* dialog = new brls::Dialog(i18n_get("confirm_delete"));
        dialog->addButton(i18n_get("btn_confirm"), []() {
            bool success = PctlManager::getInstance().deleteConfig();
            if (success) {
                brls::Application::notify(i18n_get("success"));
            } else {
                brls::Application::notify(i18n_get("error_generic"));
            }
        });
        dialog->addButton(i18n_get("btn_cancel"), []() {});
        dialog->open();
        return true;
    });
    content->addView(deleteCell);

    // Set up scrolling content
    scroll->setContentView(content);
    frame->pushContentView(scroll);

    // Register refresh action on X button
    frame->registerAction(i18n_get("hint_refresh"), brls::BUTTON_X,
        [this](brls::View* view) -> bool {
            this->refreshStatus();
            return true;
        }, false);

    // Set this activity's content view
    this->setContentView(frame);

    // Initial status refresh
    refreshStatus();
}

void MainActivity::refreshStatus() {
    PctlManager& pctl = PctlManager::getInstance();

    if (!pctl.isAvailable()) {
        if (restrictionLabel) restrictionLabel->setText(i18n_get("error_pctl"));
        if (timeLabel)        timeLabel->setText(i18n_get("error_pctl"));
        if (remainingLabel)   remainingLabel->setText(i18n_get("error_pctl"));
        if (toggleCell)       toggleCell->setFocusable(false);
        return;
    }

    PlayTimerSettings settings;
    pctl.getCurrentSettings(&settings);

    // Restriction enabled
    if (restrictionLabel) {
        restrictionLabel->setText(
            settings.restriction_enabled ? i18n_get("enabled") : i18n_get("disabled")
        );
    }

    // Rating age
    if (timeLabel) {
        timeLabel->setText(fmt::format("{}", settings.rating_age));
    }

    // SNS restriction
    if (remainingLabel) {
        remainingLabel->setText(
            settings.sns_restriction ? i18n_get("enabled") : i18n_get("disabled")
        );
    }

    // Update toggle
    if (toggleCell) {
        toggleCell->setOn(settings.restriction_enabled, false);
    }

    // Update time limit cell (placeholder since we don't have play timer data)
    if (timeLimitCell) {
        timeLimitCell->setDetailText(i18n_get("unlimited"));
    }
}
