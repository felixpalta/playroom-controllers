#ifndef __LOCKBOX_CONTROLLER_SERVER_CONFIG_H__
#define __LOCKBOX_CONTROLLER_SERVER_CONFIG_H__

static byte lockbox_controller_mac[] = {
  0x00, 0x3F, 0xC2, 0xEF, 0xFE, 0xED };

static const IPAddress GATEWAY_IP(192,168,1,1);

static const IPAddress DNS_IP(GATEWAY_IP);

static const IPAddress SUBNET_MASK(255,255,255,0);

///////////////////////////////////////////////////

// NOTE: This ip (lockbox_controller_ip) will be set,
// only if failed to obtain IP by DHCP.

//static const IPAddress lockbox_controller_ip(10,77,102,185);

static const IPAddress lockbox_controller_ip(192, 168, 1, 4);

static const uint16_t lockbox_controller_port = 10541;

#endif //__LOCKBOX_CONTROLLER_SERVER_IP_H__

