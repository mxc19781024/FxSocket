#ifndef C_SOCKET_H
#define C_SOCKET_H
/**************************************************************************************************
* @file <�ļ���>    C_SOCKET_H
* @brief <����>     ���շ���Ϣ�������ܵ�����Ϣ�ŵ��������
* @author <����>    m00410
* @date <����ʱ��>  2021��10��13��
* @note [��ע/ע������]  ���ڿ��Բ��ö��̣߳�������д�˷���
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
	* @brief <��Ҫ˵��>    ��ȡIP ��Ϣ
	* @details [��ϸ˵��]  
	* @param ����1 [in/out/inout] ��in�� ip_mreq
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	static ip_mreq& getIpmreq();

	/**
	* @brief <��Ҫ˵��>  �����������ݵ�socket
	* @details [��ϸ˵��] ���� Windows Api udp ����ͨ��
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	static CSocket& getInstanceSocket();
	/**
	* @brief <��Ҫ˵��> ��������
	* @details [��ϸ˵��] ��д������л�ȡ���͵����ݽ��з���
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void write();
	/**
	* @brief <��Ҫ˵��>  ��ϵͳ�����ȡ����
	* @details [��ϸ˵��]  ������Ϣ
	* @param ����1 [in/out/inout] ��out�� ���ض��������ݴ�С
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]  Ŀǰ�޷������첽��ȡ  �����Ż����ö��߳�
	*/
	int ReadMessage();

protected:

	/**
	* @brief <��Ҫ˵��> ѭ����ȡ
	* @details [��ϸ˵��] ����Ϣ ������Ϣ  ����
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void loop();

	/**
	* @brief <��Ҫ˵��>  ��ʼ��
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	int instAll();
private:
	CSocket();
	~CSocket();
	SOCKET m_socket;					//socket
	sockaddr_in addr;					//��ַ��    //��  AF_INET  INADDR_ANY   4414
	sockaddr_in addr1;					//Ŀ���ַ�� //�ⲿ AF_INET   234.2.2.2    4412
	int m_iRet;							//��ȡ���ݵĴ�С
	static ip_mreq m_socketMultiCast;	//IP ��Ϣ
};

#endif
