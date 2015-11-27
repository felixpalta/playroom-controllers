#include "PlayroomStateCleaning.h"
#include "door_lock.h"
#include "curtain.h"
#include "dimmers.h"
#include "buttons.h"

PlayroomStateCleaning::PlayroomStateCleaning(PlayroomStateMachineInterface &p)
    : state_machine(p)
{
}

void PlayroomStateCleaning::init()
{
  door_lock_open();
  curtain_signal_open();
  dimmers_light_set(DIMMER_TOP_LIGHT, 100);
  dimmers_light_set(DIMMER_SURROUND_LIGHT, 100);
  dimmers_light_set(DIMMER_LOCKBOX_LIGHT, 100);
  button_set_light_standby(false);
  button_set_light_game_start(false);
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
