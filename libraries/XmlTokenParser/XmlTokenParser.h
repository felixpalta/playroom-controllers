#ifndef __XMLTOKENPARSER_H__
#define __XMLTOKENPARSER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "xml_token_max_length.h"

class XmlTokenParser
{
public:

	XmlTokenParser(Stream& stream) : s(stream) {}

	bool find_string_after_whitespace(const char* str);

	bool find_attribute(const char *attr_name);

	bool read_string_until_terminator(char terminator);

	// dst buffer should be at least XML_TOKEN_MAX_LENGTH bytes
	// dst will be null-terminated after the copy;
	void null_terminated_copy_of_buf(TokenBuffer* dst);

	// <
	bool expect_left_simple_bracket();
	// >
	bool expect_right_simple_bracket();

	// </ 
	bool expect_left_closing_bracket();
	// />
	bool expect_right_closing_bracket();

	bool expect_opening_tag(const char *tag_name);
	bool expect_closing_tag(const char *tag_name);

	void skip_whitespace();

	const char* get_internal_buf();

	bool available() { return s.available(); }

private:

	Stream& s;
	TokenBuffer buf;
};

#endif // __XMLTOKENPARSER_H__
