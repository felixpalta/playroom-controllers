#ifndef __XMLTOKENPARSER_H__
#define __XMLTOKENPARSER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class XmlTokenParser
{
public:
	static const size_t XML_TOKEN_MAX_BUF_LENGTH = 128;

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

	void skip_whitespace(Stream& s);
private:
	char buf[XML_TOKEN_MAX_BUF_LENGTH];
};

#endif // __XMLTOKENPARSER_H__
