// 
// 
// 

#include "XmlRqParser.h"
#include "lockbox-controller-rq-base-protocol.h"
#include "lockbox-controller-rq-valid-protocol-values.h"

XmlRqParser xml_parser;

XmlRqParser::XmlRqParser()
{
	reset();
}

void XmlRqParser::reset()
{
	*buf = '\0';

}


static void skip_whitespace(Stream& s)
{
	while (s.available())
	{
		char c = s.peek();
		if (isWhitespace(c))
			s.read();
		else
			return;
	}
}

bool XmlRqParser::read_string_until_terminator(Stream& s, size_t max_len, char terminator)
{
	if (max_len < 1)
		return false;
	if (max_len + 1 > sizeof(buf))
		return false;
	if (terminator < 0)
		return false;
	if (!s.available())
		return false;
	size_t index = 0;
	bool terminator_found = false;
	while (index < max_len)
	{
		int c = s.read();
		if (c < 0)
			break;
		if (c == terminator)
		{
			terminator_found = true;
			break;
		}
		*(buf + index) = (char) c;
		index++;
	}
	buf[index] = '\0';
	return terminator_found;
}

bool XmlRqParser::find_string_after_whitespace(Stream& s, const char* str)
{

	skip_whitespace(s);

	const size_t next_token_length = strlen(str);

	if (next_token_length + 1 > sizeof(buf))
		return false;

	size_t index = 0;

	while (index < next_token_length)
	{
		int c = s.read();
		if (c == -1)
			return false;
		if (c != str[index])
			return false;
		++index;
	}

	return true;
}

bool XmlRqParser::find_attribute(Stream& s, const char *attr_name, size_t max_attr_length)
{
	if (max_attr_length + 1 > sizeof(buf))
		return buf;

	bool ok = find_string_after_whitespace(s, attr_name);
	if (!ok)
		return false;

	ok = find_string_after_whitespace(s, "=");
	if (!ok)
		return false;

	ok = find_string_after_whitespace(s, "\"");
	if (!ok)
		return false;

	ok = read_string_until_terminator(s, max_attr_length, '"');
	
	return ok;
}

void XmlRqParser::copy_buf_to_attr_buf(char *attr_buf, size_t attr_buf_size)
{
	strncpy(attr_buf, buf, attr_buf_size);
	attr_buf[attr_buf_size - 1] = '\0';
}

// <
bool XmlRqParser::expect_left_simple_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "<");
}
// >
bool XmlRqParser::expect_right_simple_bracket(Stream& s)
{
	return find_string_after_whitespace(s, ">");
}

// </ 
bool XmlRqParser::expect_left_closing_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "</");
}
// />
bool XmlRqParser::expect_right_closing_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "/>");
}

bool XmlRqParser::expect_opening_tag(Stream& s, const char *tag_name)
{
	bool ok = expect_left_simple_bracket(s);
	if (!ok)
		return false;

	return find_string_after_whitespace(s, tag_name);
}
bool XmlRqParser::expect_closing_tag(Stream& s, const char *tag_name)
{
	bool ok = expect_left_closing_bracket(s);
	if (!ok)
		return false;
	ok = find_string_after_whitespace(s, tag_name);
	if (!ok)
		return false;
	return expect_right_simple_bracket(s);
}

XmlRqParser::ErrorType XmlRqParser::process_stream(Stream& s, XmlRqParsingOutput* out)
{
	if (!out)
		return ERROR_OUTPUT_NULLPTR;

	reset();

	bool ok = expect_opening_tag(s, RQ_TAG_NAME);

	if (!ok)
		return ERROR_RQ_TAG_NAME;

	ok = find_attribute(s, TYPE_ATTR_NAME, MAX_RQ_TYPE_ATTR_VALUE_LENGTH);
	if (!ok)
		return ERROR_TYPE_ATTR_VALUE;

	out->type_attr_found = true;
	copy_buf_to_attr_buf(out->type_attr_buf, sizeof(out->type_attr_buf));
	
	ok = find_attribute(s, PROTOVER_ATTR_NAME, MAX_PROTO_ATTR_VALUE_LENGTH);
	if (!ok)
		return ERROR_PROTOVER_ATTR_VALUE;

	out->proto_attr_found = true;
	copy_buf_to_attr_buf(out->proto_attr_buf, sizeof(out->proto_attr_buf));

	ok = find_attribute(s, SERIAL_ATTR_NAME, MAX_SERIAL_ATTR_VALUE_LENGTH);
	if (!ok)
		return ERROR_SERIAL_ATTR_VALUE;

	out->serial_attr_found = true;
	copy_buf_to_attr_buf(out->serial_attr_buf, sizeof(out->serial_attr_buf));

	ok = expect_right_simple_bracket(s);
	if (!ok)
		return ERROR_CLOSE_TAG;

	ok = expect_opening_tag(s, DATA_TAG_NAME);
	if (!ok)
		return ERROR_DATA_TAG_NAME;

	ok = find_attribute(s, LOCKBOX_ATTR_NAME, MAX_LOCKBOX_ATTR_VALUE_LENGTH);
	if (!ok)
		return ERROR_LOCKBOX_ATTR_VALUE;
	out->lockbox_attr_found = true;
	copy_buf_to_attr_buf(out->lockbox_attr_buf, sizeof(out->lockbox_attr_buf));

	ok = expect_right_closing_bracket(s);
	if (!ok)
		return ERROR_CLOSE_TAG;

	ok = expect_closing_tag(s, RQ_TAG_NAME);
	if (!ok)
		return ERROR_RQ_CLOSING_TAG_NAME;
	
	return ERROR_NONE;
}


