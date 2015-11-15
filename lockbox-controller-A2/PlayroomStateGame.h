#ifndef PLAYROOMSTATEGAME_H
#define PLAYROOMSTATEGAME_H

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateGame : public PlayroomStateInterface
{
public:
    PlayroomStateGame(PlayroomStateMachineInterface& p);

    void init();
    void game_start_button_pressed();
    void standby_button_pressed();
    void cleaning_key_enabled();
    void cleaning_key_disabled();
private:
    PlayroomStateMachineInterface& state_machine;

};

#endif // PLAYROOMSTATEGAME_H
