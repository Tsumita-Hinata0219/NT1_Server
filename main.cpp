#define _WINDOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "WSock32.lib")



int main()
{
	WSADATA wsaData{};

	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		/* ‰Šú‰»ƒGƒ‰[ */
		printf("WinSock‚Ì‰Šú‰»‚É¸”s‚µ‚Ü‚µ‚½\n");
		return 1;
	}
	WSACleanup();


	return 0;
}