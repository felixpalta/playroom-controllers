#include "heartbeat.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

static int HEARTBEAT_LED;

static bool state;

static const unsigned PERIOD_MS = 200;

static unsigned long last_toggle_ms;

void heart_beat_init(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  HEARTBEAT_LED = pin;
  state = false;

  last_toggle_ms = millis();
}

static void toggle()
{
  state = !state;
  digitalWrite(HEARTBEAT_LED, state);
}

void heart_beat_process()
{
  unsigned long now = millis();
  if (now - last_toggle_ms >= PERIOD_MS)
  {
    toggle();
    last_toggle_ms = now;
  }
}
