#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <iostream>
#pragma once

enum ORDER {
	CMD_UserLogin_L = 'L',          //��½
	CMD_getAircaftInformtion = 'A', //�ɻ���Ϣ
	CMD_DataCount = 'D',
};
/*
*@enum ����ϵ����
*/
enum EPosType
{
	E_LLH_COOR,   //��γ������
	E_WORLD_COOR   //��������
};

/*
*@enum ��ά�ڵ��������
*/
enum ENodeOptionType
{
	E_SHOW = 0,    //��ʾ
	E_HIDE = 1,    //����
};

/*
*@struct SPosition
*@brief ����ֵ
*@details ��������ϵ�����Լ���Ӧ������ֵ
*@author g00427
*/
struct SPosition
{
	EPosType type;  //��������
	double x;
	double y;
	double z;
};

/*
*@struct SNodeInfo3D
*@brief ��ά�ڵ���Ϣ
*@details �ڵ����֡����ꡢ�������͵�
*@author g00427
*/
struct SNodeInfo3D
{
	std::string name;            //����
	SPosition postion;           //����
	ENodeOptionType optionType;  //����
	double time;                 //ʱ��
};

#endif