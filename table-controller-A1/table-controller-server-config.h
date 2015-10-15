#ifndef __TABLE_CONTROLLER_SERVER_CONFIG_H__
#define __TABLE_CONTROLLER_SERVER_CONFIG_H__

static byte table_controller_mac[] = {
  0x00, 0x2F, 0xC1, 0x20, 0x2F, 0x65};

static const IPAddress GATEWAY_IP(192,168,1,1);

static const IPAddress DNS_IP(GATEWAY_IP);

static const IPAddress SUBNET_MASK(255,255,255,0);

///////////////////////////////////////////////////

// NOTE: This ip (TABLE_CONTROLLER_IP) will be set,
// only if failed to obtain IP by DHCP.

//IPAddress TABLE_CONTROLLER_IP(10,77,102,185);

static const IPAddress TABLE_CONTROLLER_IP(192, 168, 1, 10);

static const uint16_t TABLE_CONTROLLER_LISTENING_PORT = 10542;

#endif // __TABLE_CONTROLLER_SERVER_CONFIG_H__

