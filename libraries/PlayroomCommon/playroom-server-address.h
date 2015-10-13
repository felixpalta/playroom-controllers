#ifndef __PLAYROOM_SERVER_ADDRESS_H__
#define __PLAYROOM_SERVER_ADDRESS_H__

// NOTE: The client will first attempt to connect to the server by DNS name,
// then, if that failed, it will try to connect by IP address.

//#define DNS_SERVER_NAME_PRESENT

#ifdef DNS_SERVER_NAME_PRESENT

static const char PLAYROOM_SERVER_DNS_ADDRESS[] = "localhost";

#endif

static const IPAddress PLAYROOM_SERVER_IP_ADDRESS(10,77,102,224);

#endif // __PLAYROOM_SERVER_ADDRESS_H__
