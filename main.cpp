#include "HTTP_Feed.h"
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char* argv[])
{
	//52.5.238.97
	char* serverName = "io.adafruit.com";
	char* port = "80";

	HTTP_Feed connector;
	connector.CreateSocket(serverName, port);
	connector.ConnectTCP();
	
	char data[2048] = "";
	strcat_s(data, sizeof(data), "GET /api/groups/weather/send.json?x-aio-key=f27fa137d3934140a44974f37003de00&testfeed=5 HTTP/1.1\r\n");
	strcat_s(data, sizeof(data), "Host: io.adafruit.com\r\n");
	//strcat_s(data, sizeof(data), "Connection: close\r\n");
	//strcat_s(data, sizeof(data), "Accept-Charset: ISO-8859-1,UTF-8;q=0.7,*;q=0.7\r\n");
	//strcat_s(data, sizeof(data), "Cache-Control: no-cache\r\n");
	//strcat_s(data, sizeof(data), "Accept-Language: de,en;q=0.7,en-us;q=0.3\r\n\r\n");
	
	char* recData = NULL;
	connector.Send(data);
	connector.Receive(recData);

	printf("Received:\n%s", recData);

	getchar();
	connector.Shutdown();
	return 0;
}
