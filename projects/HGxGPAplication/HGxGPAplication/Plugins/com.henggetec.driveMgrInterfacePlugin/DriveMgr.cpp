#include <DriveMgr.h>

#include <QVariant>

#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/manipulator/GxFreedomManipulator.h>
#include <PlatformPlugin/object/GxAircraftOption.h>
#include <osgDB/ConvertUTF>
#include <public/common/UnitEventCommon.h>
#include <gdpScene/manipulator/GxLockManipulator.h>
#include "service/event/ctkEvent.h"
#include "GxPluginFrameworkCore.h"
#include "GxServiceQuerier.h"
#include "GxPluginFwLauncherInterface.h"
#include "service/event/ctkEventAdmin.h"
#include "GxUiFrameworkServiceInterface.h"
#include "public/scene_ui/ISceneUiService.h"
#include "public/scene_ui/ISceneInterface.h"
#include "bdpScene/scene/GxSceneBase.h"
#include "gdpScene/CommonGeo.h"
#include "PlatformPlugin/object/GxAircraftOption.h"
#include "PlatformPlugin/object/GxSpacecraftOption.h"
#include "PlatformPlugin/object/GxGroundOption.h"
#include "bdpObject/GxObjectNode.h"
//#include "../HGxGeogDevPlat/src/gdpPlugin/object/Aircraft/GxAircraft.h"
#include "gdpScene/manipulator/GxEarthManipulator.h"
#include "gdpScene/manipulator/GxNodeTrackManipulator.h"
#include "bdpUtil/GxTimeManager.h"
#include "PlatformPlugin/object/GxExplosionOption.h"
#include "bdpObject/GxExplosionNode.h"
#include "PlatformPlugin/object/GxWakeRibbonCommon.h"
#include "PlatformPlugin/object/GxTrackLineCommon.h"
#include "GxEventHelperInterface.h"
#include "PlatformPlugin/object/GxLabelCommon.h"
#include "PlatformPlugin/object/GxRadarScanOption.h"
#include "PlatformPlugin/object/GxAirSpaceSectorOption.h"
#include "PlatformPlugin/object/GxMarkEffectLockOption.h"
#include "osg/Math"


#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

namespace FxDrive
{
	CDriveMgr::CDriveMgr()
	{
		m_pObjectInstence = HGxObject::CGxObjectManager::getInstance();
	}

	CDriveMgr::~CDriveMgr()
	{
		
	}

	osg::Vec3d vectorToVec3d(double* dVector)
	{
		return osg::Vec3d(dVector[0], dVector[1], dVector[2]);
	}

