// 
// 
// 

#include "request_processing.h"
#include "LockBoxes.h"

extern LockBoxes lock_boxes;

void print_request(const XmlRqParsingOutput& data)
{

	if (data.type_attr_found)
	{
		Serial.print("Type: ");
		Serial.println(data.type_attr_buf);
	}
	if (data.proto_attr_found)
	{
		Serial.print("ProtoVer: ");
		Serial.println(data.proto_attr_buf);
	}
	if (data.serial_attr_found)
	{
		Serial.print("Serial: ");
		Serial.println(data.serial_attr_buf);
	}
	if (data.lockbox_attr_found)
	{
		Serial.print("Lockbox: ");
		Serial.println(data.lockbox_attr_buf);
	}
}

bool process_request(ActionType action, int lock_box_number)
{
	switch (action)
	{
	case OPEN:
		return lock_boxes.open(lock_box_number);
		break;
	case CLOSE:
		return lock_boxes.close(lock_box_number);
		break;
	default:
		return false;
	}
}

bool verify_request(XmlRqParsingOutput& data, ActionType& type_attribut, int& lock_box_number)
{
	if (!data.lockbox_attr_found ||
		!data.proto_attr_buf ||
		!data.type_attr_found ||
		!data.serial_attr_found)
		return false;

	data.type_attr_buf[sizeof(data.type_attr_buf) - 1] = '\0';
	data.lockbox_attr_buf[sizeof(data.lockbox_attr_buf) - 1] = '\0';
	data.serial_attr_buf[sizeof(data.serial_attr_buf) - 1] = '\0';
	data.proto_attr_buf[sizeof(data.proto_attr_buf) - 1] = '\0';

	if (strncmp(data.type_attr_buf, TYPE_ATTR_LOCKBOX_OPEN_VALUE, sizeof(data.type_attr_buf) - 1) == 0)
		type_attribut = OPEN;
	else if (strncmp(data.type_attr_buf, TYPE_ATTR_LOCKBOX_CLOSE_VALUE, sizeof(data.type_attr_buf) - 1) == 0)
		type_attribut = CLOSE;
	else
		type_attribut = INVALID;

	if (type_attribut == INVALID)
	{
		Serial.println("Unexpected type attribute");
		return false;
	}

	if (strncmp(data.proto_attr_buf, PROTOVER_ATTR_VALUE, sizeof(data.proto_attr_buf) - 1) != 0)
	{
		Serial.println("Invalid protocol version");
		return false;
	}

	if (strncmp(data.serial_attr_buf, SERIAL_ATTR_VALUE, sizeof(data.serial_attr_buf) - 1) != 0)
	{
		Serial.println("Invalid serial number");
		return false;
	}

	size_t lockbox_attr_len = strlen(data.lockbox_attr_buf);

	if (lockbox_attr_len == 0 || lockbox_attr_len > sizeof(data.lockbox_attr_buf) - 1)
	{
		Serial.println("Invalid lockbox value length");
		return false;
	}

	lock_box_number = atoi(data.lockbox_attr_buf);

	return true;
}


