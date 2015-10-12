#include <playroom-protocol.h>
#include <XmlTokenWriter.h>
#include <XmlTokenParser.h>
#include "request_processing.h"
#include "XmlRespWriter.h"
#include "XmlRqParser.h"
#include <SPI.h>
#include <EthernetV2_0.h>
#include "lockbox-controller-server-config.h"
#include "lockbox-controller-pin-config.h"
#include "LockBoxes.h"

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(lockbox_controller_port);

LockBoxes lock_boxes;

void setup() {
  pinMode(DBG_PIN, OUTPUT);
  digitalWrite(DBG_PIN, LOW);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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
    Ethernet.begin(lockbox_controller_mac, lockbox_controller_ip, DNS_IP, GATEWAY_IP, SUBNET_MASK);
    Serial.print("Static IP: "); Serial.println(lockbox_controller_ip);
    Serial.print("Subnet mask: "); Serial.println(SUBNET_MASK);
    Serial.print("Gateway: "); Serial.println(GATEWAY_IP);
    Serial.print("DNS: "); Serial.println(DNS_IP);
  }
  
  Serial.print("Server port: "); Serial.println(lockbox_controller_port);
  
  server.begin();
  
  lock_boxes.begin();
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    XmlRqParser xml_parser(client);
    XmlRespWriter xml_resp_writer(client);

    Serial.println("\nnew client");
    if (client.connected())
    {
      while (client.available()) 
      {
        XmlRqParsingOutput data;
        XmlRqParser::ErrorType status = xml_parser.process_stream(&data);
        if (status != XmlRqParser::ERROR_NONE)
        {
          Serial.print("XML Parser ERROR:");
          Serial.println(status);
          xml_resp_writer.send_err_repsonse("Unable to parse XML request");
          break;
        }
        else
        {
          Serial.println("XML Parser OK");
          print_request(data);
          bool ok = verify_and_process_request(data);
          if (ok)
          {
            Serial.println("Request verified and executed OK");
            xml_resp_writer.send_ack_response();
            break;
          }
          else
          {
            const char *err = get_request_error();
            Serial.println(err);
            xml_resp_writer.send_err_repsonse(err);
            break;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
    
  }
}

