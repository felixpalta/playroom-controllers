#ifndef __TABLE_CONTROLLER_PIN_CONFIG_H__
#define __TABLE_CONTROLLER_PIN_CONFIG_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

static const size_t NUMBER_OF_SECTORS = 15;

// NOTE: If active level is HIGH,
// then external pull-down resistors should be connected to the pins.
static const int SECTOR_PIN_ACTIVE_LEVEL = HIGH;

struct SectorPins
{
  int sensor_pin;
  int number_led_pin;
  int arrow_led_pin;
};

/* NOTE:
    Pins 0, 1 are used by UART0 and cannot be used as common GPIO.
    Pins 50, 51, 52, 53 are used by SPI interface and cannot be used as common GPIO.
    Pins 4 and 10 are used as CS for SD card and for Ethernet shield and cannot be used as common GPIO.
    Pin 13 is used for debug indication.
*/

#define W5200_CS  10
#define SDCARD_CS 4

#define DBG_PIN 13

static const SectorPins sector_pins[NUMBER_OF_SECTORS] =
{
  /* sensor pin, number led pin, arrow led pin */
  { 12, 20, 23 }, // Sector 1
  { 11, 21, 25 }, // Sector 2
  {  9, 22, 27 }, // Sector 3
  {  8, 24, 29 }, // Sector 4
  {  7, 26, 31 }, // Sector 5
  {  6, 28, 33 }, // Sector 6
  {  5, 30, 35 }, // Sector 7
  {  3, 32, 37 }, // Sector 8
  {  2, 34, 39 }, // Sector 9
  { 14, 36, 41 }, // Sector 10
  { 15, 38, 43 }, // Sector 11
  { 16, 40, 45 }, // Sector 12
  { 17, 42, 47 }, // Sector 13
  { 18, 44, 48 }, // Sector 14
  { 19, 46, 49 }, // Sector 15

};

static const unsigned long SECTOR_TURN_TIME_MS = 5000;

static const int FULL_TURN_LIMIT = 15;

#endif // __TABLE_CONTROLLER_PIN_CONFIG_H__
