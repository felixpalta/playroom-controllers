// InputReader.h

#ifndef __INPUT_READER_H__
#define __INPUT_READER_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "lockbox-controller-rq-valid-protocol-values.h"
#include "XmlTokenParser.h"
#include "xml_token_max_length.h"

typedef enum
{
  RQ_TYPE_INVALID,
  RQ_TYPE_LOCKBOX_OPEN,
  RQ_TYPE_LOCKBOX_CLOSE,
  RQ_TYPE_LOCKBOX_ALL_OPEN,
  RQ_TYPE_LOCKBOX_ALL_CLOSE,
  RQ_TYPE_ALIVE,

} RqType;

typedef struct
{
  RqType request_type;

  bool proto_attr_found;
  TokenBuffer proto_attr_buf;

  bool serial_attr_found;
  TokenBuffer serial_attr_buf;

  bool lockbox_attr_found;
  TokenBuffer lockbox_attr_buf;

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
    ERROR_TYPE_ATTR_NAME,
    ERROR_TYPE_ATTR_VALUE,
    ERROR_TYPE_ATTR_INVALID,
    ERROR_PROTOVER_ATTR_NAME,
    ERROR_PROTOVER_ATTR_VALUE,
    ERROR_SERIAL_ATTR_NAME,
    ERROR_SERIAL_ATTR_VALUE,
    ERROR_LOCKBOX_ATTR_VALUE,
    ERROR_DATA_TAG_NAME,
  } ErrorType;

  InputReader(Stream& s) : token_parser(s) {}

  ErrorType process_stream(InputRqParsingOutput* out);

private:
  bool parse_request_type(const char* str, RqType& request_type);
  XmlTokenParser token_parser;
  
};

#endif

