#include <MsTimer2.h>
#include "dimmers.h"

#define DELAY 50

#define DIM_MIN_LEVEL 0
#define DIM_MAX_LEVEL 100

static int light = 0;

void setup()
{
  dimmers_init();
  Serial.begin(9600);
  light = 0;
  dimmers_light_set(light);
}

void loop()
{
  static const bool blinking = true;
  static bool rising = true;
  
  if (blinking) 
  {
    if (rising)
    {
      if (light < DIM_MAX_LEVEL)
      {
        light += 10;        
        dimmers_light_set(light);
      }
      else
      {
        rising = false;
      }
    }
    else
    {
      if (light > DIM_MIN_LEVEL)
      {
        light -= 10;
        dimmers_light_set(light);
      }
      else
      {
        rising = true;
      }
    }

    delay(DELAY);
  }
}

