/**************************************************************************************************
* @file ModelStructDef.h
* @note ʵ�����Գ������ṹ�嶨��
* @author w00085
* @date 2021-6-26
**************************************************************************************************/
#ifndef FX_MODEL_STRUCT_DEF_H
#define FX_MODEL_STRUCT_DEF_H

#include <QMap>

//����������
enum ESenor
{

};

//ʵ��װ�����ش���������
typedef std::vector<ESenor> VectorSenor;

//����װ����ս�������������ԣ�
struct SPreEquipCombatCap
{
	double      dHit;        //װ���������
	double      dCruise;     //װ��Ѳ������
	double      dPerformance;//װ������
	double      dEndurance;  //װ����������
	double      dCoordinate; //װ��Э������
	double      dProbe;      //װ��̽������
};

//����װ���������������ԣ�
struct SPreEquipCap
{
	double      maxVoyage;       //װ����󺽳�
	double      maxSpeed;        //װ������ٶ�
	double      maxOil;          //װ���������
	int         ammunitionNumber;//װ��Я����ҩ����
	std::string combatCapability;//��ս����
	std::string degreeDamage;    //װ�����˳̶�
};

//����װ������������Ϣ
struct SPreEquipAttachInfo
{
	SPreEquipCap       sEquipCap;     //��������
	SPreEquipCombatCap sCombatCap;    //��ս����
	VectorSenor        vecESenor;     //����������
};

//����װ������������Ϣ
struct SPreEquipBsaeInfo
{
	std::string name;            //װ������
	std::string strtype;         //װ������
	std::string description;     //װ������
	std::string model_path;      //װ����άģ��·��
	std::string image_path;      //װ����άͼƬ·��
};

//����װ��������Ϣ
struct SModelAttribute
{
	SPreEquipBsaeInfo sSPreEquipBsaeInfo;      //װ������������Ϣ
	SPreEquipAttachInfo sSPreEquipAttachInfo;  //װ������������Ϣ
};


//ʵ������������ݼ���
typedef QMap<std::string, SModelAttribute> MapModelAttributeData;  //key:ģ�����ƣ�value��װ��������Ϣ

#endif // FX_MODEL_STRUCT_DEF_H
