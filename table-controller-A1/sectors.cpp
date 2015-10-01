
#include "table-controller-pin-config.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

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

static bool check_number(int n)
{
	if (n >= 0 && (size_t) n < NUMBER_OF_SECTORS)
	{
		return true;
	}
	return false;
}

static bool check_number_convert_to_internal(int& n)
{
	int internal_number = n - 1;
	if (check_number(internal_number))
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

void sectors_rotation_started_callback();
void sectors_roatation_stopped_callback(int sector_n);

typedef int Sector;

enum
{
	SECTOR_MOST_LEFT = 0,

	SECTOR_MOST_RIGHT = 14,

	SECTOR_NONE = -1,

	SECTOR_INVALID = -2,

};

Sector next_right(const Sector s)
{
	if (s >= SECTOR_MOST_LEFT && s < SECTOR_MOST_RIGHT)
		return s + 1;
	if (s == SECTOR_MOST_RIGHT)
		return SECTOR_MOST_LEFT;
	Serial.print("ERROR: Invalid sector number: ");
	Serial.println(s);
	return SECTOR_INVALID;
}

Sector prev_left(const Sector s)
{
	if (s > SECTOR_MOST_LEFT && s <= SECTOR_MOST_RIGHT)
		return s - 1;
	if (s == SECTOR_MOST_LEFT)
		return SECTOR_MOST_RIGHT;
	Serial.print("ERROR: Invalid sector number: ");
	Serial.println(s);
	return SECTOR_INVALID;
}
//
//struct SectorsEnabled
//{
//	int number; // should be 0, 1 or 2. It is an error if more than 2 sectors are enabled.
//	Sector sector; // if 'number' is 1 or 2, then 'sector' holds number of one of the enabled sectors (doesn't matter which one).
//	SectorsEnabled(int n = 0, Sector s = SECTOR_INVALID);
//};
//
//// Returns number of sectors that are enabled.
//// Normally should be > 0 && <= 2.
//// Also, if number of sectors is 2, checks whether these
//// sectors are adjacent.
//SectorsEnabled check_enabled_sectors()
//{
//	int on_count = 0;
//	Sector enabled_sectors[2];
//	for (Sector i = 0; i < (Sector) N_ELEMS(sector_pins); ++i)
//	{
//		const SectorPins sp = sector_pins[i];
//		bool on = digitalRead(sp.sensor_pin);
//		if (on)
//		{
//			++on_count;
//			if (on_count >= 3)
//				break;
//			enabled_sectors[on_count - 1] = i;
//		}
//	}
//
//	switch (on_count)
//	{
//	case 0:
//		return SectorsEnabled();
//	case 1:
//		return SectorsEnabled(1, enabled_sectors[0]);
//	case 2:
//		if ((enabled_sectors[1] == next_right(enabled_sectors[0])) ||
//			(enabled_sectors[1] == prev_left(enabled_sectors[0])))
//		{
//			return SectorsEnabled(2, enabled_sectors[0]);
//		}
//		Serial.print("sectors not adjacent: ");
//		Serial.print(enabled_sectors[0]);
//		Serial.println(enabled_sectors[1]);
//		return SectorsEnabled();
//	default:
//		Serial.println("too many sectors are on");
//		return SectorsEnabled();
//	}
//	
//}

Sector check_enabled_sector()
{
	int on_count = 0;
	Sector enabled_sectors[2];
	for (Sector i = 0; i < (Sector) N_ELEMS(sector_pins); ++i)
	{
		const SectorPins sp = sector_pins[i];
		bool on = digitalRead(sp.sensor_pin);
		if (on)
		{
			++on_count;
			if (on_count >= 3)
				break;
			enabled_sectors[on_count - 1] = i;
		}
	}

	switch (on_count)
	{
	case 0:
		return SECTOR_NONE;
	case 1:
		return enabled_sectors[0];
	case 2:
		if ((enabled_sectors[1] == next_right(enabled_sectors[0])) ||
			(enabled_sectors[1] == prev_left(enabled_sectors[0])))
		{
			return enabled_sectors[0];
		}
		Serial.print("sectors not adjacent: ");
		Serial.print(enabled_sectors[0]);
		Serial.println(enabled_sectors[1]);
		return SECTOR_INVALID;
	default:
		Serial.println("too many sectors are on");
		return SECTOR_INVALID;
	}
}

bool check_sector_is_on(Sector s)
{
	if (!check_number(s))
		return false;
	return digitalRead(sector_pins[s].sensor_pin);
}