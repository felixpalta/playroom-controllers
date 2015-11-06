// buttons.h

#ifndef _BUTTONS_h
#define _BUTTONS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct ButtonEvent
{
  enum ButtonState
  {
    BUTTON_STATE_INVALID,
    BUTTON_STATE_PUSHED,
    BUTTON_STATE_RELEASED,
  };

  ButtonState game_start_state;
  ButtonState standby_state;
  ButtonState cleaning_state;

  ButtonEvent() :
    game_start_state(BUTTON_STATE_INVALID),
    standby_state(BUTTON_STATE_INVALID),
    cleaning_state(BUTTON_STATE_INVALID)
  {}

  void reset()
  {
    game_start_state = BUTTON_STATE_INVALID;
    standby_state = BUTTON_STATE_INVALID;
    cleaning_state = BUTTON_STATE_INVALID;
  }
};

void buttons_init();

bool buttons_process(ButtonEvent *button_event);

bool buttons_event_check_ready_and_reset(ButtonEvent *button_event);

void buttons_event_reset();

#endif

