#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include "OutWriter.h"
#include "table-controller-server-config.h"
#include "sectors.h"

#define DBG_PIN 13

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello from Table Controller");
  sectors_init();
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);

}

void loop()
{
  sectors_process_sensors();
}

static void blink(int n)
{
  for (int i = 0; i < n; ++i)
  {
    digitalWrite(DBG_PIN, HIGH);
    delay(50);
    digitalWrite(DBG_PIN, LOW);
    delay(50);
  }
}

void sectors_rotation_started_callback()
{
  Serial.println("ROTATION STARTED CALLBACK");
  blink(2);
}
void sectors_rotation_stopped_callback(int sector_n)
{
  Serial.print("ROTATION STOPPED CALLBACK: ");
  Serial.println(sector_n);
  blink(sector_n);
}
