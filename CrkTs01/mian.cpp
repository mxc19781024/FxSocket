//{#include <stdio.h>
//#include <iostream>
//#include <WinSock2.h>
//
//#pragma comment(lib,"ws2_32.lib")
//
//int main()
//{
//	char RecvBuffer[100];
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	DWORD dwError;
//	wVersionRequested = MAKEWORD(2, 2);
//	dwError = WSAStartup(wVersionRequested, &wsaData);
//	if (dwError != 0)
//	{
//		return 0;
//	}
//	SOCKADDR_IN saddr;
//	saddr.sin_addr.S_un.S_addr = inet_addr("224.0.0.100");
//	saddr.sin_family = AF_INET;
//	saddr.sin_port = htons(24567);
//
//	SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//	if (sockfd == SOCKET_ERROR)
//	{
//		closesocket(sockfd);
//		WSACleanup();
//		return 0;
//	}
//	char on = 1;
//	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
//
//	while (true)
//	{
//		printf("\n-----------------wait------------------------\n");
//		printf("¿Í»§¶Ë£º ");
//		scanf_s("%s", RecvBuffer, sizeof(RecvBuffer));
//		int rec;
//		rec = sendto(sockfd, RecvBuffer, sizeof(RecvBuffer), 0, (SOCKADDR*)&saddr, sizeof(SOCKADDR));
//		if (rec < 0)
//		{
//			printf("error\n");
//		}
//		memset(RecvBuffer, 0, 100);
//	}
//	closesocket(sockfd);
//	WSACleanup();
//
//	return 0;
//}
//}


#include "./FxUdpSocket/CUdpSocket.h"
#include <WINDOWS.H>
#pragma commen(lib,"WSOCK32.lib")

DWORD _stdcall UdpReadData(LPVOID param)
{
	CUdpSocket::getIpmreqUdpSocket();
	return GetCurrentThreadId();
}

int main()
{
	HANDLE pThreadRead = CreateThread(NULL, 0, UdpReadData, NULL, 0, NULL);
	while (true)
	{
		if (CBuffer::isempty())
		{
			CUdpSocket::getIpmreqUdpSocket().write(CBuffer::getObject().getTheTask());
		} 
		else
		{
			Sleep(50);
		}
		
	}
	system("pause");
	return 0;
}