// XmlRespWriter.h

#ifndef _XMLRESPWRITER_h
#define _XMLRESPWRITER_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "XmlTokenWriter.h"

class XmlRespWriter
{
 public:

   XmlRespWriter(Stream& s) : xml_token_writer(s) {}

   void send_ack_response();
   void send_err_repsonse(const char* err_msg);

   void send_game_start_request();

private:
  void write_response_open_tag(const char* type_attr_value);
  void write_err_tag(const char* err_msg);
  void write_response_close_tag();

  void write_request_open_tag(const char* type_attr_value);
  void write_request_close_tag();

  XmlTokenWriter xml_token_writer;
};

#endif

