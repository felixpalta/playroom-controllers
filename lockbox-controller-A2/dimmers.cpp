// 
// 
// 

#include "dimmers.h"
#include "lockbox-controller-pin-config.h"

typedef struct
{
  int on_pin;
  int zero_cross_pin;
  unsigned long period_ms;
};

void dimmers_init()
{
  pinMode(DIMMER_TOP_LIGHT_ON_PIN, OUTPUT);
  digitalWrite(DIMMER_TOP_LIGHT_ON_PIN, LOW);

}
