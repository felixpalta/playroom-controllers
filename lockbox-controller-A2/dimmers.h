#ifndef __DIMMERS_H__
#define __DIMMERS_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

typedef enum
{
  DIMMER_INVALID,
  DIMMER_TOP_LIGHT,
  DIMMER_SURROUND_LIGHT,
  DIMMER_LOCKBOX_LIGHT,
} DimmerEnum;

void dimmers_init();
bool dimmers_light_set(DimmerEnum dimmer_id, int light_level);

#endif // __DIMMERS_H__

