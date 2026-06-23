#pragma once

#include <borealis.hpp>

// Time limit setting activity - shows preset time options
class TimeLimitActivity : public brls::Activity {
public:
    TimeLimitActivity();
    ~TimeLimitActivity() override = default;

private:
    brls::DetailCell* currentCell = nullptr;
};
