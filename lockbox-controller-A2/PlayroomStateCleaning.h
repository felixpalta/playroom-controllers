#ifndef PLAYROOMSTATECLEANING_H
#define PLAYROOMSTATECLEANING_H

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateCleaning : public PlayroomStateInterface
{
public:
    PlayroomStateCleaning(PlayroomStateMachineInterface& p);

    void init();
    void game_start_button_pressed();
    void standby_button_pressed();
    void cleaning_key_enabled();
    void cleaning_key_disabled();
private:
    PlayroomStateMachineInterface& state_machine;
};

#endif // PLAYROOMSTATECLEANING_H
