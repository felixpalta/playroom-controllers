#ifndef PLAYROOMSTATENULL_H
#define PLAYROOMSTATENULL_H

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateNull : public PlayroomStateInterface
{
public:
    PlayroomStateNull() {}

    void init() {}
    void game_start_button_pressed() {}
    void standby_button_pressed() {}
    void cleaning_key_enabled() {}
    void cleaning_key_disabled() {}
};

#endif // PLAYROOMSTATENULL_H
