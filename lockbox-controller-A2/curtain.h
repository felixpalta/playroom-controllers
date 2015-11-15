// curtain.h

#ifndef _CURTAIN_h
#define _CURTAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void curtain_init();

bool curtain_signal_open();

bool curtain_signal_close();

#endif

