#ifndef __LOCKBOX_CONTROLLER_RQ_VALID_PROTOCOL_VALUES_H__
#define __LOCKBOX_CONTROLLER_RQ_VALID_PROTOCOL_VALUES_H__

static const char TYPE_ATTR_LOCKBOX_OPEN_VALUE[] = "lockbox-open";
static const char TYPE_ATTR_LOCKBOX_CLOSE_VALUE[] = "lockbox-close";
static const size_t MAX_RQ_TYPE_ATTR_VALUE_LENGTH = 64;

static const char PROTOVER_ATTR_VALUE[] = "1.1";
static const size_t MAX_PROTO_ATTR_VALUE_LENGTH = 16;

static const char SERIAL_ATTR_VALUE[] = "UDH-TX144";
static const size_t MAX_SERIAL_ATTR_VALUE_LENGTH = 16;

// 'Lockbox' attribute has numerical value, like 'lockbox="24"'
static const size_t MAX_LOCKBOX_ATTR_VALUE_LENGTH = 16;

#endif //__LOCKBOX_CONTROLLER_RQ_VALID_PROTOCOL_VALUES_H__
