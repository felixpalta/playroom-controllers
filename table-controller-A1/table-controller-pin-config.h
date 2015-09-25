#ifndef __TABLE_CONTROLLER_PIN_CONFIG_H__
#define __TABLE_CONTROLLER_PIN_CONFIG_H__

static const size_t NUMBER_OF_SECTOR_PINS = 15;

// NOTE: If active level is LOW,
// then external pull-down resistors should be connected to the pins.
static const int SECTOR_PIN_ACTIVE_LEVEL = HIGH;

static const int sector_pins[NUMBER_OF_SECTOR_PINS] =
{
	3,	// Sector 1
	4,	// Sector 2
	5,	// Sector 3
	6,	// Sector 4
	7,	// Sector 5
	8,	// Sector 6
	9,	// Sector 7
	10,	// Sector 8
	11,	// Sector 9
	12,	// Sector 10
	// NOTE: Pin 13 should NOT be used.
	14,	// Sector 11
	15,	// Sector 12
	16,	// Sector 13
	17,	// Sector 14
	18,	// Sector 15
};



#endif // __TABLE_CONTROLLER_PIN_CONFIG_H__
