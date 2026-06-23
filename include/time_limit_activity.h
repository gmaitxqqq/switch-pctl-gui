#pragma once

#include <borealis.hpp>

// Time limit setting activity - shows preset time options
class TimeLimitActivity : public brls::Activity {
public:
    TimeLimitActivity();
    ~TimeLimitActivity() override = default;

    // Override to build UI — called by framework during pushActivity()
    brls::View* createContentView() override;

private:
    brls::DetailCell* currentCell = nullptr;
};
