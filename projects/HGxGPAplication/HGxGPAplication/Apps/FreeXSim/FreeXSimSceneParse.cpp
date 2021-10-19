#include "FreeXSimSceneParse.h"
#include <QDomDocument>
#include <QFile>
#include <QDateTime>
#include <QDebug>

#include "ConfigMgr.h"

//SimInfo
const char * XML_NODE_SIMINFO_NAME = "SimInfo";
//SceneObj
const char * XML_NODE_SCENE_OBJNAME = "SceneObj";
//EnventInfo
const char * XML_NODE_ENVENT_NAME = "EnventInfo";
//ViewInfo
const char * XML_NODE_VIEW_NAME = "ViewInfo";
//HitData
const char * XML_NODE_HIT_NAME = "HitData";
//DestryData
const char * XML_NODE_DESTRY_NAME = "DestryData";
//ScreenWord
const char * XML_NODE_SCREENWORD_NAME = "ScreenWord";
//DetectData
const char * XML_NODE_DETECT_NAME = "DetectData";
//AimData
const char * XML_NODE_AIM_NAME = "AimData";
//
const char *XML_NODE_AIRSPACE_SECOTR="AirSpaceData";


FreeXSimSceneParse::FreeXSimSceneParse(FreeXSimSceneData &simData)
	:m_freexSimData(simData)
{

}


FreeXSimSceneParse::~FreeXSimSceneParse(void)
{

}
//打开场景相应文件0
void FreeXSimSceneParse::ParseScneSim( QString strFilePath)
 {
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly))
		return;

	QString errorStr;
	int errorLine;
	int errorColumn;

	QDomDocument doc;
	if (!doc.setContent(&file,true,&errorStr,&errorLine,&errorColumn))
	{
		file.close();
		return;
	}
	file.close();

	ParseFreeXSim(doc.documentElement());

}

void FreeXSimSceneParse::ParseFreeXSim( const QDomElement &ele )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			// 解析仿真信息
			if (e.tagName() == XML_NODE_SIMINFO_NAME)
			{
				ParseSimInfoEle(e);
			}//解析场景对象
			else if (e.tagName() == XML_NODE_SCENE_OBJNAME )
			{
				ParseSceneObjEle(e);
			}//解析视点数据
			else if (e.tagName() == XML_NODE_VIEW_NAME)
			{
				ParseViewEle(e);
			}//解析击中命令
			else if (e.tagName() == XML_NODE_HIT_NAME)
			{
				ParseHitEle(e);
			}//解析销毁命令
			else if (e.tagName() == XML_NODE_DESTRY_NAME)
			{
				ParseDestryEle(e);
			}//解析字幕数据
			else if (e.tagName() == XML_NODE_SCREENWORD_NAME)
			{
				ParseScreenWordEle(e);
			}//解析事件数据
			else if (e.tagName() == XML_NODE_ENVENT_NAME)
			{
				ParseEventEle(e);
			}//解析瞄准数据
			else if (e.tagName() == XML_NODE_AIM_NAME)
			{
				ParseAimDataEle(e);
			}//解析飞机飞行数据
			else if (e.tagName() == XML_NODE_AIRSPACE_SECOTR)
			{
				ParseAirSpaceEle(e);
			}
			
		}
		n = n.nextSibling();
	}
}

void FreeXSimSceneParse::ParseSimInfoEle( const QDomElement &ele )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			// 
			if (e.tagName() == "SceneName")
			{
				//
				std::string strName = e.attribute("name").toStdString();
				memcpy(m_freexSimData.getSimInfo().chBattleName,strName.c_str(),strName.size());
			}  
			else if (e.tagName() == "SimTime" )
			{
				// 1970-01-01T00:00:00.000 
				QString strText = e.attribute("referTime");
				QDateTime dateTime = QDateTime::fromString(strText,"yyyy/MM/dd hh:mm:ss");
				m_freexSimData.getSimInfo().dPreferTime = dateTime.toMSecsSinceEpoch() / 1000.0;
				m_freexSimData.getSimInfo().dTimeLen = e.attribute("timeLen").toDouble();
			}
		
		}
		n = n.nextSibling();
	}

}

void FreeXSimSceneParse::ParseSceneObjEle( const QDomElement &ele )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			// 
			if (e.tagName() == "ObjFile")
			{
				//
				QString strFile = e.attribute("filePath");
				strFile = FilePathMgr::getSimDataSetDir() + strFile;
				/// 
				ParseScneSim(strFile);
			}
			else if (e.tagName() == "Obj" )
			{
				ParseObjEle(e);
			}
		}
		n = n.nextSibling();
	}
}

