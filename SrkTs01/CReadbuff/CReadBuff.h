#ifndef C_READ_BUFF_H
#define C_READ_BUFF_H
/**************************************************************************************************
* @file <�ļ���> CReadBuff
* @brief <����>  �������õ������ݴ洢��
* @author <����> m00410
* @date <����ʱ��>  2021��10��13��
* @note [��ע/ע������]  ����ģʽ
**************************************************************************************************/
#pragma once

#include <string>
#include <iostream>
#include <queue>

class CReadBuff
{
public:
	/**
	* @brief <��Ҫ˵��>��ȡ������ж���
	* @details [��ϸ˵��]��ȡ������ж���
	* @param ����1 [in/out/inout] 
	* @param ����2 [in/out/inout] 
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	static CReadBuff& getInstanceReadBuff();

	/**
	* @brief <��Ҫ˵��>��������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] [in] �洢������
	* @param ����2 [in/out/inout] 
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void setReadBuff(const std::string& msg);

	/**
	* @brief <��Ҫ˵��>��ȡ��Ҫ���������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ��out�� ��ȡ������
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	const std::string getFirstReadbuffer();

	/**
	* @brief <��Ҫ˵��>�ж���������Ƿ�Ϊ��
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] 
	* @param ����2 [in/out/inout] ��out�� û����Ҫ���������
	* @return ����ֵ�Լ�˵��   NULL����FALSE
	* @note [��ע/ע������]
	*/
	bool emity();

private:

	CReadBuff();

	~CReadBuff();

private:
	static CReadBuff*  m_Readbuff;				//������ж���

	std::queue<std::string> m_stdReadbuff;		//�������

	static int m_count;							//������е�����

};

#endif
