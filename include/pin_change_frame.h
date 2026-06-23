/**
 * @file pin_change_frame.h
 * @brief PIN 修改界面
 */

#ifndef SWITCH_PCTL_GUI_PIN_CHANGE_FRAME_H_
#define SWITCH_PCTL_GUI_PIN_CHANGE_FRAME_H_

#include <borealis.hpp>

class PinChangeFrame : public brls::Frame {
public:
    PinChangeFrame();
    ~PinChangeFrame() override;
};

#endif  // SWITCH_PCTL_GUI_PIN_CHANGE_FRAME_H_
