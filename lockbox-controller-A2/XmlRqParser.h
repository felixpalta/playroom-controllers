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
#include "xml_token_max_length.h"

typedef struct
{
	bool type_attr_found;
	TokenBuffer type_attr_buf;

	bool proto_attr_found;
	TokenBuffer proto_attr_buf;

	bool serial_attr_found;
	TokenBuffer serial_attr_buf;

	bool lockbox_attr_found;
	TokenBuffer lockbox_attr_buf;

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

