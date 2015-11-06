// OutWriter.h

#ifndef _OUTWRITER_h
#define _OUTWRITER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <XmlTokenWriter.h>

class OutWriter
{
public:
  OutWriter(Print& s, const char *protover, const char *serial) : 
    xml_writer(s),
    valid_protover(protover),
    valid_serial(serial)
  {}

  void send_ack_response();

  void send_err_repsonse(const char* err_msg);

  void send_barrel_play_request();

  void send_barrel_sector_request(int n);

  void send_game_start_request();
  void send_cleaning_request();
  void send_standby_request();

private:
  void write_response_open_tag(const char* type_attr_value);
  void write_err_tag(const char* err_msg);
  void write_response_close_tag();

  void write_request_open_tag(const char* type_attr_value);
  void write_request_close_tag();
  void finalize_message();

  XmlTokenWriter xml_writer;
  const char *valid_protover;
  const char *valid_serial;
};

#endif

