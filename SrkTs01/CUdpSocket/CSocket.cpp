#include "CSocket.h"
#include <thread>
#include "../CparseMessage/CParseReadData.h"
#include "../CWriteBuff/CWriteBudder.h"

ip_mreq CSocket::m_socketMultiCast;

int CSocket::ReadMessage()
{
	std::cout << "-------------------------------------------------------------" << std::endl;
	int len = sizeof(sockaddr);
	char strRecv[1024] = { 0 };

	memset(strRecv, 0, sizeof(strRecv));
	m_iRet = recvfrom(m_socket, strRecv, sizeof(strRecv) - 1, 0, (sockaddr*)&addr, &len);
	if (m_iRet <= 0) {
		printf("recvfrom fail:%d\n", WSAGetLastError());
		return -1;
	}
	printf("\nrecv data:%s\n", strRecv);
	std::string msg(strRecv);

	CReadBuff::getInstanceReadBuff().setReadBuff(msg);

	/*if (msg == "hello")
	{
	scanf_s("%s", strRecv, sizeof(strRecv));
	m_iRet = sendto(m_socket, strRecv, strlen(strRecv) + 1, 0, (sockaddr*)&addr, sizeof(sockaddr));
	if (m_iRet <= 0) {
	printf("\nsend fail:%d", WSAGetLastError());
	}
	else {
	printf("\nsend data:%s\n", strRecv);
	}
	}*/
	std::cout << "-------------------------------------------------------------" << std::endl;

	return m_iRet;
}

CSocket::CSocket()
{
	instAll();
	//loop();
}


CSocket::~CSocket()
{
	closesocket(m_socket);
	WSACleanup();
}

int CSocket::instAll()
{
	m_iRet = 0;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);

	//本  AF_INET  INADDR_ANY   4414
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(4414);

	//外部 AF_INET   234.2.2.2    4412
	addr1.sin_family = AF_INET;
	addr1.sin_addr.S_un.S_addr = inet_addr("234.2.2.2");
	addr1.sin_port = htons(4412);

	int iFlag = 1;	// 0-同一台主机 1-夸主机
	m_iRet = setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&iFlag, sizeof(iFlag));
	if (m_iRet != 0) {
		printf("\nsetsockopt fail:%d\n", WSAGetLastError());
		return -1;
	}

	m_iRet = bind(m_socket, (sockaddr*)&addr, sizeof(addr));
	if (m_iRet != 0) {
		printf("\nbind fail:%d\n", WSAGetLastError());
		return -1;
	}
	printf("\nsocket:%d bind success\n", m_socket);

	// 加入组播  
	ip_mreq multiCast;

	multiCast.imr_interface.S_un.S_addr = INADDR_ANY;
	//multiCast.imr_interface.S_un.S_addr = inet_addr("192.168.101.68");
	multiCast.imr_multiaddr.S_un.S_addr = inet_addr("234.2.2.4");
	m_socketMultiCast = multiCast;
	m_iRet = setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multiCast, sizeof(multiCast));
	if (m_iRet != 0) {
		printf("setsockopt fail:%d", WSAGetLastError());
		return -1;
	}

	printf("\nudp group start\n");

}

ip_mreq & CSocket::getIpmreq()
{
	// TODO: 在此处插入 return 语句
	return m_socketMultiCast;
}

CSocket& CSocket::getInstanceSocket()
{
	// TODO: 在此处插入 return 语句
	static CSocket par;
	return par;
}

void CSocket::loop()
{
	while (true)
	{
		ReadMessage();
		
		while (!CWriteBudder::isemoty())
		{
			write();
		}
	}
}

void CSocket::write()
{
	if (!CWriteBudder::isemoty())
	{
		std::string msg = CWriteBudder::getWriteBuff().getWriteBuffer();
		m_iRet = sendto(m_socket, msg.c_str(), strlen(msg.c_str()) + 1, 0, (sockaddr*)&addr1, sizeof(sockaddr));
		if (m_iRet <= 0) {
			printf("send fail:%d\n", WSAGetLastError());
		}
		else {
			printf("send data:%s\n", msg.c_str());
		}
	}
	

}