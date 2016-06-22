#include "sectors.h"
#include "table-controller-pin-config.h"
#include "sector_test_mode.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define N_ELEMS(a) (sizeof(a)/sizeof(a[0]))

enum
{
  SECTOR_MOST_LEFT = 0,

  SECTOR_MOST_RIGHT = 14,

  SECTOR_NONE = -1,

  SECTOR_INVALID = -2,

};

static bool event_is_ready = false;

static SectorEventData sector_event_data;

static volatile bool wait_for_spin_flag = false;

static void reset_sector_sensors_state();

static void reset_wait_for_spin_flag()
{
  noInterrupts();
  wait_for_spin_flag = false;
  interrupts();
}

void set_wait_for_spin_flag()
{
  noInterrupts();
  wait_for_spin_flag = true;
  interrupts();
  reset_sector_sensors_state();
  
}

bool is_sector_event_ready(SectorEventData *data)
{
  noInterrupts();
  bool retval = event_is_ready;
  if (event_is_ready)
    *data = sector_event_data;
  interrupts();
  return retval;
}

void reset_sector_event()
{
  noInterrupts();
  event_is_ready = false;
  sector_event_data.reset();
  interrupts();
}

void sectors_init()
{
  Serial.println("Sector pins initialized");

  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];

    if (SECTOR_PIN_ACTIVE_LEVEL == HIGH)
      pinMode(sp.sensor_pin, INPUT);
    else if (SECTOR_PIN_ACTIVE_LEVEL == LOW)
      pinMode(sp.sensor_pin, INPUT_PULLUP);
    else
      Serial.println("Invalid value for sector pin active level.");

    pinMode(sp.number_led_pin, OUTPUT);
    digitalWrite(sp.number_led_pin, LOW);

    pinMode(sp.arrow_led_pin, OUTPUT);
    digitalWrite(sp.arrow_led_pin, LOW);
  }
  reset_wait_for_spin_flag();
}

static bool check_number(int n)
{
  if (n >= 0 && (size_t) n < NUMBER_OF_SECTORS)
  {
    return true;
  }
  Serial.print("check_number(): Invalid sector number: ");
  Serial.println(n);
  return false;
}

static bool check_number_convert_to_internal(int& n)
{
  int internal_number = n - 1;
//  int internal_number = n;
  if (check_number(internal_number))
  {
    n = internal_number;
    return true;
  }
  return false;
}

static int convert_to_external(int n)
{
  if (check_number(n))
  {
    return n + 1;
  }
  return SECTOR_INVALID;
}

bool sectors_all_leds_write(bool on)
{
  reset_wait_for_spin_flag();
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.number_led_pin, on);
    digitalWrite(sp.arrow_led_pin, on);
  }
  return true;
}

bool sector_all_number_leds_write(bool on)
{
  reset_wait_for_spin_flag();
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.number_led_pin, on);
  }
  return true;
}
bool sector_all_arrow_leds_write(bool on)
{
  reset_wait_for_spin_flag();
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.arrow_led_pin, on);
  }
  return true;
}

bool sector_number_led_pin_write(int n, bool on)
{
  reset_wait_for_spin_flag();
  if (!check_number_convert_to_internal(n))
    return false;

  SectorPins sp = sector_pins[n];
  pinMode(sp.number_led_pin, OUTPUT);
  digitalWrite(sp.number_led_pin, on);
  return true;
}


bool sector_arrow_led_pin_write(int n, bool on)
{
  reset_wait_for_spin_flag();
  if (!check_number_convert_to_internal(n))
    return false;

  SectorPins sp = sector_pins[n];
  pinMode(sp.arrow_led_pin, OUTPUT);
  digitalWrite(sp.arrow_led_pin, on);
  return true;
}

typedef int Sector;

static Sector next_right(const Sector s)
{
  if (s >= SECTOR_MOST_LEFT && s < SECTOR_MOST_RIGHT)
    return s + 1;
  if (s == SECTOR_MOST_RIGHT)
    return SECTOR_MOST_LEFT;
  Serial.print("ERROR: Invalid sector number: ");
  Serial.println(s);
  return SECTOR_INVALID;
}

bool sectors_show_playing_sector(int actual_sector, int playing_sector)
{
  if (!check_number_convert_to_internal(actual_sector) || !check_number_convert_to_internal(playing_sector))
    return false;
    
  if (actual_sector == playing_sector)
  {
    SectorPins sp = sector_pins[actual_sector];
    pinMode(sp.number_led_pin, OUTPUT);
    digitalWrite(sp.number_led_pin, true);
  }
  else
  {
    do
    {
      SectorPins sp = sector_pins[actual_sector];
      pinMode(sp.arrow_led_pin, OUTPUT);
      digitalWrite(sp.arrow_led_pin, true);
      
      actual_sector = next_right(actual_sector);
    } while (actual_sector != playing_sector);
    
    SectorPins sp = sector_pins[actual_sector];
    pinMode(sp.number_led_pin, OUTPUT);
    digitalWrite(sp.number_led_pin, true);
  }
  return true;
}

