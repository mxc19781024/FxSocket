#ifndef C_UDP_SOCKET_H
#define C_UDP_SOCKET_H
/**************************************************************************************************
* @file <文件名>  C_UDP_SOCKET_H
* @brief <描述>   C端通信 .xml数据接受
* @author <作者>  m00410
* @date <创建时间> 2021年10月13日
* @note [备注/注意事项]  
**************************************************************************************************/
#pragma once
#include <stdio.h>  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#include <iostream>
#include <string>
#include <vector>
#include "../NodeInfo3D.h"
#pragma comment(lib, "ws2_32.lib")
 

class CUdpSocket
{
public:
	/**
	* @brief <简要说明>  获取udp队象
	* @details [详细说明] 获取udp socket操作器
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明   udp socket操作器
	* @note [备注/注意事项]
	*/
	static CUdpSocket& getIpmreqUdpSocket();
	/**
	* @brief <简要说明> 数据数组是否有数据
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明 有 TRUE  无 FALSE
	* @note [备注/注意事项]
	*/
	bool NodeCount();
	/**
	* @brief <简要说明> 发送数据
	* @details [详细说明] 
	* @param 参数1 [in/out/inout] 【in】  发送的数据
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	bool write(const std::string& msg);
	/**
	* @brief <简要说明>  获取数据
	* @details [详细说明]  
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明   返回装结构体的数组
	* @note [备注/注意事项]
	*/
	const std::vector<SNodeInfo3D>& getNodeGather()const;
protected:
	CUdpSocket();
	~CUdpSocket();
	/**
	* @brief <简要说明>  循环执行
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void loop();
	/**
	* @brief <简要说明>  字符串切割
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明  返回字符串数组
	* @note [备注/注意事项]
	*/
	std::vector<std::string> split(const std::string & str, const std::string & pattern);
	/**
	* @brief <简要说明>  解析获取数据的数量
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseDataLogin(std::string list);
	/**
	* @brief <简要说明>  解析获取数据
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseDataAircaInfomtion(std::string & list);
	/**
	* @brief <简要说明>  解析命令
	* @details [详细说明]
	* @param 参数1 [in/out/inout] 参数1说明
	* @param 参数2 [in/out/inout] 参数2说明
	* @return 返回值以及说明
	* @note [备注/注意事项]
	*/
	void parseCommand(std::string & msg);
private:
	int m_iRet;					//数据大小
	SOCKET m_socket;			//socket文件描述符
	sockaddr_in addr;			//信息//本 AF_INET     234.2.2.2   4412
	int m_vNodeCount;			//数组元素个数
	sockaddr_in addr1;			//发送方信息//外部AF_INET   234.2.2.4     4414
	static int length;			//接受数据的条数
	char strbuf[1024];			//接受缓冲
	static CUdpSocket* m_udpSocket;					//单例队象
	std::vector<SNodeInfo3D> m_vNodeGather;			//数据储存数组
};

class CBuffer
{
public:
	static CBuffer& getObject();
	static bool isempty();
	void addTheTask(const std::string& task);
	const std::string getTheTask();
protected:
	CBuffer();
	~CBuffer();
private:
	static CBuffer m_cBuff;
	static int m_nVecNum;
	std::vector<std::string> m_vecBffer;
};
#endif
