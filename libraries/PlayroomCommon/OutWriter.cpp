// 
// 
// 

#include "OutWriter.h"

#include <playroom-protocol.h>

void OutWriter::finalize_message()
{
  xml_writer.write_newline();
}

void OutWriter::send_ack_response()
{
  write_response_open_tag(TYPE_ATTR_ACK_VALUE);
  write_response_close_tag();
  finalize_message();
}
void OutWriter::send_err_repsonse(const char* err_msg)
{
  write_response_open_tag(TYPE_ATTR_ERR_VALUE);
  write_err_tag(err_msg);
  write_response_close_tag();
  finalize_message();
}


void OutWriter::write_response_open_tag(const char* type_attr_value)
{
  if (!type_attr_value)
    return;

  xml_writer.write_tag_opening(RESP_TAG_NAME);
  xml_writer.write_attribute_text(TYPE_ATTR_NAME, type_attr_value);
  xml_writer.write_attribute_text(PROTOVER_ATTR_NAME, valid_protover);
  xml_writer.write_right_simple_bracket();

}
void OutWriter::write_err_tag(const char* err_msg)
{
  xml_writer.write_tag_opening(ERR_TAG_NAME);
  xml_writer.write_attribute_text(TXT_ATTR_NAME, err_msg);
  xml_writer.write_right_closing_bracket();
}
void OutWriter::write_response_close_tag()
{
  xml_writer.write_tag_closing(RESP_TAG_NAME);

}

void OutWriter::write_request_open_tag(const char* type_attr_value)
{
  if (!type_attr_value)
    return;

  xml_writer.write_tag_opening(RQ_TAG_NAME);
  xml_writer.write_attribute_text(TYPE_ATTR_NAME, type_attr_value);
  xml_writer.write_attribute_text(PROTOVER_ATTR_NAME, valid_protover);
  xml_writer.write_attribute_text(SERIAL_ATTR_NAME, valid_serial);
  xml_writer.write_right_simple_bracket();
}
void OutWriter::write_request_close_tag()
{
  xml_writer.write_tag_closing(RQ_TAG_NAME);
}

void OutWriter::send_barrel_play_request()
{
  write_request_open_tag(TYPE_ATTR_BARREL_PLAY_VALUE);
  write_request_close_tag();
  finalize_message();
}

void OutWriter::send_barrel_sector_request(int n)
{
  write_request_open_tag(TYPE_ATTR_BARREL_SECTOR_VALUE);
  xml_writer.write_tag_opening(DATA_TAG_NAME);
  xml_writer.write_attribute_num(ID_ATTR_NAME, n);
  xml_writer.write_right_closing_bracket();
  write_request_close_tag();
  finalize_message();
}


void OutWriter::send_game_start_request()
{
  write_request_open_tag(TYPE_ATTR_GAME_START_VALUE);
  write_request_close_tag();
  finalize_message();
}

void OutWriter::send_cleaning_request()
{
  write_request_open_tag(TYPE_ATTR_CLEANING_VALUE);
  write_request_close_tag();
  finalize_message();
}
void OutWriter::send_standby_request()
{
  write_request_open_tag(TYPE_ATTR_STANDBY_VALUE);
  write_request_close_tag();
  finalize_message();
}
