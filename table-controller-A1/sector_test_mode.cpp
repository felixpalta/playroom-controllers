#include "sector_test_mode.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

static bool enabled = false;

void sector_test_mode_enable(bool on)
{
  enabled = on;
  Serial.print("SECTOR TEST MODE: "); Serial.println(on ? "ON" : "OFF");
}

bool sector_test_mode_is_enabled()
{
  return enabled;
}