#pragma once
#include<QString>
#include <vector>
#include <map>
#include <QDomDocument>
#include <QDomElement>
/************************************************************************/
/*          �ɻ�״̬��Ϣ����                                             */
/************************************************************************/

//������Ϣ
struct stObjInfo
{

	stObjInfo()
	{
		//����
		strType = "";
		//mingzi
		strName = "";
		//��Ӫ
		strCoalition = "";
		//����
		strContry = "";
		//����
		strParentId = "";
		//id
		strId = "";
	}

	// ����
	QString strType;

	// ����
	QString strName;

	// ��Ӫ
	QString strCoalition;

	// ����
	QString strContry;

	// ����id
	QString strParentId;

	//id
	QString strId;

	//��ʻ��
	QString strPilot;


};

struct strObjReal
{
	/// ������Ϣ
	stObjInfo objInfo;

	/// ���Ӷ���
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

	//����ʱ��
	QString strMissionTime;

	//�ڼ�
	double dDuration;

	//���ɻ�id
	QString strMainAircraftID;
};

class FlyEventParse
{
public:
	FlyEventParse(void);
	~FlyEventParse(void);

	/// �������м�¼
	void parseFlyRecord(QString strFile);

	/// ������м�¼
	void saveFlyRecord(QString strFile);

	/// ����ʵ���ϵ
	void saveObjRealtion(QString strFile);

protected:

	//��������
	void parseMission(const QDomElement &ele);
	
	// 
	void parseEvent(const QDomElement &ele);

	// 
	void parseEvents(const QDomElement &ele);

	//����������Ϣ
	void parseObInfe(const QDomElement &ele, stObjInfo &objInfo);

	// 
	QString orgStrId(QString strId);

	void orgRelationInfo();

protected:

	std::vector<stEnventObj> m_vecEventObj;
	/// ʵ��
	std::map<QString,stObjInfo> m_mapId2ObjInfo;

	/// ������Ӣ���ֵ�
	std::map<QString,QString> m_mapCommondDirect;


	Mission m_Mission;

	/// �췽��ϵ
	std::map<QString,strObjReal> m_mapRedRelation;

	/// ������ϵ
	std::map<QString,strObjReal> m_mapBlueRelation;
};

