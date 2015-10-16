// 
// 
// 

#include "InputReader.h"
#include <playroom-protocol.h>
#include "table-controller-valid-protocol-values.h"

static bool strings_equal(const char *str, const char *other, size_t other_len)
{
  if (strlen(str) == other_len)
  {
    if (strncmp(str, other, other_len) == 0)
    {
      return true;
    }
  }
  return false;
}

#define BAD_RQ "Request parsing error: "

const char* InputReader::get_error(ErrorType err)
{
  switch (err)
  {
  case ERROR_NONE:
    return BAD_RQ "No error";
  case ERROR_OUTPUT_NULLPTR:
    return BAD_RQ "Nullptr to output buffer";
  case ERROR_CLOSE_TAG:
    return BAD_RQ "Expected closing tag";
  case ERROR_RQ_TAG_NAME:
    return BAD_RQ "Expected request opening tag";
  case ERROR_RQ_CLOSING_TAG_NAME:
    return BAD_RQ "Expected request closing tag";
  case ERROR_TYPE_ATTR_VALUE:
    return BAD_RQ "Expected type attribute";
  case ERROR_TYPE_ATTR_INVALID:
    return BAD_RQ "Invalid type attribute value";
  case ERROR_PROTOVER_ATTR_VALUE:
    return BAD_RQ "Expected protoVer attribute";
  case ERROR_INVALID_PROTOVER:
    return BAD_RQ "Invalid protoVer value";
  case ERROR_DATA_TAG_NAME:
    return BAD_RQ "Expected data opening tag";
  case ERROR_SECTOR_ATTR_VALUE:
    return BAD_RQ "Expected sector attribute";
  default:
    return BAD_RQ "Unexpected error";
  }
}

#undef BAD_RQ

InputReader::ErrorType InputReader::process_stream(InputRqParsingOutput* out)
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

  out->request_type = get_request_type(token_parser.get_internal_buf());
  if (out->request_type == RQ_TYPE_INVALID)
  {
    return ERROR_TYPE_ATTR_INVALID;
  }
  
  ok = token_parser.find_attribute(PROTOVER_ATTR_NAME);
  if (!ok)
    return ERROR_PROTOVER_ATTR_VALUE;

  if (!strings_equal(token_parser.get_internal_buf(), PROTOVER_ATTR_VALUE, sizeof(PROTOVER_ATTR_VALUE) - 1))
  {
    return ERROR_INVALID_PROTOVER;
  }

  ok = token_parser.expect_right_simple_bracket();
  if (!ok)
    return ERROR_CLOSE_TAG;
    
  switch (out->request_type)
  {
  case RQ_TYPE_SECTOR_NUMBER_ON:
  case RQ_TYPE_SECTOR_NUMBER_OFF:
  case RQ_TYPE_SECTOR_ARROW_ON:
  case RQ_TYPE_SECTOR_ARROW_OFF:
    ok = token_parser.expect_opening_tag(DATA_TAG_NAME);
    if (!ok)
      return ERROR_DATA_TAG_NAME;
      
    ok = token_parser.find_attribute(SECTOR_ATTR_NAME);
    if (!ok)
      return ERROR_SECTOR_ATTR_VALUE;
    out->sector_attr_found = true;
    token_parser.null_terminated_copy_of_buf(&out->sector_attr_buf);
    
    ok = token_parser.expect_right_closing_bracket();
    if (!ok)
      return ERROR_CLOSE_TAG;
    break;
  default:
    break;
  }
      
  ok = token_parser.expect_closing_tag(RQ_TAG_NAME);
  if (!ok)
    return ERROR_RQ_CLOSING_TAG_NAME;
  
  return ERROR_NONE;
}


