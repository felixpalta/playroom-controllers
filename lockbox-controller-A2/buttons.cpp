// 
// 
// 

#include "buttons.h"
#include "lockbox-controller-pin-config.h"

#define DEBOUNCE_MS 1000

struct Button
{
  int pin;
  unsigned long last_change_time_ms;

  typedef enum
  {
    RELEASED,
    PRESS_STARTED,
    PRESSED,
    RELEASE_STARTED,
  } State;

  State state;

  ButtonEvent::ButtonState check();

  

  Button(int pin_number) :
    pin(pin_number),
    last_change_time_ms(0),
    state(RELEASED) 
  {
    pinMode(pin, INPUT_PULLUP);
  }
};

ButtonEvent::ButtonState Button::check()
{
  bool pressed = !digitalRead(pin);

  switch (state)
  {
  case RELEASED:
    if (pressed)
    {
      last_change_time_ms = millis();
      state = PRESS_STARTED;
    }
    break;
  case PRESS_STARTED:
    if (!pressed)
      state = RELEASED;
    if (millis() - last_change_time_ms >= DEBOUNCE_MS)
    {
      state = PRESSED;
      return ButtonEvent::BUTTON_STATE_PUSHED;
    }
    break;
  case PRESSED:
    if (!pressed)
    {
      last_change_time_ms = millis();
      state = RELEASE_STARTED;
    }
    break;
  case RELEASE_STARTED:
    if (pressed)
      state = PRESSED;
    if (millis() - last_change_time_ms >= DEBOUNCE_MS)
    {
      state = RELEASED;
      return ButtonEvent::BUTTON_STATE_RELEASED;
    }
    break;
  default:
    break;
  }
  return ButtonEvent::BUTTON_STATE_INVALID;
}

static Button game_start_button(GAME_START_BUTTON_PIN);
static Button standby_button(STANDBY_BUTTON_PIN);
static Button cleaning_button(CLEANING_BUTTON_PIN);

void buttons_init()
{
  pinMode(GAME_START_BUTTON_LIGHT_PIN, OUTPUT);
  digitalWrite(GAME_START_BUTTON_LIGHT_PIN, LOW);
  pinMode(STANDBY_BUTTON_LIGHT_PIN, OUTPUT);
  digitalWrite(STANDBY_BUTTON_LIGHT_PIN, LOW);
  Serial.println("Buttons initialized!");
}

void button_set_light_game_start(bool on)
{
  digitalWrite(GAME_START_BUTTON_LIGHT_PIN, on);
}

void button_set_light_standby(bool on)
{
  digitalWrite(STANDBY_BUTTON_LIGHT_PIN, on);
}

bool buttons_process(ButtonEvent *event)
{
  if (!event)
    return false;

  bool retval = false;

  event->reset();

  ButtonEvent::ButtonState game_start_state = game_start_button.check();
  ButtonEvent::ButtonState standby_state = standby_button.check();
  ButtonEvent::ButtonState cleaning_state = cleaning_button.check();

  if (game_start_state != ButtonEvent::BUTTON_STATE_INVALID)
  {
    event->game_start_state = game_start_state;
    retval = true;
  }

  if (standby_state != ButtonEvent::BUTTON_STATE_INVALID)
  {
    event->standby_state = standby_state;
    retval = true;
  }

  if (cleaning_state != ButtonEvent::BUTTON_STATE_INVALID)
  {
    event->cleaning_state = cleaning_state;
    retval = true;
  }

  return retval;
}
