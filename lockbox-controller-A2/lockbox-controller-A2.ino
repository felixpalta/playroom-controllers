#include "curtain.h"
#include "door_lock.h"
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
#include "PlayroomStateMachine.h"

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(LOCKBOX_CONTROLLER_LISTENING_PORT);

EthernetClient client;
RqSender rq_sender(client, Serial, PROTOVER_ATTR_VALUE, SERIAL_ATTR_VALUE);

LockBoxes lock_boxes;

PlayroomStateMachine state_machine(PlayroomStateMachine::STATE_START);

void setup() 
{
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.println("Hello from Lockbox Controller");
  
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH);//Deselect the SD card
  
  lock_boxes.begin();
  dimmers_init();
  buttons_init();
  door_lock_init();
  curtain_init();
  
  button_set_light_game_start(true);
  button_set_light_standby(true);

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
}

ButtonEvent button_event;

void loop()
{
  
  process_serial_input();
  
  lock_boxes.process();

  curtain_process();

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
    button_set_light_game_start(true);
    break;
  case ButtonEvent::BUTTON_STATE_RELEASED:
    Serial.println("GAME START button RELEASED");
    button_set_light_game_start(false);
    break;
  default:
    break;
  }

  switch (button_event->standby_state)
  {
  case ButtonEvent::BUTTON_STATE_PUSHED:
    Serial.println("STANDBY button PUSHED");
    button_set_light_standby(true);
    break;
  case ButtonEvent::BUTTON_STATE_RELEASED:
    Serial.println("STANDBY button RELEASED");
    button_set_light_standby(false);
    break;
  default:
    break;
  }

  switch (button_event->cleaning_state)
  {
  case ButtonEvent::BUTTON_STATE_PUSHED:
    Serial.println("CLEANING button PUSHED");
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
    LOCK_CLOSE = '3',
    LOCK_OPEN = '4',
  };
  
  if (Serial.available())
  {
    char c = Serial.read();
    switch (c)
    {
    case CURTAIN_ON:
      curtain_signal_open();
      break;
    case CURTAIN_OFF:
      curtain_signal_close();
      break;
    case LOCK_CLOSE:
      door_lock_close();
      break;
    case LOCK_OPEN:
      door_lock_open();
      break;  
    default:
      break;
    }
  }
}
