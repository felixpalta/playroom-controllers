// 
// 
// 

#include "request_processing.h"
#include "sectors.h"
#include "request_names.h"

static bool verify_request(InputRqParsingOutput& data);

typedef enum
{
  ERROR_NONE,
  ERROR_SECTOR_ATTR_NOT_FOUND,
  ERROR_BAD_SECTOR_NUMBER,
  ERROR_INVALID_RQ_TYPE,
  ERROR_REQUEST_PROCESSING,
}
RqVerfyError;

static RqVerfyError error_code;

#define BAD_RQ "Request processing error: "

const char *get_request_error()
{
  switch (error_code)
  {
  case ERROR_INVALID_RQ_TYPE:
    return BAD_RQ"Invalid request type";
  case ERROR_SECTOR_ATTR_NOT_FOUND:
    return BAD_RQ"Attribute \"sector\" not found";
  case ERROR_BAD_SECTOR_NUMBER:
    return BAD_RQ"Attribute \"sector\" value incorrect";
  case ERROR_REQUEST_PROCESSING:
    return BAD_RQ"Internal error, probably incorrect arguments?";
  case ERROR_NONE:
    return BAD_RQ"No error";
  default:
    return BAD_RQ"Unexpected error";
  }
}

#undef BAD_RQ

void print_request(const InputRqParsingOutput& data)
{

  Serial.println(get_request_name(data.request_type));

  if (data.sector_attr_found)
  {
    Serial.print("Sector: ");
    Serial.println(data.sector_attr_buf);
  }

}

static bool process_alive_request()
{
  return true; // always successful
}

static bool process_request(InputRqParsingOutput& data)
{
  bool ok;
  int sector_number = atoi(data.sector_attr_buf);

  bool(*no_arg_handler)() = NULL;
  bool(*single_arg_handler)(bool) = NULL;
  bool(*two_arg_handler)(int, bool) = NULL;

  int arg_sector_number = sector_number;
  bool arg_on;

  switch (data.request_type)
  {
  case RQ_TYPE_ALIVE:
    no_arg_handler = process_alive_request;
    break;
  case RQ_TYPE_SECTOR_NUMBER_ON:
    two_arg_handler = sector_number_led_pin_write;
    arg_on = true;
    break;
  case RQ_TYPE_SECTOR_NUMBER_OFF:
    two_arg_handler = sector_number_led_pin_write;
    arg_on = false;
    break;
  case RQ_TYPE_SECTOR_ARROW_ON:
    two_arg_handler = sector_arrow_led_pin_write;
    arg_on = true;
    break;
  case RQ_TYPE_SECTOR_ARROW_OFF:
    two_arg_handler = sector_arrow_led_pin_write;
    arg_on = false;
    break;
  case RQ_TYPE_SECTOR_NUMBER_ALL_ON:
    single_arg_handler = sector_all_number_leds_write;
    arg_on = true;
    break;
  case RQ_TYPE_SECTOR_NUMBER_ALL_OFF:
    single_arg_handler = sector_all_number_leds_write;
    arg_on = false;
    break;
  case RQ_TYPE_SECTOR_ARROW_ALL_ON:
    single_arg_handler = sector_all_arrow_leds_write;
    arg_on = true;
    break;
  case RQ_TYPE_SECTOR_ARROW_ALL_OFF:
    single_arg_handler = sector_all_arrow_leds_write;
    arg_on = false;
    break;
  default:
    error_code = ERROR_INVALID_RQ_TYPE;
    return false;
  }

  if (no_arg_handler)
  {
    ok = no_arg_handler();
  }
  else if (single_arg_handler)
  {
    ok = single_arg_handler(arg_on);
  }
  else if (two_arg_handler)
  {
    ok = two_arg_handler(arg_sector_number, arg_on);
  }
  else
    ok = false; // should be impossible to get here
  if (!ok)
    error_code = ERROR_REQUEST_PROCESSING;
  return ok;
}

static bool verify_request(InputRqParsingOutput& data)
{
  switch (data.request_type)
  {
  case RQ_TYPE_ALIVE:
  case RQ_TYPE_SECTOR_NUMBER_ON:
  case RQ_TYPE_SECTOR_NUMBER_OFF:
  case RQ_TYPE_SECTOR_NUMBER_ALL_ON:
  case RQ_TYPE_SECTOR_NUMBER_ALL_OFF:
  case RQ_TYPE_SECTOR_ARROW_ON:
  case RQ_TYPE_SECTOR_ARROW_OFF:
  case RQ_TYPE_SECTOR_ARROW_ALL_ON:
  case RQ_TYPE_SECTOR_ARROW_ALL_OFF:
    break;
  default:
    error_code = ERROR_INVALID_RQ_TYPE;
    return false;
  }

  data.sector_attr_buf[sizeof(data.sector_attr_buf) - 1] = '\0';

  switch (data.request_type)
  {
  case RQ_TYPE_SECTOR_NUMBER_ON:
  case RQ_TYPE_SECTOR_NUMBER_OFF:
  case RQ_TYPE_SECTOR_ARROW_ON:
  case RQ_TYPE_SECTOR_ARROW_OFF:
  {
    if (!data.sector_attr_found)
    {
      error_code = ERROR_SECTOR_ATTR_NOT_FOUND;
      return false;
    }

    size_t sector_attr_len = strlen(data.sector_attr_buf);

    if (sector_attr_len == 0 || sector_attr_len > sizeof(data.sector_attr_buf) - 1)
    {
      error_code = ERROR_BAD_SECTOR_NUMBER;
      return false;
    }
  }
    break;
  default:
    break;
  }

  return true;
}

bool verify_and_process_request(InputRqParsingOutput& data)
{
  error_code = ERROR_NONE;

  bool ok = verify_request(data) && process_request(data);
  
  return ok;
}
