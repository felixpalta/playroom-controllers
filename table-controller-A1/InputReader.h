// InputReader.h

#ifndef __INPUT_READER_H__
#define __INPUT_READER_H__

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include "table-controller-valid-protocol-values.h"
#include "XmlTokenParser.h"
#include "xml_token_max_length.h"

typedef enum
{
  RQ_TYPE_INVALID,
  RQ_TYPE_ALIVE,
  RQ_TYPE_SECTOR_NUMBER_ON,
  RQ_TYPE_SECTOR_NUMBER_OFF,
  RQ_TYPE_SECTOR_NUMBER_ALL_ON,
  RQ_TYPE_SECTOR_NUMBER_ALL_OFF,
  RQ_TYPE_SECTOR_ARROW_ON,
  RQ_TYPE_SECTOR_ARROW_OFF,
  RQ_TYPE_SECTOR_ARROW_ALL_ON,
  RQ_TYPE_SECTOR_ARROW_ALL_OFF,

} RqType;

typedef struct
{
  RqType request_type;

  bool sector_attr_found;
  TokenBuffer sector_attr_buf;

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
    ERROR_INVALID_PROTOVER,
    ERROR_SERIAL_ATTR_NAME,
    ERROR_SERIAL_ATTR_VALUE,
    ERROR_INVALID_SERIAL,
    ERROR_DATA_TAG_NAME,
    ERROR_SECTOR_ATTR_VALUE,
  } ErrorType;

  InputReader(Stream& s) : token_parser(s) {}

  ErrorType process_stream(InputRqParsingOutput* out);

private:
  bool parse_request_type(const char* str, RqType& request_type);
  XmlTokenParser token_parser;
  
};

#endif

