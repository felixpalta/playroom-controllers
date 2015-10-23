#ifndef __MYSERIAL_H__
#define __MYSERIAL_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SwitchablePrinter.h>

extern SwitchablePrinter MySerial;
#endif

