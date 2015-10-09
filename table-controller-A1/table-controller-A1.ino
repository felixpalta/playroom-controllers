#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include <playroom-server-address.h>
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

bool get_connect_msg(int code, const char **msg)
{
  static const int SUCCESS = 1;
  static const int TIMED_OUT = -1;
  static const int INVALID_SERVER = -2;
  static const int TRUNCATED = -3;
  static const int INVALID_RESPONSE = -4;

  switch (code)
  {
  case SUCCESS:
    return true;
  case TIMED_OUT:
    *msg = "Connection timed out";
    return false;
  case INVALID_SERVER:
    *msg = "Invalid server";
    return false;
  case TRUNCATED:
    *msg = "Truncated";
    return false;
  case INVALID_RESPONSE:
    *msg = "Invalid response";
    return false;
  default:
    *msg = "Unknown code";
    return false;
  }
}

static bool connect_to_server()
{
  Serial.print("Connecting to server by DNS name: ");
  Serial.println(PLAYROOM_SERVER_DNS_ADDRESS);
  int code = client.connect(PLAYROOM_SERVER_DNS_ADDRESS, table_controller_port);
  const char *err_msg = NULL;
  if (!get_connect_msg(code, &err_msg))
  {
    Serial.println("Connection to server by DNS name failed");
    if (err_msg)
      Serial.println(err_msg);
    Serial.print("Connecting to server by IP address: ");
    Serial.println(PLAYROOM_SERVER_IP_ADDRESS);
    code = client.connect(PLAYROOM_SERVER_IP_ADDRESS, table_controller_port);
    err_msg = NULL;
    if (!get_connect_msg(code, &err_msg))
    {
      Serial.println("Connection to server by IP address failed");
      if (err_msg)
        Serial.println(err_msg);
      return false;
    }
  }
  
  Serial.println("Connection to server successfull");

  return true;
}

void sectors_rotation_started_callback()
{
  Serial.println("ROTATION STARTED CALLBACK");

  if (!connect_to_server())
  {
    return;
  }
  if (!client.connected())
  {
    Serial.println("Error: server instantly closed the connection");
    client.stop();
    return;
  }

  OutWriter out_writer(client);

  out_writer.send_barrel_play_request();

  if (!client.connected())
  {
    Serial.println("Error: server closed connection right after receiving from client");
    client.stop();
    return;
  }
  Serial.println("Response from server: ");
  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
  if (!client.connected())
  {
    Serial.println("OK: Responce received, server closed the connection");
  }
  else
  {
    Serial.println("FAIL: Responce received, but server didn't close the connection");
  }
  Serial.println("Stopping the client...");
  client.stop();
  return;

}
void sectors_rotation_stopped_callback(int sector_n)
{
  Serial.print("ROTATION STOPPED CALLBACK: ");
  Serial.println(sector_n);
  blink(sector_n);
}