void FreeXSimSceneParse::ParseObjEle( const QDomElement &ele )
{
	// 属性
	SimEntityObj simEntity;
	simEntity.entiyInfo.parentId = ele.attribute("parentId").toInt();
	simEntity.entiyInfo.nEntityID = ele.attribute("id").toInt();
	simEntity.entiyInfo.enCamp = (EnCamp)ele.attribute("camp").toInt();
	simEntity.entiyInfo.enEquType = (EnEquType)ele.attribute("equType").toInt();
	simEntity.entiyInfo.enSpaceType = (EnAirspace)ele.attribute("spaceType").toInt();
	simEntity.entiyInfo.enCommuState = (EnConeState)ele.attribute("commuState").toUInt();
	QString strName = ele.attribute("name");
	memcpy(simEntity.entiyInfo.cName, strName.toStdString().c_str(), strName.toStdString().size());

	if (ele.hasAttribute("comPridId"))
	{
		// 组合主实体id
		simEntity.entiyInfo.nComPriId = ele.attribute("comPridId").toInt();
	}

	if (ele.hasAttribute("sepLevel"))
	{
		simEntity.entiyInfo.sepLevel = ele.attribute("sepLevel").toInt();
	}

	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			// 
			if (e.tagName() == "Position")
			{
				//
				simEntity.entiyInfo.dLon = e.attribute("lon").toDouble();
				simEntity.entiyInfo.dLat = e.attribute("lat").toDouble();
				simEntity.entiyInfo.dAlt = e.attribute("alt").toDouble();
			}
			else if (e.tagName() == "Posture" )
			{
				simEntity.entiyInfo.dAzAngle = e.attribute("az").toDouble();
				simEntity.entiyInfo.dPicthAngle = e.attribute("pitch").toDouble();
				simEntity.entiyInfo.dRolAngle = e.attribute("roll").toDouble();
			}
			else if (e.tagName() == "ExsitTime")
			{
				simEntity.dExsitStartTime = e.attribute("startTime").toDouble();
				simEntity.dExsitEndTime = e.attribute("endTime").toDouble();
			}
			else if (e.tagName() == "Commus")
			{
				ParseObjCommusEle(e,simEntity);
			}
			else if (e.tagName() == "TrackData")
			{
				simEntity.strTrackFilePath = e.attribute("filePath");

				simEntity.strTrackFilePath = FilePathMgr::getTrackDataDir() + simEntity.strTrackFilePath;
				// 解析航迹数据
				m_simDataParse.ParseTracLineData(simEntity.strTrackFilePath,simEntity.vecTrackData);
			}
			else if (e.tagName() == "Senors")
			{
				ParseSenorsEle(e,simEntity);
			}
			else if (e.tagName() == "SubInfos")
			{
				// 解析组件信息
				ParseSubInfoEle(e,simEntity);
			}
			else if(e.tagName() == "OffSet")
			{
				// 组件
				simEntity.entiyInfo.dOffsetX = e.attribute("px").toDouble();
				simEntity.entiyInfo.dOffsetX = e.attribute("py").toDouble();
				simEntity.entiyInfo.dOffsetX = e.attribute("pz").toDouble();
				// 旋转角度
				simEntity.entiyInfo.dRoteX = e.attribute("rx").toDouble();
				simEntity.entiyInfo.dRoteY = e.attribute("ry").toDouble();
				simEntity.entiyInfo.dRoteZ = e.attribute("rz").toDouble();
			}
		}
		n = n.nextSibling();
	}

	m_freexSimData.getEntityObjs()[simEntity.entiyInfo.enCamp].push_back(simEntity);
}

void FreeXSimSceneParse::ParseSubInfoEle( const QDomElement &ele, SimEntityObj &simEntity )
{
	if (ele.hasAttribute("subNum"))
	{
		simEntity.entiyInfo.sepNum = ele.attribute("subNum").toInt();
	}

	simEntity.vecSepSubInfo.clear();
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			// 
			if (e.tagName() == "Sub")
			{
				SepareSubInfo subInfo;
				
				subInfo.nSubId = e.attribute("subId").toInt();

				subInfo.chLevel = e.attribute("level").toInt();

				subInfo.dTime = e.attribute("sepTime").toDouble();

				simEntity.vecSepSubInfo.push_back(subInfo);
			}
		}
		n = n.nextSibling();
	}
	
	int nSize = simEntity.vecSepSubInfo.size() > MAX_SEPSUB_NUM ? MAX_SEPSUB_NUM : simEntity.vecSepSubInfo.size();

	for (int i = 0; i < nSize; ++i)
	{
		simEntity.entiyInfo.sepInfo[i] = simEntity.vecSepSubInfo[i];
	}
}

