#include "dimmers.h"
#include <MsTimer2.h>

/*
Pin    |  Interrrupt # | Arduino Platform
---------------------------------------
2      |  0            |  All -But it is INT1 on the Leonardo
3      |  1            |  All -But it is INT0 on the Leonardo
18     |  5            |  Arduino Mega Only
19     |  4            |  Arduino Mega Only
20     |  3            |  Arduino Mega Only
21     |  2            |  Arduino Mega Only
0      |  0            |  Leonardo
1      |  3            |  Leonardo
7      |  4            |  Leonardo
The Arduino Due has no standard interrupt pins as an iterrupt can be attached to almosty any pin. 
*/

#define DIMMING_STEPS_NUMBER 100
#define DIM_MIN_LEVEL 10
#define DIM_MAX_LEVEL 90
#define TRIAC_ON_PULSE_WIDTH_US 10
#define AC_HALF_PERIOD_US 10000UL
#define DIMMING_STEP_WIDTH_US ((AC_HALF_PERIOD_US - TRIAC_ON_PULSE_WIDTH_US)/DIMMING_STEPS_NUMBER)

#define AC_LOAD 5    // Output to Opto Triac pin
#define DIMMING_START_LEVEL 50

static int dimming = DIM_MAX_LEVEL;  // Dimming level (0-100)  0 = ON, 100 = OFF
static unsigned long dimming_delay = 0;
static unsigned long last_zero_cross_time_us = 0;

static void ms_timer_irq_handler();

static void zero_cross_irq_handler()
{
  last_zero_cross_time_us = micros();
  MsTimer2::set(1, ms_timer_irq_handler);
  MsTimer2::start();  
}

static void triac_pulse()
{
  digitalWrite(AC_LOAD, HIGH);
  delayMicroseconds(TRIAC_ON_PULSE_WIDTH_US);
  digitalWrite(AC_LOAD, LOW);
}

static void ms_timer_irq_handler()
{
  if (micros() - last_zero_cross_time_us >= dimming_delay)
  {
    MsTimer2::stop();
    triac_pulse();
  }
}

void dimmers_init()
{
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output

  dimming = DIMMING_START_LEVEL;
  dimming_delay = DIMMING_STEP_WIDTH_US * dimming;
  Serial.println("Dimming step width, us: ");
  Serial.println(DIMMING_STEP_WIDTH_US);
  attachInterrupt(0, zero_cross_irq_handler, RISING);  // Choose the zero cross interrupt # from the table above
}

void dimmers_light_set(int level)
{
  // covert light percent to dimming percent
  level = constrain(level, 0, 100);
  level = 100 - level;
  
  dimming = constrain(level, DIM_MIN_LEVEL, DIM_MAX_LEVEL);
  
  dimming_delay = DIMMING_STEP_WIDTH_US * dimming;
    
}

