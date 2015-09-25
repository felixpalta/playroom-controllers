#include "XmlTokenParser.h"

const char* XmlTokenParser::get_internal_buf()
{
	buf[sizeof(buf) - 1] = '\0';
	return buf;
}

void XmlTokenParser::skip_whitespace(Stream& s)
{
	while (s.available())
	{
		char c = s.peek();
		if (isSpace(c))
			s.read();
		else
			return;
	}
}

bool XmlTokenParser::read_string_until_terminator(Stream& s, char terminator)
{
	if (terminator < 0)
		return false;
	if (!s.available())
		return false;
	size_t index = 0;
	bool terminator_found = false;
	while (index < sizeof(buf))
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
	if (index >= sizeof(buf))
		buf[sizeof(buf) - 1] = '\0';
	else
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

bool XmlTokenParser::find_attribute(Stream& s, const char *attr_name)
{

	bool ok = find_string_after_whitespace(s, attr_name);
	if (!ok)
		return false;

	ok = find_string_after_whitespace(s, "=");
	if (!ok)
		return false;

	ok = find_string_after_whitespace(s, "\"");
	if (!ok)
		return false;

	ok = read_string_until_terminator(s, '"');

	return ok;
}

void XmlTokenParser::null_terminated_copy_of_buf(TokenBuffer* dst)
{
	strncpy(*dst, buf, sizeof(*dst) - 1);
	(*dst)[sizeof(*dst) - 1] = '\0';
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
