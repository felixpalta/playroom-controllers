#include "LockBoxes.h"
#include "lockbox-controller-pin-config.h"

#define BLINK_PULSE_MS 200
#define BLINK_PAUSE_MS 2000
#define BLINK_NUMBER 10

LockBoxes::LockBoxes() : 
  n_blinks_left(0),
  current_blinking_lockbox(0),
  last_blink_time(0)
{
}

void LockBoxes::begin()
{
  close_all();
}

const char* LockBoxes::get_last_error() const
{
  switch (error_code)
  {
  case ERROR_INVALID_LOCKBOX_NUMBER:
    return "Invalid lockbox number";
  case ERROR_NONE:
  default:
    return "Lockboxes: no error";
  }
}

boolean LockBoxes::check_number_convert_to_internal(int& n)
{
  int internal_number = n - 1;
//  int internal_number = n;
  if (internal_number >= 0 && (size_t)internal_number < LOCKBOX_PIN_NUMBER)
  {
    n = internal_number;
    return true;
  }
  Serial.print("Invalid lockbox number: ");
  Serial.println(n);
  
  return false;
}

void LockBoxes::blink_once()
{
  if (current_blinking_lockbox >= 0 && (size_t)current_blinking_lockbox < LOCKBOX_PIN_NUMBER)
  {
    int pin = lockbox_pins[current_blinking_lockbox];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(BLINK_PULSE_MS);
    digitalWrite(pin, LOW);
  }
  else
    Serial.println("blink_once(): Invalid lockbox number");
}

boolean LockBoxes::open(int n)
{
  if (!check_number_convert_to_internal(n))
  {  
    error_code = ERROR_INVALID_LOCKBOX_NUMBER;
    return false;
  }
  close_all();
  current_blinking_lockbox = n;
  blink_once();
  n_blinks_left = BLINK_NUMBER;
  last_blink_time = millis();
  return true;
}

boolean LockBoxes::open_all()
{
  Serial.println("Forbidden to open all lockboxes at once.");
  return true;
}

boolean LockBoxes::close(int n)
{
  if (!check_number_convert_to_internal(n))
  {
    error_code = ERROR_INVALID_LOCKBOX_NUMBER;
    return false;
  }

  digitalWrite(lockbox_pins[n], LOW);
  pinMode(lockbox_pins[n], OUTPUT);
  n_blinks_left = 0;
  return true;
}

boolean LockBoxes::close_all()
{
  for (size_t i = 0; i < LOCKBOX_PIN_NUMBER; ++i)
  {
    digitalWrite(lockbox_pins[i], LOW);
    pinMode(lockbox_pins[i], OUTPUT);
  }
  n_blinks_left = 0;
  return true;
}

void LockBoxes::process()
{
  if (n_blinks_left > 0 && (millis() - last_blink_time) >= BLINK_PAUSE_MS)
  {
    --n_blinks_left;
    blink_once();
    last_blink_time = millis();
  }
}
