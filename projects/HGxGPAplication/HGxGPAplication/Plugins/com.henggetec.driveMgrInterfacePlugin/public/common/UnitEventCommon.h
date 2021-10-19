/**************************************************************************************************
* @file UnitEventCommon.h
* @note 实体事件常量、结构体定义
* @author 
* @date 2021-1-29
**************************************************************************************************/
#ifndef FX_UNIT_EVENT_COMMON_H
#define FX_UNIT_EVENT_COMMON_H

#include <QColor>
#include <QString>
#include <QDate>
#include <QTime>

//装备信息展示事件信息
struct SFxRemoveUintEventConstants
{
	static std::string Topic() { return "freex/RemoveUint"; }

	static std::string RemoveUintInfo() { return "RemoveUintInfo"; }
};

struct SRemoveUintInfo
{
	std::string ID;              //装备名称
	SRemoveUintInfo()
	{
		ID = "";
	}
};
Q_DECLARE_METATYPE(SRemoveUintInfo)


//装备信息展示事件信息
struct SFxEquipmentInfoEventConstants
{
	static std::string Topic() { return "freex/EquipmentInfo/published"; }

	static std::string EquipmentInfo() { return "EquipmentInfo"; }
};

//单个装备能力
struct SEquipCap
{
	double      dHit;       //装备打击能力
	double      dCruise;    //装备巡航能力
	double      dPerformance;//装备性能
	double      dEndurance;  //装备续航能力
	double      dCoordinate; //装备协调能力
	double      dProbe;      //装备探测能力
};
Q_DECLARE_METATYPE(SEquipCap)
struct SEquipmentInfo
{
	std::string ID;              //装备名称
	std::string name;            //装备名称
	std::string strtype;         //装备类型
	std::string description;     //装备描述
	std::string model_path;      //装备三维模型路径
	std::string image_path;      //装备二维图片路径
	double      maxVoyage;       //装备最大航程
	double      maxSpeed;        //装备最大速度
	double      maxOil;          //装备最大油量
	int         ammunitionNumber;//装备携带弹药个数
	std::string combatCapability;//作战能力
	std::string degreeDamage;    //装备毁伤程度
	SEquipCap   sSEquipCap;      //装备能力
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

//甘特图事件信息
struct SFxGanttChartInfoEventConstants
{
	static std::string Topic() { return "freex/GanttChartInfo/published"; }

	static std::string GanttChartInfoEvent() { return "GanttChartInfoEvent"; }
};
typedef std::vector<std::pair<double,double>> VecPari;
struct SGanttChartInfo
{
	QString                   m_strID;//对象ID
	QString                   m_strName;//对象名称
	VecPari                   m_vecTimePari;//对象事件信息
	QString                   m_strType;//事件类型
	QColor                    m_colorEvent;//绘制颜色
	SGanttChartInfo()
	{

	}
};
Q_DECLARE_METATYPE(SGanttChartInfo)

//告警信息展示事件信息
struct SFxAlarmInfoEventConstants
{
	static std::string Topic() { return "event/AlarmInfo/published"; }

	static std::string AlarmInfo() { return "AlarmInfo"; }
};
/**
* 告警级别
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
//告警信息展示事件信息
struct SFxEquipInfoEventConstants
{
	static std::string Topic() { return "event/EquipInfo/published"; }

	static std::string EquipInfo() { return "EquipInfo"; }
};

#endif // FX_EFFECT_COMMON_H
