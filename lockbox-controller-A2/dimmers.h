// dimmers.h

#ifndef _DIMMERS_h
#define _DIMMERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif



void dimmers_init();

void dimmers_irq_handler();

#endif

