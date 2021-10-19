#include "FlyEventParse.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

const char *STR_QF = "起飞";
const char *STR_FS = "发射";
const char *STR_MZ = "命中";
const char *STR_CH = "摧毁";
const char *STR_JL = "降落";

FlyEventParse::FlyEventParse(void)
{
	m_mapCommondDirect["HasTakenOff"] = STR_QF;
	m_mapCommondDirect["HasFired"] = STR_FS;
	m_mapCommondDirect["HasBeenHitBy"] = STR_MZ;
	m_mapCommondDirect["HasBeenDestroyed"] = STR_CH;
	m_mapCommondDirect["HasLanded"] = STR_JL;
}

FlyEventParse::~FlyEventParse(void)
{

}

void FlyEventParse::parseFlyRecord( QString strFile )
{
	QDomDocument doc("mydocument");
	QFile file(strFile);
	if (!file.open(QIODevice::ReadOnly))
		return;
	if (!doc.setContent(&file)) {
		file.close();
		return;
	}
	file.close();

	// print out the element names of all elements that are direct children
	// of the outermost element.
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Mission")
			{
				parseMission(e);
			}
			else if(e.tagName() == "Events")
			{
				parseEvents(e);
			}
		}
		n = n.nextSibling();
	}
}

void FlyEventParse::saveFlyRecord( QString strFile )
{
	QFile file(strFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	for (int iIndex = 0; iIndex < m_vecEventObj.size(); ++iIndex)
	{
		const stEnventObj &stEventInfo = m_vecEventObj[iIndex];
		QString strText;
		strText = QString::number(stEventInfo.dTime) + " ";
		
		if (m_mapCommondDirect[stEventInfo.strComond] == STR_CH || m_mapCommondDirect[stEventInfo.strComond] == STR_MZ)
		{
			
			strText += stEventInfo.priObject.strName.split(" ").at(0) + "("+ stEventInfo.priObject.strId +")" +"被";
		}
		else
		{
			strText += stEventInfo.priObject.strName.split(" ").at(0) + "("+ stEventInfo.priObject.strId +")"+ m_mapCommondDirect[stEventInfo.strComond];
		}

		if (stEventInfo.secondObj.strId != "")
		{
			if(stEventInfo.secondObj.strParentId != "")
			{
				if (stEventInfo.secParentObj.strId != " ")
				{
					strText += stEventInfo.secondObj.strName.split(" ").at(0) + "("+ stEventInfo.secondObj.strId +")"; 
				}
				else
				{
					if (m_mapId2ObjInfo.count(stEventInfo.secondObj.strParentId))
					{
						strText += m_mapId2ObjInfo[stEventInfo.secondObj.strParentId].strName.split(" ").at(0) + "("+ m_mapId2ObjInfo[stEventInfo.secondObj.strParentId].strId +")"; 
					}
				}
			}
			else
			{
				strText += stEventInfo.secondObj.strName.split(" ").at(0) + "("+ stEventInfo.secondObj.strId +")";
			}

			if (m_mapCommondDirect[stEventInfo.strComond] == STR_CH || m_mapCommondDirect[stEventInfo.strComond] == STR_MZ)
			{
				strText += m_mapCommondDirect[stEventInfo.strComond];
			}
		}
		else
		{
			if (m_mapCommondDirect[stEventInfo.strComond] == STR_CH || m_mapCommondDirect[stEventInfo.strComond] == STR_MZ)
			{
				strText += m_mapCommondDirect[stEventInfo.strComond];
			}
		}

		strText += "\n";
		out << strText;
	}
}

void FlyEventParse::saveObjRealtion( QString strFile )
{
	
	orgRelationInfo();

	QFile file(strFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	/// 
	out << "Red Begin" << "\n";
	for (std::map<QString,strObjReal>::iterator itr = m_mapRedRelation.begin();
		itr != m_mapRedRelation.end(); ++itr)
	{
		/// 
		out << "Obj Begin" << "\n";
		const strObjReal & objReal = itr->second;
		out << objReal.objInfo.strId << " " << objReal.objInfo.strName << "\n";

		out << "Child Begin" << "\n";
		for (int j = 0; j < objReal.vecChild.size(); ++j)
		{
			out << objReal.vecChild[j].strId << " " << objReal.vecChild[j].strName << "\n";
		}
		out << "Child End" << "\n";
		out << "Obj End" << "\n";
	}
	out << "Red End" << "\n";


	out << "Blue Begin" << "\n";
	for (std::map<QString,strObjReal>::iterator itr = m_mapBlueRelation.begin();
		itr != m_mapBlueRelation.end(); ++itr)
	{
		/// 
		out << "Obj Begin" << "\n";
		const strObjReal & objReal = itr->second;
		out << objReal.objInfo.strId << " " << objReal.objInfo.strName << "\n";

		out << "Child Begin" << "\n";
		for (int j = 0; j < objReal.vecChild.size(); ++j)
		{
			out << objReal.vecChild[j].strId << " " << objReal.vecChild[j].strName << "\n";
		}
		out << "Child End" << "\n";
		out << "Obj End";
	}
	out << "Blue End" << "\n";
}

void FlyEventParse::parseMission( const QDomElement &ele )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Title")
			{
				m_Mission.strTitle = e.text();
			}
			else if (e.tagName() == "MissionTime")
			{
				m_Mission.strMissionTime = e.text();
			}
			else if (e.tagName() == "Duration")
			{
				m_Mission.dDuration = e.text().toDouble();
			}
			else if (e.tagName() == "MainAircraftID")
			{
				m_Mission.strMainAircraftID = e.text();
			}
		}
		n = n.nextSibling();
	}
}

