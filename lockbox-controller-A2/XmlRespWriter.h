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
	 void send_ack_response(Stream& s);
	 void send_err_repsonse(Stream& s, const char* err_msg);

private:
	void write_open_tag(Stream& s, const char* type_attr_value);
	void write_err_tag(Stream& s, const char* err_msg);
	void write_close_tag(Stream& s);

	XmlTokenWriter xml_token_writer;
};

extern XmlRespWriter xml_resp_writer;

#endif

