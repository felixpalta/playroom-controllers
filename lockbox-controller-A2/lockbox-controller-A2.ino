
#include <XmlTokenParser.h>
#include "request_processing.h"
#include "XmlRespWriter.h"
#include "XmlRqParser.h"
#include <SPI.h>
#include <EthernetV2_0.h>
#include "lockbox-controller-server-config.h"
#include "LockBoxes.h"

// Initialize the Ethernet server library
// with the IP address and port you want to use 
EthernetServer server(lockbox_controller_port);

#define W5200_CS  10
#define SDCARD_CS 4

LockBoxes lock_boxes;

XmlRqParser xml_parser;

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	pinMode(SDCARD_CS, OUTPUT);
	digitalWrite(SDCARD_CS, HIGH);//Deselect the SD card

	// start the Ethernet connection and the server:
	Ethernet.begin(lockbox_controller_mac, lockbox_controller_ip);

	server.begin();
	Serial.print("Lockbox controller is at ");
	Serial.print(Ethernet.localIP());
	Serial.println(String(" : ") + lockbox_controller_port);

	lock_boxes.begin();
}

void loop() {
	// listen for incoming clients
	EthernetClient client = server.available();
	if (client) {
		Serial.println("\nnew client");
		if (client.connected()){
			while (client.available()) {
				XmlRqParsingOutput data;
				XmlRqParser::ErrorType status = xml_parser.process_stream(client, &data);
				if (status != XmlRqParser::ERROR_NONE)
				{
					Serial.print("XML Parser ERROR:");
					Serial.println(status);
					xml_resp_writer.send_err_repsonse(client, "Unable to parse XML request");
					break;
				}
				else
				{
					Serial.println("XML Parser OK");
					print_request(data);
					ActionType action = INVALID;
					int lock_box_number = -1;
					bool ok = verify_request(data, action, lock_box_number);
					if (ok)
					{
						bool result = process_request(action, lock_box_number);
						if (result)
						{
							Serial.println("Request executed OK");
							xml_resp_writer.send_ack_response(client);
							break;
						}
						else
						{
							const char *err = lock_boxes.get_last_error();
							Serial.println(err);
							xml_resp_writer.send_err_repsonse(client, err);
							break;
						}
					}
					else
					{
						const char *err = get_request_error();
						Serial.println(err);
						xml_resp_writer.send_err_repsonse(client, err);
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
