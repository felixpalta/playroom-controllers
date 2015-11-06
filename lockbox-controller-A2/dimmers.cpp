#include "dimmers.h"
#include "lockbox-controller-pin-config.h"
#include <FlexiTimer2.h>

#define N_ELEMS(arr) (sizeof(arr)/sizeof(arr[0]))

#define DIMMING_STEPS_NUMBER 100
#define DIM_MIN_LEVEL 10
#define DIM_MAX_LEVEL 90
#define TRIAC_ON_PULSE_WIDTH_US 10
#define AC_HALF_PERIOD_US 10000UL

#define DIMMING_START_LEVEL 50

struct Dimmer
{
  DimmerEnum id;
  int on_pin;
  uint8_t dimming_percent;
  bool dimming_enabled;
  bool pulse_pending;
};

static volatile Dimmer dimmers[] =
{
  { DIMMER_TOP_LIGHT, DIMMER_TOP_LIGHT_ON_PIN, 0, false, false },
  { DIMMER_SURROUND_LIGHT, DIMMER_SURROUND_LIGHT_ON_PIN, 0, false, false },
  { DIMMER_LOCKBOX_LIGHT, DIMMER_LOCKBOX_LIGHT_ON_PIN, 0, false, false },
};

#define N_DIMMERS N_ELEMS(dimmers)

static volatile uint8_t percent_ticker = 0;

static volatile uint8_t n_pending_dimmers = 0;

////////////////////////////////////////////////////////////////////

static void ms_timer_irq_handler();

static void zero_cross_irq_handler();

static void set_dimming_to_100(const volatile Dimmer *dimmer);

static void set_dimming_to_0(const volatile Dimmer *dimmer);

static void triac_pulse(const volatile Dimmer *dimmer);

static volatile Dimmer *get_dimmer(DimmerEnum dimmer_id)
{
  for (uint8_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].id == dimmer_id)
      return &dimmers[i];
  }
  return 0;
}

////////////////////////////////////////////////////////////////////

void dimmers_init()
{
  for (uint8_t i = 0; i < N_DIMMERS; ++i)
  {
    pinMode(dimmers[i].on_pin, OUTPUT);
    digitalWrite(dimmers[i].on_pin, LOW);
  }
  
  n_pending_dimmers = 0;

  pinMode(common_zero_cross_irq_pin.pin, INPUT);
  attachInterrupt(common_zero_cross_irq_pin.irq_n, zero_cross_irq_handler, RISING);
}

bool dimmers_light_set(DimmerEnum dimmer_id, int light_level)
{
  volatile Dimmer *dimmer = get_dimmer(dimmer_id);
  if (dimmer)
  {
    // convert light percent to dimming percent
    light_level = constrain(light_level, 0, 100);
    int dimming_level = 100 - light_level;

    if (dimming_level < DIM_MIN_LEVEL)
    {
      noInterrupts();
      dimmer->dimming_percent = 0;
      dimmer->dimming_enabled = false;
      dimmer->pulse_pending = false;
      set_dimming_to_0(dimmer);
      interrupts();
      Serial.print("Dimmer #"); Serial.print(dimmer_id); Serial.print(" set to ");Serial.println("0% and stopped");
    }
    else if (dimming_level > DIM_MAX_LEVEL)
    {
      noInterrupts();
      dimmer->dimming_percent = 0;
      dimmer->dimming_enabled = false;
      dimmer->pulse_pending = false;
      set_dimming_to_100(dimmer);
      interrupts();
      Serial.print("Dimmer #"); Serial.print(dimmer_id); Serial.print(" set to ");Serial.println("100% and stopped");
      
    }
    else
    {
      noInterrupts();
      dimmer->dimming_percent = dimming_level;
      dimmer->dimming_enabled = true;
      dimmer->pulse_pending = false;
      interrupts();
      Serial.print("Dimmer #"); Serial.print(dimmer_id); Serial.print(" set to "); Serial.println(dimming_level);
    }

    return true;
  }

  Serial.print("Invalid Dimmer ID: "); Serial.println(dimmer_id);
  return false;
}

////////////////////////////////////////////////////////////////////

static void zero_cross_irq_handler()
{
  percent_ticker = 0;
  
  FlexiTimer2::stop();
  
  n_pending_dimmers = 0;
  
  for (uint8_t i = 0; i < N_DIMMERS; ++i)
  {
    if (dimmers[i].dimming_enabled)
    {
      ++n_pending_dimmers;
      dimmers[i].pulse_pending = true;
    }
  }

  if (n_pending_dimmers != 0)
  {
    // Timer ticks every 100 us, providing 1% resolution over 10ms span of AC half-period.
    FlexiTimer2::set(1, 1.0/10000, ms_timer_irq_handler);
    FlexiTimer2::start();
  }
}

static void ms_timer_irq_handler()
{
  ++percent_ticker;
  for (uint8_t i = 0; i < N_DIMMERS; ++i)
  {
    volatile Dimmer& d = dimmers[i];
    if (d.dimming_enabled && d.pulse_pending && (percent_ticker >= d.dimming_percent))
    {
      triac_pulse(&d);
      d.pulse_pending = false;
      --n_pending_dimmers;
    }
  }
  
  if (n_pending_dimmers == 0)
  {
    FlexiTimer2::stop();
    for (uint8_t i = 0; i < N_DIMMERS; ++i)
    {
      dimmers[i].pulse_pending = false;
    }
  }
}

////////////////////////////////////////////////////////////////////

static void set_dimming_to_100(const volatile Dimmer *dimmer)
{
  if (dimmer)
  {
    pinMode(dimmer->on_pin, OUTPUT);
    digitalWrite(dimmer->on_pin, LOW);
  }
}

static void set_dimming_to_0(const volatile Dimmer *dimmer)
{
  if (dimmer)
  {
    pinMode(dimmer->on_pin, OUTPUT);
    digitalWrite(dimmer->on_pin, HIGH);
  }
}

static void triac_pulse(const volatile Dimmer *dimmer)
{
  if (dimmer)
  {
    pinMode(dimmer->on_pin, OUTPUT);
    digitalWrite(dimmer->on_pin, HIGH);
    delayMicroseconds(TRIAC_ON_PULSE_WIDTH_US);
    digitalWrite(dimmer->on_pin, LOW);
  }
}
