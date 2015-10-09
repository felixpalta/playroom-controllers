#ifndef __PLAYROOM_SERVER_ADDRESS_H__
#define __PLAYROOM_SERVER_ADDRESS_H__

// NOTE: The client will first attempt to connect to the server by DNS name,
// then, if that failed, it will try to connect by IP address.

static const char PLAYROOM_SERVER_DNS_ADDRESS[] = "server1";

static const IPAddress PLAYROOM_SERVER_IP_ADDRESS(192, 168, 1, 6);

#endif // __PLAYROOM_SERVER_ADDRESS_H__
