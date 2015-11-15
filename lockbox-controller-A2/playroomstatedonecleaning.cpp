#include "PlayroomStateDoneCleaning.h"

PlayroomStateDoneCleaning::PlayroomStateDoneCleaning(PlayroomStateMachineInterface& p) :
    state_machine(p)
{

}

void PlayroomStateDoneCleaning::init()
{
}

void PlayroomStateDoneCleaning::game_start_button_pressed()
{
    // Ignored. Must switch to STANDBY first.
}
void PlayroomStateDoneCleaning::standby_button_pressed()
{
    state_machine.set_state(state_machine.STATE_STANDBY);
}
void PlayroomStateDoneCleaning::cleaning_key_enabled()
{
    state_machine.set_state(state_machine.STATE_CLEANING);

}
void PlayroomStateDoneCleaning::cleaning_key_disabled()
{
    // Ignored.
}
