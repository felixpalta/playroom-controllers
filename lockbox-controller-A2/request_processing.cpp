// 
// 
// 

#include "request_processing.h"
#include "LockBoxes.h"
#include "dimmers.h"
#include <request_names.h>
#include "door_lock.h"

extern LockBoxes lock_boxes;

static const char *processing_err_msg = "No processing error";

static bool processing_error_occured = false;

static bool verify_request(InputRqParsingOutput& data);

typedef enum
{
  ERROR_NONE,
  ERROR_LOCKBOX_ATTR_NOT_FOUND,
  ERROR_PERCENT_ATTR_NOT_FOUND,
  ERROR_BAD_TYPE,
  ERROR_BAD_PROTOVER,
  ERROR_BAD_SERIAL,
  ERROR_BAD_LOCKBOX_NUMBER,
  ERROR_BAD_PERCENT_VALUE,
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
  case ERROR_LOCKBOX_ATTR_NOT_FOUND:
    return "Lockbox attribute not found";
  case ERROR_PERCENT_ATTR_NOT_FOUND:
    return "Percent attribute not found";
  case ERROR_BAD_TYPE:
    return "Invalid type attribute";
  case ERROR_BAD_PROTOVER:
    return "Invalid protocol version";
  case ERROR_BAD_SERIAL:
    return "Invalid serial number";
  case ERROR_BAD_LOCKBOX_NUMBER:
    return "Invalid lockbox number value";
  case ERROR_BAD_PERCENT_VALUE:
    return "Invalid percent value";
  case ERROR_INVALID_RQ_TYPE:
    return "Invalid request type";
  case ERROR_NONE:
    return "Request processing: no error";
  default:
    return "Unexpected error";
  }
}

void print_request(const InputRqParsingOutput& data)
{

  Serial.println(get_request_name(data.request_type));

  if (data.id_attr_found)
  {
    Serial.print("Lockbox: ");
    Serial.println(data.id_attr_buf);
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


static bool process_request(InputRqParsingOutput& data)
{
  bool ok;
  int lockbox_number;
  int percent_value;
  DimmerEnum dimmer_id;

  switch (data.request_type)
  {
  case RQ_TYPE_LOCKBOX_CLOSE:
  case RQ_TYPE_LOCKBOX_OPEN:
    lockbox_number = atoi(data.id_attr_buf);
    break;
  case RQ_TYPE_SURROUND_LIGHT:
    dimmer_id = DIMMER_SURROUND_LIGHT;
    break;
  case RQ_TYPE_TOP_LIGHT:
    dimmer_id = DIMMER_TOP_LIGHT;
    break;
  case RQ_TYPE_LOCKBOX_LIGHT:
    dimmer_id = DIMMER_LOCKBOX_LIGHT;
    break;
    break;
  default:
    break;
  }

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
  case RQ_TYPE_LOCKBOX_ALL_CLOSE:
    ok = lock_boxes.close_all();
    if (!ok)
    {
      processing_error_occured = true;
      processing_err_msg = lock_boxes.get_last_error();
      return false;
    }
    return true;
  case RQ_TYPE_LOCKBOX_ALL_OPEN:
    ok = lock_boxes.open_all();
    if (!ok)
    {
      processing_error_occured = true;
      processing_err_msg = lock_boxes.get_last_error();
      return false;
    }
    return true;
  case RQ_TYPE_TOP_LIGHT:
  case RQ_TYPE_SURROUND_LIGHT:
  case RQ_TYPE_LOCKBOX_LIGHT:
    percent_value = atoi(data.percent_attr_buf);
    ok = dimmers_light_set(dimmer_id, percent_value);
    if (!ok)
    {
      processing_error_occured = true;
      processing_err_msg = "Dimmer request error";
      return false;
    }
    return true;
  case RQ_TYPE_DOOR_LOCK:
    ok = door_lock_close();
    if (!ok)
    {
      processing_error_occured = true;
      processing_err_msg = "door_lock_close() request error";
      return false;
    }
    return true;
  case RQ_TYPE_DOOR_UNLOCK:
    ok = door_lock_open();
    if (!ok)
    {
      processing_error_occured = true;
      processing_err_msg = "door_lock_open() request error";
      return false;
    }
    return true;
  default:
    error_code = ERROR_INVALID_RQ_TYPE;
    return false;
  }
}

static bool verify_request(InputRqParsingOutput& data)
{

  switch (data.request_type)
  {
  case RQ_TYPE_ALIVE:
  case RQ_TYPE_LOCKBOX_OPEN:
  case RQ_TYPE_LOCKBOX_CLOSE:
  case RQ_TYPE_LOCKBOX_ALL_OPEN:
  case RQ_TYPE_LOCKBOX_ALL_CLOSE:
  case RQ_TYPE_TOP_LIGHT:
  case RQ_TYPE_SURROUND_LIGHT:
  case RQ_TYPE_LOCKBOX_LIGHT:
  case RQ_TYPE_DOOR_LOCK:
  case RQ_TYPE_DOOR_UNLOCK:
    break;
  default:
    error_code = ERROR_INVALID_RQ_TYPE;
    return false;
  }

  data.id_attr_buf[sizeof(data.id_attr_buf) - 1] = '\0';
  data.percent_attr_buf[sizeof(data.percent_attr_buf) - 1] = '\0';

  if (data.request_type == RQ_TYPE_LOCKBOX_CLOSE || data.request_type == RQ_TYPE_LOCKBOX_OPEN)
  {
    if (!data.id_attr_found)
    {
      error_code = ERROR_LOCKBOX_ATTR_NOT_FOUND;
      return false;
    }

    size_t lockbox_attr_len = strlen(data.id_attr_buf);

    if (lockbox_attr_len == 0 || lockbox_attr_len > sizeof(data.id_attr_buf) - 1)
    {
      error_code = ERROR_BAD_LOCKBOX_NUMBER;
      return false;
    }
  }
  else if (data.request_type == RQ_TYPE_TOP_LIGHT || data.request_type == RQ_TYPE_SURROUND_LIGHT || data.request_type == RQ_TYPE_LOCKBOX_LIGHT)
  {
    if (!data.percent_attr_buf)
    {
      error_code = ERROR_PERCENT_ATTR_NOT_FOUND;
      return false;
    }

    size_t percent_attr_len = strlen(data.percent_attr_buf);
    if (percent_attr_len == 0 || percent_attr_len > sizeof(data.percent_attr_buf) - 1)
    {
      error_code = ERROR_BAD_PERCENT_VALUE;
      return false;
    }
  }

  return true;
}

bool verify_and_process_request(InputRqParsingOutput& data)
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
