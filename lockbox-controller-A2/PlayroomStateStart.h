// PlayroomStateStart.h

#ifndef _PLAYROOMSTATESTART_h
#define _PLAYROOMSTATESTART_h

#include "PlayroomStateInterface.h"
#include "PlayroomStateMachineInterface.h"

class PlayroomStateStart : public PlayroomStateInterface
{
public:
  PlayroomStateStart(PlayroomStateMachineInterface& p);

  void init();
  void game_start_button_pressed();
  void standby_button_pressed();
  void cleaning_key_enabled();
  void cleaning_key_disabled();
private:
  PlayroomStateMachineInterface& state_machine;

};


#endif

