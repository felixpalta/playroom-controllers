
#include "table-controller-pin-config.h"

#define N_ELEMS(a) (sizeof(a)/sizeof(a[0]))

void sectors_init()
{
	Serial.println("Sector pins initialized");

	for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
	{
		if (SECTOR_PIN_ACTIVE_LEVEL == HIGH)
			pinMode(sector_pins[i], INPUT);
		else if (SECTOR_PIN_ACTIVE_LEVEL == LOW)
			pinMode(sector_pins[i], INPUT_PULLUP);
		else
			Serial.println("Invalid value for sector pin active level.");
	}
}
