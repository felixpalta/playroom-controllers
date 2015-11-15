#ifndef __PLAYROOM_PROTOCOL_H__
#define __PLAYROOM_PROTOCOL_H__

static const char RESP_TAG_NAME[] = "response";

static const char RQ_TAG_NAME[] = "request";

static const char TYPE_ATTR_NAME[] = "t";

static const char TYPE_ATTR_LOCKBOX_OPEN_VALUE[] = "lockbox-open";

static const char TYPE_ATTR_LOCKBOX_CLOSE_VALUE[] = "lockbox-close";

static const char TYPE_ATTR_LOCKBOX_ALL_OPEN_VALUE[] = "lockbox-all-open";

static const char TYPE_ATTR_LOCKBOX_ALL_CLOSE_VALUE[] = "lockbox-all-close";

static const char TYPE_ATTR_ALIVE_VALUE[] = "alive";

static const char PROTOVER_ATTR_NAME[] = "protoVer";

static const char SERIAL_ATTR_NAME[] = "serial";

static const char DATA_TAG_NAME[] = "data";

static const char TYPE_ATTR_ACK_VALUE[] = "ack";

static const char TYPE_ATTR_ERR_VALUE[] = "err";

static const char TYPE_ATTR_GAME_START_VALUE[] = "game-start";
static const char TYPE_ATTR_CLEANING_VALUE[] = "cleaning";
static const char TYPE_ATTR_STANDBY_VALUE[] = "standby";

static const char TYPE_ATTR_BARREL_PLAY_VALUE[] = "barrel-play";

static const char TYPE_ATTR_BARREL_SECTOR_VALUE[] = "barrel-sector";

static const char ID_ATTR_NAME[] = "id";

static const char ERR_TAG_NAME[] = "err";

static const char TXT_ATTR_NAME[] = "txt";

static const char TYPE_ATTR_SECTOR_NUMBER_ON_VALUE[] = "sector-number-on";
static const char TYPE_ATTR_SECTOR_NUMBER_OFF_VALUE[] = "sector-number-off";

static const char TYPE_ATTR_SECTOR_ARROW_ON_VALUE[] = "sector-arrow-on";
static const char TYPE_ATTR_SECTOR_ARROW_OFF_VALUE[] = "sector-arrow-off";

static const char TYPE_ATTR_SECTOR_NUMBER_ALL_ON_VALUE[] = "sector-number-all-on";
static const char TYPE_ATTR_SECTOR_NUMBER_ALL_OFF_VALUE[] = "sector-number-all-off";

static const char TYPE_ATTR_SECTOR_ARROW_ALL_ON_VALUE[] = "sector-arrow-all-on";
static const char TYPE_ATTR_SECTOR_ARROW_ALL_OFF_VALUE[] = "sector-arrow-all-off";

static const char TYPE_ATTR_TOP_LIGHT_VALUE[] = "top-light";
static const char TYPE_ATTR_SURROUND_LIGHT_VALUE[] = "surround-light";
static const char TYPE_ATTR_LOCKBOX_LIGHT_VALUE[] = "lockbox-light";

static const char TYPE_ATTR_BARREL_WAIT_VALUE[] = "barrel-wait";

static const char PERCENT_ATTR_NAME[] = "percent";

static const char TYPE_ATTR_DOOR_LOCK_VALUE[] = "door-lock";
static const char TYPE_ATTR_DOOR_UNLOCK_VALUE[] = "door-unlock";

#endif // __PLAYROOM_PROTOCOL_H__
