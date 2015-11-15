#ifndef PLAYROOMSTATEDONECLEANING_H
#define PLAYROOMSTATEDONECLEANING_H

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateDoneCleaning : public PlayroomStateInterface
{
public:
    PlayroomStateDoneCleaning(PlayroomStateMachineInterface& p);

    void init();
    void game_start_button_pressed();
    void standby_button_pressed();
    void cleaning_key_enabled();
    void cleaning_key_disabled();
private:
    PlayroomStateMachineInterface& state_machine;

};

#endif // PLAYROOMSTATEDONECLEANING_H
