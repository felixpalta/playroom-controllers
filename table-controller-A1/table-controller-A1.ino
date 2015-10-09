#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include "OutWriter.h"
#include "table-controller-server-config.h"
#include "sectors.h"

#define DBG_PIN 13

EthernetClient client;

#define W5200_CS  10
#define SDCARD_CS 4

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello from Table Controller");

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);//Deselect the SD card

  // start the Ethernet connection and the server:
  if (!Ethernet.begin(table_controller_mac))
  {
    Serial.println("DHCP failed, trying to set IP manually...");
    Ethernet.begin(table_controller_mac, table_controller_ip);
  }

  Serial.print("Table controller is at ");
  Serial.print(Ethernet.localIP());
  Serial.println(String(" : ") + table_controller_port);

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
