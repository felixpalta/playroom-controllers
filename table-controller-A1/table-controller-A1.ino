#include "sectors.h"

void setup()
{
	Serial.begin(9600);
	Serial.println("Hello from Table Controller");
	sectors_init();
}

void loop()
{
	sectors_process_sensors();
}

void sectors_rotation_started_callback()
{
	Serial.println("ROTATION STARTED CALLBACK");
}
void sectors_rotation_stopped_callback(int sector_n)
{
	Serial.print("ROTATION STOPPED CALLBACK: ");
	Serial.println(sector_n);
}
