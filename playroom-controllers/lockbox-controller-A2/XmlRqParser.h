// XmlRqParser.h

#ifndef _XMLRQPARSER_h
#define _XMLRQPARSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "lockbox-controller-rq-valid-protocol-values.h"

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
		ERROR_OPEN_TAG,
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
		ERROR_INTERNAL_BUFFER_FULL,
		ERROR_TYPE_ATTR_TOO_LONG,
		ERROR_PROTOVER_ATTR_TOO_LONG,
		ERROR_SERIAL_ATTR_TOO_LONG,
		ERROR_DATA_ATTR_NAME_TOO_LONG,
		ERROR_DATA_ATTR_VALUE_TOO_LONG,
		ERROR_EQUALS_SIGN_EXPECTED,
		ERROR_OPENING_QUOTES_EXPECTED,
		ERROR_CLOSING_QUOTES_EXPECTED,
		ERROR_IMPOSSIBLE,
	} ErrorType;

	XmlRqParser();

	void reset();

	ErrorType process_stream(Stream& s, XmlRqParsingOutput* out);

private:


	static const size_t MAX_TOKEN_BUF_LENGTH = 128;

	char buf[MAX_TOKEN_BUF_LENGTH];

	bool find_string_after_whitespace(Stream& s, const char* str);
	
	bool find_attribute(Stream& s, const char *attr_name, size_t max_attr_length);

	bool read_string_until_terminator(Stream& s, size_t max_len, char terminator);

	void copy_buf_to_attr_buf(char *attr_buf, size_t attr_buf_size);

	// <
	bool expect_left_simple_bracket(Stream& s);
	// >
	bool expect_right_simple_bracket(Stream& s);

	// </ 
	bool expect_left_closing_bracket(Stream& s);
	// />
	bool expect_right_closing_bracket(Stream& s);

	bool expect_opening_tag(Stream& s, const char *tag_name);
	bool expect_closing_tag(Stream& s, const char *tag_name);

	bool append_to_buf(char c);
	
};

// Poor-man singleton.
extern XmlRqParser xml_parser;

#endif

