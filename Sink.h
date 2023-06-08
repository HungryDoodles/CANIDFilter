#ifndef _SINK_H_
#define _SINK_H_
#include "Config.h"
#include <stdint.h>


#ifdef USE_WIN_SOCK
#include <WinSock2.h>
#include <Ws2tcpip.h>
#endif
#ifdef USE_UNIX_SOCK
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#endif

class Sink
{
	sockaddr_in dest;

#ifdef USE_WIN_SOCK
	SOCKET s = INVALID_SOCKET;
#endif
#ifdef USE_UNIX_SOCK
	int s = -1;
#endif

public:
	Sink(const char* addr, int16_t port);
	~Sink();

	// returns actual bytes sent
	int Send(const char* data, int length);
};

#endif