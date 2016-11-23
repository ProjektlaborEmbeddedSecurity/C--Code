#include "HTTP_Feed.h"
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

HTTP_Feed::HTTP_Feed()
{
	StartUp();
}

HTTP_Feed::~HTTP_Feed()
{
	//freeaddrinfo(_Result);
	//freeaddrinfo(_Ptr);
	Shutdown();
	//delete _ServerName;
	//delete _Port;
	
}

bool HTTP_Feed::StartUp()
{
	//WSA Starten
	int iResult = WSAStartup(MAKEWORD(2, 2), &_WSAData);
	if (iResult != 0)
		_StartUp = false;
	else
		_StartUp = true;

	_ConnectSocket = INVALID_SOCKET;

	return _StartUp;
}

bool HTTP_Feed::CreateSocket(char* serverName, char* port)
{
	_ServerName = serverName;
	_Port = port;

	if (!_StartUp)
		StartUp();

	int iResult;

	ZeroMemory(&_Hints, sizeof(_Hints));
	_Hints.ai_family = AF_UNSPEC;
	_Hints.ai_socktype = SOCK_STREAM;
	_Hints.ai_protocol = IPPROTO_TCP;

	//Servername
	iResult = getaddrinfo(serverName, port, &_Hints, &_Result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}

	_Ptr = _Result;
	//Socket erzeugen
	_ConnectSocket = socket(_Ptr->ai_family, _Ptr->ai_socktype,
		_Ptr->ai_protocol);

	if (_ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(_Result);
		WSACleanup();
		return false;
	}
	return true;
}


bool HTTP_Feed::ConnectTCP()
{
	int iResult;

	if (!_StartUp)
		StartUp();

	if (_ConnectSocket == INVALID_SOCKET)
		CreateSocket(_ServerName, _Port);
	
	//Connect
	iResult = connect(_ConnectSocket, _Ptr->ai_addr, (int)_Ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(_ConnectSocket);
		_ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(_Result);

	if (_ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}

	return true;
}

bool HTTP_Feed::Send(char* data)
{
	int length = (int)strlen(data);
	int iResult;
	int count = 0;

	// Send an initial buffer
	while (count < length)
	{
		iResult = send(_ConnectSocket, data, length, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(_ConnectSocket);
			WSACleanup();
			return false;
		}
		else
		{
			count += iResult;
		}
	}
	printf("Bytes Sent: %ld\n", count);

	return true;
}

bool HTTP_Feed::Receive(char* data)
{
	int count = 0;
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;	
	char recvbuf[DEFAULT_BUFLEN];
	
	// Receive data until the server closes the connection
	do {
		iResult = recv(_ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			count += iResult;
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0)
			printf("Receiving ended: %ld\n", count);
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	data = recvbuf;
	return true;
}


bool HTTP_Feed::Shutdown()
{	
	int iResult = closesocket(_ConnectSocket);
	iResult += WSACleanup();
	return iResult == 0;
	//return true;
}

