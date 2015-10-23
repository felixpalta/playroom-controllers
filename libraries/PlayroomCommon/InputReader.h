// InputReader.h

#ifndef __INPUT_READER_H__
#define __INPUT_READER_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "XmlTokenParser.h"
#include "request_names.h"

typedef struct
{
  RqType request_type;

  bool id_attr_found;
  TokenBuffer id_attr_buf;

} InputRqParsingOutput;

class InputReader
{
public:
  typedef enum
  {
    ERROR_NONE,
    ERROR_OUTPUT_NULLPTR,
    ERROR_CLOSE_TAG,
    ERROR_RQ_TAG_NAME,
    ERROR_RQ_CLOSING_TAG_NAME,
    ERROR_TYPE_ATTR_VALUE,
    ERROR_TYPE_ATTR_INVALID,
    ERROR_PROTOVER_ATTR_VALUE,
    ERROR_INVALID_PROTOVER,
    ERROR_DATA_TAG_NAME,
    ERROR_ID_ATTR_VALUE,
  } ErrorType;

  InputReader(XmlTokenParser& t, const char *protover_value) : 
    token_parser(t),
    valid_protover(protover_value)
  {
  }

  ErrorType process_stream(InputRqParsingOutput* out);

  static const char* get_error(ErrorType err);

private:
  XmlTokenParser &token_parser;
  const char *valid_protover;
};

#endif

