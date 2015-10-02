// 
// 
// 

#include "XmlRqParser.h"
#include "lockbox-controller-rq-base-protocol.h"
#include "lockbox-controller-rq-valid-protocol-values.h"

bool XmlRqParser::parse_request_type(const char* str, RqType& request_type)
{
	if (strncmp(str, TYPE_ATTR_ALIVE_VALUE, sizeof(TYPE_ATTR_ALIVE_VALUE) - 1) == 0)
	{
		request_type = RQ_TYPE_ALIVE;
		return true;
	}

	if (strncmp(str, TYPE_ATTR_LOCKBOX_OPEN_VALUE, sizeof(TYPE_ATTR_LOCKBOX_OPEN_VALUE) - 1) == 0)
	{
		request_type = RQ_TYPE_LOCKBOX_OPEN;
		return true;
	}

	if (strncmp(str, TYPE_ATTR_LOCKBOX_CLOSE_VALUE, sizeof(TYPE_ATTR_LOCKBOX_CLOSE_VALUE) - 1) == 0)
	{
		request_type = RQ_TYPE_LOCKBOX_CLOSE;
		return true;
	}

	request_type = RQ_TYPE_INVALID;
	return false;
}

XmlRqParser::ErrorType XmlRqParser::process_stream(XmlRqParsingOutput* out)
{
	if (!out)
		return ERROR_OUTPUT_NULLPTR;

	memset(out, 0, sizeof(*out));
	out->request_type = RQ_TYPE_INVALID;

	bool rq_open_tag_found = false;
	while (token_parser.available())
	{
		rq_open_tag_found = token_parser.expect_opening_tag(RQ_TAG_NAME);
		if (rq_open_tag_found)
			break;
	}

	if (!rq_open_tag_found)
		return ERROR_RQ_TAG_NAME;

	bool ok = token_parser.find_attribute(TYPE_ATTR_NAME);
	if (!ok)
		return ERROR_TYPE_ATTR_VALUE;

	RqType request_type;
	ok = parse_request_type(token_parser.get_internal_buf(), request_type);
	if (!ok)
	{
		out->request_type = RQ_TYPE_INVALID;
		return ERROR_TYPE_ATTR_INVALID;
	}
	out->request_type = request_type;
	
	ok = token_parser.find_attribute(PROTOVER_ATTR_NAME);
	if (!ok)
		return ERROR_PROTOVER_ATTR_VALUE;

	out->proto_attr_found = true;
	token_parser.null_terminated_copy_of_buf(&out->proto_attr_buf);

	ok = token_parser.find_attribute(SERIAL_ATTR_NAME);
	if (!ok)
		return ERROR_SERIAL_ATTR_VALUE;

	out->serial_attr_found = true;
	token_parser.null_terminated_copy_of_buf(&out->serial_attr_buf);

	ok = token_parser.expect_right_simple_bracket();
	if (!ok)
		return ERROR_CLOSE_TAG;

	if (request_type == RQ_TYPE_LOCKBOX_CLOSE || request_type == RQ_TYPE_LOCKBOX_OPEN)
	{
		ok = token_parser.expect_opening_tag(DATA_TAG_NAME);
		if (!ok)
			return ERROR_DATA_TAG_NAME;

		ok = token_parser.find_attribute(LOCKBOX_ATTR_NAME);
		if (!ok)
			return ERROR_LOCKBOX_ATTR_VALUE;
		out->lockbox_attr_found = true;
		token_parser.null_terminated_copy_of_buf(&out->lockbox_attr_buf);

		ok = token_parser.expect_right_closing_bracket();
		if (!ok)
			return ERROR_CLOSE_TAG;
	}
	ok = token_parser.expect_closing_tag(RQ_TAG_NAME);
	if (!ok)
		return ERROR_RQ_CLOSING_TAG_NAME;
	
	return ERROR_NONE;
}


