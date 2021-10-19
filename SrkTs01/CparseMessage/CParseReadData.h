#ifndef C_PARSE_READ_DATA_H
#define C_PARSE_READ_DATA_H

/**************************************************************************************************
* @file <CParseMessage>
* @brief <���ݽ���>
* @author <m00410>
* @date <2021��10��13��>
* @note [Ŀǰֻ��.xml���ݵķ���]
**************************************************************************************************/

#pragma once
#include "../CReadbuff/CReadBuff.h"
#include "../CSeverMain/globalvars.h"
#include <string>
#include <iostream>
#include <windows.h>
#include <tinyxml.h>


/**
* @class <CParseReadData>
* @brief <���ݽ���>
* @details [ֻ����ӽ�������ʱ���Ż�ʹ�ñ��ļ���ӽ�������]
* @author <m00410>
* @note [Ŀǰֻ��.xml���ݵķ���]
*/
class CParseReadData
{
public:
	/**
	* @brief <�ȴ������������յ����Ȼ����������>
	* @details [�����������е�����]
	* @param ����1 [in/out/inout] NULL
	* @param ����2 [in/out/inout] NULL
	* @return �ɹ�����TRUE
	* @note [�ں��ڿ���ʹ�ö��߳̽����Ż�]
	*/
	bool parseData();

	/**
	* @brief <��ȡ��������Ĵ�����>
	* @details [���õ���ģʽ��ʹȫ�ֿ���ʹ��]
	* @param ����1 [in/out/inout] NULL
	* @param ����2 [in/out/inout] NULL
	* @return ���ؽ�������
	* @note [��ע/ע������]
	*/
	static CParseReadData& getInstance();
protected:
	/**
	* @brief <����ȡ����.xml�ļ����н���>
	* @details [��Ӧ����ֵ���ж�ȡ]
	* @param ����1 [in] Ԫ�ؽڵ�
	* @param ����2 [in/out/inout] 
	* @return ����ֵ�Լ�˵�� 
	* @note [�����ݽṹ������޸�ʱ����Ҫ�Խ����ļ������޸�]
	*/
	void xmlParse(TiXmlElement *pEle);
	/**
	* @brief <��ȡ.xml�ļ�������ת��Ϊ�ĵ�>
	* @details [����C++Դ��ʵ��]
	* @param ����1 [in/out/inout] in   ·��
	* @param ����2 [in/out/inout] NULL
	* @return  ����ֵ�Լ�˵��
	* @note [����ֲ����Ҫͬʱ��ֲC++.xmlԴ��]
	*/
	int readXmlFile(std::string filepPath);
	/**
	* @brief <��������>
	* @details [���յ������ݽ��в�֣���Ӧ��Ӧ��������д���]
	* @param ����1 [in/out/inout] ��in�� ������һ������
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [�ں�����Ҫ����������ʱ����Ҫ��Ӷ�Ӧ�Ľ�������]
	*/
	void parseCommand(std::string & msg);
	/**
	* @brief <��������>
	* @details [�ڸտ�ʼ�õ�.xml����ʱ����Ҫ�ȷ������ݵĴ�С]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseDataLogin();
	/**
	* @brief <������һ��һ���ķ���>
	* @details [��� parseDataLogin���� ������һ��һ������]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseDataAircaInfomtion();
	/**
	* @brief <������Ч������>
	* @details [��ϸ˵��]
	* @param ����1 [in/out/inout] ����1˵��
	* @param ����2 [in/out/inout] ����2˵��
	* @return ����ֵ�Լ�˵��
	* @note [��ע/ע������]
	*/
	void parseDataInvalidCommand();
private:
	CParseReadData();

	~CParseReadData();

	std::vector<SNodeInfo3D> m_vNodeGather;			//����.xml���ݵ�����
};

#endif


