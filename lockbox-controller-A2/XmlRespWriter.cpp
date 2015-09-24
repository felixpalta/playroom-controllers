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

	s.print("<");
	s.print(RESP_TAG_NAME);
	s.print(' ');
	s.print(TYPE_ATTR_NAME);
	s.print("=\"");
	s.print(type_attr_value);
	s.print('"');
	s.print(' ');
	s.print(PROTOVER_ATTR_NAME);
	s.print("=\"");
	s.print(PROTOVER_ATTR_VALUE);
	s.print('"');
	s.println('>');
}
void XmlRespWriter::write_err_tag(Stream& s, const char* err_msg)
{
	if (!err_msg)
		return;
	s.print('<');
	s.print(ERR_TAG_NAME);
	s.print(' ');
	s.print(TXT_ATTR_NAME);
	s.print("=\"");
	s.print(err_msg);
	s.print('"');
	s.println("/>");
}
void XmlRespWriter::write_close_tag(Stream& s)
{
	s.print("</");
	s.print(RESP_TAG_NAME);
	s.println('>');
}


XmlRespWriter xml_resp_writer;

