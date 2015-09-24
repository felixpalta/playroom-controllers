// request_processing.h

#ifndef _REQUEST_PROCESSING_h
#define _REQUEST_PROCESSING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "XmlRqParser.h"

typedef enum
{
	OPEN,
	CLOSE,
	INVALID,
} ActionType;

void print_request(const XmlRqParsingOutput& data);
bool process_request(ActionType action, int lock_box_number);
bool verify_request(XmlRqParsingOutput& data, ActionType& type_attribut, int& lock_box_number);
const char *get_request_error();

#endif

