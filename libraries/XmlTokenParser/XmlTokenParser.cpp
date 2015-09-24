#include "XmlTokenParser.h"


void XmlTokenParser::skip_whitespace(Stream& s)
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

bool XmlTokenParser::read_string_until_terminator(Stream& s, size_t max_len, char terminator)
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
		*(buf + index) = (char)c;
		index++;
	}
	buf[index] = '\0';
	return terminator_found;
}

bool XmlTokenParser::find_string_after_whitespace(Stream& s, const char* str)
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

bool XmlTokenParser::find_attribute(Stream& s, const char *attr_name, size_t max_attr_length)
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

void XmlTokenParser::copy_buf_to_attr_buf(char *attr_buf, size_t attr_buf_size)
{
	strncpy(attr_buf, buf, attr_buf_size);
	attr_buf[attr_buf_size - 1] = '\0';
}

// <
bool XmlTokenParser::expect_left_simple_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "<");
}
// >
bool XmlTokenParser::expect_right_simple_bracket(Stream& s)
{
	return find_string_after_whitespace(s, ">");
}

// </ 
bool XmlTokenParser::expect_left_closing_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "</");
}
// />
bool XmlTokenParser::expect_right_closing_bracket(Stream& s)
{
	return find_string_after_whitespace(s, "/>");
}

bool XmlTokenParser::expect_opening_tag(Stream& s, const char *tag_name)
{
	bool ok = expect_left_simple_bracket(s);
	if (!ok)
		return false;

	return find_string_after_whitespace(s, tag_name);
}
bool XmlTokenParser::expect_closing_tag(Stream& s, const char *tag_name)
{
	bool ok = expect_left_closing_bracket(s);
	if (!ok)
		return false;
	ok = find_string_after_whitespace(s, tag_name);
	if (!ok)
		return false;
	return expect_right_simple_bracket(s);
}
