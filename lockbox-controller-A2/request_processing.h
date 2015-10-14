// request_processing.h

#ifndef _REQUEST_PROCESSING_h
#define _REQUEST_PROCESSING_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "InputReader.h"

typedef enum
{
  OPEN,
  CLOSE,
  INVALID,
} ActionType;

void print_request(const InputRqParsingOutput& data);
bool verify_and_process_request(InputRqParsingOutput& data);
const char *get_request_error();

#endif

