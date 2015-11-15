#include "PlayroomStateGame.h"

PlayroomStateGame::PlayroomStateGame(PlayroomStateMachineInterface& p) :
    state_machine(p)
{

}

void PlayroomStateGame::init()
{
    // Send game-start request to server.
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
