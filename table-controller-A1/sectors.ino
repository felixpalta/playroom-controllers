
#include "table-controller-pin-config.h"

#define N_ELEMS(a) (sizeof(a)/sizeof(a[0]))

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
}

static bool check_number_convert_to_internal(int& n)
{
	int internal_number = n - 1;
	if (internal_number >= 0 && (size_t)internal_number < NUMBER_OF_SECTORS)
	{
		n = internal_number;
		return true;
	}
	Serial.print("Invalid sector number: ");
	Serial.println(n);

	return false;
}

bool sector_number_led_pin_write(int n, bool on)
{
	if (!check_number_convert_to_internal(n))
		return false;

	SectorPins sp = sector_pins[n];
	pinMode(sp.number_led_pin, OUTPUT);
	digitalWrite(sp.number_led_pin, on);
	return true;
}


bool sector_arrow_led_pin_write(int n, bool on)
{
	if (!check_number_convert_to_internal(n))
		return false;

	SectorPins sp = sector_pins[n];
	pinMode(sp.arrow_led_pin, OUTPUT);
	digitalWrite(sp.arrow_led_pin, on);
	return true;
}
