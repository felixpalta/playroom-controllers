#include "PlayroomStateCleaning.h"

PlayroomStateCleaning::PlayroomStateCleaning(PlayroomStateMachineInterface &p)
    : state_machine(p)
{
}

void PlayroomStateCleaning::init()
{
    // Send 'game-stop' request to server.
}

void PlayroomStateCleaning::game_start_button_pressed()
{
    // Ignored.
}

void PlayroomStateCleaning::standby_button_pressed()
{
    // Ignored.
}

void PlayroomStateCleaning::cleaning_key_enabled()
{
    // Ignored.
}

void PlayroomStateCleaning::cleaning_key_disabled()
{
    state_machine.set_state(state_machine.STATE_CLEANING_DONE);
}