void FreeXSimSceneParse::ParseObjCommusEle( const QDomElement &ele, SimEntityObj &simEntity )
{
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			if (e.tagName() == "TimeSeg")
			{
				TimeSegState timeSeg;

				timeSeg.dStartTime = e.attribute("startTime").toDouble();
				timeSeg.dEndTime = e.attribute("endTime").toDouble();
				timeSeg.nState = e.attribute("state").toInt();
				simEntity.vecCommusState.push_back(timeSeg);
			}
		}

		n = n.nextSibling();
	}
}

void FreeXSimSceneParse::ParseSenorsEle( const QDomElement &ele ,SimEntityObj &simEntity)
{
	simEntity.entiyInfo.nSenorNum = ele.attribute("num").toInt();
	QDomNode n = ele.firstChild();

	std::vector<Senor> vecSenor;

	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Senor")
			{
				Senor senor;
				ParseSenorEle(e,senor,simEntity);
				senor.nEntityID = simEntity.entiyInfo.nEntityID;
				vecSenor.push_back(senor);
			}
			else if (e.tagName() == "SenorData")
			{
				QString strFilePath = e.attribute("filePath");
				strFilePath = FilePathMgr::getSenorDataDir() + strFilePath;
				simEntity.vecSenorDataPath.push_back(strFilePath);
				m_simDataParse.ParseSenorData(strFilePath,simEntity.vecSenorData);
			}
			else if (e.tagName() == XML_NODE_DETECT_NAME)
			{	
				// 探测跟踪消息
				ParseDetectEle(e,simEntity);
			}
		}
		n = n.nextSibling();
	}

	/// 赋值给simEntity
	int iSize = vecSenor.size() > SENOR_MAX_NUM ? SENOR_MAX_NUM : vecSenor.size();
	for (int i =0; i < iSize ; ++i)
	{
		simEntity.entiyInfo.senor[i] = vecSenor[i];
	}

}

void FreeXSimSceneParse::ParseSenorEle( const QDomElement &ele ,Senor &senor ,SimEntityObj &simEntity )
{
	senor.nSeorId = ele.attribute("senorID").toInt();
	senor.nBeamNum = ele.attribute("beamNum").toInt();
	senor.dAzStartAngle = ele.attribute("azStartAngle").toDouble();
	senor.dAzEndAngle = ele.attribute("azEndAngle").toDouble();
	senor.dPicthStartAngle = ele.attribute("pitchStartAngle").toDouble();
	senor.dPicthEndAngle = ele.attribute("pitchEndAngle").toDouble();
	senor.dRaduis = ele.attribute("radius").toDouble();
	senor.nPrefer = ele.attribute("prefer").toInt();
	senor.enState = (EnMachState)ele.attribute("state").toInt();
	senor.dScanPeriod = ele.attribute("period").toDouble();

	QDomNode n = ele.firstChild();

	std::vector<RadarBeamInfo> vecBeam;
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.
		if(!e.isNull()) 
		{
			if (e.tagName() == "Beam")
			{
				RadarBeamInfo beam;
				ParseRadarBeamEle(e,beam);
				beam.nSenorId = senor.nSeorId;
				vecBeam.push_back(beam);
			}
			else if (e.tagName() == "States")
			{
				std::vector<TimeSegState> vecStates;
				ParseSenorStateEle(e,vecStates);

				if (vecStates.size() > 0)
				{
					simEntity.mapSenorsState[senor.nSeorId] = vecStates;
				}
			}
		}
		n = n.nextSibling();
	}

	int nSize = vecBeam.size() > BEAM_MAX_NUM ? BEAM_MAX_NUM : vecBeam.size();

	for (int nIndex = 0; nIndex < nSize; ++nIndex)
	{
		// 
		senor.beam[nIndex] = vecBeam[nIndex];
	}
}

void FreeXSimSceneParse::ParseRadarBeamEle( const QDomElement &ele, RadarBeamInfo &radarBeam )
{
	// 
	radarBeam.dAzAngle = ele.attribute("azAngle").toDouble();
	radarBeam.dHorWidth = ele.attribute("horWidth").toDouble();
	radarBeam.dPitchAngle = ele.attribute("pitchAngle").toDouble();
	radarBeam.dVerWidth = ele.attribute("verWidth").toDouble();
	radarBeam.nBeamId = ele.attribute("beamId").toInt();
	
}

