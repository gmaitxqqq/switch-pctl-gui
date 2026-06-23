#include <borealis.hpp>
#include "main_activity.h"
#include "pctl_manager.h"
#include "i18n.h"

#include <fmt/format.h>

// Forward declarations for click handlers (called after full construction)
static void onToggleRestriction(bool on);
static void onTimeLimitClick(brls::View* view);
static void onPinChangeClick(brls::View* view);
static void onDeleteClick(brls::View* view);

// Global pointer to activity (set in constructor, used by handlers)
static MainActivity* s_instance = nullptr;

void onToggleRestriction(bool on) {
    bool success = PctlManager::getInstance().setRestrictionEnabled(on);
    if (success) {
        brls::Application::notify(i18n_get("success"));
    } else {
        brls::Application::notify(i18n_get("error_generic"));
    }
    if (s_instance) s_instance->refreshStatus();
}

bool onTimeLimitClick(brls::View* view) {
    brls::Application::pushActivity(new TimeLimitActivity());
    return true;
}

bool onPinChangeClick(brls::View* view) {
    bool success = PctlManager::getInstance().showPin();
    if (!success) {
        brls::Application::notify(i18n_get("error_generic"));
    }
    return true;
}

bool onDeleteClick(brls::View* view) {
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
}

// ── Helper: safely create a status row (Label + value Label) ──
static brls::Box* makeStatusRow(const std::string& titleText, brls::Label*& outValueLabel) {
    brls::Box* row = new brls::Box(brls::Axis::ROW);
    row->setJustifyContent(brls::JustifyContent::SPACE_BETWEEN);
    row->setAlignItems(brls::AlignItems::CENTER);
    row->setHeight(40);
    row->setGrow(1);

    brls::Label* titleLbl = new brls::Label();
    titleLbl->setText(titleText);
    titleLbl->setFontSize(16);

    outValueLabel = new brls::Label();
    outValueLabel->setFontSize(16);
    outValueLabel->setHorizontalAlign(brls::HorizontalAlign::RIGHT);
    outValueLabel->setGrow(1);

    row->addView(titleLbl);
    row->addView(outValueLabel);
    return row;
}

MainActivity::MainActivity() {
    s_instance = this;

    // Create the main AppletFrame
    brls::AppletFrame* frame = new brls::AppletFrame();
    frame->setTitle(i18n_get("app_title"));

    // Create scrollable content container
    brls::ScrollingFrame* scroll = new brls::ScrollingFrame();
    brls::Box* content = new brls::Box(brls::Axis::COLUMN);
    content->setPadding(20, 20, 20, 20);

    // ── Status Section ──────────────────────────────────────────────
    content->addView(makeStatusRow(i18n_get("status_restriction"), restrictionLabel));
    content->addView(makeStatusRow("分级年龄", timeLabel));
    content->addView(makeStatusRow("社交限制", remainingLabel));

    // Separator
    brls::Rectangle* sep = new brls::Rectangle(nvgRGBA(128, 128, 128, 255));
    sep->setHeight(1);
    content->addView(sep);

    // ── Toggle Restriction ──────────────────────────────────────────
    // Get initial state safely — if pctl not available, default to false
    bool isRestricted = false;
    {
        PctlManager& pctl = PctlManager::getInstance();
        if (pctl.isAvailable()) {
            isRestricted = pctl.isRestrictionEnabled();
        }
    }

    toggleCell = new brls::BooleanCell();
    toggleCell->init(i18n_get("menu_toggle"), isRestricted, onToggleRestriction);
    content->addView(toggleCell);

    // ── Menu Items ──────────────────────────────────────────────────
    timeLimitCell = new brls::DetailCell();
    timeLimitCell->setText(i18n_get("menu_time_limit"));
    timeLimitCell->setDetailText("—");
    timeLimitCell->registerClickAction(onTimeLimitClick);
    content->addView(timeLimitCell);

    pinCell = new brls::DetailCell();
    pinCell->setText(i18n_get("menu_pin_change"));
    pinCell->setDetailText("");
    pinCell->registerClickAction(onPinChangeClick);
    content->addView(pinCell);

    deleteCell = new brls::DetailCell();
    deleteCell->setText(i18n_get("menu_delete"));
    deleteCell->setDetailText("");
    deleteCell->registerClickAction(onDeleteClick);
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

    // Initial status refresh (deferred to after construction completes)
    // Note: refreshStatus() will be called by the framework's onShow()
}

void MainActivity::onContentAvailable() {
    // Safe time to call refreshStatus — content view is fully created
    refreshStatus();
}

void MainActivity::refreshStatus() {
    PctlManager& pctl = PctlManager::getInstance();

    // Guard: ensure labels exist before touching them
    if (!pctl.isAvailable()) {
        if (restrictionLabel) restrictionLabel->setText(i18n_get("error_pctl"));
        if (timeLabel)        timeLabel->setText(i18n_get("error_pctl"));
        if (remainingLabel)   remainingLabel->setText(i18n_get("error_pctl"));
        if (toggleCell)       toggleCell->setFocusable(false);
        return;
    }

    PlayTimerSettings settings;
    if (!pctl.getCurrentSettings(&settings)) {
        if (restrictionLabel) restrictionLabel->setText(i18n_get("error_generic"));
        if (timeLabel)        timeLabel->setText(i18n_get("error_generic"));
        if (remainingLabel)   remainingLabel->setText(i18n_get("error_generic"));
        return;
    }

    // Restriction enabled
    if (restrictionLabel) {
        restrictionLabel->setText(
            settings.restriction_enabled ? i18n_get("enabled") : i18n_get("disabled")
        );
    }

    // Rating age
    if (timeLabel) {
        timeLabel->setText(fmt::format("{}", (int)settings.rating_age));
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

    // Update time limit cell
    if (timeLimitCell) {
        timeLimitCell->setDetailText(i18n_get("unlimited"));
    }
}
