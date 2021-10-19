#ifndef C_WRITE_BUFFER_H
#define C_WRITE_BUFFER_H
/**************************************************************************************************
* @file <�ļ���>  C_WRITE_BUFFER_H
* @brief <����>   �������ݶ���
* @author <����>  m00410
* @date <����ʱ��> 2021��10��13��
* @note [��ע/ע������] 
**************************************************************************************************/
#include <string>
#include <iostream>
#include <queue>
#include "../CSeverMain/globalvars.h"
#pragma once

class CWriteBudder
{
private:
	std::queue<std::string> m_stdWritebuff;				//�����������

	CWriteBudder();

	~CWriteBudder();

	static CWriteBudder* m_WriteBuff;					//����������ж��������

	static int m_count;									//������								//
public:
	/**
	* @brief <��Ҫ˵��>  ��ȡдʱ������ж���
	* @details [��ϸ˵��] ����������ж��������
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��  ����������ж��������
	* @note [��ע/ע������]
	*/
	static CWriteBudder& getWriteBuff();
	/**
	* @brief <��Ҫ˵��>  �ж��Ƿ�Ϊ��
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��  NULL ���� TRUE  NOTNULL Ϊ FALSE 
	* @note [��ע/ע������]
	*/
	static bool  isemoty();
	/**
	* @brief <��Ҫ˵��> ��ӷ�������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout]  ��in��  ׼�����͵�����
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void setWriteBuff(const std::string& msg);
	/**
	* @brief <��Ҫ˵��>  ��ӷ�������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ��in��  ׼�����͵�����
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void setWriteBuff(const SNodeInfo3D & msg);
	/**
	* @brief <��Ҫ˵��>  ��ȡ��������
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵�� ���ط��͵���Ϣ
	* @note [��ע/ע������]
	*/
	const std::string getWriteBuffer();
	/**
	* @brief <��Ҫ˵��>  ���ݵ�����
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	static int& getCount()
	{
		return m_count;
	}
};
#endif
