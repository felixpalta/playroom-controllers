#ifndef __TABLE_CONTROLLER_SERVER_CONFIG_H__
#define __TABLE_CONTROLLER_SERVER_CONFIG_H__

static byte table_controller_mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

static const IPAddress GATEWAY_IP(192,168,1,1);

static const IPAddress DNS_IP(GATEWAY_IP);

static const IPAddress SUBNET_MASK(255,255,255,0);

///////////////////////////////////////////////////

// NOTE: This ip (table_controller_ip) will be set,
// only if failed to obtain IP by DHCP.

//IPAddress table_controller_ip(10,77,102,185);

static const IPAddress table_controller_ip(192, 168, 1, 120);

static const uint16_t table_controller_port = 10542;

#endif // __TABLE_CONTROLLER_SERVER_CONFIG_H__
