#include <RqSender.h>
#include "buttons.h"
#include <FlexiTimer2.h>
#include <SwitchablePrinter.h>
#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include "request_processing.h"
#include <OutWriter.h>
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

EthernetClient client;
RqSender rq_sender(client, Serial, PROTOVER_ATTR_VALUE, SERIAL_ATTR_VALUE);

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

ButtonEvent button_event;

void loop()
{
  
  process_serial_input();
  
  lock_boxes.process();

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
    OutWriter xml_resp_writer(client, PROTOVER_ATTR_VALUE, SERIAL_ATTR_VALUE);

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

  switch (button_event->game_start_state)
  {
  case ButtonEvent::BUTTON_STATE_PUSHED:
    Serial.println("GAME START button PUSHED");
    digitalWrite(GAME_START_BUTTON_LIGHT_PIN, HIGH);
//    rq_sender.send_request(OUT_RQ_TYPE_GAME_START, /*unused*/ 0);
    break;
  case ButtonEvent::BUTTON_STATE_RELEASED:
    Serial.println("GAME START button RELEASED");
    digitalWrite(GAME_START_BUTTON_LIGHT_PIN, LOW);
    break;
  default:
    break;
  }

  switch (button_event->standby_state)
  {
  case ButtonEvent::BUTTON_STATE_PUSHED:
    Serial.println("STANDBY button PUSHED");
//    rq_sender.send_request(OUT_RQ_TYPE_STANDBY, /*unused*/ 0);
    digitalWrite(STANDBY_BUTTON_LIGHT_PIN, HIGH);
    break;
  case ButtonEvent::BUTTON_STATE_RELEASED:
    Serial.println("STANDBY button RELEASED");
    digitalWrite(STANDBY_BUTTON_LIGHT_PIN, LOW);
    break;
  default:
    break;
  }

  switch (button_event->cleaning_state)
  {
  case ButtonEvent::BUTTON_STATE_PUSHED:
    Serial.println("CLEANING button PUSHED");
//    rq_sender.send_request(OUT_RQ_TYPE_CLEANING, /*unused*/ 0);
    break;
  case ButtonEvent::BUTTON_STATE_RELEASED:
    Serial.println("CLEANING button RELEASED");
    break;
  default:
    break;
  }
}

void process_serial_input()
{
  enum
  {
    CURTAIN_ON = '1',
    CURTAIN_OFF = '2',
    LOCK_ON = '3',
    LOCK_OFF = '4',
  };
  
  static bool curtain_state_pulse = false;
  
  if (Serial.available())
  {
    char c = Serial.read();
    switch (c)
    {
    case CURTAIN_ON:
      pinMode(CURTAIN_OPEN_PIN, OUTPUT);
      digitalWrite(CURTAIN_OPEN_PIN, HIGH);
      delay(100);
      digitalWrite(CURTAIN_OPEN_PIN, LOW);
      break;
    case CURTAIN_OFF:
      pinMode(CURTAIN_OPEN_PIN, OUTPUT);
      digitalWrite(CURTAIN_CLOSE_PIN, HIGH);
      delay(100);
      digitalWrite(CURTAIN_CLOSE_PIN, LOW);
      break;
    case LOCK_ON:
      break;
    case LOCK_OFF:
      break;  
    default:
      break;
    }
  }
}
