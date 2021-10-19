#pragma once
#include<QString>
#include <vector>
#include <map>
#include <QDomDocument>
#include <QDomElement>
/************************************************************************/
/*          飞机状态信息解析                                             */
/************************************************************************/

//基础信息
struct stObjInfo
{

	stObjInfo()
	{
		//类型
		strType = "";
		//mingzi
		strName = "";
		//阵营
		strCoalition = "";
		//国家
		strContry = "";
		//父亲
		strParentId = "";
		//id
		strId = "";
	}

	// 类型
	QString strType;

	// 名称
	QString strName;

	// 阵营
	QString strCoalition;

	// 国家
	QString strContry;

	// 父亲id
	QString strParentId;

	//id
	QString strId;

	//驾驶者
	QString strPilot;


};

struct strObjReal
{
	/// 对象信息
	stObjInfo objInfo;

	/// 孩子对象
	std::vector<stObjInfo> vecChild;
};

//???????????/
struct stEnventObj
{
	stEnventObj()
	{
		dTime = 0;
		strComond = "";
	}

	double dTime;

	stObjInfo priObject;

	stObjInfo secondObj;

	stObjInfo secParentObj;

	QString strComond;
};

//ren wu 
struct Mission
{
	//tiaoti
	QString strTitle;

	//任务时间
	QString strMissionTime;

	//期间
	double dDuration;

	//主飞机id
	QString strMainAircraftID;
};

class FlyEventParse
{
public:
	FlyEventParse(void);
	~FlyEventParse(void);

	/// 解析飞行记录
	void parseFlyRecord(QString strFile);

	/// 保存飞行记录
	void saveFlyRecord(QString strFile);

	/// 保存实体关系
	void saveObjRealtion(QString strFile);

protected:

	//解析任务
	void parseMission(const QDomElement &ele);
	
	// 
	void parseEvent(const QDomElement &ele);

	// 
	void parseEvents(const QDomElement &ele);

	//解析基本信息
	void parseObInfe(const QDomElement &ele, stObjInfo &objInfo);

	// 
	QString orgStrId(QString strId);

	void orgRelationInfo();

protected:

	std::vector<stEnventObj> m_vecEventObj;
	/// 实体
	std::map<QString,stObjInfo> m_mapId2ObjInfo;

	/// 命令中英文字典
	std::map<QString,QString> m_mapCommondDirect;


	Mission m_Mission;

	/// 红方关系
	std::map<QString,strObjReal> m_mapRedRelation;

	/// 蓝方关系
	std::map<QString,strObjReal> m_mapBlueRelation;
};

