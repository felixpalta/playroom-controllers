#include <RqSender.h>
#include <SwitchablePrinter.h>
#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include <MsTimer2.h>
#include <playroom-server-address.h>
#include <OutWriter.h>
#include "table-controller-pin-config.h"
#include "table-controller-server-config.h"
#include "table-controller-valid-protocol-values.h"
#include "sectors.h"
#include "sector_test_mode.h"
#include <InputReader.h>
#include <request_names.h>
#include "request_processing.h"
#include "MySerial.h"

EthernetClient client;
RqSender rq_sender(client, MySerial, PROTOVER_ATTR_VALUE, SERIAL_ATTR_VALUE);
EthernetServer server(TABLE_CONTROLLER_LISTENING_PORT);

void setup()
{
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);
  
  Serial.begin(115200);
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
  
  server.begin();
#endif

  sectors_init();
  
  Serial.println("Starting MsTimer2...");
  MsTimer2::set(10, sectors_process_sensors);
  MsTimer2::start();
  
  MySerial.print_enable(true);
}

void loop()
{
  process_serial_input();
  process_incoming_connections();
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

void process_serial_input()
{
  enum
  {
    SECTOR_TEST_ON = '1',
    SECTOR_TEST_OFF = '2',
  };
  if (Serial.available())
  {
    char c = Serial.read();
    switch (c)
    {
    case SECTOR_TEST_ON:
      sector_test_mode_enable(true);
      break;
    case SECTOR_TEST_OFF:
      sector_test_mode_enable(false);
      break;
    default:
      break;
    }
  }
}

void process_incoming_connections()
{
  EthernetClient client = server.available();
  if (client)
  {
    XmlTokenParser xml_token_parser(/* input */ client, /* echo */ Serial);
    InputReader xml_parser(xml_token_parser, PROTOVER_ATTR_VALUE);
    OutWriter out_writer(client, PROTOVER_ATTR_VALUE, SERIAL_ATTR_VALUE);

    MySerial.println("\nnew client");
    if (client.connected())
    {
      while (client.available())
      {
        InputRqParsingOutput data;
        InputReader::ErrorType status = xml_parser.process_stream(&data);
        Serial.println();
        if (status != InputReader::ERROR_NONE)
        {
          Serial.print("XML Parser ERROR: ");
          const char *err_msg = xml_parser.get_error(status);
          Serial.println(err_msg);
          out_writer.send_err_repsonse(err_msg);
          delay(1);
          disconnect_client(client);
          break;
        }
        else
        {
          MySerial.println("XML Parser OK");
          out_writer.send_ack_response();
          MySerial.println("ACK Sent");

          delay(1);
          disconnect_client(client);

          print_request(data);
          bool ok = verify_and_process_request(data);
          if (ok)
          {
            MySerial.println("Request verified and executed OK");
            break;
          }
          else
          {
            const char *err = get_request_error();
            Serial.println(err);
            break;
          }
        }
      }
      MySerial.println("Incoming request processing done");
    }
    else
      MySerial.println("Incoming client instantly closed the connection");
  }
}

static void disconnect_client(Client& client)
{
  client.stop();
  MySerial.println("client disonnected");
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

  rq_sender.send_request(OUT_RQ_TYPE_BARREL_PLAY, /* not used */ 0);
  blink(2);
}
void sectors_rotation_stopped_callback(int sector_n)
{
  Serial.print("ROTATION STOPPED CALLBACK: ");
  Serial.println(sector_n);
  
  rq_sender.send_request(OUT_RQ_TYPE_BARREL_SECTOR, sector_n);
  blink(sector_n);
}
