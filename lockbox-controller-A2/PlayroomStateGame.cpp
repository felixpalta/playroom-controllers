#include "PlayroomStateGame.h"
#include "door_lock.h"
#include "curtain.h"
#include "dimmers.h"
#include "buttons.h"
#include <RqSender.h>

extern RqSender rq_sender;

PlayroomStateGame::PlayroomStateGame(PlayroomStateMachineInterface& p) :
    state_machine(p)
{

}

void PlayroomStateGame::init()
{
  door_lock_open();
  curtain_signal_close();
  dimmers_light_set(DIMMER_TOP_LIGHT, 70);
  dimmers_light_set(DIMMER_SURROUND_LIGHT, 70);
  dimmers_light_set(DIMMER_LOCKBOX_LIGHT, 0);
  button_set_light_standby(false);
  button_set_light_game_start(true);
  rq_sender.send_request(OUT_RQ_TYPE_GAME_START, /*unused*/ 0);
}

void PlayroomStateGame::game_start_button_pressed()
{
    // Ignored. Or send game-start request again.
}
void PlayroomStateGame::standby_button_pressed()
{
    // Ignored.
}
void PlayroomStateGame::cleaning_key_enabled()
{
    // state cleaning sends 'game-stop' request on its init().
    state_machine.set_state(state_machine.STATE_CLEANING);
}
void PlayroomStateGame::cleaning_key_disabled()
{
    // Ignored.
}
