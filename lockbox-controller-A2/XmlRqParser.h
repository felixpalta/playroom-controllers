// XmlRqParser.h

#ifndef _XMLRQPARSER_h
#define _XMLRQPARSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "lockbox-controller-rq-valid-protocol-values.h"
#include "XmlTokenParser.h"

typedef struct
{
	bool type_attr_found;
	char type_attr_buf[MAX_RQ_TYPE_ATTR_VALUE_LENGTH];

	bool proto_attr_found;
	char proto_attr_buf[MAX_PROTO_ATTR_VALUE_LENGTH];

	bool serial_attr_found;
	char serial_attr_buf[MAX_SERIAL_ATTR_VALUE_LENGTH];

	bool lockbox_attr_found;
	char lockbox_attr_buf[MAX_LOCKBOX_ATTR_VALUE_LENGTH];
} XmlRqParsingOutput;

class XmlRqParser
{
public:
	typedef enum
	{
		ERROR_NONE,
		ERROR_OUTPUT_NULLPTR,
		ERROR_CLOSE_TAG,
		ERROR_RQ_TAG_NAME,
		ERROR_RQ_CLOSING_TAG_NAME,
		ERROR_TYPE_ATTR_NAME,
		ERROR_TYPE_ATTR_VALUE,
		ERROR_PROTOVER_ATTR_NAME,
		ERROR_PROTOVER_ATTR_VALUE,
		ERROR_SERIAL_ATTR_NAME,
		ERROR_SERIAL_ATTR_VALUE,
		ERROR_LOCKBOX_ATTR_VALUE,
		ERROR_DATA_TAG_NAME,
	} ErrorType;

	XmlRqParser();

	ErrorType process_stream(Stream& s, XmlRqParsingOutput* out);

private:
	XmlTokenParser token_parser;
	
};

#endif

