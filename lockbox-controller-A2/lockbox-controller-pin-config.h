#ifndef __LOCKBOX_CONTROLLER_PIN_CONFIG_H__
#define __LOCKBOX_CONTROLLER_PIN_CONFIG_H__

#define W5200_CS  10
#define SDCARD_CS 4

#define DBG_PIN 13

static const size_t LOCKBOX_PIN_NUMBER = 24;

static const int lockbox_pins[LOCKBOX_PIN_NUMBER] =
{
  22, // Lockbox 1
  23, // Lockbox 2
  24, // Lockbox 3
  25, // Lockbox 4
  26, // Lockbox 5
  27, // Lockbox 6
  28, // Lockbox 7
  29, // Lockbox 8
  30, // Lockbox 9
  31, // Lockbox 10
  32, // Lockbox 11
  33, // Lockbox 12
  34, // Lockbox 13
  35, // Lockbox 14
  36, // Lockbox 15
  37, // Lockbox 16
  38, // Lockbox 17
  39, // Lockbox 18
  40, // Lockbox 19
  41, // Lockbox 20
  42, // Lockbox 21
  43, // Lockbox 22
  44, // Lockbox 23
  45, // Lockbox 24
};

static const int DIMMER_TOP_LIGHT_ON_PIN = 5;
static const int DIMMER_SURROUND_LIGHT_ON_PIN = 7;
static const int DIMMER_LOCKBOX_LIGHT_ON_PIN = 9;

static const int GAME_START_BUTTON_PIN = 11;
static const int STANDBY_BUTTON_PIN = 12;
static const int CLEANING_BUTTON_PIN = 14;

static const int GAME_START_BUTTON_LIGHT_PIN = 15;
static const int STANDBY_BUTTON_LIGHT_PIN = 16;

struct InterruptPin
{
  int irq_n;
  int pin;
};

static const InterruptPin common_zero_cross_irq_pin = 
    {  0,  2 };
// {  1,  3 };
// {  2,  21 };
// {  3,  20 };
// {  4,  19 };
// {  5,  18 };


#endif // __LOCKBOX_CONTROLLER_PIN_CONFIG_H__
