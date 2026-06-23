#pragma once

#include <borealis.hpp>
#include <string>

class MainActivity : public brls::Activity {
public:
    MainActivity();
    ~MainActivity();

    // borealis lifecycle
    brls::View* createContentView() override;
    void onContentAvailable() override;
    void willAppear(bool firstTime) override;

private:
    // UI pointers — set in createContentView(), valid in/after onContentAvailable()
    brls::BooleanCell* toggleCell  = nullptr;
    brls::DetailCell*  timeLimitCell = nullptr;
    brls::DetailCell*  pinCell        = nullptr;
    brls::DetailCell*  deleteCell     = nullptr;
    brls::Label*       statusLabel    = nullptr;

    void refreshStatus();
};
