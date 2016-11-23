#pragma once
#ifndef _HTTP_FEED_H
#define _HTTP_FEED_H

//#include <winsock2.h>
//#include <ws2tcpip.h>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>



#define DEFAULT_BUFLEN 8096

class HTTP_Feed
{
private:
	//winsocket dll initialisieren
	bool _StartUp;
	WSADATA _WSAData;
	SOCKET _ConnectSocket;
	char* _ServerName;
	char* _Port;

	//Socket erzeugen
	struct addrinfo *_Result;
	struct addrinfo *_Ptr;
	struct addrinfo _Hints;

public:
	HTTP_Feed();
	~HTTP_Feed();

	bool StartUp();

	bool CreateSocket(char* serverName, char* port);

	bool ConnectTCP();

	bool Send(char* data);
	bool Receive(char* data);

	bool Shutdown();
};

#endif 