#ifndef C_SOCKET_H
#define C_SOCKET_H
/**************************************************************************************************
* @file <文件名>    C_SOCKET_H
* @brief <描述>     接收发消息，将接受到的消息放到任务队列
* @author <作者>    m00410
* @date <创建时间>  2021年10月13日
* @note [备注/注意事项]  后期可以采用多线程，将读端写端分离
**************************************************************************************************/

#pragma once
#include <string> 
#include <stdio.h>  
#include <iostream>
#include <winsock2.h>  
#include <ws2tcpip.h> 
#pragma comment(lib, "ws2_32.lib")
#include "../CReadbuff/CReadBuff.h"

class CSocket
{
public:
	/**
	* @brief <简要说明>    获取IP 信息
	* @details [详细说明]  
	* @param 参数1 [in/out/inout] 【in】 ip_mreq
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	static ip_mreq& getIpmreq();

	/**
	* @brief <简要说明>  建立处理数据的socket
	* @details [详细说明] 采用 Windows Api udp 进行通信
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	static CSocket& getInstanceSocket();
	/**
	* @brief <简要说明> 发送数据
	* @details [详细说明] 从写任务队列获取发送的数据进行发送
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void write();
	/**
	* @brief <简要说明>  从系统缓冲读取数据
	* @details [详细说明]  接受消息
	* @param 参数1 [in/out/inout] 【out】 返回读到的数据大小
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]  目前无法做到异步读取  后期优化采用多线程
	*/
	int ReadMessage();

protected:

	/**
	* @brief <简要说明> 循环读取
	* @details [详细说明] 读消息 解析消息  发送
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void loop();

	/**
	* @brief <简要说明>  初始化
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	int instAll();
private:
	CSocket();
	~CSocket();
	SOCKET m_socket;					//socket
	sockaddr_in addr;					//地址域    //本  AF_INET  INADDR_ANY   4414
	sockaddr_in addr1;					//目标地址域 //外部 AF_INET   234.2.2.2    4412
	int m_iRet;							//读取数据的大小
	static ip_mreq m_socketMultiCast;	//IP 信息
};

#endif