// either returns SECTOR_NONE or any one of enabled sectors, never returns SECTOR_INVALID
static Sector check_enabled_sector()
{
  for (Sector i = 0; i < (Sector) N_ELEMS(sector_pins); ++i)
  {
    const SectorPins sp = sector_pins[i];
    bool on = (digitalRead(sp.sensor_pin) == SECTOR_PIN_ACTIVE_LEVEL);
    if (on)
      return i;
  }
  return SECTOR_NONE;
}

typedef enum
{
  STATE_READY,
  STATE_WAIT_FOR_DIRECTION,
  STATE_WAIT_FOR_NEXT_SECTOR,
  STATE_WAIT_FOR_STOP,
} State;

static State state = STATE_READY;

static void reset_sector_sensors_state()
{
  state = STATE_READY;
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.number_led_pin, false);
    digitalWrite(sp.arrow_led_pin, false);
  }
}

void read_sector_sensors_and_light_their_leds()
{  
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.number_led_pin, digitalRead(sp.sensor_pin));
  }
}

void read_sector_sensors_and_light_all_leds()
{  
  for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
  {
    SectorPins sp = sector_pins[i];
    digitalWrite(sp.number_led_pin, digitalRead(sp.sensor_pin));
    digitalWrite(sp.arrow_led_pin, digitalRead(sp.sensor_pin));
  }
}

//#define SECTORS_DEBUG_MSG

void sectors_process_sensors()
{
  static Sector prev_sector;

  static unsigned long prev_time;

  if (sector_test_mode_is_enabled())
  {
    read_sector_sensors_and_light_all_leds();
    return;
  }

  if (!wait_for_spin_flag)
    return;
  
  read_sector_sensors_and_light_their_leds();

  if (state == STATE_READY)
  {
    Sector s = check_enabled_sector();
    switch (s)
    {
    case SECTOR_NONE:
      return;
    case SECTOR_INVALID:
      reset_sector_sensors_state();
      return;
    default:
      if (!check_number(s))
      {
        reset_sector_sensors_state();
        return;
      }
      prev_time = millis();
      prev_sector = s;
      state = STATE_WAIT_FOR_DIRECTION;
      #ifdef SECTORS_DEBUG_MSG
      Serial.print(prev_time);
      Serial.print(": First sector: ");
      Serial.println(s);
      #endif
      return;
    }
  }
  else if (state == STATE_WAIT_FOR_DIRECTION)
  {
    if (millis() - prev_time >= 2 * SECTOR_TURN_TIME_MS)
    {
      #ifdef SECTORS_DEBUG_MSG
      Serial.println("Waited for direction detection for too long...");
      #endif
      reset_sector_sensors_state();
      return;
    }

    Sector s = check_enabled_sector();

    switch (s)
    {
    case SECTOR_NONE:
      return;
    case SECTOR_INVALID:
      reset_sector_sensors_state();
      return;
    default:
      if (s == prev_sector)
      {
        prev_time = millis();
      }
      else
      {
        prev_sector = s;
        prev_time = millis();

        event_is_ready = true;
        sector_event_data.set_event_started();
        state = STATE_WAIT_FOR_STOP;
      }
      return;
    }
  }
  else if (state == STATE_WAIT_FOR_STOP)
  {
    Sector s = check_enabled_sector();
    if (millis() - prev_time >= SECTOR_TURN_TIME_MS)
    {
      switch (s)
      {
      case SECTOR_NONE:
        event_is_ready = true;
        sector_event_data.set_event_stopped(convert_to_external(prev_sector));
        wait_for_spin_flag = false;
        break;
      case SECTOR_INVALID:
        #ifdef SECTORS_DEBUG_MSG
        Serial.println("Invalid sector detected while waiting for STOP");
        #endif
        break;
      default:
        if (!check_number(s))
        {
          #ifdef SECTORS_DEBUG_MSG
          Serial.println("Wrong number sector detected while waiting for STOP");
          #endif
          break;
        }
        event_is_ready = true;
        sector_event_data.set_event_stopped(convert_to_external(s));
        wait_for_spin_flag = false;
        break;
      }
      reset_sector_sensors_state();
      return;
    }
    if (s != SECTOR_NONE && s != prev_sector && check_number(s))
    {
      prev_time = millis();
      prev_sector = s;
    }
  }
  else
  {
    #ifdef SECTORS_DEBUG_MSG
    Serial.print("Invalid sector state: ");
    Serial.println(state);
    #endif
    reset_sector_sensors_state();
  }
}
