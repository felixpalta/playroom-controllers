#include "PlayroomStateStandby.h"
#include "door_lock.h"
#include "curtain.h"
#include "dimmers.h"
#include "buttons.h"

PlayroomStateStandby::PlayroomStateStandby(PlayroomStateMachineInterface& p) :
    state_machine(p)
{
}

void PlayroomStateStandby::init()
{
  door_lock_close();
  curtain_signal_close();
  dimmers_light_set(DIMMER_TOP_LIGHT, 0);
  dimmers_light_set(DIMMER_SURROUND_LIGHT, 0);
  dimmers_light_set(DIMMER_LOCKBOX_LIGHT, 0);
  button_set_light_standby(true);
  button_set_light_game_start(false);
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
