#ifndef __TABLE_CONTROLLER_SERVER_CONFIG_H__
#define __TABLE_CONTROLLER_SERVER_CONFIG_H__

byte table_controller_mac[] = {
  0xAB, 0xAC, 0xBE, 0xEF, 0xFE, 0xED };

// NOTE: This ip (table_controller_ip) will be set,
// only if failed to obtain IP by DHCP.

//IPAddress table_controller_ip(10,77,102,185);

IPAddress table_controller_ip(192, 168, 1, 120);


uint16_t table_controller_port = 10542;

#endif // __TABLE_CONTROLLER_SERVER_CONFIG_H__
