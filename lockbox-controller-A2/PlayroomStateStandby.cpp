#include "PlayroomStateStandby.h"

PlayroomStateStandby::PlayroomStateStandby(PlayroomStateMachineInterface& p) :
    state_machine(p)
{

}

void PlayroomStateStandby::init()
{
}

void PlayroomStateStandby::game_start_button_pressed()
{
    state_machine.set_state(state_machine.STATE_GAME);
}
void PlayroomStateStandby::standby_button_pressed()
{
    // Ignored.
}
void PlayroomStateStandby::cleaning_key_enabled()
{
    state_machine.set_state(state_machine.STATE_CLEANING);
}
void PlayroomStateStandby::cleaning_key_disabled()
{
    // Ignored.
}
