// 
// 
// 

#include "request_names.h"
#include <playroom-protocol.h>

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
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_LOCKBOX_OPEN, TYPE_ATTR_LOCKBOX_OPEN_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_LOCKBOX_CLOSE, TYPE_ATTR_LOCKBOX_CLOSE_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_LOCKBOX_ALL_OPEN, TYPE_ATTR_LOCKBOX_ALL_OPEN_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_LOCKBOX_ALL_CLOSE, TYPE_ATTR_LOCKBOX_ALL_CLOSE_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_TOP_LIGHT, TYPE_ATTR_TOP_LIGHT_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_SURROUND_LIGHT, TYPE_ATTR_SURROUND_LIGHT_VALUE),
  RQ_TYPE_STRBUF_AND_LEN(RQ_TYPE_LOCKBOX_LIGHT, TYPE_ATTR_LOCKBOX_LIGHT_VALUE),

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

const char* get_request_name(RqType rq_type)
{
  for (size_t i = 0; i < N_ELEMS(rq_types); ++i)
  {
    const RqTypeString& rq_type_string = rq_types[i];
    if (rq_type_string.rq_type == rq_type)
      return rq_type_string.s;
  }
  return "Invalid Request Type";
}


RqType get_request_type(const char *str)
{
  RqType request_type;

  for (size_t i = 0; i < N_ELEMS(rq_types); ++i)
  {
    const RqTypeString& rq_type_string = rq_types[i];
    if (strings_equal(str, rq_type_string.s, rq_type_string.len))
    {
      request_type = rq_type_string.rq_type;
      return request_type;
    }
  }

  request_type = RQ_TYPE_INVALID;
  return request_type;
}