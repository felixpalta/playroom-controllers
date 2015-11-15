#ifndef PLAYROOMSTATESTANDBY_H
#define PLAYROOMSTATESTANDBY_H

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateStandby : public PlayroomStateInterface
{
public:
    PlayroomStateStandby(PlayroomStateMachineInterface& p);

    void init();
    void game_start_button_pressed();
    void standby_button_pressed();
    void cleaning_key_enabled();
    void cleaning_key_disabled();
private:
    PlayroomStateMachineInterface& state_machine;

};
#endif // PLAYROOMSTATESTANDBY_H
