// 
// 
// 

#include "InputReader.h"
#include <playroom-protocol.h>
#include "table-controller-valid-protocol-values.h"

struct RqTypeString
{
  RqType rq_type;
  const char *s;
  size_t len;
};

#define RQ_TYPE_STRBUF_AND_LEN(rq_type, strbuf) { rq_type, strbuf, sizeof(strbuf) - 1 }

static const RqTypeString rq_types[] =
{
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_ALIVE, TYPE_ATTR_ALIVE_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_NUMBER_ON, TYPE_ATTR_SECTOR_NUMBER_ON_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_NUMBER_OFF, TYPE_ATTR_SECTOR_NUMBER_OFF_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_NUMBER_ALL_ON, TYPE_ATTR_SECTOR_NUMBER_ALL_ON_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_NUMBER_ALL_OFF, TYPE_ATTR_SECTOR_NUMBER_ALL_OFF_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_ARROW_ON, TYPE_ATTR_SECTOR_ARROW_ON_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_ARROW_OFF, TYPE_ATTR_SECTOR_ARROW_OFF_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_ARROW_ALL_ON, TYPE_ATTR_SECTOR_ARROW_ALL_ON_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SECTOR_ARROW_ALL_OFF, TYPE_ATTR_SECTOR_ARROW_ALL_OFF_VALUE),

};


#define N_ELEMS(a) (sizeof(a)/sizeof(a[0]))

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

bool InputReader::parse_request_type(const char* str, RqType& request_type)
{
  for (size_t i = 0; i < N_ELEMS(rq_types); ++i)
  {
    const RqTypeString& rq_type_string = rq_types[i];
    if (strings_equal(str, rq_type_string.s, rq_type_string.len))
    {
      request_type = rq_type_string.rq_type;
      return true;
    }
  }

  request_type = RQ_TYPE_INVALID;
  return false;
}

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

  if (!strings_equal(token_parser.get_internal_buf(), PROTOVER_ATTR_VALUE, sizeof(PROTOVER_ATTR_VALUE) - 1))
  {
    return ERROR_INVALID_PROTOVER;
  }

  ok = token_parser.find_attribute(SERIAL_ATTR_NAME);
  if (!ok)
    return ERROR_SERIAL_ATTR_VALUE;

  if (!strings_equal(token_parser.get_internal_buf(), SERIAL_ATTR_VALUE, sizeof(SERIAL_ATTR_VALUE) - 1))
  {
    return ERROR_INVALID_SERIAL;
  }

  ok = token_parser.expect_right_simple_bracket();
  if (!ok)
    return ERROR_CLOSE_TAG;

  switch (request_type)
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


