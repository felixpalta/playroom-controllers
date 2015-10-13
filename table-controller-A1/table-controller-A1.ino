#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include <MsTimer2.h>
#include <playroom-server-address.h>
#include "OutWriter.h"
#include "table-controller-pin-config.h"
#include "table-controller-server-config.h"
#include "sectors.h"
#include "sector_test_mode.h"

EthernetClient client;

void setup()
{
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);
  
  Serial.begin(9600);
  Serial.println("Hello from Table Controller");

#ifndef SECTOR_TEST_MODE

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);//Deselect the SD card
  
  Serial.println("Attempting to lease IP via DHCP...");
  
  // start the Ethernet connection and the server:
  if (Ethernet.begin(table_controller_mac))
  {
    Serial.println("IP received via DHCP");
    Serial.println("Table controller is at ");
    Serial.println(Ethernet.localIP());
  }
  else
  {
    Serial.println("DHCP failed, trying to set IP manually...");
    Ethernet.begin(table_controller_mac, TABLE_CONTROLLER_IP, DNS_IP, GATEWAY_IP, SUBNET_MASK);
    Serial.print("Static IP: "); Serial.println(TABLE_CONTROLLER_IP);
    Serial.print("Subnet mask: "); Serial.println(SUBNET_MASK);
    Serial.print("Gateway: "); Serial.println(GATEWAY_IP);
    Serial.print("DNS: "); Serial.println(DNS_IP);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  Serial.print("Listening port: "); Serial.println(TABLE_CONTROLLER_LISTENING_PORT);
  Serial.print("Outgoing port: "); Serial.println(PLAYROOM_SERVER_LISTENING_PORT);
  
#endif

  sectors_init();
  
  Serial.println("Starting MsTimer2...");
  MsTimer2::set(10, sectors_process_sensors);
  MsTimer2::start();
}

void loop()
{
  SectorEventData sector_event_data;
  if (is_sector_event_ready(&sector_event_data))
  {
    reset_sector_event();
    SectorEvent event_type = sector_event_data.event;
    switch (event_type)
    {
      case SECTOR_EVENT_STARTED:
        sectors_rotation_started_callback();
        break;
      case SECTOR_EVENT_STOPPED:
        sectors_rotation_stopped_callback(sector_event_data.sector_number);
        break;
      default:
        Serial.print("Unexpected event type: "); Serial.println(event_type);
        break;
    }
  }
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

static bool get_connect_msg(int code, const char **msg)
{
  static const int SUCCESS = 1;
  static const int TIMED_OUT = -1;
  static const int INVALID_SERVER = -2;
  static const int TRUNCATED = -3;
  static const int INVALID_RESPONSE = -4;

  switch (code)
  {
  case SUCCESS:
    *msg = "Connection OK";
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
  int code;
  const char *err_msg = NULL;
  
#ifdef DNS_SERVER_NAME_PRESENT
  Serial.print("Connecting to server by DNS name: ");
  Serial.println(PLAYROOM_SERVER_DNS_ADDRESS);
  code = client.connect(PLAYROOM_SERVER_DNS_ADDRESS, PLAYROOM_SERVER_LISTENING_PORT);
  Serial.print("Code: ");
  Serial.println(code);
#endif

  if (!get_connect_msg(code, &err_msg))
  {
    Serial.println("Connection to server by DNS name failed");
    if (err_msg)
      Serial.println(err_msg);
    Serial.print("Connecting to server by IP address: ");
    Serial.println(PLAYROOM_SERVER_IP_ADDRESS);
    code = client.connect(PLAYROOM_SERVER_IP_ADDRESS, PLAYROOM_SERVER_LISTENING_PORT);
    Serial.print("Code: ");
    Serial.println(code);
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

#define BARREL_PLAY_RQ 1
#define BARREL_SECTOR_RQ 2

static void send_request(int rq_type, int n)
{
  if (!connect_to_server())
  {
    return;
  }
  Serial.println("Connected to server");
  if (!client.connected())
  {
    Serial.println("Error: server instantly closed the connection");
    client.stop();
    return;
  }

  OutWriter out_writer(client);

  switch (rq_type)
  {
    case BARREL_PLAY_RQ:
      out_writer.send_barrel_play_request();
      break;
    case BARREL_SECTOR_RQ:
      out_writer.send_barrel_sector_request(n);
      break;
    default:
      Serial.print("Uknown request type: ");
      Serial.println(rq_type);
      return;
  }

  if (!client.connected())
  {
    Serial.println("Error: server closed connection right after receiving from client");
    client.stop();
    return;
  }
  Serial.println("Raw response from server: ");
  
  static const unsigned long READ_TIMEOUT = 1000;
  
  static unsigned long last_read_time;
  
  last_read_time = millis();
  
  while (true)
  {
    if (client.available())
    {
      last_read_time = millis();
      char c = client.read();
      Serial.print(c);
    }
    if (millis() - last_read_time >= READ_TIMEOUT)
    {
      Serial.println("\nRead timeout!");
      break;
    }
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

void sectors_rotation_started_callback()
{
  Serial.println("ROTATION STARTED CALLBACK");

  send_request(BARREL_PLAY_RQ, /* not used */ 0);
  blink(2);
}
void sectors_rotation_stopped_callback(int sector_n)
{
  Serial.print("ROTATION STOPPED CALLBACK: ");
  Serial.println(sector_n);
  
  send_request(BARREL_SECTOR_RQ, sector_n);
  blink(sector_n);
}
