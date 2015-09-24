// 
// 
// 

#include "XmlRespWriter.h"
#include "lockbox-controller-rq-base-protocol.h"
#include "lockbox-controller-rq-valid-protocol-values.h"

void XmlRespWriter::send_ack_response(Stream& s)
{
	write_open_tag(s, TYPE_ATTR_ACK_VALUE);
	write_close_tag(s);
}
void XmlRespWriter::send_err_repsonse(Stream& s, const char* err_msg)
{
	write_open_tag(s, TYPE_ATTR_ERR_VALUE);
	write_err_tag(s, err_msg);
	write_close_tag(s);
}


void XmlRespWriter::write_open_tag(Stream& s, const char* type_attr_value)
{
	if (!type_attr_value)
		return;

	xml_token_writer.write_tag_opening(s, RESP_TAG_NAME);
	xml_token_writer.write_attribute_text(s, TYPE_ATTR_NAME, type_attr_value);
	xml_token_writer.write_attribute_text(s, PROTOVER_ATTR_NAME, PROTOVER_ATTR_VALUE);
	xml_token_writer.write_right_simple_bracket(s);

}
void XmlRespWriter::write_err_tag(Stream& s, const char* err_msg)
{
	xml_token_writer.write_tag_opening(s, ERR_TAG_NAME);
	xml_token_writer.write_attribute_text(s, TXT_ATTR_NAME, err_msg);
	xml_token_writer.write_right_simple_bracket(s);
}
void XmlRespWriter::write_close_tag(Stream& s)
{
	xml_token_writer.write_tag_closing(s, RESP_TAG_NAME);

}


XmlRespWriter xml_resp_writer;

