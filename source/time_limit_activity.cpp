#include "time_limit_activity.h"
#include "pctl_manager.h"
#include "i18n.h"

#include <borealis.hpp>
#include <fmt/format.h>

TimeLimitActivity::TimeLimitActivity() {
    // Minimal constructor — all UI in createContentView()
}

brls::View* TimeLimitActivity::createContentView() {
    brls::AppletFrame* frame = new brls::AppletFrame();
    frame->setTitle(i18n_get("time_title"));

    brls::ScrollingFrame* scroll = new brls::ScrollingFrame();
    brls::Box* content = new brls::Box(brls::Axis::COLUMN);
    content->setPadding(20, 20, 20, 20);

    PctlManager& pctl = PctlManager::getInstance();

    // Show current time limit
    int currentMinutes = pctl.getPlayTimerMinutes();
    {
        currentCell = new brls::DetailCell();
        currentCell->setText(i18n_get("status_time"));
        if (currentMinutes > 0) {
            currentCell->setDetailText(fmt::format("{} {}", currentMinutes, i18n_get("minutes")));
        } else {
            currentCell->setDetailText(i18n_get("unlimited"));
        }
        content->addView(currentCell);
    }

    // Preset time options: 5, 15, 20, 30 minutes
    struct Preset {
        int minutes;
        std::string label;
    };

    Preset presets[] = {
        {5,  i18n_get("time_set_5")},
        {15, i18n_get("time_set_15")},
        {20, i18n_get("time_set_20")},
        {30, i18n_get("time_set_30")},
    };

    for (const auto& preset : presets) {
        brls::DetailCell* cell = new brls::DetailCell();
        cell->setText(preset.label);
        cell->setDetailText("");
        cell->registerClickAction([preset, this](brls::View* view) -> bool {
            bool success = PctlManager::getInstance().setPlayTimerMinutes(preset.minutes);
            if (success) {
                brls::Application::notify(i18n_get("success"));
                // Update current display
                if (currentCell) {
                    currentCell->setDetailText(
                        fmt::format("{} {}", preset.minutes, i18n_get("minutes"))
                    );
                }
            } else {
                brls::Application::notify(i18n_get("error_generic"));
            }
            return true;
        });
        content->addView(cell);
    }

    // Unlimited option (set minutes to 0)
    brls::DetailCell* unlimitedCell = new brls::DetailCell();
    unlimitedCell->setText(i18n_get("unlimited"));
    unlimitedCell->setDetailText("");
    unlimitedCell->registerClickAction([this](brls::View* view) -> bool {
        bool success = PctlManager::getInstance().setPlayTimerMinutes(0);
        if (success) {
            brls::Application::notify(i18n_get("success"));
            if (currentCell) {
                currentCell->setDetailText(i18n_get("unlimited"));
            }
        } else {
            brls::Application::notify(i18n_get("error_generic"));
        }
        return true;
    });
    content->addView(unlimitedCell);

    scroll->setContentView(content);
    frame->pushContentView(scroll);

    return frame;
}
