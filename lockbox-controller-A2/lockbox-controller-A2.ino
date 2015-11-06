#include "buttons.h"
#include <MsTimer2.h>
#include <SwitchablePrinter.h>
#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include "request_processing.h"
#include "XmlRespWriter.h"
#include <InputReader.h>
#include <request_names.h>
#include <SPI.h>
#include <EthernetV2_0.h>
#include <playroom-server-address.h>
#include "lockbox-controller-server-config.h"
#include "lockbox-controller-pin-config.h"
#include "lockbox-controller-rq-valid-protocol-values.h"
#include "LockBoxes.h"
#include "dimmers.h"

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(LOCKBOX_CONTROLLER_LISTENING_PORT);

LockBoxes lock_boxes;

void setup() {
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("Hello from Lockbox Controller");
  
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);//Deselect the SD card

  
  Serial.println("Attempting to lease IP via DHCP...");
  
  // start the Ethernet connection and the server:
  if (Ethernet.begin(lockbox_controller_mac))
  {
    Serial.println("IP received via DHCP");
    Serial.println("Table controller is at ");
    Serial.println(Ethernet.localIP());
  }
  else
  {
    Serial.println("DHCP failed, trying to set IP manually...");
    Ethernet.begin(lockbox_controller_mac, LOCKBOX_CONTROLLER_IP, DNS_IP, GATEWAY_IP, SUBNET_MASK);
    Serial.print("Static IP: "); Serial.println(LOCKBOX_CONTROLLER_IP);
    Serial.print("Subnet mask: "); Serial.println(SUBNET_MASK);
    Serial.print("Gateway: "); Serial.println(GATEWAY_IP);
    Serial.print("DNS: "); Serial.println(DNS_IP);
  }
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  Serial.print("Listening port: "); Serial.println(LOCKBOX_CONTROLLER_LISTENING_PORT);
  Serial.print("Outgoing port: "); Serial.println(PLAYROOM_SERVER_LISTENING_PORT);
  
  server.begin();
  
  lock_boxes.begin();
  dimmers_init();
  buttons_init();

#if 0
  dimmers_light_set(DIMMER_TOP_LIGHT, 10);
  dimmers_light_set(DIMMER_SURROUND_LIGHT, 60);
  dimmers_light_set(DIMMER_LOCKBOX_LIGHT, 80);
#endif
}

void loop()
{
  lock_boxes.process();


  ButtonEvent button_event;
  if (buttons_process(&button_event))
  {
    process_button_event(&button_event);
  }

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    XmlTokenParser xml_token_parser(/* input */ client, /* echo */ Serial);
    InputReader xml_parser(xml_token_parser, PROTOVER_ATTR_VALUE);
    XmlRespWriter xml_resp_writer(client);

    Serial.println("\nnew client");
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
          xml_resp_writer.send_err_repsonse(err_msg);
          delay(1);
          disconnect_client(client);
          break;
        }
        else
        {
          Serial.println("XML Parser OK");
          xml_resp_writer.send_ack_response();
          Serial.println("ACK Sent");
          
          delay(1);
          disconnect_client(client);
          
          print_request(data);
          bool ok = verify_and_process_request(data);
          if (ok)
          {
            Serial.println("Request verified and executed OK");
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
      Serial.println("Incoming request processing done");
    }
    else
      Serial.println("Incoming client instantly closed the connection");
  }
}

static void disconnect_client(Client& client)
{
  client.stop();
  Serial.println("client disonnected");
}

static void process_button_event(const ButtonEvent *button_event)
{
  if (!button_event)
    return;
}