	void CDriveMgr::addUnit(SNewObjectData objectData)
	{
		if (objectData.enEquType == GROUND_EUT)
		{
			HGxObject::CGxGroundOption* pGroundOptionEle = new HGxObject::CGxGroundOption;
			//pGroundOptionEle->nameEle()->offset() = osg::Vec3d(100, 50, 50);
			//pGroundOptionEle->dynamic() = false;
			pGroundOptionEle->modelEle()->scale() = osg::Vec3d(50, 50, 50);
			pGroundOptionEle->modelEle()->autoRotate() = false;
			pGroundOptionEle->modelEle()->autoScale() = true;
			pGroundOptionEle->imageEle()->autoScale() = true;
			pGroundOptionEle->strID = objectData.strID;
			std::string strname = objectData.strName;
			pGroundOptionEle->nameEle()->text() = strname;
			pGroundOptionEle->nameEle()->visible() = false;
			pGroundOptionEle->position() = vectorToVec3d(objectData.vecPositionLLH);
			if (m_pObjectInstence)
			{
				m_pObjectInstence->addObject(*pGroundOptionEle);
				m_pObjectsIndexMap[objectData.strID] = pGroundOptionEle;
			}
		}
		else
		{
			HGxObject::CGxAircraftOption* pAirplaneEle = new HGxObject::CGxAircraftOption;
			//pAirplaneEle->driveStyle() = (FxBaseObject::EDriveStyle)stData.nDriveStyle;//::EDriveStyle::DRIVE_DATA;
			pAirplaneEle->nameEle()->color() = osg::Vec4(objectData.nameColor[0], objectData.nameColor[1], objectData.nameColor[2], objectData.nameColor[3]);
			//pAirplaneEle->nameEle()->merge() = false;
			pAirplaneEle->nameEle()->visible() = false;//objectData.nameVisible;
			pAirplaneEle->nameEle()->fontSize() = objectData.nameFontSize;
			pAirplaneEle->nameEle()->offset() = osg::Vec3d(100, 50, 50);
			//pAirplaneEle->imageEle()->merge() = false;
			//pAirplaneEle->imageEle()->filePath() = FeFileReg->GetFullPath(stData.strImageFilePath);
			pAirplaneEle->imageEle()->visible() = false;
			pAirplaneEle->imageEle()->imageSize() = osg::Vec2(objectData.imageImageSize[0], objectData.imageImageSize[1]);
			pAirplaneEle->dynamic() = false;
			//pAirplaneEle->pointEle()->pointSize() = 11.0f;
			pAirplaneEle->pointEle()->color() = osg::Vec4(1.0, 0.0, 0.0, 1.0);
			//pAirplaneEle->pathEle()->lineType() = FxBaseObject::POINTLINE;
			pAirplaneEle->modelEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(objectData.strModelFilePath);
			// 		osg::Vec3d vecPosture(osg::RadiansToDegrees(objectData.vecPosture[0]), osg::RadiansToDegrees(objectData.vecPosture[1]), osg::RadiansToDegrees(objectData.vecPosture[2]));
			// 		pAirplaneEle->internalAttitude() = vecPosture;
			//pAirplaneEle->modelEle()->useColor() = true;
			//pAirplaneEle->modelEle()->color() = osg::Vec4(stData.modelColor[0], stData.modelColor[1], stData.modelColor[2], stData.modelColor[3]);
			//pAirplaneEle->modelEle()->scale() = osg::Vec3d(20, 20, 20); //osg::Vec3d(objectData.modelScale[0], objectData.modelScale[1], objectData.modelScale[2]);
			pAirplaneEle->modelEle()->autoRotate() = false;
			pAirplaneEle->modelEle()->autoScale() = true;//objectData.modelAuto;
			pAirplaneEle->modelEle()->visible() = true;
			pAirplaneEle->strID = objectData.strID;
			//QString strName = "飞机—";//飞机
			pAirplaneEle->useAbsAttInRealTime() = false;
			pAirplaneEle->strID = objectData.strID;
			//QString strName = "飞机—";//飞机
			std::string strname = objectData.strName;
			std::string strID = objectData.strID;
			std::string strLabel = strname + "-" + strID;
			pAirplaneEle->nameEle()->text() = strLabel;		  //osgDB::convertStringFromCurrentCodePageToUTF8(strName.data())
			double dCurJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
			osg::Vec3d vecXYZ;
			HGxAlgor::DegreeLLH2WGS84XYZ(vectorToVec3d(objectData.vecPositionLLH), vecXYZ);
			std::vector<osg::Vec3d> vecPoint;
			vecPoint.push_back(vectorToVec3d(objectData.vecPositionLLH));
			pAirplaneEle->airLine().dBeginTime = dCurJD;
			pAirplaneEle->airLine().dEndTime = dCurJD + 0.01;
			pAirplaneEle->airLine().nMode = HGxObject::E_AIRLINE_BEZIER;
			pAirplaneEle->airLine().vecKeyPoint = vecPoint;
			if (m_pObjectInstence)
			{
				m_pObjectInstence->addObject(*pAirplaneEle, true);
				m_pObjectsIndexMap[strID] = pAirplaneEle;

				HGxObject::CGxPathModel *pPathMode = m_pObjectInstence->getPathModel(pAirplaneEle->strID);
				osg::Vec3d vecPosture(objectData.vecPosture[0], objectData.vecPosture[1], objectData.vecPosture[2]);
				pPathMode->addData(vectorToVec3d(objectData.vecPositionLLH), vecPosture);

				HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
				if (eventService)
				{
					ctkDictionary props;
					QVariant varLog;
					SEquipmentInfo sSEquipmentInfo;
					sSEquipmentInfo.ID = strID;
					sSEquipmentInfo.name = strLabel;
					sSEquipmentInfo.description = objectData.sPreEquipInfo.description;
					sSEquipmentInfo.model_path = objectData.sPreEquipInfo.model_path;
					sSEquipmentInfo.image_path = objectData.sPreEquipInfo.image_path;
					sSEquipmentInfo.maxVoyage = objectData.sPreEquipInfo.maxVoyage;
					sSEquipmentInfo.maxSpeed = objectData.sPreEquipInfo.maxSpeed;
					sSEquipmentInfo.maxOil = objectData.sPreEquipInfo.maxOil;
					sSEquipmentInfo.ammunitionNumber = objectData.sPreEquipInfo.ammunitionNumber;
					sSEquipmentInfo.combatCapability = objectData.sPreEquipInfo.combatCapability;
					sSEquipmentInfo.degreeDamage = objectData.sPreEquipInfo.degreeDamage;
					varLog.setValue(sSEquipmentInfo);
					props[SFxEquipmentInfoEventConstants::EquipmentInfo().c_str()] = varLog;
					ctkEvent event = ctkEvent(SFxEquipmentInfoEventConstants::Topic().c_str(), props);
					eventService->sendEvent(event);

					ctkDictionary propsEquipInfo;
					QVariant varLogsEquipInfo;
					SEquipInfo sSEquipInfo;
					std::string str = objectData.strName;
					if (objectData.enSpaceType == AIR_SPACE)
					{
						sSEquipInfo.strType = "航天器";
						sSEquipInfo.strID = "飞机 " + str;//"发现号 OV-103";
						sSEquipInfo.strIconPath = "texture/OV-103.png";
					}
					if (objectData.enSpaceType == GROUND_SPACE)
					{
						sSEquipInfo.strType = "地面装备";

						sSEquipInfo.strID = "坦克 " + str;//"发现号 OV-103";
						sSEquipInfo.strIconPath = "texture/car.png";
					}

					sSEquipInfo.dEnergyNum = 0.5;
					sSEquipInfo.dLoadNum = 0.33;
					sSEquipInfo.dParaA = 268;
					sSEquipInfo.dParaB = 97;
					sSEquipInfo.dParaC = 255;
					varLogsEquipInfo.setValue(sSEquipInfo);
					propsEquipInfo[SFxEquipInfoEventConstants::EquipInfo().c_str()] = varLogsEquipInfo;
					ctkEvent eventEquipInfo = ctkEvent(SFxEquipInfoEventConstants::Topic().c_str(), propsEquipInfo);
					eventService->sendEvent(eventEquipInfo);
				}
			}
		}
	}

	HGxObject::CGxObjectEle* CDriveMgr::getUnitEle(std::string strID)
	{
		if (m_pObjectsIndexMap.find(strID) != m_pObjectsIndexMap.end())  //驱动实体单元
		{
			return m_pObjectsIndexMap[strID];
		}
	}

	void CDriveMgr::driveUnit(SDriveObjectData objectData)
	{
		std::string strID = objectData.strID;

		if (m_pObjectsIndexMap.find(strID) != m_pObjectsIndexMap.end())  //驱动实体单元
		{
			HGxObject::CGxPathModel *pPathMode = m_pObjectInstence->getPathModel(strID);
			osg::Vec3d vecPosture(objectData.vecPosture[0], objectData.vecPosture[1], objectData.vecPosture[2]);
			pPathMode->addData(vectorToVec3d(objectData.vecPositionLLH), vecPosture);
		}
	}

