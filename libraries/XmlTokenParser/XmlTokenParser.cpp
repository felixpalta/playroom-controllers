#include "XmlTokenParser.h"

const char* XmlTokenParser::get_internal_buf()
{
	buf[sizeof(buf) - 1] = '\0';
	return buf;
}

void XmlTokenParser::skip_whitespace()
{
	while (s.available())
	{
		char c = s.peek();

    if (isSpace(c))
      Serial.print((char) s.read());
		else
			return;
	}
}

bool XmlTokenParser::read_string_until_terminator(char terminator)
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
    Serial.print((char) c);
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

bool XmlTokenParser::find_string_after_whitespace(const char* str)
{

	skip_whitespace();

	const size_t next_token_length = strlen(str);

	if (next_token_length + 1 > sizeof(buf))
		return false;

	size_t index = 0;

	while (index < next_token_length)
	{
		int c = s.read();
    Serial.print((char) c);
		if (c == -1)
			return false;
		if (c != str[index])
			return false;
		++index;
	}

	return true;
}

bool XmlTokenParser::find_attribute(const char *attr_name)
{

	bool ok = find_string_after_whitespace(attr_name);
	if (!ok)
		return false;

	ok = find_string_after_whitespace("=");
	if (!ok)
		return false;

	ok = find_string_after_whitespace("\"");
	if (!ok)
		return false;

	ok = read_string_until_terminator('"');

	return ok;
}

void XmlTokenParser::null_terminated_copy_of_buf(TokenBuffer* dst)
{
	strncpy(*dst, buf, sizeof(*dst) - 1);
	(*dst)[sizeof(*dst) - 1] = '\0';
}

// <
bool XmlTokenParser::expect_left_simple_bracket()
{
	return find_string_after_whitespace("<");
}
// >
bool XmlTokenParser::expect_right_simple_bracket()
{
	return find_string_after_whitespace(">");
}

// </ 
bool XmlTokenParser::expect_left_closing_bracket()
{
	return find_string_after_whitespace("</");
}
// />
bool XmlTokenParser::expect_right_closing_bracket()
{
	return find_string_after_whitespace("/>");
}

bool XmlTokenParser::expect_opening_tag(const char *tag_name)
{
	bool ok = expect_left_simple_bracket();
	if (!ok)
		return false;

	return find_string_after_whitespace(tag_name);
}
bool XmlTokenParser::expect_closing_tag(const char *tag_name)
{
	bool ok = expect_left_closing_bracket();
	if (!ok)
		return false;
	ok = find_string_after_whitespace(tag_name);
	if (!ok)
		return false;
	return expect_right_simple_bracket();
}
