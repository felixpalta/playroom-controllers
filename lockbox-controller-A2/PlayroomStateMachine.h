#ifndef PLAYROOMSTATEMACHINE_H
#define PLAYROOMSTATEMACHINE_H

#include "PlayroomStateMachineInterface.h"
#include "PlayroomStateCleaning.h"
#include "PlayroomStateDoneCleaning.h"
#include "PlayroomStateStandby.h"
#include "PlayroomStateGame.h"
#include "PlayroomStateNull.h"

class PlayroomStateMachine : public PlayroomStateMachineInterface
{
public:
    PlayroomStateMachine(State init_state);

    void game_start_button_pressed_handler();
    void standby_button_pressed_handler();
    void cleaning_key_enabled_handler();
    void cleaning_key_disabled_handler();

    void set_state(State state_name);

private:
    PlayroomStateGame game;
    PlayroomStateCleaning cleaning;
    PlayroomStateDoneCleaning done_cleaning;
    PlayroomStateStandby standby;

    PlayroomStateNull state_null;

    PlayroomStateInterface *state;

    PlayroomStateMachine(const PlayroomStateMachine&);
    PlayroomStateMachine& operator=(const PlayroomStateMachine&);
};

#endif // PLAYROOMSTATEMACHINE_H
