#include "LockBoxes.h"
#include "lockbox-controller-pin-config.h"

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
//  int internal_number = n - 1;
  int internal_number = n;
  if (internal_number >= 0 && (size_t)internal_number < LOCKBOX_PIN_NUMBER)
  {
    n = internal_number;
    return true;
  }
  Serial.print("Invalid lockbox number: ");
  Serial.println(n);
  
  return false;
}



boolean LockBoxes::open(int n)
{
  if (!check_number_convert_to_internal(n))
  {  
    error_code = ERROR_INVALID_LOCKBOX_NUMBER;
    return false;
  }

  digitalWrite(lockbox_pins[n], HIGH);
  pinMode(lockbox_pins[n], OUTPUT);
  return true;
}

boolean LockBoxes::open_all()
{
  for (size_t i = 0; i < LOCKBOX_PIN_NUMBER; ++i)
  {
    digitalWrite(lockbox_pins[i], HIGH);
    pinMode(lockbox_pins[i], OUTPUT);
  }
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
  return true;
}

boolean LockBoxes::close_all()
{
  for (size_t i = 0; i < LOCKBOX_PIN_NUMBER; ++i)
  {
    digitalWrite(lockbox_pins[i], LOW);
    pinMode(lockbox_pins[i], OUTPUT);
  }
  return true;
}


