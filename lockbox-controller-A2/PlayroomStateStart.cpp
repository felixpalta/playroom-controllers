#include "PlayroomStateStart.h"
#include "door_lock.h"
#include "dimmers.h"
#include "buttons.h"

PlayroomStateStart::PlayroomStateStart(PlayroomStateMachineInterface& p) :
state_machine(p)
{

}

void PlayroomStateStart::init()
{
  door_lock_open();
  dimmers_light_set(DIMMER_TOP_LIGHT, 50);
  dimmers_light_set(DIMMER_SURROUND_LIGHT, 50);
  dimmers_light_set(DIMMER_LOCKBOX_LIGHT, 0);
  button_set_light_standby(true);
  button_set_light_game_start(true);
}

void PlayroomStateStart::game_start_button_pressed()
{
  // Ignored. 
}

void PlayroomStateStart::standby_button_pressed()
{
  state_machine.set_state(state_machine.STATE_STANDBY);
}

void PlayroomStateStart::cleaning_key_enabled()
{
  // Ignored.
}
void PlayroomStateStart::cleaning_key_disabled()
{
  // Ignored.
}
