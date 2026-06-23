#pragma once

#include <borealis.hpp>

// Main activity - shows parental control status and menu items
// Uses AppletFrame for Horizon-style UI with header/footer
class MainActivity : public brls::Activity {
public:
    MainActivity();
    ~MainActivity() override = default;

    // Override to build UI — called by framework during pushActivity()
    brls::View* createContentView() override;

    // Called after createContentView() — safe time to init data
    void onContentAvailable() override;

    // Refresh the status display from pctl service
    void refreshStatus();

private:
    // Status display labels
    brls::Label* pinLabel       = nullptr;
    brls::Label* restrictionLabel = nullptr;
    brls::Label* timeLabel      = nullptr;
    brls::Label* remainingLabel = nullptr;

    // Toggle cell for restriction on/off
    brls::BooleanCell* toggleCell = nullptr;

    // Menu item cells
    brls::DetailCell* timeLimitCell = nullptr;
    brls::DetailCell* pinCell       = nullptr;
    brls::DetailCell* deleteCell    = nullptr;
    brls::DetailCell* unlinkCell    = nullptr;
};
