#ifndef __TABLE_CONTROLLER_PIN_CONFIG_H__
#define __TABLE_CONTROLLER_PIN_CONFIG_H__

static const size_t NUMBER_OF_SECTORS = 15;

// NOTE: If active level is LOW,
// then external pull-down resistors should be connected to the pins.
static const int SECTOR_PIN_ACTIVE_LEVEL = HIGH;

struct SectorPins
{
	int sensor_pin;
	int number_led_pin;
	int arrow_led_pin;
};

static const SectorPins sector_pins[NUMBER_OF_SECTORS] =
{
	/* sensor pin, number led pin, arrow led pin */
	{ 3, 22, 23 }, // Sector 1
	{ 4, 24, 25 }, // Sector 2
	{ 5, 26, 27 }, // Sector 3
	{ 6, 28, 29 }, // Sector 4
	{ 7, 30, 31 }, // Sector 5
	{ 8, 32, 33 }, // Sector 6
	{ 9, 34, 35 }, // Sector 7
	{ 10, 36, 37 }, // Sector 8
	{ 11, 38, 39 }, // Sector 9
	{ 12, 40, 41 }, // Sector 10
	{ 14, 42, 43 }, // Sector 11
	{ 15, 44, 45 }, // Sector 12
	{ 16, 46, 47 }, // Sector 13
	{ 17, 48, 49 }, // Sector 14
	{ 18, 50, 51 }, // Sector 15

};

#endif // __TABLE_CONTROLLER_PIN_CONFIG_H__