	void CDriveMgr::driveAircraftUnit(VectorDriverAircraftObjectsData driverData)
	{
		for (int n = 0; n < driverData.size(); n++)
		{
			SDriverAircraftObjectData sDriverAircraftObjectData = driverData.at(n);
			std::string strID = sDriverAircraftObjectData.strID;

			if (m_pObjectsIndexMap.find(strID) != m_pObjectsIndexMap.end())  //驱动实体单元
			{
				//CGxAircraft* pAircraft = dynamic_cast<CGxAircraft*>( HGxObject::CGxObjectManager::getInstance()->getObject(strID));

						//pAircraftEle->internalPosition() = vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH);
						//osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
						//pAircraftEle->internalAttitude() = vecPosture;
						//pAircraftEle->modelEle()->posture() = vecPosture;
				HGxObject::CGxPathModel *pPathMode = m_pObjectInstence->getPathModel(strID);
				osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
				pPathMode->addData(vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH), vecPosture);
				//HGxObject::CGxAircraftOption* pAir+craftEle = dynamic_cast<HGxObject::CGxAircraftOption*>(m_pObjectsIndexMap[strID]);
				//if (pAircraftEle)
				//{
				//	pAircraftEle->internalPosition() = vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH);
				//	osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
				//	//pAircraftEle->internalAttitude() = vecPosture;
				//	pAircraftEle->modelEle()->posture() = vecPosture;
				//	//pAircraftEle->modelEle()->position() = vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH);
				//	
				//}
			}
			else  //添加实体单元
			{
				HGxObject::CGxAircraftOption* pAirplaneEle = new HGxObject::CGxAircraftOption;
				//pAirplaneEle->driveStyle() = (FxBaseObject::EDriveStyle)stData.nDriveStyle;//::EDriveStyle::DRIVE_DATA;
				pAirplaneEle->nameEle()->color() = osg::Vec4(sDriverAircraftObjectData.nameColor[0], sDriverAircraftObjectData.nameColor[1], sDriverAircraftObjectData.nameColor[2], sDriverAircraftObjectData.nameColor[3]);
				//pAirplaneEle->nameEle()->merge() = false;
				pAirplaneEle->nameEle()->visible() = sDriverAircraftObjectData.nameVisible;
				pAirplaneEle->nameEle()->fontSize() = sDriverAircraftObjectData.nameFontSize;
				pAirplaneEle->nameEle()->visible() = false;
				
				//pAirplaneEle->imageEle()->merge() = false;
				//pAirplaneEle->imageEle()->filePath() = FeFileReg->GetFullPath(stData.strImageFilePath);
				pAirplaneEle->imageEle()->visible() = false;
				pAirplaneEle->imageEle()->imageSize() = osg::Vec2(sDriverAircraftObjectData.imageImageSize[0], sDriverAircraftObjectData.imageImageSize[1]);
				pAirplaneEle->dynamic() = false;
				//pAirplaneEle->pointEle()->pointSize() = 11.0f;
				pAirplaneEle->pointEle()->color() = osg::Vec4(1.0, 0.0, 0.0, 1.0);
				//pAirplaneEle->pathEle()->lineType() = FxBaseObject::POINTLINE;
				pAirplaneEle->modelEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(sDriverAircraftObjectData.strModelFilePath);
				osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
				//pAirplaneEle->internalAttitude() = vecPosture;
				//pAirplaneEle->modelEle()->useColor() = true;
				//pAirplaneEle->modelEle()->color() = osg::Vec4(stData.modelColor[0], stData.modelColor[1], stData.modelColor[2], stData.modelColor[3]);
				//pAirplaneEle->modelEle()->scale() = osg::Vec3d(20, 20, 20);// osg::Vec3d(sDriverAircraftObjectData.modelScale[0], sDriverAircraftObjectData.modelScale[1], sDriverAircraftObjectData.modelScale[2]);
				pAirplaneEle->modelEle()->autoRotate() = false;
				pAirplaneEle->modelEle()->autoScale() = sDriverAircraftObjectData.modelAuto;
				//pAirplaneEle->modelEle()->horizonCull() = false;
				//pAirplaneEle->modelEle()->color() = osg::Vec4d(1.0,0.0,0.0,1.0);
				//pAirplaneEle->modelEle()->blend() = true;
				//pAirplaneEle->modelEle()->light() = true;
				//pAirplaneEle->modelEle()->posture() = osg::Vec3d(0, 0, 0);
				//pAirplaneEle->internalPosition() = vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH);
				pAirplaneEle->useAbsAttInRealTime() = false;
				pAirplaneEle->strID = sDriverAircraftObjectData.strID;
				//QString strName = "飞机—";//飞机
				std::string strname = sDriverAircraftObjectData.strName;
				std::string strID = sDriverAircraftObjectData.strID;
				std::string strLabel = strname + "-" + strID;
				pAirplaneEle->nameEle()->text() = strLabel;		  //osgDB::convertStringFromCurrentCodePageToUTF8(strName.data())
				double dCurJD = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
				osg::Vec3d vecXYZ;
				HGxAlgor::DegreeLLH2WGS84XYZ(vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH), vecXYZ);
				pAirplaneEle->internalPosition() = vecXYZ;
				std::vector<osg::Vec3d> vecPoint;
				vecPoint.push_back(vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH));
				pAirplaneEle->airLine().dBeginTime = dCurJD;
				pAirplaneEle->airLine().dEndTime = dCurJD + 0.01;
				pAirplaneEle->airLine().nMode = HGxObject::E_AIRLINE_BEZIER;
				pAirplaneEle->airLine().vecKeyPoint = vecPoint;

				if (m_pObjectInstence)
				{
					m_pObjectInstence->addObject(*pAirplaneEle,true);
					m_pObjectsIndexMap[strID] = pAirplaneEle;

					HGxObject::CGxPathModel *pPathMode = m_pObjectInstence->getPathModel(pAirplaneEle->strID);
					osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
					pPathMode->addData(vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH), vecPosture);


