#pragma once
#include <QString>
#include <QDomElement>
// 
#include "SimStructDef.h"
#include "SimDataParse.h"

class FreeXSimSceneData
{
	
public:

	//实体载体
	std::map< int,std::vector<SimEntityObj> > &getEntityObjs()
	{
		return m_mapCampEntityObj;
	}

	// 事件信息数组
	QString &getEvenFilePath()
	{
		return m_strEvenFilePath;
	}
	//事件命令数组
	std::vector<EventCommand> &getEnventInfo()
	{
		return m_vecEnventInfo; 
	}

	// 击中事件
	QString &getHitFilePath()
	{
		return m_strHitFilePath;
	}
	//击中命令
	std::vector<HitCommand> &getHitCommand()
	{
		return m_vecHitCommand;
	}

	// 销毁命令
	QString &getDesFilePath()
	{
		return m_strDesFilePath;
	}
	//销毁
	std::vector<DestoryCommand> &getDestoryCommand()
	{
		return m_vecDestoryCommand;
	}

	// 字幕消息
	QString &getScrInfoFilePath()
	{
		return m_strScrInfoFilePath;
	}
	std::vector<ScreenWordInfo> & getScreenWordInfo()
	{
		return m_vecScreenWordInfo;
	}

	//// 视点信息
	QString &getViewFilePath()
	{
		return m_strViewFilePath;
	}
	std::vector<ViewInfoMsg> &getViewCommandInfo()
	{
		return m_vecViewCommandInfo;
	}

	//瞄准信息
	std::vector<AimInfoMsg> &getAimInfoMsg()
	{
		return m_vecAimInfo;
	}
	QString &getAimFilePath()
	{
		return m_strAimDataPath;
	}

	//想定信息？？？ 
	SimInfo &getSimInfo()
	{
		return m_simInfo;
	}

	//飞机空间信息路径
	QString &getAirSpaceFilePath()
	{
		return m_strAirSpacePath;
	}
	std::vector<AirSpeceSectorInfo> &getAirSpaceInfo()
	{
		return m_vecAirSpaceInfo;
	}
protected:

	// 红蓝双方实体数据
	std::map< int,std::vector<SimEntityObj> > m_mapCampEntityObj;

	// 事件信息数组
	QString m_strEvenFilePath;
	std::vector<EventCommand> m_vecEnventInfo; 

	// 击中事件
	QString m_strHitFilePath;
	std::vector<HitCommand> m_vecHitCommand;

	// 销毁命令
	QString m_strDesFilePath;
	std::vector<DestoryCommand> m_vecDestoryCommand;

	// 视点信息
	QString m_strViewFilePath;
	std::vector<ViewInfoMsg> m_vecViewCommandInfo;

	// 字幕消息
	QString m_strScrInfoFilePath;
	std::vector<ScreenWordInfo> m_vecScreenWordInfo;

	//瞄准信息
	QString m_strAimDataPath;
	std::vector<AimInfoMsg> m_vecAimInfo;

	//飞机空间信息路径
	QString m_strAirSpacePath;
	std::vector<AirSpeceSectorInfo> m_vecAirSpaceInfo;

	//场景信息
	SimInfo m_simInfo;
};

// 想定文件的解析
class FreeXSimSceneParse
{
public:
	FreeXSimSceneParse(FreeXSimSceneData &simData);
	~FreeXSimSceneParse(void);

	// 解析场景想点文件
	void ParseScneSim(QString strFilePath);

protected:
	
	void ParseFreeXSim(const QDomElement &ele);

	// 解析仿真信息
	void ParseSimInfoEle(const QDomElement &ele);

	// 解析场景对象
	void ParseSceneObjEle(const QDomElement &ele);

	// 解析实体对象
	void ParseObjEle(const QDomElement &ele);
	//
	void ParseSubInfoEle( const QDomElement &ele, SimEntityObj &simEntity );
	// 解析通信状态时段
	void ParseObjCommusEle(const QDomElement &ele, SimEntityObj &simEntity);

	// 解析传感器信息
	void ParseSenorsEle(const QDomElement &ele, SimEntityObj &simEntity);

	// 解析传感器数据
	void ParseSenorEle(const QDomElement &ele, Senor &senor, SimEntityObj &simEntity);

	// 解析雷达波束
	void ParseRadarBeamEle(const QDomElement &ele, RadarBeamInfo &radarBeam);

	// 解析雷达关闭状态
	void ParseSenorStateEle(const QDomElement &ele, std::vector<TimeSegState> &vecStates);

	// 解析瞄准数据
	void ParseAimDataEle(const QDomElement &ele);
	// 解析事件数据
	void ParseEventEle(const QDomElement &ele);

	// 解析视点数据
	void ParseViewEle(const QDomElement &ele);

	// 解析字幕数据
	void ParseScreenWordEle(const QDomElement &ele);

	// 解析击中命令
	void ParseHitEle(const QDomElement &ele);

	// 解析销毁命令
	void ParseDestryEle(const QDomElement &ele);

	//跟踪探测
	void ParseDetectEle(const QDomElement &ele,SimEntityObj &simEntity);

	//飞机空间飞行
	void ParseAirSpaceEle(const QDomElement &ele);

protected:

	//数据模拟解析
	SimDataParse m_simDataParse;
	//解析数据
	FreeXSimSceneData & m_freexSimData;
};

