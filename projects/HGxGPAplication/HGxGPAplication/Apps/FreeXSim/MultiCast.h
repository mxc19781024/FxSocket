#ifndef _MULTICAST_H
#define _MULTICAST_H

/**************************************************************************************************
* @file CMultiCast.h
* @note ¶à²¥Í¨ÐÅ
* @author 
* @data 
**************************************************************************************************/

#ifdef _WIN32
	#include <Winsock2.h>
	#include <ws2tcpip.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#ifdef _WIN32
	#pragma comment(lib,"ws2_32.lib")
#endif
#include <stdio.h>
#define sendBuffSize 4096
#define recvBuffSize 4096

class CMultiCast  
{
public:
	int m_sockdes;
	int m_sock_address_len;
	int m_mc_sock_address_len;
	struct sockaddr_in m_sock_address;
	struct sockaddr_in m_mc_sock_address;
	char * m_mc_ip_address;
	int m_port;
public:
	/*
	int ABCDToDCBA(unsigned int abcd);
	int DCBAToABCD(unsigned int dcba);
	*/
	void InitSocket();
	int RecvData(char *buffer, int &buffer_len);
	int SendData(char * buffer, int buff_len);
 
	int SendData( char * buffer, int buff_len,unsigned long strIp,int iPort );
	//	int SendData2(char *buffer, int buff_len);
	int JoinGroup(char *ip_address,unsigned int port, int kind);//kind =0 send; 1 receive 
//	int CMultiCast::JoinGroup(char *ip_address, unsigned int port, int kind)
	CMultiCast();
	virtual ~CMultiCast();

};

#endif 
