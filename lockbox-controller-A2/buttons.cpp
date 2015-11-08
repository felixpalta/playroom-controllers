// 
// 
// 

#include "buttons.h"
#include "lockbox-controller-pin-config.h"

#define DEBOUNCE_MS 1000

struct Button
{
  int pin;
  bool delay;
  unsigned long debounce_start_ms;
  bool press_detected;

  ButtonEvent::ButtonState check();

  Button(int pin_number) :
    pin(pin_number),
    delay(false),
    debounce_start_ms(0),
    press_detected(false) 
  {
    pinMode(pin, INPUT_PULLUP);
  }
};

ButtonEvent::ButtonState Button::check()
{
  bool state = !digitalRead(pin);
  if (delay && millis() - debounce_start_ms > DEBOUNCE_MS)
  {
    delay = false;
  }
  else if (!delay)
  {
    if (!press_detected && state)
    {
      press_detected = true;
      delay = true;
      debounce_start_ms = millis();
      return ButtonEvent::BUTTON_STATE_PUSHED;
    }
    else if (press_detected && !state)
    {
      press_detected = false;
      delay = true;
      debounce_start_ms = millis();
      return ButtonEvent::BUTTON_STATE_RELEASED;
    }
  }
  return ButtonEvent::BUTTON_STATE_INVALID;
}

static Button game_start_button(GAME_START_BUTTON_PIN);
static Button standby_button(STANDBY_BUTTON_PIN);
static Button cleaning_button(CLEANING_BUTTON_PIN);

void buttons_init()
{
  pinMode(GAME_START_BUTTON_LIGHT_PIN, OUTPUT);
  pinMode(STANDBY_BUTTON_LIGHT_PIN, OUTPUT);
  Serial.println("Buttons initialized!");
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
