// door_lock.h

#ifndef _DOOR_LOCK_h
#define _DOOR_LOCK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void door_lock_init();

bool door_lock_open();
bool door_lock_close();
bool door_lock_switch();

#endif

