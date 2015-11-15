// 
// 
// 

#include "curtain.h"
#include "lockbox-controller-pin-config.h"

#define PULSE_MS 200

void curtain_init()
{
  pinMode(CURTAIN_OPEN_PIN, OUTPUT);
  digitalWrite(CURTAIN_OPEN_PIN, LOW);
  pinMode(CURTAIN_CLOSE_PIN, OUTPUT);
  digitalWrite(CURTAIN_CLOSE_PIN, LOW);
}

bool curtain_signal_open()
{
  digitalWrite(CURTAIN_OPEN_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(CURTAIN_OPEN_PIN, LOW);
  return true;
}

bool curtain_signal_close()
{
  digitalWrite(CURTAIN_CLOSE_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(CURTAIN_CLOSE_PIN, LOW);
  return true;
}