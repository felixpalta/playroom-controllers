
#include "table-controller-pin-config.h"

#define N_ELEMS(a) (sizeof(a)/sizeof(a[0]))

void sectors_init()
{
	Serial.println("Sector pins initialized");

	for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
	{
		pinMode(sector_pins[i], INPUT);
	}
}
