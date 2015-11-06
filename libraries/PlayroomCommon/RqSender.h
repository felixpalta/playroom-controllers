// RqSender.h

#ifndef _RQSENDER_h
#define _RQSENDER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Client.h>
#include <Print.h>
#include "OutWriter.h"
#include "out_request_names.h"

class RqSender
{
public:

  RqSender(Client& cl, Print& logger, const char *protover, const char *serial) : 
    client(cl),
    ok_printer(logger),
    out_writer(cl, protover, serial),
    last_read_time(0)
  {
  }

  void send_request(OutRqType rq_type, int n);

private:
  bool connect_to_server();
  static bool get_connect_msg(int code, const char **msg);
  Client& client;
  Print& ok_printer;
  OutWriter out_writer;

  unsigned long last_read_time;

  static const unsigned long READ_TIMEOUT = 1000;
};

#endif

