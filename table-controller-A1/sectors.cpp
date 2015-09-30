
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

void sectors_rotation_started_callback();
void sectors_roatation_stopped_callback(int sector_n);



struct SectorPollResult
{
private:
	size_t number_of_sectors_; // can be 0, 1 or 2, or -1 as invalid value.
public:
	int sectors[2];

	size_t number_of_sectors() const { return number_of_sectors_; }

	// true if 0 sectors are on, false if more than 2 sectors are on.
	SectorPollResult(bool valid)
		: number_of_sectors_(valid ? 0 : -1), sectors() {}

	SectorPollResult(int sector)
		: number_of_sectors_(1)
	{
		sectors[0] = sector;
	}

	SectorPollResult(int first, int second)
		: number_of_sectors_(2)
	{
		sectors[0] = first;
		sectors[1] = second;
	}
};

// Need to keep track of two previous sector states to correctly detect movement.
static SectorPollResult sector_oldest(true);
static SectorPollResult sector_old(true);

static SectorPollResult detect_current_sector()
{
	int count = 0;
	int sectors[2];
	for (size_t i = 0; i < N_ELEMS(sector_pins); ++i)
	{
		bool state = digitalRead(sector_pins[i].sensor_pin);
		if (state)
		{
			++count;
			if (count >= 3)
				break;
			sectors[count - 1] = i;
			
		}
	}
	switch (count)
	{
	case 0:
		return SectorPollResult(true);
	case 1:
		return SectorPollResult(sectors[0]);
	case 2:
		SectorPollResult(sectors[0], sectors[1]);
	default:
		return SectorPollResult(false);
	
	}

}

bool sector_state_get(int i)
{
	return digitalRead(sector_pins[i].sensor_pin); 
}


void sector_sensors_process()
{

}
