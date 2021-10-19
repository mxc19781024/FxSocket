#ifndef NODE_INFO_3D_H
#define NODE_INFO_3D_H

#include <iostream>
#define LOCAL_ADDRESS "234.2.2.2"
#define LOCAL_POST 4412
#define REMOTE_ADDRESS "234.2.2.4"
#define REMOTE_POST 4414

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
	E_LLH_COOR ,   //��γ������
	E_WORLD_COOR   //��������
};

/*
*@enum ��ά�ڵ��������
*/
enum ENodeOptionType
{
	E_SHOW,    //��ʾ
	E_HIDE,    //����
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
