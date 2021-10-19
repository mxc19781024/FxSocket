// MultiCast.cpp: implementation of the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////
#include "MultiCast.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiCast::CMultiCast()
{
	InitSocket();
}

CMultiCast::~CMultiCast()
{
	// 
	closesocket(m_sockdes);
	WSACleanup();

}


/*********************************************************************
 * 函数名称:CMultiCast::InitSocket
 * 说明:初始化socket，windows下必须，unix下不需要
 * 入口参数:
 * 返回值:
 * void  -- 
 * 作者: 
 * 时间 : 2006-03-25 13:51:00 
*********************************************************************/
void CMultiCast::InitSocket()
{
	int ret = 0;  
#ifdef _WIN32
	WORD version; 
	WSADATA data;
	version = (MAKEWORD(2, 2)); 
	ret = WSAStartup(version, &data); 
	if (ret != 0) 
	{  
		ret = WSAGetLastError(); 

		if (ret == WSANOTINITIALISED) 
		{  
			printf("not initialised"); 
		}
		WSACleanup();
	}
#endif

	//m_sockdes = socket (AF_INET, SOCK_DGRAM, 0);
	
}


/*********************************************************************
 * 函数名称:CMultiCast::JoinGroup
 * 说明:加入组播
 * 入口参数:
 * char *ip_address -- 组播地址
 *  unsigned int port -- 组播端口
 *  int kind -- 收发状态，0-收;1-发  反了？？（0-发;1-收）
 * 返回值:
 * int  -- 
 * 作者: 
 * 时间 : 2006-03-25 13:52:00 
*********************************************************************/
int CMultiCast::JoinGroup(char *ip_address, unsigned int port, int kind)
{
	//InitSocket();
	// open socket
	m_sockdes = socket (AF_INET, SOCK_DGRAM, 0);
	if (m_sockdes == INVALID_SOCKET)
	{
		perror("socket create error!");
		//TRACE ("socket() failed, Err: %d\n", WSAGetLastError());
		return -1;
	}
 	m_mc_ip_address = ip_address;
	m_port = port;
	
	//multicast address
	m_mc_sock_address.sin_family = AF_INET;
	m_mc_sock_address.sin_addr.s_addr = inet_addr(m_mc_ip_address);
	m_mc_sock_address.sin_port = htons(m_port);	
	m_mc_sock_address_len = sizeof(m_mc_sock_address);
	
//	/* set receive buffer size */
//	int bufsize = recvBuffSize;
//	if (setsockopt (m_sockdes, SOL_SOCKET, SO_RCVBUF,(char*) &bufsize, sizeof (int)) != 0) {
//		perror("set socket receive buffer error!");
//		return ;
//	}
//   
//	/* set send buffer size */
//	bufsize = sendBuffSize;
//	if (setsockopt (m_sockdes, SOL_SOCKET, SO_SNDBUF, 
//				 (char*) &bufsize, sizeof (int)) != 0) {
//	 perror("set socket send buffer error!");
//	 return ;
//	}

	  
      	// bind socket
	m_sock_address.sin_addr.s_addr = htons(INADDR_ANY);
	m_sock_address.sin_family = AF_INET;
	m_sock_address.sin_port =  htons(m_port);
	m_sock_address_len = sizeof(m_sock_address);
	//printf("ip %s,port %d\n",m_mc_ip_address,m_port);
	
		// multicast: disable loopback
	char loopback = 1;
	if (setsockopt (m_sockdes, IPPROTO_IP, IP_MULTICAST_LOOP, (char*) &loopback, sizeof (loopback)) == -1) 
	{
	
		return -1;
	}
	
	// set reuse socket option 
	int reuse = 1;
	if (setsockopt (m_sockdes, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof (int)) != 0) 
	{
		 perror("set socket reuseradd error!");
	 	return false;
	}

	if (kind !=0)
	{
		// 为源地址绑定 端口、IP
		if(bind(m_sockdes, (struct sockaddr*)&m_sock_address, m_sock_address_len) == -1)
		{
			perror("bind error!");
	 		return false;
		}
     
		//multicast: join
		struct ip_mreq group; // Multicast interface structure  
		group.imr_multiaddr.s_addr = inet_addr (m_mc_ip_address);
		group.imr_interface.s_addr = htonl (INADDR_ANY);
		if (setsockopt (m_sockdes, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &group, sizeof (ip_mreq)) == -1)
		{
			perror("add group error!");
			return false;
		}
	
		unsigned int ttl = 64;
		if (setsockopt(m_sockdes, IPPROTO_IP, IP_MULTICAST_TTL, (char*) &ttl, sizeof (ttl)) == -1) 
		{
			perror("ttl error!");
			return false;
		}
		
	}
	return 0;
}

/*********************************************************************
 * 函数名称:CMultiCast::SendData
 * 说明:从组播发送数据
 * 入口参数:
 * char *buffer -- 发送数据缓冲
 *  int buff_len -- 缓冲大小
 * 返回值:
 * int  -- 
 * 作者: 
 * 时间 : 2006-03-25 13:54:22 
*********************************************************************/
int CMultiCast::SendData(char *buffer, int buff_len)
{
	//往ip和端口发送数据
	//printf("buffer %s,length %d\n",buffer,buff_len);
	int re = sendto(m_sockdes,buffer,buff_len,0,(struct sockaddr *)&m_mc_sock_address,m_mc_sock_address_len);	
	if (re<0)
	{
		perror("SEND ERROR");
		return -1;
	}
	//printf("buffer %s,length %d\n",buffer,buff_len);
	return 0;
}

/*********************************************************************
 * 函数名称:CMultiCast::RecvData
 * 说明:接收组播数据
 * 入口参数:
 * char *buffer -- 接收缓冲
 *  int &buffer_len -- 接收缓冲大小
 * 返回值:
 * int  -- 
 * 作者:  
 * 时间 : 2006-03-25 13:55:07 
*********************************************************************/
int CMultiCast::RecvData(char *buffer, int &buffer_len)
{
	
	//printf("start receive data\n");
	int n = recvfrom (m_sockdes, buffer, buffer_len, 0, (struct sockaddr*)&m_mc_sock_address, &m_mc_sock_address_len);
	if (n < 0)
	{
		perror("Receive error!\n");
		return -1;
	}
	if(n<buffer_len)
	{
		buffer_len = n;
	}
	//printf("received data %s,length %d\n",buffer,buffer_len);
	return 0;
}


int CMultiCast::SendData( char * buffer, int buff_len,unsigned long strIp,int iPort )
{
	struct sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = strIp;
	socketAddr.sin_port = htons(iPort);	

	int re = sendto(m_sockdes,buffer,buff_len,0,(struct sockaddr *)&socketAddr,sizeof(socketAddr));	
	if (re<0)
	{
		perror("SEND ERROR");
		return -1;
	}
	//printf("buffer %s,length %d\n",buffer,buff_len);
	return 0;
}