void FlyEventParse::parseEvent( const QDomElement &ele )
{
	stEnventObj stEvent;
	QDomNode n = ele.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Time")
			{
				stEvent.dTime = e.text().toDouble();
			}
			else if (e.tagName() == "PrimaryObject")
			{
				parseObInfe(e,stEvent.priObject);
			}
			else if (e.tagName() == "Action")
			{
				stEvent.strComond = e.text();
			}
			else if (e.tagName() == "SecondaryObject")
			{
				parseObInfe(e,stEvent.secondObj);
			}
			else if(e.tagName() == "ParentObject")
			{
				parseObInfe(e,stEvent.secParentObj);
			}
		}
		n = n.nextSibling();
	}
	m_vecEventObj.push_back(stEvent);
}

void FlyEventParse::parseEvents( const QDomElement &ele )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Event")
			{
				parseEvent(e);
			}
		}
		n = n.nextSibling();
	}
}

void FlyEventParse::parseObInfe( const QDomElement &ele, stObjInfo &objInfo )
{
	/// 
	objInfo.strId = QString::number(ele.attribute("ID").toInt() - m_Mission.strMainAircraftID.toInt() + 1) ;
	QDomNode n = ele.firstChild();
	while(!n.isNull()) {
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Type")
			{
				objInfo.strType = e.text();
			}
			else if (e.tagName() == "Name")
			{
				objInfo.strName = e.text();
			}
			else if (e.tagName() == "Pilot2")
			{
				objInfo.strPilot = e.text();
			}
			else if (e.tagName() == "Coalition")
			{
				objInfo.strCoalition = e.text();
			}
			else if (e.tagName() == "Country")
			{
				objInfo.strContry = e.text();
			}
			else if (e.tagName() == "Parent")
			{
				objInfo.strParentId = QString::number(e.text().toInt() - m_Mission.strMainAircraftID.toInt() + 1);
			}
		}
		n = n.nextSibling();
	}

	if (!m_mapId2ObjInfo.count(objInfo.strId))
	{
		m_mapId2ObjInfo[objInfo.strId] = objInfo;
	}
}

QString FlyEventParse::orgStrId( QString strId )
{
	int nMainNum = m_Mission.strMainAircraftID.toInt();
	int nCurNum = strId.toInt();

	return "";
}

void FlyEventParse::orgRelationInfo()
{
	for (std::map<QString,stObjInfo>::iterator itr = m_mapId2ObjInfo.begin(); 
		itr != m_mapId2ObjInfo.end(); ++itr)
	{
		/// 
		stObjInfo & obj = itr->second;

		if (obj.strParentId.isEmpty())
		{
			/// 
			if (obj.strCoalition.trimmed() == "Allies" )
			{
				// 红方
				if (!m_mapRedRelation.count(obj.strId))
				{
					m_mapRedRelation[obj.strId].objInfo = obj;
				}
			}
			else if (obj.strCoalition.trimmed() == "Enemies")
			{
				// 蓝方
				if (!m_mapBlueRelation.count(obj.strId))
				{
					m_mapBlueRelation[obj.strId].objInfo = obj;
				}
			}
		}
		else
		{
			///
			if (obj.strCoalition.trimmed() == "Allies" )
			{
				// 红方
				if (m_mapRedRelation.count(obj.strParentId))
				{
					m_mapRedRelation[obj.strParentId].vecChild.push_back(obj);
				}
				else
				{
					if (m_mapId2ObjInfo.count(obj.strParentId))
					{
						m_mapRedRelation[obj.strParentId].objInfo = m_mapId2ObjInfo[obj.strParentId];
						m_mapRedRelation[obj.strParentId].vecChild.push_back(obj);
					}
				}
			}
			else if (obj.strCoalition.trimmed() == "Enemies")
			{
				// 蓝方
				if (m_mapBlueRelation.count(obj.strParentId))
				{
					m_mapBlueRelation[obj.strParentId].vecChild.push_back(obj);
				}
				else
				{
					if (m_mapId2ObjInfo.count(obj.strParentId))
					{
						m_mapBlueRelation[obj.strParentId].objInfo = m_mapId2ObjInfo[obj.strParentId];
						m_mapBlueRelation[obj.strParentId].vecChild.push_back(obj);
					}
				}
			}
		}

		

	}
}
