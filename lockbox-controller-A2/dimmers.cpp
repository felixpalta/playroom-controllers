// 
// 
// 

#include "dimmers.h"
#include "lockbox-controller-pin-config.h"

void dimmers_init()
{
  pinMode(DIMMER_TOP_LIGHT_ON_PIN, OUTPUT);
  digitalWrite(DIMMER_TOP_LIGHT_ON_PIN, LOW);

}
