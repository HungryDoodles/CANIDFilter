#include "Sink.h"
#include <iostream>

#ifdef USE_WIN_SOCK

Sink::Sink(const char* addr, int16_t port)
{
	dest.sin_family = AF_INET;
	inet_pton(AF_INET, addr, &dest.sin_addr.S_un.S_addr);
	dest.sin_port = htons(port);

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}
Sink::~Sink() 
{
	if (s != INVALID_SOCKET)
		closesocket(s);
}

int Sink::Send(const char* data, int length)
{
	if (s == INVALID_SOCKET)
		return 0;

#ifdef _DEBUG
	std::cout << "Sink::Send(" << data << ", " << length << ")" << std::endl;
#endif

	return sendto(s, data, length, 0, (sockaddr *)&dest, sizeof(dest));
}




#endif
#ifdef USE_UNIX_SOCK

Sink::Sink(const char* addr, int16_t port)
{
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(addr);
	dest.sin_port = htons(port);

	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}
Sink::~Sink()
{
	if (s >= 0)
		close(s);
}

int Sink::Send(const char* data, int length)
{
	if (s < 0)
		return 0;

#ifdef _DEBUG
	std::cout << "Sink::Send(" << data << ", " << length << ")" << std::endl;
#endif

	return sendto(s, data, length, 0, (sockaddr*)&dest, sizeof(dest));
}

#endif
