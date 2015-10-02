// 
// 
// 

#include "XmlRespWriter.h"
#include "lockbox-controller-rq-base-protocol.h"
#include "lockbox-controller-rq-valid-protocol-values.h"

void XmlRespWriter::send_ack_response()
{
	write_open_tag(TYPE_ATTR_ACK_VALUE);
	write_close_tag();
}
void XmlRespWriter::send_err_repsonse(const char* err_msg)
{
	write_open_tag(TYPE_ATTR_ERR_VALUE);
	write_err_tag(err_msg);
	write_close_tag();
}


void XmlRespWriter::write_open_tag(const char* type_attr_value)
{
	if (!type_attr_value)
		return;

	xml_token_writer.write_tag_opening(RESP_TAG_NAME);
	xml_token_writer.write_attribute_text(TYPE_ATTR_NAME, type_attr_value);
	xml_token_writer.write_attribute_text(PROTOVER_ATTR_NAME, PROTOVER_ATTR_VALUE);
	xml_token_writer.write_right_simple_bracket();

}
void XmlRespWriter::write_err_tag(const char* err_msg)
{
	xml_token_writer.write_tag_opening(ERR_TAG_NAME);
	xml_token_writer.write_attribute_text(TXT_ATTR_NAME, err_msg);
	xml_token_writer.write_right_simple_bracket();
}
void XmlRespWriter::write_close_tag()
{
	xml_token_writer.write_tag_closing(RESP_TAG_NAME);

}

