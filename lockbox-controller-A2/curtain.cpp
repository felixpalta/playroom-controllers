// 
// 
// 

#include "curtain.h"
#include "lockbox-controller-pin-config.h"

#define PULSE_MS 50

#define FULL_MOVE_TIME_MS 15000

typedef enum
{
  STOPPED,
  CLOSING,
  OPENING,
} CurtainState;

static CurtainState curtain_state;

unsigned long last_change_time;

static void pulse_opening_line()
{
  digitalWrite(CURTAIN_OPEN_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(CURTAIN_OPEN_PIN, LOW);
}

static void pulse_closing_line()
{
  digitalWrite(CURTAIN_CLOSE_PIN, HIGH);
  delay(PULSE_MS);
  digitalWrite(CURTAIN_CLOSE_PIN, LOW);
}

void curtain_init()
{
  pinMode(CURTAIN_OPEN_PIN, OUTPUT);
  digitalWrite(CURTAIN_OPEN_PIN, LOW);
  pinMode(CURTAIN_CLOSE_PIN, OUTPUT);
  digitalWrite(CURTAIN_CLOSE_PIN, LOW);

  curtain_state = STOPPED;

}

void curtain_process()
{
  switch (curtain_state)
  {
    case STOPPED:
      break;
    case OPENING:
      if (millis() - last_change_time >= FULL_MOVE_TIME_MS)
      {
        pulse_opening_line();
        curtain_state = STOPPED;
      }
      break;
    case CLOSING:
      if (millis() - last_change_time >= FULL_MOVE_TIME_MS)
      {
        pulse_closing_line();
        curtain_state = STOPPED;
      }
      break;
    default:
      break;
  }
}

bool curtain_signal_open()
{
  switch (curtain_state)
  {
    case STOPPED:
      pulse_opening_line();
      last_change_time = millis();
      curtain_state = OPENING;
      break;
    case OPENING:
      break;
    case CLOSING:
      pulse_closing_line();
      pulse_opening_line();

      last_change_time = millis();
      curtain_state = OPENING;
      break;
    default:
      return false;;
  };

  return true;
}

bool curtain_signal_close()
{
  switch (curtain_state)
  {
    case STOPPED:
      pulse_closing_line();
      last_change_time = millis();
      curtain_state = CLOSING;
      break;
    case CLOSING:
      break;
    case OPENING:
      pulse_opening_line();
      pulse_closing_line();

      last_change_time = millis();
      curtain_state = CLOSING;
      break;
    default:
      return false;;
  };

  return true;
}

