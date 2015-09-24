#ifndef __LOCKBOX_CONTROLLER_SERVER_CONFIG_H__
#define __LOCKBOX_CONTROLLER_SERVER_CONFIG_H__

byte lockbox_controller_mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//IPAddress lockbox_controller_ip(10,77,102,185);

IPAddress lockbox_controller_ip(192, 168, 1, 77);


uint16_t lockbox_controller_port = 10541;

#endif //__LOCKBOX_CONTROLLER_SERVER_IP_H__
