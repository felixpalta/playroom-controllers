// request_names.h

#ifndef _REQUEST_NAMES_h
#define _REQUEST_NAMES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

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


const char* get_request_name(RqType rq_type);

RqType get_request_type(const char *str);

#endif

