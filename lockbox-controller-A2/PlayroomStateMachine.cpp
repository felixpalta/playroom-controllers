#include "PlayroomStateMachine.h"

PlayroomStateMachine::PlayroomStateMachine(State init_state) :
    game(*this),
    cleaning(*this),
    done_cleaning(*this),
    standby(*this),
    state_null(),
    state(&state_null)
{
    set_state(init_state);
}

void PlayroomStateMachine::set_state(State state_name)
{
    switch (state_name)
    {
    case STATE_GAME:
        state = &game;
        break;
    case STATE_CLEANING:
        state = &cleaning;
        break;
    case STATE_CLEANING_DONE:
        state = &done_cleaning;
        break;
    case STATE_STANDBY:
        state = &standby;
        break;
    default:
        state = &state_null;
        break;
    }

    state->init();
}

void PlayroomStateMachine::game_start_button_pressed_handler()
{
    state->game_start_button_pressed();
}

void PlayroomStateMachine::standby_button_pressed_handler()
{
    state->standby_button_pressed();
}

void PlayroomStateMachine::cleaning_key_enabled_handler()
{
    state->cleaning_key_enabled();
}

void PlayroomStateMachine::cleaning_key_disabled_handler()
{
    state->cleaning_key_disabled();
}

