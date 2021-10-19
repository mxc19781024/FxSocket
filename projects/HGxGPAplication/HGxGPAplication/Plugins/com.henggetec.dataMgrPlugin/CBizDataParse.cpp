#include <time.h>
#include <QDateTime>
#include <QFileInfo>
#include <QTextStream>
#include <CBizDataParse.h>
#include <bdpUtil\GxTimeManager.h>
#include <bdpDB\GxRegister.h>
#include <bdpObject\GxCommon.h>
#include <bdpAlgor/GxCoordConverter.h>
#include <gdpAlgor/GxCommonUtils.h>
#include <iostream>
#include "GxPluginFwLauncherInterface.h"
#include "public\IDriveMgrInterface.h"
#include "GxServiceQuerier.h"
#include "bdpObject\GxObjectManager.h"
CBizDataParse::CBizDataParse(std::string strFilePath)
	:m_strFileName(strFilePath)
	,m_dSimuBeginTime(0)
	,m_dSimuEndTime(0)
	,m_strDriverPath("")
{
}
CBizDataParse::~CBizDataParse()
{
}
void CBizDataParse::SetFilePath(std::string strFilePath)
{
	m_strFileName = strFilePath;
	QFileInfo fi(m_strFileName.c_str());
	m_strFilePath = fi.path().toStdString();
}
void CBizDataParse::SetEquipInfoData(MapEquipInfoData mapEquipInfoData)
{
	m_mapEquipInfoData = mapEquipInfoData;
}
double CBizDataParse::GetSimuStartTime()
{
	return m_dSimuBeginTime;
}
double CBizDataParse::GetSimuStopTime()
{
	return m_dSimuEndTime;
}
std::map<int,VectorDriverAircraftObjectsData> CBizDataParse::GetMapIndexToVecDriverData()
{
	return m_mapIndexToVecDriverData;
}
void CBizDataParse::run()
{
	bool bRsut = false;//文件解析结果
	if (!m_strFileName.empty())
	{
		QFile file(m_strFileName.c_str());
		if (file.open(QIODevice::ReadOnly))
		{	
			QDomDocument doc;
			if (doc.setContent(&file))
			{
				QDomElement	root = doc.documentElement();
				bRsut = elementParse(root);
			}
			file.close();
		}		
	}
	emit SignalFileParseFinish(bRsut);
}
bool CBizDataParse::elementParse(const QDomElement& ele)
{
	if(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0))
	{
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
	}
	for (QDomElement domEle=ele.firstChildElement(); 
		!domEle.isNull(); domEle=domEle.nextSiblingElement())
	{
		if (domEle.tagName() == "ScenarioBeginTime")
		{
			m_dSimuBeginTime = parseTimeText(domEle.text());	
			if(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0))
			{
				HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setLogicTimeBegin(m_dSimuBeginTime);
				HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->setSimulateTimeCurrent(m_dSimuBeginTime);
			}
		}
		else if(domEle.tagName() == "ObjectList")
		{
			parseObjectList(domEle);
		}
		else if(domEle.tagName() == "AirEventPath")
		{
			ReadRoleCommand(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(domEle.text().toStdString()));
		}
		else if (domEle.tagName() == "AirSenorPath")
		{
			ReadSenorCommand(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(domEle.text().toStdString()));
		}
		else if(domEle.tagName() == "AirDriverPath")
		{
			m_strDriverPath = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(domEle.text().toStdString());
			if (readDriverFile(m_strDriverPath))
			{
				emit SignalsendDriverData();
			}
		}
	}
	return true;
}
void CBizDataParse::parseObjectList( const QDomElement& ele )
{
	m_vecdriverData.clear();
	for (QDomElement childEle=ele.firstChildElement(); !childEle.isNull(); childEle=childEle.nextSiblingElement())
	{
		parseObjectEle(childEle);
	}
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->addSpacecraftUnit(m_vecdriverData);
		for (auto itr = m_vecEffectData.begin(); itr != m_vecEffectData.end(); itr++)
		{
			driveMgrInterface->addSensorUnit((*itr).get());
		}
	}
}
 void CBizDataParse::parseObjectEle( const QDomElement& ele )
 {
 	if (!ele.hasChildNodes())
 	{
 		return;
 	}
	SNewSpacecraftObjectEleData sNewObjectEleData;
 	if (ele.attribute("Type").toStdString() == "Spacecraft")
 	{
		sNewObjectEleData.sEntityObjectEleData.strEntityType = ele.attribute("Type").toStdString();
		parseSpacecraftObjectEle(ele, sNewObjectEleData.sSpacecraftObjectEleData);
		for (QDomElement childEle = ele.firstChildElement(); !childEle.isNull(); childEle = childEle.nextSiblingElement())
		{
			if (childEle.tagName() == "EntityEle")
			{
				parseEntityEle(childEle, sNewObjectEleData.sEntityObjectEleData);
			}
		}
		m_vecdriverData.push_back(sNewObjectEleData);
 	}
	else if (ele.attribute("Type").toStdString() == "CameraImaging")
	{
		osg::ref_ptr<HGxObject::CGxCameraImagingOption> cameraImagingOpt = new HGxObject::CGxCameraImagingOption;
		parseCameraImgEle(ele, *cameraImagingOpt);
		m_vecEffectData.push_back(cameraImagingOpt);
	}
	else if (ele.attribute("Type").toStdString() == "ConeScan")
	{
		osg::ref_ptr<HGxObject::CGxConeScanOption> coneScanOpt = new HGxObject::CGxConeScanOption;
		parseConeScanEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "Disturb")
	{
		osg::ref_ptr<HGxObject::CGxDisturbOption> coneScanOpt = new HGxObject::CGxDisturbOption;
		parseDisturbEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "LinkLine")
	{
		osg::ref_ptr<HGxObject::CGxLinklineOption> coneScanOpt = new HGxObject::CGxLinklineOption;
		parseLinkLineEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "LinkTublar")
	{
		osg::ref_ptr<HGxObject::CGxLinkTubularOption> coneScanOpt = new HGxObject::CGxLinkTubularOption;
		parseLinkTubularEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "RadarScan")
	{
		osg::ref_ptr<HGxObject::CGxRadarScanOption> coneScanOpt = new HGxObject::CGxRadarScanOption;
		parseRadarScanEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "RectScan")
	{
		osg::ref_ptr<HGxObject::CGxRectScanEle> coneScanOpt = new HGxObject::CGxRectScanEle;
		parseRectScanEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "SignalJamming")
	{
		osg::ref_ptr<HGxObject::CGxSignalJammingOption> coneScanOpt = new HGxObject::CGxSignalJammingOption;
		parseSignalJammingEle(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "ConcentricRings")
	{
		osg::ref_ptr<HGxObject::CGxConcentricRingsOption> coneScanOpt = new HGxObject::CGxConcentricRingsOption;
		parseConcentricRing(ele, *coneScanOpt);
		m_vecEffectData.push_back(coneScanOpt);
	}
	else if (ele.attribute("Type").toStdString() == "Ground")
	{
		for (QDomElement childEle = ele.firstChildElement(); !childEle.isNull(); childEle = childEle.nextSiblingElement())
		{
			if (childEle.tagName() == "EntityEle")
			{
				parseEntityEle(childEle, sNewObjectEleData.sEntityObjectEleData);
			}
		}
		SNewObjectData sNewAircraftObjectData;
		sNewAircraftObjectData.vecPosture[0] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPosture.x();
		sNewAircraftObjectData.vecPosture[1] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPosture.x();
		sNewAircraftObjectData.vecPosture[2] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPosture.x();
		sNewAircraftObjectData.vecPositionLLH[0] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPositionLLH.x();//lists.at(10).toDouble();
		sNewAircraftObjectData.vecPositionLLH[1] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPositionLLH.y();//lists.at(11).toDouble();
		sNewAircraftObjectData.vecPositionLLH[2] = sNewObjectEleData.sEntityObjectEleData.sObjectEleData.vecPositionLLH.z();//lists.at(12).toDouble();
		sNewAircraftObjectData.enEquType = GROUND_EUT;
		strcpy(sNewAircraftObjectData.strID, sNewObjectEleData.sEntityObjectEleData.strID.c_str());
		strcpy(sNewAircraftObjectData.strName, sNewObjectEleData.sEntityObjectEleData.sTextObjectEleData.strText.c_str());
		// 获取数据管理服务
		HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
		if (driveMgrInterface)
		{
			driveMgrInterface->addUnit(sNewAircraftObjectData);
		}
	}
 }
void CBizDataParse::parseSpacecraftObjectEle(const QDomElement& ele, SSpacecraftObjectEleData &sSpacecraftEleData)
 {
	 if (ele.hasChildNodes())
	 {
		 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
		 {
			 if (domEle.tagName() == "OrbitMode")
			 {
				 sSpacecraftEleData.eOrbitMode = (ESpacecraftOrbitMode)domEle.text().toInt();
			 }
			 else if (domEle.tagName() == "Sgp4Data")
			 {
				 parseSgp4Data(domEle, sSpacecraftEleData);
			 }
		 }
	 }
 }
 void CBizDataParse::parseSgp4Data(const QDomElement& ele, SSpacecraftObjectEleData &sSpacecraftEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement();
		 !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "Tle1")
		 {
			 sSpacecraftEleData.stSgp4Data.strLine1 = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Tle2")
		 {
			 sSpacecraftEleData.stSgp4Data.strLine2 = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "StartTime")
		 {
			 std::string  strBeginTime = domEle.text().toStdString();
			 sSpacecraftEleData.stSgp4Data.dBeginTime = HGxAlgor::getJDFromString(strBeginTime);
		 }
		 else if (domEle.tagName() == "EndTime")
		 {
			 std::string  strEndTime = domEle.text().toStdString();
			 sSpacecraftEleData.stSgp4Data.dEndTime = HGxAlgor::getJDFromString(strEndTime);
		 }
		 else if (domEle.tagName() == "DeltaTime")
		 {
			 sSpacecraftEleData.stSgp4Data.dDeltaTime = domEle.text().toDouble();
		 }
	 }
 }
 void CBizDataParse::parseEntityEle( const QDomElement& ele, SEntityObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "ID")
 		{
			sEleData.strID = domEle.text().toStdString();
 		}
 		else if (domEle.tagName() == "ParentID")
 		{
			sEleData.strParentID = domEle.text().toStdString();
 		}
 		else if (domEle.tagName() == "Type")
 		{
			sEleData.strEntityType = domEle.text().toStdString();
 		}
 		else if (domEle.tagName() == "Dynamic")
 		{
			sEleData.bDynamic = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "Camp")
 		{
			sEleData.eCamp = (EnCamp)domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "BaseEle")
 		{
 			parseBaseEle(domEle, sEleData.sObjectEleData);
 		}	
 		else if (domEle.tagName() == "TextEle")
 		{
 			parseTextEle(domEle, sEleData.sTextObjectEleData);
 		}
 		else if (domEle.tagName() == "ImageEle")
 		{
 			parseImageEle(domEle, sEleData.sImageObjectEleData);
 		}
 		else if (domEle.tagName() == "ModelEle")
 		{
 			parseModelEle(domEle, sEleData.sModelObjectEleData);
 		}
 		else if (domEle.tagName() == "PointEle")
 		{
 			parsePointEle(domEle, sEleData.sPointObjectEleData);
 		}
 		else if (domEle.tagName() == "PathEle")
 		{
 			parsePathEle(domEle, sEleData.sLineObjectEleData);
 		}	
 	}
 }
 void CBizDataParse::parseBaseEle( const QDomElement& ele, SObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "Visible")
 		{
			sEleData.bVisible = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "Position")
 		{
			sEleData.vecPositionLLH = parseVec3d(domEle.text());
 		}
 		else if (domEle.tagName() == "Posture")
 		{
			sEleData.vecPosture = parseVec3d(domEle.text());
 		}
 	}
 }
 void CBizDataParse::parseTextEle( const QDomElement& ele, STextObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "Text")
 		{
			sEleData.strText = domEle.text().toStdString();
 		}
 		else if (domEle.tagName() == "Font")
 		{
			sEleData.strFont = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(domEle.text().toStdString());
 		}
 		else if (domEle.tagName() == "FontSize")
 		{
			sEleData.fFontSize = domEle.text().toFloat();
 		}
 		else if (domEle.tagName() == "Color")
 		{
			sEleData.vecColor = parseVec4(domEle.text());
 		}
 		else if (domEle.tagName() == "OutLine")
 		{
			sEleData.bOutLine = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "OutLineColor")
 		{
			sEleData.vecOutLineColor = parseVec4(domEle.text());
 		}	
 		else if (domEle.tagName() == "Offset")
 		{
			sEleData.vecOffset = parseVec3d(domEle.text());
 		}
 		else if (domEle.tagName() == "AutoScale")
 		{
			sEleData.bAutoScale = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "AutoRotate")
 		{
			sEleData.bAutoRotate = domEle.text().toInt();
 		}	
 	}
 }
 void CBizDataParse::parseImageEle( const QDomElement& ele, SImageObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "FilePath")
 		{
			sEleData.strImageFilePath = domEle.text().toStdString();
 		}
 		else if (domEle.tagName() == "Opacity")
 		{
			sEleData.fOpacity = domEle.text().toFloat();
 		}
 		else if (domEle.tagName() == "ImageSize")
 		{
			sEleData.vecImageSize = parseVec2(domEle.text());
 		}
 		else if (domEle.tagName() == "AutoScale")
 		{
			sEleData.bAutoScale = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "AutoRotate")
 		{
			sEleData.bAutoRotate = domEle.text().toInt();
 		}	
 	}
 }
 void CBizDataParse::parseModelEle( const QDomElement& ele, SModelObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "FilePath")
 		{
			sEleData.strFilePath = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(domEle.text().toStdString());
 		}
 		else if (domEle.tagName() == "Position")
 		{
			sEleData.vecPosition = parseVec3d(domEle.text());
 		}
 		else if (domEle.tagName() == "Posture")
 		{
			sEleData.vecPosture = parseVec3d(domEle.text());
 		}
 		else if (domEle.tagName() == "Scale")
 		{
			sEleData.vecScale = parseVec3d(domEle.text());
 		}
 		else if (domEle.tagName() == "AutoScale")
 		{
			sEleData.bAutoScale = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "AutoRotate")
 		{
			sEleData.bAutoRotate = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "UseColor")
 		{
			sEleData.bUseColor = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "Color")
 		{
			sEleData.vecColor = parseVec4(domEle.text());
 		}
 		else if (domEle.tagName() == "Blend")
 		{
			sEleData.bBlend = domEle.text().toInt();
 		}
 	}
 }
 void CBizDataParse::parsePointEle( const QDomElement& ele, SPointObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "PointSize")
 		{
			sEleData.fPointSize = domEle.text().toFloat();
 		}
 		else if (domEle.tagName() == "Color")
 		{
			sEleData.vecColor = parseVec4(domEle.text());
 		}
 	}
 }
 void CBizDataParse::parsePathEle( const QDomElement& ele, SLineObjectEleData &sEleData)
 {
 	for (QDomElement domEle=ele.firstChildElement(); 
 		!domEle.isNull(); domEle=domEle.nextSiblingElement())
 	{
 		if (domEle.tagName() == "PointSize")
 		{
			sEleData.fPointSize = domEle.text().toFloat();
 		}
 		else if (domEle.tagName() == "Color")
 		{
			sEleData.vecColor = parseVec4(domEle.text());
 		}
 		else if (domEle.tagName() == "LineType")
 		{
			sEleData.eLineType = (ELineType)domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "LineWidth")
 		{
			sEleData.fLineWidth = domEle.text().toFloat();
 		}
 		else if (domEle.tagName() == "Loop")
 		{
			sEleData.bLoop = domEle.text().toInt();
 		}
 		else if (domEle.tagName() == "Visible")
 		{
			sEleData.bVisible = domEle.text().toInt();
 		}
 	}
 }
 std::vector<std::pair<double, double>> CBizDataParse::parseEffectTimepari(const QDomElement& ele)
 {
	 std::vector<std::pair<double, double>> vecTimePari;
	 for (QDomElement timePariEle = ele.firstChildElement(); !timePariEle.isNull(); timePariEle = timePariEle.nextSiblingElement())
	 {
		 if (timePariEle.tagName() == "TimePari")
		 {
			 double dStartTime = 0.0, dEndTime = 0.0;
			 for (QDomElement timeEle = timePariEle.firstChildElement(); !timeEle.isNull(); timeEle = timeEle.nextSiblingElement())
			 {
				 if (timeEle.tagName() == "StartTime")
				 {
					 std::string  strEndTime = timeEle.text().toStdString();
					 dStartTime = HGxCommon::getJDFromString(strEndTime);
				 }
				 else if (timeEle.tagName() == "EndTime")
				 {
					 std::string  strEndTime = timeEle.text().toStdString();
					 dEndTime = HGxCommon::getJDFromString(strEndTime);
				 }
			 }
			 vecTimePari.push_back(std::make_pair(dStartTime, dEndTime));
		 }
	 }
	 return vecTimePari;
 }
 void CBizDataParse::parseCameraImgEle(const QDomElement& ele, HGxObject::CGxCameraImagingOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement();
		 !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID= domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Entity")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseConeScanEle(const QDomElement& ele, HGxObject::CGxConeScanOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 //sEleData.nameEle()->text() = domEle.text().toStdString();
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Angle")
		 {
			 sEleData.angle() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Lenght")
		 {
			 sEleData.lenght() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "FaceColor")
		 {
			 sEleData.faceColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "LineColor")
		 {
			 sEleData.lineColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "BindEntity")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseDisturbEle(const QDomElement& ele, HGxObject::CGxDisturbOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "SendRadius")
		 {
			 sEleData.sendRadius() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "ReceiveRadius")
		 {
			 sEleData.receiveRadius() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Color")
		 {
			 sEleData.color() = parseVec4(domEle.text());;
		 }
		 else if (domEle.tagName() == "SendEntity")
		 {
			 sEleData.sendEntityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "ReceiveEntity")
		 {
			 sEleData.receiveEntityID() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseLinkLineEle(const QDomElement& ele, HGxObject::CGxLinklineOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "SegmentPoint")
		 {
			 sEleData.segmentPoint() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "SegmentNum")
		 {
			 sEleData.segmentNum() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "Color")
		 {
			 sEleData.color() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "SendEntity")
		 {
			 sEleData.sendEntityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "ReceiveEntity")
		 {
			 sEleData.receiveEntityID() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseLinkTubularEle(const QDomElement& ele, HGxObject::CGxLinkTubularOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "StartPoint")
		 {
			 sEleData.startPoint() = parseVec3(domEle.text());
		 }
		 else if (domEle.tagName() == "EndPoint")
		 {
			 sEleData.endPoint() = parseVec3(domEle.text());
		 }
		 else if (domEle.tagName() == "StartRadius")
		 {
			 sEleData.startRadius() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "EndRadius")
		 {
			 sEleData.endRadius() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "StartAlpha")
		 {
			 sEleData.startAlpha() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "EndAlpha")
		 {
			 sEleData.endAlpha() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Section")
		 {
			 sEleData.numSection() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "FrameDelta")
		 {
			 sEleData.frameDelta() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "BaseColor")
		 {
			 sEleData.baseColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "SendEntity")
		 {
			 sEleData.sendEntityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "ReceiveEntity")
		 {
			 sEleData.receiveEntityID() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseRadarScanEle(const QDomElement& ele, HGxObject::CGxRadarScanOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "AngleYaw")
		 {
			 sEleData.angleYaw() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "AnglePitch")
		 {
			 sEleData.anglePitch() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "AngleRoll")
		 {
			 sEleData.angleRoll() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Radius")
		 {
			 sEleData.radius() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "LoopTime")
		 {
			 sEleData.loopTime() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "AzMin")
		 {
			 sEleData.azMin() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "AzMax")
		 {
			 sEleData.azMax() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "ElevMin")
		 {
			 sEleData.elevMin() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "ElevMax")
		 {
			 sEleData.elevMax() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "ElfinAzMax")
		 {
			 sEleData.elfinAzMax() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "ElfinElevMax")
		 {
			 sEleData.elfinElevMax() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "Color")
		 {
			 sEleData.color() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "LineColor")
		 {
			 sEleData.lineColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "ElfinColor")
		 {
			 sEleData.elfinColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "ScanType")
		 {
			 sEleData.scanType() = (HGxObject::ERadarScanType)domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "BindEntity")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
	 }
 }
 void CBizDataParse::parseRadarBeamEle(const QDomElement& ele, HGxObject::CGxRadarBeamOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Lenght")
		 {
			 sEleData.lenght() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Azimuth")
		 {
			 sEleData.azimuth() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Pitch")
		 {
			 sEleData.pitch() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Angle")
		 {
			 sEleData.angle() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "FaceColor")
		 {
			 sEleData.faceColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "LineColor")
		 {
			 sEleData.lineColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "BindEntity")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
	 }
 }
 void CBizDataParse::parseRectScanEle(const QDomElement& ele, HGxObject::CGxRectScanEle& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "AngleLR")
		 {
			 sEleData.angleLR() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "AngleFB")
		 {
			 sEleData.angleFB() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "Lenght")
		 {
			 sEleData.lenght() = domEle.text().toDouble();
		 }
		 else if (domEle.tagName() == "FaceColor")
		 {
			 sEleData.faceColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "LineColor")
		 {
			 sEleData.lineColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "BindEntity")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 void CBizDataParse::parseSignalJammingEle(const QDomElement& ele, HGxObject::CGxSignalJammingOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "EntityID")
		 {
			 sEleData.entityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "MapVecPari2Intensity")
		 {
			 for (QDomElement dom = domEle.firstChildElement();
				 !dom.isNull(); dom = dom.nextSiblingElement())
			 {
				 std::string std1 = dom.tagName().toStdString();
				 if (dom.tagName() == "VecPari2Intensity")
				 {
					 parseMapVecPari2IntensityELe(dom, sEleData);
				 }
			 }
		 }
	 }
 }
 void CBizDataParse::parseMapVecPari2IntensityELe(const QDomElement& ele, HGxObject::CGxSignalJammingOption& sEleData)
 {
	 double dIntensity = 0.0, dStartTime = 0.0, dEndTime = 0.0;
	 std::map<std::pair<double, double>, double> mapVecPari2Intensity;
	 for (QDomElement domEle = ele.firstChildElement();
		 !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "Intensity")
		 {
			 dIntensity = domEle.text().toFloat();
		 }
		 if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
			 std::pair<double, double> pairKey(dStartTime,dEndTime);
			 sEleData.pushIntensity(pairKey,dIntensity);
		 }
	 }
 }
 void CBizDataParse::parseConcentricRing(const QDomElement& ele, HGxObject::CGxConcentricRingsOption& sEleData)
 {
	 for (QDomElement domEle = ele.firstChildElement(); !domEle.isNull(); domEle = domEle.nextSiblingElement())
	 {
		 if (domEle.tagName() == "ID")
		 {
			 sEleData.strID = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "Name")
		 {
			 sEleData.strName = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "StartPoint")
		 {
			 sEleData.startPoint() = parseVec3(domEle.text());
		 }
		 else if (domEle.tagName() == "EndPoint")
		 {
			 sEleData.endPoint() = parseVec3(domEle.text());
		 }
		 else if (domEle.tagName() == "StartRadius")
		 {
			 sEleData.startRadius() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "EndRadius")
		 {
			 sEleData.endRadius() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "NumWave")
		 {
			 sEleData.numWave() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "BaseColor")
		 {
			 sEleData.baseColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "WaveColor")
		 {
			 sEleData.waveColor() = parseVec4(domEle.text());
		 }
		 else if (domEle.tagName() == "WaveWidth")
		 {
			 sEleData.waveWidth() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "WidthBetweenWave")
		 {
			 sEleData.widthBetweenWave() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "WaveStartOffset")
		 {
			 sEleData.waveStartOffset() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "MoveSpeed")
		 {
			 sEleData.moveSpeed() = domEle.text().toFloat();
		 }
		 else if (domEle.tagName() == "IsSmooth")
		 {
			 sEleData.smooth() = domEle.text().toInt();
		 }
		 else if (domEle.tagName() == "SendEntity")
		 {
			 sEleData.sendEntityID() = domEle.text().toStdString();
			 sEleData.parent() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "ReceiveEntity")
		 {
			 sEleData.receiveEntityID() = domEle.text().toStdString();
		 }
		 else if (domEle.tagName() == "VecTimePari")
		 {
			 sEleData.setEffectWorkingList(parseEffectTimepari(domEle));
		 }
	 }
 }
 //解析地面站构建元素
 void CBizDataParse::parseGround(const QDomElement& ele, SNewObjectData& objectData)
 {
 }
 void CBizDataParse::ClearData()
{
	m_strFileName = "";
	m_strFilePath = "";
	m_dSimuBeginTime = 0.0;
	m_dSimuEndTime = 0.0;
	m_strDriverPath = "";
	m_mapIndexToVecDriverData.erase(m_mapIndexToVecDriverData.begin(),m_mapIndexToVecDriverData.end());
	m_mapTimeToRoleCommand.erase(m_mapTimeToRoleCommand.begin(), m_mapTimeToRoleCommand.end());
	m_mapTimeToZaijiRadarStatus.erase(m_mapTimeToZaijiRadarStatus.begin(), m_mapTimeToZaijiRadarStatus.end());
	m_vecdriverData.clear();
	m_vecEffectData.clear();
}
std::map<float, RoleCommandVec> CBizDataParse::GetMapIndexRoleCommandData()
{
	return m_mapTimeToRoleCommand;
}
std::map<float, vecZaijiRadarState> CBizDataParse::GetMapIndexSenorCommandData()
{
	return m_mapTimeToZaijiRadarStatus;
}
bool CBizDataParse::ReadRoleCommand(std::string strPath)
{
	std::cout << "读取事件数据..." << std::endl;
	QString strFile = strPath.c_str(); "/RecEventSequence.txt";
	QFile file(strFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream txtInput(&file);
		QString strLine;
		QStringList headList;
		QStringList lists;
		while (!txtInput.atEnd())
		{
			strLine = txtInput.readLine();
			headList = strLine.split(' ');
			int nHeadLength = headList.size();
			int nLen = headList.at(3).toInt();
			if (nHeadLength == 4)
			{
				for (int i = 0; i < nLen; i++)
				{
					RoleCommand roleCommand;
					strLine = txtInput.readLine();
					//利用正则表达式将多个空格替换为一个
					strLine.replace(QRegExp("[\\s]+"), " ");
					lists = strLine.split(' ');
					int nIndex = 0;
					roleCommand.SimTimeSec = lists.at(nIndex++).toDouble();
					roleCommand.recplatfmID = lists.at(nIndex++).toStdString();
					roleCommand.C1 = lists.at(nIndex++).toShort();
					roleCommand.C2 = lists.at(nIndex++).toShort();
					roleCommand.WorkState = lists.at(nIndex++).toStdString();
					roleCommand.strNumbCo = lists.at(nIndex++).toStdString();
					m_mapTimeToRoleCommand[int(roleCommand.SimTimeSec)].push_back(roleCommand);
				}
			}
		}
		std::cout << "读取事件数据完成" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}
bool CBizDataParse::ReadSenorCommand(std::string strPath)
{
	std::cout << "读取载机雷达数据..." << std::endl;
	QString strFile = strPath.c_str(); "/RecRadarStatus.txt";
	QFile file(strFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream txtInput(&file);
		QString strLine;
		int nCount = 0;
		QStringList headList;
		QStringList lists;
		while (!txtInput.atEnd())
		{
			strLine = txtInput.readLine();
			headList = strLine.split(' ');
			int nHeadLength = headList.size();
			int nLen = headList.at(3).toInt();
			if (nHeadLength == 4)
			{
				for (int i = 0; i < nLen; i++)
				{
					ZaijiRadarState zaijiRadarState;
					strLine = txtInput.readLine();
					//利用正则表达式将多个空格替换为一个
					strLine.replace(QRegExp("[\\s]+"), " ");
					lists = strLine.split(' ');
					int nIndex = 0;
					float fTime = lists.at(nIndex++).toFloat();
					zaijiRadarState.SimuTime = fTime;
					zaijiRadarState.ValidRadiatFlag = lists.at(nIndex++).toShort();
					zaijiRadarState.Shapetype = lists.at(nIndex++).toShort();
					zaijiRadarState.IDbelong = std::to_string(_Longlong(lists.at(nIndex++).toLongLong()));
					if (zaijiRadarState.IDbelong == "0")
					{
						continue;
					}
					zaijiRadarState.yawCurline = lists.at(nIndex++).toFloat();
					zaijiRadarState.picthCurline = lists.at(nIndex++).toFloat();
					zaijiRadarState.yawCnline = lists.at(nIndex++).toFloat();
					zaijiRadarState.picthCnline = lists.at(nIndex++).toFloat();
					zaijiRadarState.BeamNearDis = lists.at(nIndex++).toFloat();
					zaijiRadarState.dectin = lists.at(nIndex++).toFloat();
					zaijiRadarState.BeamMidDis = lists.at(nIndex++).toFloat();
					zaijiRadarState.dectout = lists.at(nIndex++).toFloat();
					zaijiRadarState.alpha = lists.at(nIndex++).toFloat();
					zaijiRadarState.beta = lists.at(nIndex++).toFloat();
					m_mapTimeToZaijiRadarStatus[int(fTime)].push_back(zaijiRadarState);
				}
			}
		}
		std::cout << "读取载机雷达数据完成" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}
bool CBizDataParse::readDriverFile(std::string strPath)
{
	std::cout<<"读取平台数据...."<<std::endl;
	QString strFile = strPath.c_str();
	QFile file(strFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream txtInput(&file);
		QString strLine;
		int nCount = 0;
		QStringList headList;
		QStringList lists;
		while(!txtInput.atEnd())
		{
			strLine = txtInput.readLine();		
			headList = strLine.split(' ');
			int nHeadLength = headList.size();
			int nLen = headList.at(3).toInt();
			if(nHeadLength == 4)
			{
				for(int i=0;i < nLen;i++)
				{
					SDriverAircraftObjectData sDriverAircraftObjectData;
					strLine = txtInput.readLine();
					lists = strLine.split(' ');
					int nIndex = 0;
					std::string strID = std::to_string(_Longlong(lists.at(nIndex++).toLongLong()));
					strcpy(sDriverAircraftObjectData.strID, strID.c_str());
					if(sDriverAircraftObjectData.strID == 0)
					{
						continue;
					}
					//sDriverAircraftObjectData.nType	= lists.at(5).toInt();
					if (lists.at(3).toInt() == 3)
					{
						std::string strModelName = "J11战斗机";
						if (m_mapEquipInfoData.find(strModelName) != m_mapEquipInfoData.end())
						{
							strcpy(sDriverAircraftObjectData.strName, strModelName.c_str());
							std::string strModelFilePath = m_mapEquipInfoData[strModelName].model_path;//"Model/054.ive";//std::to_string(_Longlong(lists.at(nIndex++).toLongLong()));
							strcpy(sDriverAircraftObjectData.strModelFilePath, strModelFilePath.c_str());
							sDriverAircraftObjectData.sPreEquipInfo = m_mapEquipInfoData[strModelName];
						}
					}
					else if (lists.at(3).toInt() == 5)
					{
						std::string strModelName = "KD_导弹";//std::to_string(_Longlong(lists.at(nIndex++).toLongLong()));
						if (m_mapEquipInfoData.find(strModelName) != m_mapEquipInfoData.end())
						{
							std::string strModelFilePath = m_mapEquipInfoData[strModelName].model_path;//"Model/daodan-IVE/KD-dd.ive";//std::to_string(_Longlong(lists.at(nIndex++).toLongLong()));
							strcpy(sDriverAircraftObjectData.strModelFilePath, strModelFilePath.c_str());
							strcpy(sDriverAircraftObjectData.strName, strModelName.c_str());
							sDriverAircraftObjectData.sPreEquipInfo = m_mapEquipInfoData[strModelName];
						}
					}
					if (lists.at(4).toInt() == 1)  //红方
					{
						sDriverAircraftObjectData.enCamp = RED_CAMP;
						sDriverAircraftObjectData.modelColor[0] = 1.0;
						sDriverAircraftObjectData.modelColor[1] = 0.0;
						sDriverAircraftObjectData.modelColor[2] = 0.0;
						sDriverAircraftObjectData.modelColor[3] = 1.0;
						sDriverAircraftObjectData.nameColor[0] = 1.0;
						sDriverAircraftObjectData.nameColor[1] = 0.0;
						sDriverAircraftObjectData.nameColor[2] = 0.0;
						sDriverAircraftObjectData.nameColor[3] = 1.0;
					}
					else if (lists.at(4).toInt() == 2)//蓝方
					{
						sDriverAircraftObjectData.enCamp = BLUE_CAMP;
						sDriverAircraftObjectData.modelColor[0] = 0.0;
						sDriverAircraftObjectData.modelColor[1] = 0.0;
						sDriverAircraftObjectData.modelColor[2] = 1.0;
						sDriverAircraftObjectData.modelColor[3] = 1.0;
						sDriverAircraftObjectData.nameColor[0] = 0.0;
						sDriverAircraftObjectData.nameColor[1] = 0.0;
						sDriverAircraftObjectData.nameColor[2] = 1.0;
						sDriverAircraftObjectData.nameColor[3] = 1.0;
					}
					sDriverAircraftObjectData.modelScale[0] = 10.0;
					sDriverAircraftObjectData.modelScale[1] = 10.0;
					sDriverAircraftObjectData.modelScale[2] = 10.0;
					sDriverAircraftObjectData.nameFontSize = 20.0;
					sDriverAircraftObjectData.imageImageSize[0] = 64.0;
					sDriverAircraftObjectData.imageImageSize[1] = 64.0;
					std::string strImageFilePath = "Model/purple.png";
					strcpy(sDriverAircraftObjectData.strImageFilePath, strImageFilePath.c_str());
					sDriverAircraftObjectData.dTime = lists.at(5).toDouble();
					sDriverAircraftObjectData.vecPosture[0] = lists.at(6).toDouble();
					sDriverAircraftObjectData.vecPosture[1] = lists.at(8).toDouble();
					sDriverAircraftObjectData.vecPosture[2] = -lists.at(7).toDouble();
					osg::Vec3d vecLLH = osg::Vec3d(lists.at(10).toDouble(), lists.at(11).toDouble(), lists.at(12).toDouble());
					//osg::Vec3d vecXYZ = osg::Vec3d(0, 0, 0);
					//HGxAlgor::DegreeLLH2WGS84XYZ(vecLLH, vecXYZ);
					sDriverAircraftObjectData.vecPositionLLH[0] = vecLLH.x() + 1.0;//lists.at(10).toDouble();
					sDriverAircraftObjectData.vecPositionLLH[1] = vecLLH.y();//lists.at(11).toDouble();
					sDriverAircraftObjectData.vecPositionLLH[2] = vecLLH.z();//lists.at(12).toDouble();
					sDriverAircraftObjectData.nDriveStyle = 1;
					//剔除经纬高均为0的错误数据
					if (sDriverAircraftObjectData.vecPositionLLH[0] == 0.0 && sDriverAircraftObjectData.vecPositionLLH[1] == 0.0 
						&& sDriverAircraftObjectData.vecPositionLLH[2] == 0.0)
					{
						continue;
					}
					m_mapIndexToVecDriverData[nCount].push_back(sDriverAircraftObjectData);
				}	
				nCount++;
			}
		}			
		int nSize = m_mapIndexToVecDriverData.size();
		if (nSize <= 0)
		{
			return false;
		}
		std::cout<<"读取平台数据完成"<<std::endl;
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}
double CBizDataParse::parseTimeText(const QString& strTime)
{
	//QDateTime dateTime;
	//dateTime.fromString(strTime,"yyyy-MM-dd hh:mm:ss");
	QDateTime dateTime = QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss");
	QDate date = dateTime.date();
	QTime time = dateTime.time();
	return HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->calToJD(date.year()
		, date.month()
		, date.day()
		, time.hour()
		, time.minute()
		, time.second());
}
QString CBizDataParse::createTimeText(double dJD)
{
	int nYear, nMonth, nDay, nHour, nMin;
	double dSec;
	HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->jdToCal(dJD, nYear, nMonth, nDay
		, nHour, nMin, dSec);
	QDateTime dateTime(QDate(nYear, nMonth, nDay), QTime(nHour, nMin, dSec));
	return dateTime.toString("yyyy-MM-dd hh:mm:ss");
}
osg::Vec2 CBizDataParse::parseVec2(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 1)
	{
		return osg::Vec2(str.at(0).toFloat(), str.at(1).toFloat());
	}
	else
	{
		return osg::Vec2();
	}
}
osg::Vec2d CBizDataParse::parseVec2d(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 1)
	{
		return osg::Vec2d(str.at(0).toDouble(), str.at(1).toDouble());
	}
	else
	{
		return osg::Vec2d();
	}
}
osg::Vec3 CBizDataParse::parseVec3(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 2)
	{
		return osg::Vec3(str.at(0).toFloat(), str.at(1).toFloat(), str.at(2).toFloat());
	}
	else
	{
		return osg::Vec3();
	}
}
osg::Vec3d CBizDataParse::parseVec3d(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 2)
	{
		return osg::Vec3d(str.at(0).toDouble(), str.at(1).toDouble(), str.at(2).toDouble());
	}
	else
	{
		return osg::Vec3d();
	}
}
osg::Vec4 CBizDataParse::parseVec4(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 3)
	{
		osg::Vec4 vecColor(str.at(0).toFloat(), str.at(1).toFloat(), str.at(2).toFloat(), str.at(3).toFloat());
		if (vecColor.x() > 1.0 || vecColor.y() > 1.0 || vecColor.z() > 1.0)
		{
			vecColor.x() /= 255.0;
			vecColor.y() /= 255.0;
			vecColor.z() /= 255.0;
		}
		if (vecColor.w() > 1.0)
		{
			vecColor.w() /= 255.0;
		}
		return vecColor;
	}
	else
	{
		return osg::Vec4();
	}
}
osg::Vec4d CBizDataParse::parseVec4d(const QString& strVec)
{
	QStringList str = strVec.split(",");
	if (str.size() > 3)
	{
		return osg::Vec4d(str.at(0).toDouble(), str.at(1).toDouble(), str.at(2).toDouble(), str.at(3).toDouble());
	}
	else
	{
		return osg::Vec4d();
	}
}
QString CBizDataParse::createVec2(osg::Vec2 vec, int prec)
{
	QString str = QString::number(vec.x(), 'f', prec) + "," + QString::number(vec.y(), 'f', prec);
	return str;
}
QString CBizDataParse::createVec2d(osg::Vec2d vec, int prec)
{
	QString str = QString::number(vec.x(), 'lf', prec) + "," + QString::number(vec.y(), 'lf', prec);
	return str;
}
QString CBizDataParse::createVec3(osg::Vec3 vec, int prec)
{
	QString str = QString::number(vec.x(), 'f', prec) + ","
		+ QString::number(vec.y(), 'f', prec) + ","
		+ QString::number(vec.z(), 'f', prec);
	return str;
}
QString CBizDataParse::createVec3d(osg::Vec3d vec, int prec)
{
	QString str = QString::number(vec.x(), 'lf', prec) + ","
		+ QString::number(vec.y(), 'lf', prec) + ","
		+ QString::number(vec.z(), 'lf', prec);
	return str;
}
QString CBizDataParse::createVec4(osg::Vec4 vec, int prec)
{
	QString str = QString::number(vec.x(), 'f', prec) + ","
		+ QString::number(vec.y(), 'f', prec) + ","
		+ QString::number(vec.z(), 'f', prec) + ","
		+ QString::number(vec.w(), 'f', prec);
	return str;
}
QString CBizDataParse::createVec4d(osg::Vec4d vec, int prec)
{
	QString str = QString::number(vec.x(), 'lf', prec) + ","
		+ QString::number(vec.y(), 'lf', prec) + ","
		+ QString::number(vec.z(), 'lf', prec) + ","
		+ QString::number(vec.w(), 'lf', prec);
	return str;
}