#ifndef __XMLTOKENWRITER_H__
#define __XMLTOKENWRITER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class XmlTokenWriter
{
private:

	Stream& s;
	// <
	void write_left_simple_bracket();
	// </
	void write_left_closing_bracket();

	typedef enum
	{
		ERROR_NONE,
		ERROR_ZERO_STRING,
		ERROR_NULL_PTR,
	} Error;

	Error error;

	bool check_string_set_error(const char* str);

public:
	
	XmlTokenWriter(Stream& stream) : s(stream) {}

	const char* get_error();

	// <tagname + space
	void write_tag_opening(const char* str);
	// > + newline
	void write_right_simple_bracket();
	
	// /> + newline
	void write_right_closing_bracket();
	
	// </tagname>
	void write_tag_closing(const char *str);

	// attr_name="attr_value" + space
	void write_attribute_text(const char* attr_name, const char *attr_value);
	void write_attribute_num(const char* attr_name, int n);
};

#endif //__XMLTOKENWRITER_H__