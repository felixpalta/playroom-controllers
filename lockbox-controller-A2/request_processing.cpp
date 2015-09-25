// 
// 
// 

#include "request_processing.h"
#include "LockBoxes.h"

extern LockBoxes lock_boxes;

static const char *processing_err_msg = "No processing error";

static bool processing_error_occured = false;

static bool verify_request(XmlRqParsingOutput& data);

static const char* get_request_name(RqType rq);

typedef enum
{
	ERROR_NONE,
	ERROR_TYPE_ATTR_NOT_FOUND,
	ERROR_PROTOVER_ATTR_NOT_FOUND,
	ERROR_SERIAL_ATTR_NOT_FOUND,
	ERROR_LOCKBOX_ATTR_NOT_FOUND,
	ERROR_BAD_TYPE,
	ERROR_BAD_PROTOVER,
	ERROR_BAD_SERIAL,
	ERROR_BAD_LOCKBOX_NUMBER,
	ERROR_INVALID_RQ_TYPE,
}
RqVerfyError;

static RqVerfyError error_code;

const char *get_request_error()
{
	if (processing_error_occured)
		return processing_err_msg;

	switch (error_code)
	{
	case ERROR_TYPE_ATTR_NOT_FOUND:
		return "Type attribute not found";

	case ERROR_PROTOVER_ATTR_NOT_FOUND:
		return "ProtoVer attribute not found";

	case ERROR_SERIAL_ATTR_NOT_FOUND:
		return "Serial number attribute not found";

	case ERROR_LOCKBOX_ATTR_NOT_FOUND:
		return "Lockbox attribute not found";

	case ERROR_BAD_TYPE:
		return "Invalid type attribute";
	case ERROR_BAD_PROTOVER:
		return "Invalid protocol version";
	case ERROR_BAD_SERIAL:
		return "Invalid serial number";
	case ERROR_BAD_LOCKBOX_NUMBER:
		return "Invalid lockbox number value";
	case ERROR_INVALID_RQ_TYPE:
		return "Invalid request type";
	case ERROR_NONE:
		return "Request processing: no error";
	default:
		return "Unexpected error";
	}
}

static const char* get_request_name(RqType rq)
{
	switch (rq)
	{
	case RQ_TYPE_ALIVE:
		return "RQ ALIVE";
	case RQ_TYPE_LOCKBOX_CLOSE:
		return "RQ LOCKBOX_CLOSE";
	case RQ_TYPE_LOCKBOX_OPEN:
		return "RQ LOCKBOX_OPEN";
	default:
	case RQ_TYPE_INVALID:
		return "RQ INVALID";
	}
}

void print_request(const XmlRqParsingOutput& data)
{

	Serial.println(get_request_name(data.request_type));

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

static bool process_alive_request()
{
	return true; // always successful
}

static const char* get_alive_error()
{
	return "IMPOSSIBLE: Alive request processing error";
}


static bool process_request(XmlRqParsingOutput& data)
{
	bool ok;
	int lockbox_number = atoi(data.lockbox_attr_buf);

	switch (data.request_type)
	{
	case RQ_TYPE_ALIVE:
		ok = process_alive_request();
		if (!ok)
		{
			processing_error_occured = true;
			processing_err_msg = get_alive_error();
			return false;
		}
		return true;
	case RQ_TYPE_LOCKBOX_CLOSE:
		ok = lock_boxes.close(lockbox_number);
		if (!ok)
		{
			processing_error_occured = true;
			processing_err_msg = lock_boxes.get_last_error();
			return false;
		}
		return true;
	case RQ_TYPE_LOCKBOX_OPEN:
		ok = lock_boxes.open(lockbox_number);
		if (!ok)
		{
			processing_error_occured = true;
			processing_err_msg = lock_boxes.get_last_error();
			return false;
		}
		return true;
	default:
		error_code = ERROR_INVALID_RQ_TYPE;
		return false;
	}
}

static bool verify_request(XmlRqParsingOutput& data)
{
	if (!data.proto_attr_buf)
	{
		error_code = ERROR_PROTOVER_ATTR_NOT_FOUND;
		return false;
	}
	if (!data.serial_attr_found)
	{
		error_code = ERROR_SERIAL_ATTR_NOT_FOUND;
		return false;
	}

	switch (data.request_type)
	{
	case RQ_TYPE_ALIVE:
	case RQ_TYPE_LOCKBOX_OPEN:
	case RQ_TYPE_LOCKBOX_CLOSE:
		break;
	default:
		error_code = ERROR_INVALID_RQ_TYPE;
		return false;
	}

	data.lockbox_attr_buf[sizeof(data.lockbox_attr_buf) - 1] = '\0';
	data.serial_attr_buf[sizeof(data.serial_attr_buf) - 1] = '\0';
	data.proto_attr_buf[sizeof(data.proto_attr_buf) - 1] = '\0';

	if (strncmp(data.proto_attr_buf, PROTOVER_ATTR_VALUE, sizeof(data.proto_attr_buf) - 1) != 0)
	{
		error_code = ERROR_BAD_PROTOVER;
		return false;
	}

	if (strncmp(data.serial_attr_buf, SERIAL_ATTR_VALUE, sizeof(data.serial_attr_buf) - 1) != 0)
	{
		error_code = ERROR_BAD_SERIAL;
		return false;
	}

	if (data.request_type == RQ_TYPE_LOCKBOX_CLOSE || data.request_type == RQ_TYPE_LOCKBOX_OPEN)
	{
		if (!data.lockbox_attr_found)
		{
			error_code = ERROR_LOCKBOX_ATTR_NOT_FOUND;
			return false;
		}

		size_t lockbox_attr_len = strlen(data.lockbox_attr_buf);

		if (lockbox_attr_len == 0 || lockbox_attr_len > sizeof(data.lockbox_attr_buf) - 1)
		{
			error_code = ERROR_BAD_LOCKBOX_NUMBER;
			return false;
		}
	}

	return true;
}

bool verify_and_process_request(XmlRqParsingOutput& data)
{
	processing_error_occured = false;
	error_code = ERROR_NONE;

	bool ok = verify_request(data);
	if (!ok)
		return false;

	ok = process_request(data);
	if (!ok)
		return false;
	return true;
}
