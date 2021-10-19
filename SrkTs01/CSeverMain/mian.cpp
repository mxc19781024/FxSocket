//#include <stdio.h>
//#include <iostream>
//#include <WinSock2.h>
//#include <ws2tcpip.h>
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
//
//	SOCKADDR_IN saddr;
//	SOCKADDR_IN caddr;
//
//	saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
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
//
//	char on = 1;
//
//	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
//
//	dwError = bind(sockfd, (SOCKADDR*)&saddr, sizeof(SOCKADDR));
//
//	if (dwError != 0)
//	{
//		closesocket(sockfd);
//		WSACleanup();
//		return 0;
//	}
//
//
//	ip_mreq multiCast;
//	//multiCast.imr_interface.S_un.S_addr = INADDR_ANY;
//	multiCast.imr_interface.S_un.S_addr = inet_addr("192.168.0.104");
//	multiCast.imr_multiaddr.S_un.S_addr = inet_addr("234.2.2.2");
//	dwError = setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
//	
//
//	while (true)
//	{
//		printf("\n-----------------------------wait-------------------------------\n");
//		int retNum = 0;
//		int lengh = sizeof(SOCKADDR);
//		retNum = recvfrom(sockfd, RecvBuffer, sizeof(RecvBuffer),0,(SOCKADDR*)&caddr,&lengh);
//		if (retNum <= 0)
//		{
//			printf("error!\n");
//		}
//		else
//		{
//			printf("data : %s \n", RecvBuffer);
//		}
//	}
//
//	closesocket(sockfd);
//	WSACleanup();
//
//	return 0;
//
//}

#include "../CUdpSocket/CSocket.h"
#include "../CparseMessage/CParseReadData.h"
#include "../CWriteBuff/CWriteBudder.h"
#include <iostream>
#include <windows.h>
#include <WINSOCK.H>
#pragma commen(lib,"WSOCK32.lib")

DWORD _stdcall UdpDataParseThread(LPVOID param)
{
	while (true)
	{
		if (CReadBuff::getInstanceReadBuff().emity())
		{
			std::cout << "Data Parse Thread id: " << GetCurrentThreadId() << std::endl;
			CParseReadData::getInstance().parseData();
		}
		else {
			std::cout << "Data Parse Thread id: " << GetCurrentThreadId() << std::endl;
			Sleep(500);
		}
	}
}

DWORD _stdcall UdpReadDataThread(LPVOID param)
{
	while (true)
	{
		std::cout << "\nRead Data Thread id: " << GetCurrentThreadId() << std::endl;
		CSocket::getInstanceSocket().ReadMessage();
	}
}


DWORD _stdcall UdpWriteDataThread(LPVOID param)
{
	while (true)
	{
		if (!CWriteBudder::isemoty())
		{
			std::cout << "\nWrite Data Thread id: " << GetCurrentThreadId() << std::endl;
			CSocket::getInstanceSocket().write();
		}
		else
		{
			std::cout << "\nWrite Data Thread id: " << GetCurrentThreadId() << std::endl;
			Sleep(500);
		}
	}
}
int main()
{
	HANDLE hRecvThread = CreateThread(NULL, 0, UdpReadDataThread, NULL, 0, NULL);
	HANDLE hParseThread = CreateThread(NULL, 0, UdpDataParseThread, NULL, 0, NULL);
	HANDLE hWriteThread = CreateThread(NULL, 0, UdpWriteDataThread, NULL, 0, NULL);

	while (true)
	{
		Sleep(500);
		std::cout << "Main Thread id£º" << GetCurrentThreadId() << std::endl;
	}

	system("pause");
	return 0;
}