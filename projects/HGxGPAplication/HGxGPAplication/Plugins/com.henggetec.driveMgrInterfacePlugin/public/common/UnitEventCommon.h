/**************************************************************************************************
* @file UnitEventCommon.h
* @note ʵ���¼��������ṹ�嶨��
* @author 
* @date 2021-1-29
**************************************************************************************************/
#ifndef FX_UNIT_EVENT_COMMON_H
#define FX_UNIT_EVENT_COMMON_H

#include <QColor>
#include <QString>
#include <QDate>
#include <QTime>

//װ����Ϣչʾ�¼���Ϣ
struct SFxRemoveUintEventConstants
{
	static std::string Topic() { return "freex/RemoveUint"; }

	static std::string RemoveUintInfo() { return "RemoveUintInfo"; }
};

struct SRemoveUintInfo
{
	std::string ID;              //װ������
	SRemoveUintInfo()
	{
		ID = "";
	}
};
Q_DECLARE_METATYPE(SRemoveUintInfo)


//װ����Ϣչʾ�¼���Ϣ
struct SFxEquipmentInfoEventConstants
{
	static std::string Topic() { return "freex/EquipmentInfo/published"; }

	static std::string EquipmentInfo() { return "EquipmentInfo"; }
};

//����װ������
struct SEquipCap
{
	double      dHit;       //װ���������
	double      dCruise;    //װ��Ѳ������
	double      dPerformance;//װ������
	double      dEndurance;  //װ����������
	double      dCoordinate; //װ��Э������
	double      dProbe;      //װ��̽������
};
Q_DECLARE_METATYPE(SEquipCap)
struct SEquipmentInfo
{
	std::string ID;              //װ������
	std::string name;            //װ������
	std::string strtype;         //װ������
	std::string description;     //װ������
	std::string model_path;      //װ����άģ��·��
	std::string image_path;      //װ����άͼƬ·��
	double      maxVoyage;       //װ����󺽳�
	double      maxSpeed;        //װ������ٶ�
	double      maxOil;          //װ���������
	int         ammunitionNumber;//װ��Я����ҩ����
	std::string combatCapability;//��ս����
	std::string degreeDamage;    //װ�����˳̶�
	SEquipCap   sSEquipCap;      //װ������
	SEquipmentInfo()
	{
		name = "";
		description = "";
		model_path = "";
		degreeDamage = "";
		combatCapability = "";
		maxVoyage = 0;
		maxSpeed = 0; 
		maxOil = 0;   
		ammunitionNumber = 0;
	}
};
Q_DECLARE_METATYPE(SEquipmentInfo)

//����ͼ�¼���Ϣ
struct SFxGanttChartInfoEventConstants
{
	static std::string Topic() { return "freex/GanttChartInfo/published"; }

	static std::string GanttChartInfoEvent() { return "GanttChartInfoEvent"; }
};
typedef std::vector<std::pair<double,double>> VecPari;
struct SGanttChartInfo
{
	QString                   m_strID;//����ID
	QString                   m_strName;//��������
	VecPari                   m_vecTimePari;//�����¼���Ϣ
	QString                   m_strType;//�¼�����
	QColor                    m_colorEvent;//������ɫ
	SGanttChartInfo()
	{

	}
};
Q_DECLARE_METATYPE(SGanttChartInfo)

//�澯��Ϣչʾ�¼���Ϣ
struct SFxAlarmInfoEventConstants
{
	static std::string Topic() { return "event/AlarmInfo/published"; }

	static std::string AlarmInfo() { return "AlarmInfo"; }
};
/**
* �澯����
*/
enum ELevel {
	LEVEL_ORDINARY,
	LEVEL_IMPORTANCE
};

struct SAlarmInfo
{
	ELevel level;
	QDate	data;
	QTime   time;
	QString info;
	SAlarmInfo()
	{
		level = ELevel::LEVEL_ORDINARY;
		data = QDate::currentDate();
		time = QTime::currentTime();
		info = "";
	}
};
Q_DECLARE_METATYPE(SAlarmInfo)

struct SEquipInfo
{
	std::string strIndex;
	std::string strID;
	std::string strType;
	std::string strIconPath;
	double		dEnergyNum;
	double		dLoadNum;
	double		dParaA;
	double		dParaB;
	double		dParaC;
	SEquipInfo()
	{
		strIndex = "";
		strID = "";
		strType = "";
		strIconPath = "";
		dEnergyNum = 0;
		dLoadNum = 0;
		dParaA = 0;
		dParaB = 0;
		dParaC = 0;
	}
};
Q_DECLARE_METATYPE(SEquipInfo)
//�澯��Ϣչʾ�¼���Ϣ
struct SFxEquipInfoEventConstants
{
	static std::string Topic() { return "event/EquipInfo/published"; }

	static std::string EquipInfo() { return "EquipInfo"; }
};

#endif // FX_EFFECT_COMMON_H
