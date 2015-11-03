#include "dimmers.h"
#include "lockbox-controller-pin-config.h"
#include <MsTimer2.h>

#define N_ELEMS(arr) (sizeof(arr)/sizeof(arr[0]))

#define DIMMING_STEPS_NUMBER 100
#define DIM_MAX_LEVEL 90
#define TRIAC_ON_PULSE_WIDTH_US 10
#define AC_HALF_PERIOD_US 10000UL
#define DIMMING_STEP_WIDTH_US ((AC_HALF_PERIOD_US - TRIAC_ON_PULSE_WIDTH_US)/DIMMING_STEPS_NUMBER)

#define DIMMING_START_LEVEL 50

struct Dimmer
{
  DimmerEnum id;
  int on_pin;
  int dimming_level; // 0% dimming - full light, 100% dimming - no light.
  unsigned long dimming_delay_us;
  bool dimming_enabled;
  bool pulse_pending;
};

static Dimmer dimmers[] =
{
  { DIMMER_TOP_LIGHT, DIMMER_TOP_LIGHT_ON_PIN, 0, 0, false, false },
  { DIMMER_SURROUND_LIGHT, DIMMER_SURROUND_LIGHT_ON_PIN, 0, 0, false, false },
  { DIMMER_LOCKBOX_LIGHT, DIMMER_LOCKBOX_LIGHT_ON_PIN, 0, 0, false, false },
};

#define N_DIMMERS N_ELEMS(dimmers)

static unsigned long last_zero_cross_time_us = 0;

static bool timer_enabled = false;

////////////////////////////////////////////////////////////////////

static void ms_timer_irq_handler();

static void zero_cross_irq_handler();

static void set_dimming_to_100(const Dimmer *dimmer);

static void triac_pulse(const Dimmer *dimmer);

static bool enabled_dimmers_present();

static Dimmer *get_dimmer(DimmerEnum dimmer_id)
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].id == dimmer_id)
      return &dimmers[i];
  }
  return 0;
}

////////////////////////////////////////////////////////////////////

void dimmers_init()
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    pinMode(dimmers[i].on_pin, OUTPUT);
    digitalWrite(dimmers[i].on_pin, LOW);
  }

  pinMode(common_zero_cross_irq_pin.pin, INPUT);
  attachInterrupt(common_zero_cross_irq_pin.irq_n, zero_cross_irq_handler, RISING);
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
      dimmer->dimming_enabled = false;
      dimmer->pulse_pending = false;
      set_dimming_to_100(dimmer);
    }
    else
    {
      dimmer->dimming_level = dimming_level;
      dimmer->dimming_delay_us = dimming_level * DIMMING_STEP_WIDTH_US;
      dimmer->dimming_enabled = true;
      dimmer->pulse_pending = true;
    }
    Serial.print("Dimmer #"); Serial.print(dimmer_id); Serial.print(" set to "); Serial.println(dimming_level);
    return true;
  }

  Serial.print("Invalid Dimmer ID: "); Serial.println(dimmer_id);
  return false;
}

////////////////////////////////////////////////////////////////////

static void zero_cross_irq_handler()
{
  last_zero_cross_time_us = micros();
  
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].dimming_enabled)
      dimmers[i].pulse_pending = true;
  }

  if (!timer_enabled)
  {
    timer_enabled = true;
    MsTimer2::set(1, ms_timer_irq_handler);
    MsTimer2::start();
  }
}

static void ms_timer_irq_handler()
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    Dimmer& d = dimmers[i];
    if (d.dimming_enabled && d.pulse_pending && ((micros() - last_zero_cross_time_us) >= d.dimming_delay_us))
    {
      triac_pulse(&d);
      d.pulse_pending = false;
    }
  }
  
  if (!enabled_dimmers_present())
  {
    MsTimer2::stop();
    timer_enabled = false;
    for (size_t i = 0; i < N_DIMMERS; ++i)
    {
      dimmers[i].pulse_pending = false;
    }
  }
}

////////////////////////////////////////////////////////////////////

static void set_dimming_to_100(const Dimmer *dimmer)
{
  if (dimmer)
  {
    pinMode(dimmer->on_pin, OUTPUT);
    digitalWrite(dimmer->on_pin, LOW);
  }
}

// Return true if there is at least one enabled dimmer in the list of dimmers.
static bool enabled_dimmers_present()
{
  for (size_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].dimming_enabled == true)
      return true;
  }
  return false;
}

static void triac_pulse(const Dimmer *dimmer)
{
  if (dimmer)
  {
    pinMode(dimmer->on_pin, OUTPUT);
    digitalWrite(dimmer->on_pin, HIGH);
    delayMicroseconds(TRIAC_ON_PULSE_WIDTH_US);
    digitalWrite(dimmer->on_pin, LOW);
  }
}