void FreeXSimSceneParse::ParseSenorStateEle( const QDomElement &ele, std::vector<TimeSegState> &vecStates )
{
	//
	QDomNode n = ele.firstChild();
	while(!n.isNull()) 
	{
		QDomElement e = n.toElement(); // try to convert the node to an element.

		if(!e.isNull()) 
		{
			if (e.tagName() == "TimeSeg")
			{
				TimeSegState timeSeg;

				timeSeg.dStartTime = e.attribute("startTime").toDouble();
				timeSeg.dEndTime = e.attribute("endTime").toDouble();
				timeSeg.nState = e.attribute("state").toInt();
				vecStates.push_back(timeSeg);
			}
		}

		n = n.nextSibling();
	}
}

void FreeXSimSceneParse::ParseAimDataEle(const QDomElement &ele)
{
	QString strEvenFile= ele.attribute("filePath");
	m_freexSimData.getAimFilePath() = FilePathMgr::getSimDataSetDir() + strEvenFile;
	m_simDataParse.ParseAimMsgData(m_freexSimData.getAimFilePath(),m_freexSimData.getAimInfoMsg());
}

void FreeXSimSceneParse::ParseEventEle( const QDomElement &ele)
{
	QString strEvenFile= ele.attribute("filePath");

	m_freexSimData.getEvenFilePath() = FilePathMgr::getSimDataSetDir() + strEvenFile;
	m_simDataParse.ParseEventData(m_freexSimData.getEvenFilePath(),m_freexSimData.getEnventInfo());
}

//解析视点数据
void FreeXSimSceneParse::ParseViewEle( const QDomElement &ele )
{
	QString strViewFilePath = ele.attribute("filePath");

	//数据模型         视点信息        =  文件路径：：仿真数据集目录       + 名称为filePath的属性       
	m_freexSimData.getViewFilePath() = FilePathMgr::getSimDataSetDir() + strViewFilePath;

	//解析视点数据
	m_simDataParse.ParseViewData(m_freexSimData.getViewFilePath() ,m_freexSimData.getViewCommandInfo());
}

//解析字幕数据
void FreeXSimSceneParse::ParseScreenWordEle( const QDomElement &ele )
{
	//返回名为名称的属性。如果属性不存在，则返回值。
	QString strScrInfoFilePath = ele.attribute("filePath");
	//字幕消息
	m_freexSimData.getScrInfoFilePath() = FilePathMgr::getSimDataSetDir() + strScrInfoFilePath;
	//解析字幕数据
	m_simDataParse.ParseScreenWord(m_freexSimData.getScrInfoFilePath(),m_freexSimData.getScreenWordInfo());
}

void FreeXSimSceneParse::ParseHitEle( const QDomElement &ele )
{
	QString strHitFilePath = ele.attribute("filePath");
	//击中事件
	m_freexSimData.getHitFilePath() = FilePathMgr::getSimDataSetDir() + strHitFilePath;
	//解析
	m_simDataParse.ParseHitData(m_freexSimData.getHitFilePath(),m_freexSimData.getHitCommand());
}

void FreeXSimSceneParse::ParseDestryEle( const QDomElement &ele )
{
	QString strDesFilePath = ele.attribute("filePath");
	//销毁事件
	m_freexSimData.getDesFilePath() = FilePathMgr::getSimDataSetDir() + strDesFilePath;
	m_simDataParse.ParseDestryData(m_freexSimData.getDesFilePath(),m_freexSimData.getDestoryCommand());
}

void FreeXSimSceneParse::ParseDetectEle( const QDomElement &ele,SimEntityObj &simEntity )
{
	QString strDetectFilePath = ele.attribute("filePath");
	//跟踪探测
	simEntity.m_strDetectFilePath = FilePathMgr::getDetectDataDir() + strDetectFilePath;
	m_simDataParse.ParseDetectData(simEntity.m_strDetectFilePath,simEntity.vecDetectCommand);
}

void FreeXSimSceneParse::ParseAirSpaceEle( const QDomElement &ele )
{
	QString strDetectFilePath = ele.attribute("filePath");
	//飞机空间飞行
	m_freexSimData.getAirSpaceFilePath() = FilePathMgr::getSimDataSetDir() + strDetectFilePath;
	m_simDataParse.ParseAirSpaceData(m_freexSimData.getAirSpaceFilePath(),m_freexSimData.getAirSpaceInfo());
	
}

