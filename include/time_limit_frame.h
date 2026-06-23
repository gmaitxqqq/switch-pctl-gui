/**
 * @file time_limit_frame.h
 * @brief 时长设置界面
 */

#ifndef SWITCH_PCTL_GUI_TIME_LIMIT_FRAME_H_
#define SWITCH_PCTL_GUI_TIME_LIMIT_FRAME_H_

#include <borealis.hpp>

class TimeLimitFrame : public brls::Frame {
public:
    TimeLimitFrame();
    ~TimeLimitFrame() override;
};

#endif  // SWITCH_PCTL_GUI_TIME_LIMIT_FRAME_H_
