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

	bool find_string_after_whitespace(Stream& s, const char* str);

	bool find_attribute(Stream& s, const char *attr_name);

	bool read_string_until_terminator(Stream& s, char terminator);

	// dst buffer should be at least XML_TOKEN_MAX_LENGTH bytes
	// dst will be null-terminated after the copy;
	void null_terminated_copy_of_buf(TokenBuffer* dst);

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

	void skip_whitespace(Stream& s);

	const char* get_internal_buf();

private:
	TokenBuffer buf;
};

#endif // __XMLTOKENPARSER_H__