					osg::ref_ptr<HGxObject::CWakeRibbonEle> opWakeRibbon = new HGxObject::CWakeRibbonEle;
					std::string strID = sDriverAircraftObjectData.strID;
					opWakeRibbon->entityID() = strID;
					opWakeRibbon->parent() = strID;
					opWakeRibbon->strID = strID + "WakeRibbon";
					opWakeRibbon->strName = strID;
					opWakeRibbon->size() = 5000;
					opWakeRibbon->width() = 10;
					addSensorUnit(opWakeRibbon);

					osg::ref_ptr<HGxObject::GxLabelEle> sLabelEle = new HGxObject::GxLabelEle;
					std::string strPara = pAirplaneEle->strID;
					sLabelEle->entityID() = strPara;
					sLabelEle->parent() = strPara;
					strPara = "Label_" + strPara;
					sLabelEle->strID = strPara;
					strPara = pAirplaneEle->strName;
					sLabelEle->strName = strPara;
					strPara = "装备名称：" + strPara + ";";
					if (sDriverAircraftObjectData.enCamp == 1)
					{
						pAirplaneEle->camp() = HGxObject::E_ENEMY_OBJECT;
						strPara = strPara + "阵营：红方";
						//sLabelEle->lineColor()
						sLabelEle->lineColor() = osg::Vec4(1.0, 0.0, 0.0, 1.0);
					}
					else if (sDriverAircraftObjectData.enCamp == 2)
					{
						pAirplaneEle->camp() = HGxObject::E_OUR_OBJECT;
						strPara = strPara + "阵营：蓝方";
						sLabelEle->lineColor() = osg::Vec4(0.0, 0.0, 1.0, 1.0);
					}
					else
					{
						pAirplaneEle->camp() = HGxObject::E_NEUTRAL_OBJECT;
						strPara = strPara + "阵营：白方";
						sLabelEle->lineColor() = osg::Vec4(0.0, 0.1, 0.0, 1.0);
					}

					sLabelEle->text() = strPara;

					sLabelEle->backgroundColor() = osg::Vec4(1, 1, 1, 1.0);
					sLabelEle->textColor() = osg::Vec4(0.0, 0.0, 0.0, 1.0);
					sLabelEle->width() = 3.0;
					sLabelEle->fontSize() = 10;
					addSensorUnit(sLabelEle);


					HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
					if (eventService)
					{
						ctkDictionary props;
						QVariant varLog;
						SEquipmentInfo sSEquipmentInfo;
						sSEquipmentInfo.name = strID;
						sSEquipmentInfo.description = sDriverAircraftObjectData.sPreEquipInfo.description;
						sSEquipmentInfo.model_path = sDriverAircraftObjectData.sPreEquipInfo.model_path;
						sSEquipmentInfo.image_path = sDriverAircraftObjectData.sPreEquipInfo.image_path;
						sSEquipmentInfo.maxVoyage = sDriverAircraftObjectData.sPreEquipInfo.maxVoyage;
						sSEquipmentInfo.maxSpeed = sDriverAircraftObjectData.sPreEquipInfo.maxSpeed;
						sSEquipmentInfo.maxOil = sDriverAircraftObjectData.sPreEquipInfo.maxOil;
						sSEquipmentInfo.ammunitionNumber = sDriverAircraftObjectData.sPreEquipInfo.ammunitionNumber;
						sSEquipmentInfo.combatCapability = sDriverAircraftObjectData.sPreEquipInfo.combatCapability;
						sSEquipmentInfo.degreeDamage = sDriverAircraftObjectData.sPreEquipInfo.degreeDamage;
						sSEquipmentInfo.sSEquipCap.dHit = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dHit;
						sSEquipmentInfo.sSEquipCap.dCruise = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dCruise;
						sSEquipmentInfo.sSEquipCap.dPerformance = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dPerformance;
						sSEquipmentInfo.sSEquipCap.dEndurance = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dEndurance;
						sSEquipmentInfo.sSEquipCap.dCoordinate = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dCoordinate;
						sSEquipmentInfo.sSEquipCap.dProbe = sDriverAircraftObjectData.sPreEquipInfo.sPreEquipCap.dProbe;

						varLog.setValue(sSEquipmentInfo);
						props[SFxEquipmentInfoEventConstants::EquipmentInfo().c_str()] = varLog;
						ctkEvent event = ctkEvent(SFxEquipmentInfoEventConstants::Topic().c_str(), props);
						eventService->sendEvent(event);

						ctkDictionary propsEquipInfo;
						QVariant varLogsEquipInfo;
						SEquipInfo sSEquipInfo;
						sSEquipInfo.strType = osgDB::convertStringFromCurrentCodePageToUTF8(sDriverAircraftObjectData.sPreEquipInfo.combatCapability);
						sSEquipInfo.strID = osgDB::convertStringFromCurrentCodePageToUTF8(sDriverAircraftObjectData.sPreEquipInfo.strtype + strID);//"发现号 OV-103";
						sSEquipInfo.strIconPath = "texture/OV-103.png";
						sSEquipInfo.dEnergyNum = 0.5;
						sSEquipInfo.dLoadNum = 0.33;
						sSEquipInfo.dParaA = 268;
						sSEquipInfo.dParaB = 97;
						sSEquipInfo.dParaC = 255;
						varLogsEquipInfo.setValue(sSEquipInfo);
						propsEquipInfo[SFxEquipInfoEventConstants::EquipInfo().c_str()] = varLogsEquipInfo;
						ctkEvent eventEquipInfo = ctkEvent(SFxEquipInfoEventConstants::Topic().c_str(), propsEquipInfo);
						eventService->sendEvent(eventEquipInfo);
					}
				}

			}
		}

	}

	void CDriveMgr::addSpacecraftUnit(VectorSpacecraftObjectsData driverData)
	{
		for (int n = 0; n < driverData.size(); n++)
		{
			SNewSpacecraftObjectEleData sSpacecraftObjectData = driverData.at(n);
			std::string strID = sSpacecraftObjectData.sEntityObjectEleData.strID;

			if (sSpacecraftObjectData.sEntityObjectEleData.strEntityType != "Spacecraft")
			{
				continue;
			}

			if (m_pObjectsIndexMap.find(strID) != m_pObjectsIndexMap.end())  //驱动实体单元
			{
/*
// 				CGxAircraft* pAircraft = dynamic_cast<CGxAircraft*>(HGxObject::CGxObjectManager::getInstance()->getObject(strID));
// 				if (pAircraft)
// 
// 				{
// 					HGxObject::CGxAircraftOption* pAircraftEle = dynamic_cast<HGxObject::CGxAircraftOption*>(pAircraft->getObjectOption());
// 					if (pAircraftEle)
// 					{
// 						//pAircraftEle->internalPosition() = vectorToVec3d(sDriverAircraftObjectData.vecPositionLLH);
// 						//osg::Vec3d vecPosture(osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[0]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[1]), osg::RadiansToDegrees(sDriverAircraftObjectData.vecPosture[2]));
// 						//pAircraftEle->internalAttitude() = vecPosture;
// 						//pAircraftEle->modelEle()->posture() = vecPosture;
// 
// 					}
// 				}
*/
			}
			else  //添加实体单元
			{
				osg::ref_ptr<HGxObject::CGxSpacecraftOption> rpSatEle = new HGxObject::CGxSpacecraftOption;
				rpSatEle->strID = sSpacecraftObjectData.sEntityObjectEleData.strID;
				rpSatEle->nameEle()->text() = sSpacecraftObjectData.sEntityObjectEleData.sTextObjectEleData.strText;
				rpSatEle->nameEle()->visible() = sSpacecraftObjectData.sEntityObjectEleData.sObjectEleData.bVisible;
				rpSatEle->nameEle()->fontSize() = sSpacecraftObjectData.sEntityObjectEleData.sTextObjectEleData.fFontSize;
				rpSatEle->nameEle()->color() = sSpacecraftObjectData.sEntityObjectEleData.sTextObjectEleData.vecColor;
				rpSatEle->imageEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(sSpacecraftObjectData.sEntityObjectEleData.sImageObjectEleData.strImageFilePath);
				rpSatEle->imageEle()->visible() = true;
				//rpSatEle->imageEle()->filePath() =  FeFileReg->GetFullPath("texture/effect/干扰特效.png");
				//rpSatEle->modelEle()->filePath() = FeFileReg->GetFullPath("Model/WX/Bd1h.ive");
				rpSatEle->modelEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath(sSpacecraftObjectData.sEntityObjectEleData.sModelObjectEleData.strFilePath);
				//rpSatEle->modelEle()->useColor() = true;
				//rpSatEle->modelEle()-> = osg::Vec4d(1.0, 0.0, 0.0, 0.5);
				//rpSatEle->modelEle()->scale() = osg::Vec3(10000.0,10000.0,10000.0);
				//rpSatEle->imageEle()->autoRotate() = false;
				//rpSatEle->modelEle()->visible() =false;
				std::string aa = "Earth";
				rpSatEle->parent() = sSpacecraftObjectData.sEntityObjectEleData.strParentID;
				rpSatEle->dynamic() = true;
				rpSatEle->orbitMode() = (HGxObject::ESpacecraftOrbitMode)sSpacecraftObjectData.sSpacecraftObjectEleData.eOrbitMode;//HGxObject::E_SatDynamicOrbit;
				rpSatEle->pointEle()->size() = sSpacecraftObjectData.sEntityObjectEleData.sPointObjectEleData.fPointSize;
				rpSatEle->pointEle()->visible() = true;
				rpSatEle->pointEle()->color() = sSpacecraftObjectData.sEntityObjectEleData.sPointObjectEleData.vecColor;
				rpSatEle->pathEle()->type() = (HGxObject::ELineType)sSpacecraftObjectData.sEntityObjectEleData.sLineObjectEleData.eLineType;
				rpSatEle->pathEle()->visible() = sSpacecraftObjectData.sEntityObjectEleData.sLineObjectEleData.bVisible;
				rpSatEle->pathEle()->color() = sSpacecraftObjectData.sEntityObjectEleData.sLineObjectEleData.vecColor;
				rpSatEle->sgp4Data().strLine1 = sSpacecraftObjectData.sSpacecraftObjectEleData.stSgp4Data.strLine1;
				rpSatEle->sgp4Data().strLine2 = sSpacecraftObjectData.sSpacecraftObjectEleData.stSgp4Data.strLine2;
				rpSatEle->sgp4Data().dBeginTime = sSpacecraftObjectData.sSpacecraftObjectEleData.stSgp4Data.dBeginTime;
				rpSatEle->sgp4Data().dEndTime = sSpacecraftObjectData.sSpacecraftObjectEleData.stSgp4Data.dEndTime;
				rpSatEle->sgp4Data().dDeltaTime = sSpacecraftObjectData.sSpacecraftObjectEleData.stSgp4Data.dDeltaTime;

				if (m_pObjectInstence)
				{
					m_pObjectInstence->addObject(*rpSatEle.get(),true);
					m_pObjectsIndexMap[strID] = rpSatEle;

					HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
					if (eventService)
					{
// 						ctkDictionary props;
// 						QVariant varLog;
// 						SEquipmentInfo sSEquipmentInfo;
// 						sSEquipmentInfo.ID = strID;
// 						sSEquipmentInfo.name = strID;
// 						varLog.setValue(sSEquipmentInfo);
// 						props[SFxEquipmentInfoEventConstants::EquipmentInfo().c_str()] = varLog;
// 						ctkEvent event = ctkEvent(SFxEquipmentInfoEventConstants::Topic().c_str(), props);
// 						eventService->sendEvent(event);


						ctkDictionary propsEquipInfo;
						QVariant varLogsEquipInfo;
						SEquipInfo sSEquipInfo;
						sSEquipInfo.strType = "航空器";
						sSEquipInfo.strID = "卫星 " + sSpacecraftObjectData.sEntityObjectEleData.sTextObjectEleData.strText;//"发现号 OV-103";
						sSEquipInfo.strIconPath = "texture/weixing.png";
						sSEquipInfo.dEnergyNum = 0.5;
						sSEquipInfo.dLoadNum = 0.33;
						sSEquipInfo.dParaA = 268;
						sSEquipInfo.dParaB = 97;
						sSEquipInfo.dParaC = 255;
						varLogsEquipInfo.setValue(sSEquipInfo);
						propsEquipInfo[SFxEquipInfoEventConstants::EquipInfo().c_str()] = varLogsEquipInfo;
						ctkEvent eventEquipInfo = ctkEvent(SFxEquipInfoEventConstants::Topic().c_str(), propsEquipInfo);
						eventService->sendEvent(eventEquipInfo);
					}
				}

			}
		}

	}

	//void CDriveMgr::addAttachUnit(AttachBase* pAttachBase)
	//{
	//	switch (pAttachBase->eEnAttachType)
	//	{
	//	case Radar_Beam:
	//	{
	//		
	//	}break;
	//	case Radar_Senor:
	//	{
	//		RadarSenorData *pRadarSenor = dynamic_cast<RadarSenorData*> (pAttachBase);
	//		if (pRadarSenor)
	//		{
	//			osg::ref_ptr<HGxObject::CGxRadarScanOption> rpRadarScan = new HGxObject::CGxRadarScanOption;
	//			rpRadarScan->strID = "RadarScan";
	//			rpRadarScan->strName = QString::fromUtf8("RadarScan").toStdString();
	//			rpRadarScan->strID = QString("%1_%2").arg(pRadarSenor->nEntityID).arg(pRadarSenor->nId).toStdString();
	//			rpRadarScan->entityID() = QString::number(pRadarSenor->nEntityID).toStdString();
	//			rpRadarScan->parent() = QString::number(pRadarSenor->nEntityID).toStdString();
	//			rpRadarScan->radius() = pRadarSenor->dRaduis;
	//			rpRadarScan->azMin() = pRadarSenor->dAzStartAngle;
	//			rpRadarScan->azMax() = pRadarSenor->dAzEndAngle;
	//			rpRadarScan->elevMin() = pRadarSenor->dPicthStartAngle;
	//			rpRadarScan->elevMax() = pRadarSenor->dPicthEndAngle;
	//			//rpRadarScan->lineColor() = pRadarSenor->;
	//			//rpRadarScan->elfinColor() = redRadarSurfaceColor;
	//			//rpRadarScan->color() = redRadarSurfaceColor;
	//			m_pObjectInstence->addObject(*rpRadarScan.get(), false);
	//			m_pObjectsIndexMap[rpRadarScan->strID] = rpRadarScan.get();
	//		}
	//	}break;
	//	case Air_SpeceSector:
	//	{
	//		AirSpeceSectorData *pAirSpeceSectorInfo = dynamic_cast<AirSpeceSectorData*> (pAttachBase);
	//		if (pAirSpeceSectorInfo)
	//		{
	//			osg::ref_ptr<HGxObject::CGxAirSpaceSectorOption> rpAirSpaceSector = new HGxObject::CGxAirSpaceSectorOption;
	//			osg::Vec4  veccolor = osg::Vec4(pAirSpeceSectorInfo->vecColor[0], pAirSpeceSectorInfo->vecColor[1], pAirSpeceSectorInfo->vecColor[2], pAirSpeceSectorInfo->vecColor[3]);
	//			veccolor.a() = 0.8;
	//			rpAirSpaceSector->bottomColor() = veccolor;
	//			veccolor.a() = 0.2;
	//			rpAirSpaceSector->topColor() = veccolor;
	//			HGxObject::SLineStyle style;
	//			style.vecColor = veccolor;
	//			rpAirSpaceSector->topLineStyle() = style;
	//			rpAirSpaceSector->strID = QString("AirSpace_%1").arg(pAirSpeceSectorInfo->nId).toStdString(); 
	//			rpAirSpaceSector->isTerrain() = true;
	//			rpAirSpaceSector->center() = osg::Vec3d(pAirSpeceSectorInfo->vecPosition[0], pAirSpeceSectorInfo->vecPosition[1], pAirSpeceSectorInfo->vecPosition[2]);
	//			rpAirSpaceSector->startAngle() = pAirSpeceSectorInfo->dStartAngle;
	//			rpAirSpaceSector->swingAngle() = pAirSpeceSectorInfo->dSwingAngle;
	//			rpAirSpaceSector->radius() = pAirSpeceSectorInfo->dRadius;
	//			rpAirSpaceSector->height() = pAirSpeceSectorInfo->dH;

	//			m_pObjectInstence->addObject(*rpAirSpaceSector.get(), false);
	//			m_pObjectsIndexMap[rpAirSpaceSector->strID] = rpAirSpaceSector.get();
	//		}

	//	}break;
	//	case Aim:
	//	{
	//		AimSenorData *pAimInfoMsg = dynamic_cast<AimSenorData*> (pAttachBase);
	//		if (pAimInfoMsg)
	//		{
	//			osg::ref_ptr<HGxObject::GxEarthLockEle> pLockEle = new HGxObject::GxEarthLockEle;
	//			pLockEle->strID = QString("Aim_%1_%2").arg(pAimInfoMsg->nEntityID).arg(pAimInfoMsg->nId).toStdString();
	//			pLockEle->sendEntityID() = QString::number(pAimInfoMsg->nEntityID).toStdString();
	//			pLockEle->receiveEntityID() = QString::number(pAimInfoMsg->nTargetId).toStdString();
	//			pLockEle->animationTime() = osg::minimum(pAimInfoMsg->dContinueTime*0.2, 0.5);
	//			pLockEle->lockTime() = osg::maximum(pAimInfoMsg->dContinueTime*0.2, 2.0);
	//			pLockEle->lockNum() = (pAimInfoMsg->dContinueTime*0.6) / pLockEle->lockTime();
	//			pLockEle->lockColor() = osg::Vec4(0.0, 0, 1.0, 0.5);
	//			pLockEle->lockCircleRatio() = 5.0;
	//			pLockEle->circleColor() = osg::Vec4(1, 1, 1, 0.01);

	//			m_pObjectInstence->addObject(*pLockEle.get(), false);
	//			m_pObjectsIndexMap[pLockEle->strID] = pLockEle.get();
	//		}
	//	}break;
	//	case Label:
	//	{
	//		LabelInfo *pLabelInfo = dynamic_cast<LabelInfo*> (pAttachBase);
	//		if (pLabelInfo)
	//		{
	//			osg::ref_ptr<HGxObject::GxLabelEle> sLabelEle = new HGxObject::GxLabelEle;
	//			std::string strPara = QString::number(pLabelInfo->nEntityID).toStdString();;
	//			sLabelEle->entityID() = strPara;
	//			sLabelEle->parent() = strPara;
	//			strPara = "Label_" + strPara;
	//			sLabelEle->strID = strPara;
	//			strPara = pLabelInfo->strName;
	//			sLabelEle->strName = strPara;
	//			sLabelEle->lineColor() = pLabelInfo->vecLineColor;
	//			sLabelEle->text() = pLabelInfo->strPara;
	//			sLabelEle->backgroundColor() = pLabelInfo->vecBackgroundColor;
	//			sLabelEle->textColor() = pLabelInfo->vecTextColor;
	//			sLabelEle->width() = 3.0;
	//			sLabelEle->fontSize() = 10;

	//			m_pObjectInstence->addObject(*sLabelEle.get(), false);
	//			m_pObjectsIndexMap[sLabelEle->strID] = sLabelEle.get();
	//		}
	//	}break;
	//	case Earth_Lock:
	//	{
	//	
	//	}break;
	//	case Track_Line:
	//	{
	//		osg::ref_ptr<HGxObject::CGxTrackLineEle> rpTrackLine = new HGxObject::CGxTrackLineEle;
	//		std::string strID = QString::number(pAttachBase->nEntityID).toStdString();
	//		rpTrackLine->entityID() = strID;
	//		rpTrackLine->parent() = strID;
	//		strID = strID + "TrackLine";
	//		rpTrackLine->strID = strID;
	//		rpTrackLine->nameEle()->text() = strID;
	//		//opWakeFlame->color() = osg::Vec4(sNewObjectData.nameColor[0], sNewObjectData.nameColor[1], sNewObjectData.nameColor[2], sNewObjectData.nameColor[3]);
	//		
	//		m_pObjectInstence->addObject(*rpTrackLine.get(), false);
	//		m_pObjectsIndexMap[rpTrackLine->strID] = rpTrackLine.get();
	//	}break;
	//	case Wake_Ribbon:
	//	{
	//		osg::ref_ptr<HGxObject::CWakeRibbonEle> rpWakeRibbon = new HGxObject::CWakeRibbonEle;
	//		std::string strID = QString::number(pAttachBase->nEntityID).toStdString();
	//		rpWakeRibbon->entityID() = strID;
	//		rpWakeRibbon->parent() = strID;
	//		rpWakeRibbon->strID = strID + "WakeRibbon";
	//		rpWakeRibbon->strName = strID;
	//		rpWakeRibbon->size() = 5000;
	//		rpWakeRibbon->width() = 10;

	//		m_pObjectInstence->addObject(*rpWakeRibbon.get(), false);
	//		m_pObjectsIndexMap[rpWakeRibbon->strID] = rpWakeRibbon.get();
	//	}
	//	break;
	//	default:
	//		break;
	//	}
	//}

	//void CDriveMgr::updateAttachUnit(AttachBase* pAttachBase)
	//{

	//}

	void CDriveMgr::addSensorUnit(HGxObject::CGxEffectEle* sObjectEle)
	{
		m_pObjectInstence->addObject(*sObjectEle, false);
		m_pObjectsIndexMap[sObjectEle->strID] = sObjectEle;
		if (sObjectEle)
		{
			HGxObject::CGxEffectEle::EffectWorkingList listWorkingList;
			sObjectEle->getEffectWorkingList(listWorkingList);
			if (listWorkingList.size() != 0)
			{
				HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
				if (eventService)
				{
					ctkDictionary props;
					QVariant varLog;
					SGanttChartInfo stGanttChartInfo;
					stGanttChartInfo.m_strID = sObjectEle->strID.data();
					stGanttChartInfo.m_strName = sObjectEle->strName.data();
					//stGanttChartInfo.m_strType = sObjectEle->entityType().data();
					stGanttChartInfo.m_vecTimePari = listWorkingList;
					stGanttChartInfo.m_colorEvent = QColor(255, 128, 0);
					varLog.setValue(stGanttChartInfo);
					props[SFxGanttChartInfoEventConstants::GanttChartInfoEvent().c_str()] = varLog;
					ctkEvent event = ctkEvent(SFxGanttChartInfoEventConstants::Topic().c_str(), props);
					eventService->sendEvent(event);
				}
			}
		}
	}

	void CDriveMgr::destroyUnit(std::string destroyID)
	{
		if (m_pObjectInstence)
		{
			if (m_pObjectsIndexMap.count(destroyID))
			{
				HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
				if (eventService)
				{
					QString str = destroyID.data();

					ctkDictionary props;
					QVariant varLog;
					SAlarmInfo sAlarmInfo;
					sAlarmInfo.level = static_cast<ELevel>(1);
					sAlarmInfo.time = QTime::currentTime();
					sAlarmInfo.data = QDate::currentDate();
					sAlarmInfo.info = str;
					varLog.setValue(sAlarmInfo);
					props[SFxAlarmInfoEventConstants::AlarmInfo().c_str()] = varLog;
					ctkEvent event = ctkEvent(SFxAlarmInfoEventConstants::Topic().c_str(), props);
					eventService->sendEvent(event);
				}
				ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
				if (pSceneUiService)
				{
					ISceneInterface* pISceneInterface = pSceneUiService->scene(QStringLiteral("3D"));
					if (pISceneInterface)
					{
						if (pISceneInterface->fxScene())
						{
							HGxObject::CGxEntityNode* pEntity = dynamic_cast<HGxObject::CGxEntityNode*>(HGxObject::CGxObjectManager::getInstance()->getObjectNode(destroyID, pISceneInterface->fxScene()->getID()));
							if (pEntity != NULL)
							{
								// 				//爆炸对象
								HGxObject::CGxExplosionOption *pExplosionEle = new HGxObject::CGxExplosionOption();
								//std::vector<std::string> vecID;
								//vecID.push_back(m_strSceneID);
								std::string strID = "test" + destroyID;
								pExplosionEle->strID = strID;
								pExplosionEle->size() = 50;

								//pExplosionEle->parent() = destroyID;
								pExplosionEle->entityID() = destroyID;
								pExplosionEle->internalPosition() = pEntity->getObjectEle()->asElemObjectEle()->internalPosition();
								pExplosionEle->size() = 1000;
								pExplosionEle->play() = true;
								m_pObjectInstence->addObject(*pExplosionEle, 0);
								m_pObjectsIndexMap[strID] = pExplosionEle;
							}
						}
					}
				}

				m_pObjectInstence->removeObject(destroyID);
				m_pObjectsIndexMap[destroyID] = NULL;
				m_pObjectsIndexMap.erase(destroyID);

				if (eventService)
				{
					ctkDictionary props;
					QVariant varLog;
					SRemoveUintInfo sSEquipmentInfo;
					sSEquipmentInfo.ID = destroyID;
					varLog.setValue(sSEquipmentInfo);
					props[SFxRemoveUintEventConstants::RemoveUintInfo().c_str()] = varLog;
					ctkEvent event = ctkEvent(SFxRemoveUintEventConstants::Topic().c_str(), props);
					eventService->sendEvent(event);
				}
			}
		}
	}

	void CDriveMgr::firstTrackUnit(const std::string strFirstTrackID)
	{

	}

	void CDriveMgr::thirdTrackUnit(const std::string strThirdTrackID)
	{

		ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
		if (pSceneUiService)
		{
			ISceneInterface* pISceneInterface = pSceneUiService->scene(QStringLiteral("3D"));
			if (pISceneInterface)
			{
				if (pISceneInterface->fxScene())
				{
					HGxObject::CGxEntityNode* pEntity = dynamic_cast<HGxObject::CGxEntityNode*>(HGxObject::CGxObjectManager::getInstance()->getObjectNode(strThirdTrackID, pISceneInterface->fxScene()->getID()));
					if (pEntity == NULL)
					{
						return;
					}
					HGxScene::CGxManipulatorManager* pGxManipulatorManager = pISceneInterface->fxScene()->getManipulatorSystem();
					if (pGxManipulatorManager)
					{
						pGxManipulatorManager->activeManipulator(NODE_TRACKING_MANIPULATOR_KEY);
						HGxScene::CGxNodeTrackingManipulator* pManipulator = dynamic_cast<HGxScene::CGxNodeTrackingManipulator*>(pGxManipulatorManager->getCurrentManipulator());
						if (pManipulator)
						{
							pManipulator->setTrackMode(HGxScene::CGxNodeTrackingManipulator::E_Track_LockNodePosOnly);
							pManipulator->setTrackingNode(pEntity->getEntityRotateMT());
							pManipulator->setLockModeHomeParam(2300, -10.0, 90.0);
						}

// 						if (!pGxManipulatorManager->activeManipulator(THIRD_LOCK_MANIPULATOR_KEY))
// 						{
// 							return;
// 						}
// 
// 						HGxScene::CGxLockManipulator* pCurrentManiputator = dynamic_cast<HGxScene::CGxLockManipulator*> (pGxManipulatorManager->getCurrentManipulator());
// 
// 						if (pCurrentManiputator)
// 						{
// 							HGxObject::CGxEntityNode* pGxObjectNode = dynamic_cast<HGxObject::CGxEntityNode*> (HGxObject::CGxObjectManager::getInstance()->getObjectNode(strThirdTrackID, pISceneInterface->fxScene()->getID()));
// 							if (pGxObjectNode)

// 							{
// 								pCurrentManiputator->setLockType(HGxScene::CGxLockManipulator::E_THIRD_PERSON);
// 								pCurrentManiputator->setNode(pGxObjectNode->getEntityRotateMT());

// 								pCurrentManiputator->setThirdPersonHomeParam(23000, 50.0, -10.0);

// 							}
// 						}

					}

				}
			}
		}
	}

	void CDriveMgr::clearUnit()
	{
		//异步发送事件
		ctkDictionary event;
		HGxEventHelperInstance.sendEvent("event/clearScene", event);
		if (m_pObjectInstence)
		{
			MapObjectsIndex::iterator itr = m_pObjectsIndexMap.begin();
			for (itr;itr != m_pObjectsIndexMap.end();++itr)
			{
				m_pObjectInstence->removeObject(itr->first);
				m_pObjectsIndexMap[itr->first] = NULL;
			}
			m_pObjectsIndexMap.clear();
		}
		ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
		if (pSceneUiService)
		{
			ISceneInterface* pISceneInterface = pSceneUiService->scene(QStringLiteral("3D"));
			if (pISceneInterface)
			{
				if (pISceneInterface->fxScene())
				{
					HGxScene::CGxManipulatorManager* pGxManipulatorManager = pISceneInterface->fxScene()->getManipulatorSystem();
					if (pGxManipulatorManager)
					{
						if (!pGxManipulatorManager->activeManipulator(FREE_EARTH_MANIPULATOR_KEY))
						{
							return;
						}
						HGxScene::CGxEarthManipulator* pManipulator = static_cast<HGxScene::CGxEarthManipulator*> (pGxManipulatorManager->getCurrentManipulator());
						if (pManipulator)
						{
							pManipulator->locateHome();
						}	
					}
				}
			}
		}
	}

}
