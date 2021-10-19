#ifndef C_UDP_SOCKET_H
#define C_UDP_SOCKET_H
/**************************************************************************************************
* @file <�ļ���>  C_UDP_SOCKET_H
* @brief <����>   C��ͨ�� .xml���ݽ���
* @author <����>  m00410
* @date <����ʱ��> 2021��10��13��
* @note [��ע/ע������]  
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
	* @brief <��Ҫ˵��>  ��ȡudp����
	* @details [��ϸ˵��] ��ȡudp socket������
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��   udp socket������
	* @note [��ע/ע������]
	*/
	static CUdpSocket& getIpmreqUdpSocket();
	/**
	* @brief <��Ҫ˵��> ���������Ƿ�������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵�� �� TRUE  �� FALSE
	* @note [��ע/ע������]
	*/
	bool NodeCount();
	/**
	* @brief <��Ҫ˵��> ��������
	* @details [��ϸ˵��] 
	* @param ����1 [in/out/inout] ��in��  ���͵�����
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	bool write(const std::string& msg);
	/**
	* @brief <��Ҫ˵��>  ��ȡ����
	* @details [��ϸ˵��]  
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��   ����װ�ṹ�������
	* @note [��ע/ע������]
	*/
	const std::vector<SNodeInfo3D>& getNodeGather()const;
protected:
	CUdpSocket();
	~CUdpSocket();
	/**
	* @brief <��Ҫ˵��>  ѭ��ִ��
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void loop();
	/**
	* @brief <��Ҫ˵��>  �ַ����и�
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��  �����ַ�������
	* @note [��ע/ע������]
	*/
	std::vector<std::string> split(const std::string & str, const std::string & pattern);
	/**
	* @brief <��Ҫ˵��>  ������ȡ���ݵ�����
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseDataLogin(std::string list);
	/**
	* @brief <��Ҫ˵��>  ������ȡ����
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseDataAircaInfomtion(std::string & list);
	/**
	* @brief <��Ҫ˵��>  ��������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseCommand(std::string & msg);
private:
	int m_iRet;					//���ݴ�С
	SOCKET m_socket;			//socket�ļ�������
	sockaddr_in addr;			//��Ϣ//�� AF_INET     234.2.2.2   4412
	int m_vNodeCount;			//����Ԫ�ظ���
	sockaddr_in addr1;			//���ͷ���Ϣ//�ⲿAF_INET   234.2.2.4     4414
	static int length;			//�������ݵ�����
	char strbuf[1024];			//���ܻ���
	static CUdpSocket* m_udpSocket;					//��������
	std::vector<SNodeInfo3D> m_vNodeGather;			//���ݴ�������
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
