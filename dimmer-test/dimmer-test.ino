/*

AC Voltage dimmer with Zero cross detection
Author: Charith Fernanado <a href="http://www.inmojo.com"> http://www.inmojo.com
</a>
Adapted by DIY_bloke
License: Creative Commons Attribution Share-Alike 3.0 License.
Attach the Zero cross pin of the module to Arduino External Interrupt pin
Select the correct Interrupt # from the below table 
(the Pin numbers are digital pins, NOT physical pins: 
digital pin 2 [INT0]=physical pin 4 and digital pin 3 [INT1]= physical pin 5)
check: <a href="http://arduino.cc/en/Reference/attachInterrupt"> http://www.inmojo.com
</a>

Pin    |  Interrrupt # | Arduino Platform
---------------------------------------
2      |  0            |  All -But it is INT1 on the Leonardo
3      |  1            |  All -But it is INT0 on the Leonardo
18     |  5            |  Arduino Mega Only
19     |  4            |  Arduino Mega Only
20     |  3            |  Arduino Mega Only
21     |  2            |  Arduino Mega Only
0      |  0            |  Leonardo
1      |  3            |  Leonardo
7      |  4            |  Leonardo
The Arduino Due has no standard interrupt pins as an iterrupt can be attached to almosty any pin. 

In the program pin 2 is chosen
*/

#define DIMMING_STEPS_NUMBER 100
#define DIM_MIN_LEVEL 10
#define DIM_MAX_LEVEL 90
#define TRIAC_ON_PULSE_WIDTH_US 10
#define AC_HALF_PERIOD_US 10000UL

int AC_LOAD = 5;    // Output to Opto Triac pin
int dimming = DIM_MAX_LEVEL;  // Dimming level (0-100)  0 = ON, 100 = OFF

#define DIMMING_STEP_WIDTH_US ((AC_HALF_PERIOD_US - TRIAC_ON_PULSE_WIDTH_US)/DIMMING_STEPS_NUMBER)



//the interrupt function must take no parameters and return nothing
void zero_crosss_int()  //function to be fired at the zero crossing to dim the light
{
  int dimtime = (DIMMING_STEP_WIDTH_US*dimming);    // For 60Hz =>65    
  delayMicroseconds(dimtime);    // Wait till firing the TRIAC
  digitalWrite(AC_LOAD, HIGH);   // Fire the TRIAC
  delayMicroseconds(TRIAC_ON_PULSE_WIDTH_US);         // triac On propogation delay (for 60Hz use 8.33)
  digitalWrite(AC_LOAD, LOW);    // No longer trigger the TRIAC (the next zero crossing will swith it off) TRIAC
  
}

bool rising = false;

#define DELAY 20

bool blinking = true;

#define DIMMING_START_LEVEL 10

void setup()
{
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output
  attachInterrupt(0, zero_crosss_int, RISING);  // Choose the zero cross interrupt # from the table above
  dimming = DIMMING_START_LEVEL;
  Serial.begin(9600);
}

void loop()
{
  if (blinking) 
  {
    if (rising)
    {
      if (dimming < DIM_MAX_LEVEL)
      {
        dimming += 10;
        Serial.println(dimming);
        delay(DELAY);
      }
      else
      {
        rising = false;
      }
    }
    else
    {
      if (dimming > DIM_MIN_LEVEL)
      {
        dimming -= 10;
        Serial.println(dimming);
        delay(DELAY);
      }
      else
      {
        rising = true;
      }
    }
  }
}
