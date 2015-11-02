#include "dimmers.h"
#include <MsTimer2.h>

#define N_ELEMS(arr) (sizeof(arr)/sizeof(arr[0]))

#define N_DIMMERS N_ELEMS(dimmer_pins)

#define DIMMING_STEPS_NUMBER 100
#define DIM_MAX_LEVEL 90
#define TRIAC_ON_PULSE_WIDTH_US 10
#define AC_HALF_PERIOD_US 10000UL
#define DIMMING_STEP_WIDTH_US ((AC_HALF_PERIOD_US - TRIAC_ON_PULSE_WIDTH_US)/DIMMING_STEPS_NUMBER)

#define DIMMING_START_LEVEL 50

struct Dimmer
{
  DimmerEnum id;
  int dimming_level; // 0% dimming - full light, 100% dimming - no light.
  unsigned long dimming_delay_us;
  bool dynamic_dimming;
};

static Dimmer dimmers[N_DIMMERS] =
{
  { DIMMER_TOP_LIGHT, 0, 0, false },
  { DIMMER_SURROUND_LIGHT, 0, 0, false },
  { DIMMER_LOCKBOX_LIGHT, 0, 0, false },
};

static unsigned long last_zero_cross_time_us = 0;

static bool timer_enabled = false;

static void ms_timer_irq_handler();

static void zero_cross_irq_handler()
{
  last_zero_cross_time_us = micros();
  if (!timer_enabled)
  {
    timer_enabled = true;
    MsTimer2::set(1, ms_timer_irq_handler);
    MsTimer2::start();
  }
}

static void set_dimming_to_100(DimmerEnum dimmer_id)
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmer_pins[i].number == dimmer_id)
    {
      pinMode(dimmer_pins[i].pin, OUTPUT);
      digitalWrite(dimmer_pins[i].pin, LOW);
      return;
    }
  }
}

static bool enabled_dimmers_present()
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].dynamic_dimming == true)
      return true;
  }
  return false;
}

static void triac_pulse(DimmerEnum id)
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmer_pins[i].number == id)
    {
      digitalWrite(dimmer_pins[i].pin, HIGH);
      delayMicroseconds(TRIAC_ON_PULSE_WIDTH_US);
      digitalWrite(dimmer_pins[i].pin, LOW);
      return;
    }
  }
}

static void ms_timer_irq_handler()
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].dynamic_dimming == true &&
      micros() - last_zero_cross_time_us >= dimmers[i].dimming_delay_us)
    {
      triac_pulse(dimmers[i].id);
      dimmers[i].dynamic_dimming = false;
      dimmers[i].dimming_level = 0;
      dimmers[i].dimming_delay_us = 0;
    }
  }
  
  if (!enabled_dimmers_present())
  {
    MsTimer2::stop();
    timer_enabled = false;
  }

}

void dimmers_init()
{
  
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    pinMode(dimmer_pins[i].pin, OUTPUT);
    digitalWrite(dimmer_pins[i].pin, LOW);
  }

  pinMode(common_zero_cross_irq_pin.pin, INPUT);
  attachInterrupt(common_zero_cross_irq_pin.irq_n, zero_cross_irq_handler, RISING);  // Choose the zero cross interrupt # from the table above
}

static Dimmer *get_dimmer(DimmerEnum dimmer_id)
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].id == dimmer_id)
      return &dimmers[i];
  }
  return 0;
}

bool dimmers_light_set(DimmerEnum dimmer_id, int light_level)
{
  Dimmer *dimmer = get_dimmer(dimmer_id);
  if (dimmer)
  {
    // covert light percent to dimming percent
    light_level = constrain(light_level, 0, 100);
    int dimming_level = 100 - light_level;

    if (dimming_level > DIM_MAX_LEVEL)
    {
      dimmer->dimming_level = 0;
      dimmer->dimming_delay_us = 0;
      dimmer->dynamic_dimming = false;
      set_dimming_to_100(dimmer_id);
    }
    else
    {
      dimmer->dimming_level = dimming_level;
      dimmer->dimming_delay_us = dimming_level * DIMMING_STEP_WIDTH_US;
      dimmer->dynamic_dimming = true;
    }
    return true;
  }

  Serial.println("Invalid Dimmer ID");
  return false;
}

