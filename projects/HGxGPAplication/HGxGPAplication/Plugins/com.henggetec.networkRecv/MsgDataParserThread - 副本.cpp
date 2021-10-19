#include "MsgDataParserThread.h"
#include <osg/CullFace>

#include <QMetaType>
#include <osg/Vec4>
#include "bdpNet\GxNetworkManager.h"
#include "NetworkRecv.h"
#include "GxUdpParam.h"
#include "bdpDB/GxRegister.h"
#include "bdpAlgor/GxCoordConverter.h"
#include "GxPluginFwLauncherInterface.h"
#include "IDriveMgrInterface.h"
#include "GxServiceQuerier.h"
#include "common/UnitCommon.h"
//#include "bdpObject/GxExplosionNode.h"
#include "service/event/ctkEventAdmin.h"
#include "common/UnitEventCommon.h"
#include "PlatformPlugin/object/GxRadarScanOption.h"
#include "PlatformPlugin/object/GxAirSpaceSectorOption.h"
#include "PlatformPlugin/object/GxMarkEffectLockOption.h"
#include "../HGxBasicDevPlat/src/bdpObject/GxObjectManager.h"
#include "PlatformPlugin/object/GxTrackLineCommon.h"
#include "PlatformPlugin/object/GxWakeRibbonCommon.h"
#include "PlatformPlugin/object/GxLabelCommon.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

//////////////////////////////////////////////////////////////////////////
const osg::Vec4 redRadarSurfaceColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarConeColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarSideColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarSurfaceLineColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarConeLineColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarSideLineColor(1,106.0/255,106.0/255, 0.3);
const osg::Vec4 redRadarOutLineColor(1,106.0/255,106.0/255, 1);

const osg::Vec4 blueRadarSurfaceColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarConeColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarSideColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarSurfaceLineColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarConeLineColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarSideLineColor(106.0/255,165.0/255, 1, 0.3);
const osg::Vec4 blueRadarOutLineColor(106.0/255,165.0/255, 1,1);

std::string DataEleConvertor::getEntityID(int id)
{
	return QString("%1").arg(id).toStdString();
}

std::string DataEleConvertor::getSensorID(int sensorOwnerID, int sensorID)
{
	return QString("%1_%2").arg(sensorOwnerID).arg(sensorID).toStdString();
}

std::string DataEleConvertor::getAirSpaceID(int enId)
{
	return QString("AirSpace_%1").arg(enId).toStdString();
}

std::string DataEleConvertor::getCommAttriID(int nCommEffectOwnerEntityID)
{
	return QString("commEffect_%1").arg(nCommEffectOwnerEntityID).toStdString();
}

std::string DataEleConvertor::getDetectAttriID(int nEntityID, int nTargetID)
{
	return QString("detect_%1_%2").arg(nEntityID).arg(nTargetID).toStdString();
}

std::string DataEleConvertor::getHitEffectID(int nTargetId)
{
	return QString("hitEffect_%1").arg(nTargetId).toStdString();
}

std::string DataEleConvertor::getAimEffectID( int nEntityID, int nTargetID )
{
	return QString("Aim_%1_%2").arg(nEntityID).arg(nTargetID).toStdString();
}

bool DataEleConvertor::isCommEffectVisible(unsigned short usCommuState)
{
	return (LINK_CON == usCommuState);
}

osg::Vec4 DataEleConvertor::getPathColor( int camp )
{
	osg::Vec4 color(1, 1, 1, 1);
	switch(camp)
	{
	case 1://红方
		color = osg::Vec4(255.0/255.0, 106.0/255.0, 106.0/255.0, 0.5);
		break;
	case 2://蓝方
		color = osg::Vec4(106.0/255.0, 165.0/255.0, 255.0/255.0, 0.5);
	default:
		break;
	}
	return color;
}

osg::Vec4 DataEleConvertor::getModelColor( int camp )
{
	osg::Vec4 color(1, 1, 1, 1);
	switch(camp)
	{
	case 1://红方
		color = osg::Vec4(255.0/255.0, 18.0/255.0, 68.0/255.0, 1);
		break;
	case 2://蓝方
		color = osg::Vec4(57.0/255.0, 136.0/255.0, 255.0/255.0, 1);
	default:
		break;
	}
	return color;
}

SNewObjectData DataEleConvertor::getEntityAttri(const NewEntity &newEntityInfo)
{
	SNewObjectData sNewAircraftObjectData;

	// 不同的业务实体类型显示样式等设置
	std::string modelPath = "";
	std::string imagePath = "";
	sNewAircraftObjectData.enEquType = newEntityInfo.enEquType;
	switch (newEntityInfo.enEquType)
	{
	case AIRCAFT_EUT:
	{
		modelPath = "Model/entity/su-27.ive";
		imagePath = "Model/F-18.png";
	}
	break;
	case TANK_EUT:
	{
		if (newEntityInfo.enCamp == BLUE_CAMP)
		{
			modelPath = "Model/entity/t-90_blue.ive";
		}
		else if (newEntityInfo.enCamp == RED_CAMP)
		{
			modelPath = "Model/entity/t-90_red.ive";
		}
		imagePath = "Model/Tank.png";
	}
	break;
	case MISSILE_VECHICLE_EUT:
	{
		if (newEntityInfo.enCamp == BLUE_CAMP)
		{
			modelPath = "Model/entity/DF-100_blue.ive";
		}
		else if (newEntityInfo.enCamp == RED_CAMP)
		{
			modelPath = "Model/entity/DF-100_red.ive";
		}
		imagePath = "Model/entity/DF-100.png";
	}
	break;
	case MISSILE_EUT:
	{
		modelPath = "Model/entity/daodan.ive";
		imagePath = "Model/entity/daodan.png";
	}
	break;
	case CARRIER_EUT:
	{
		modelPath = "Model/entity/hangmu.ive";
		imagePath = "Model/entity/hangmu.png";
	}
	break;
	case FRIGATE_EUT:
	{
		modelPath = "Model/054.ive";
		imagePath = "Model/054.png";
	}
	break;
	case COPTER_EUT:
	{
		modelPath = "Model/WZ_9.ive";
	}
	default:
		break;
	}

	if (newEntityInfo.sepNum == 0)
	{
		if (newEntityInfo.nComPriId > 0)
		{
			if (newEntityInfo.enEquType == MISSILE_EUT)
			{
				if (newEntityInfo.enCamp == BLUE_CAMP)
				{
					modelPath = "Model/entity/PL-15_blue.ive";
				}
				else if (newEntityInfo.enCamp == RED_CAMP)
				{
					modelPath = "Model/entity/PL-15_red.ive";
				}
			}
		}
	}

	if (newEntityInfo.sepNum == 0)
	{
		QString strName = newEntityInfo.cName;
		if (strName.contains("坦克"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 200, 30);
		}
		else if (strName.contains("导弹车") || strName.contains("指挥车"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 100, 20);
		}
		else if (strName.contains("飞机"))
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 120, 25);
		}
		else if (newEntityInfo.nComPriId > 0 && newEntityInfo.enEquType == MISSILE_EUT)
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 50, 2.0);
		}
		else
		{
			//entityAttri->modelEle()->setAutoScaleDetial(true, 1, 30, 2.5);
		}
	}
	else
	{

	}

	sNewAircraftObjectData.dTime = newEntityInfo.dTime;
	std::string strID = std::to_string(_Longlong(newEntityInfo.nEntityID));
	strcpy(sNewAircraftObjectData.strID, strID.c_str());
	strcpy(sNewAircraftObjectData.strModelFilePath, modelPath.c_str());

	strcpy(sNewAircraftObjectData.strName, newEntityInfo.cName);
	//sNewAircraftObjectData.sPreEquipInfo = m_mapEquipInfoData[strModelName];
	if (newEntityInfo.enCamp == 1)  //红方
	{
		sNewAircraftObjectData.modelColor[0] = 1.0;
		sNewAircraftObjectData.modelColor[1] = 0.0;
		sNewAircraftObjectData.modelColor[2] = 0.0;
		sNewAircraftObjectData.modelColor[3] = 1.0;
		sNewAircraftObjectData.nameColor[0] = 1.0;
		sNewAircraftObjectData.nameColor[1] = 0.0;
		sNewAircraftObjectData.nameColor[2] = 0.0;
		sNewAircraftObjectData.nameColor[3] = 1.0;
	}
	else if (newEntityInfo.enCamp == 2)//蓝方
	{
		sNewAircraftObjectData.modelColor[0] = 0.0;
		sNewAircraftObjectData.modelColor[1] = 0.0;
		sNewAircraftObjectData.modelColor[2] = 1.0;
		sNewAircraftObjectData.modelColor[3] = 1.0;
		sNewAircraftObjectData.nameColor[0] = 0.0;
		sNewAircraftObjectData.nameColor[1] = 0.0;
		sNewAircraftObjectData.nameColor[2] = 1.0;
		sNewAircraftObjectData.nameColor[3] = 1.0;
	}
	sNewAircraftObjectData.modelScale[0] = 10.0;
	sNewAircraftObjectData.modelScale[1] = 10.0;
	sNewAircraftObjectData.modelScale[2] = 10.0;
	sNewAircraftObjectData.nameFontSize = 20.0;
	sNewAircraftObjectData.imageImageSize[0] = 64.0;
	sNewAircraftObjectData.imageImageSize[1] = 64.0;

	strcpy(sNewAircraftObjectData.strImageFilePath, imagePath.c_str());
	sNewAircraftObjectData.vecPosture[0] = newEntityInfo.dPicthAngle;
	sNewAircraftObjectData.vecPosture[1] = newEntityInfo.dRolAngle; 
	sNewAircraftObjectData.vecPosture[2] = -newEntityInfo.dAzAngle;

	osg::Vec3d vecLLH = osg::Vec3d(newEntityInfo.dLon, newEntityInfo.dLat, newEntityInfo.dAlt);
// 	osg::Vec3d vecXYZ = osg::Vec3d(0, 0, 0);
// 	HGxAlgor::DegreeLLH2WGS84XYZ(vecLLH, vecXYZ);

	sNewAircraftObjectData.vecPositionLLH[0] = vecLLH.x();//lists.at(10).toDouble();
	sNewAircraftObjectData.vecPositionLLH[1] = vecLLH.y();//lists.at(11).toDouble();
	sNewAircraftObjectData.vecPositionLLH[2] = vecLLH.z();//lists.at(12).toDouble();

	sNewAircraftObjectData.nDriveStyle = 1;

	return sNewAircraftObjectData;
}



//////////////////////////////////////////////////////////////////////////
//
void CSimControlCenter::executeMsg(TimeMsg& msg)
{
	emit sig_timeUpdate(msg.dTime);
}

void CSimControlCenter::executeMsg(ScreenWordInfo& msg)
{
	std::string screenInfo = msg.chMsg;
	emit sig_screenInfoNotify(screenInfo);
}

void CSimControlCenter::executeMsg(EventCommand& msg)
{
	std::string eventCmd(msg.chDescript);
	HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
	if (eventService)
	{
		QString str = msg.chDescript;
		
		ctkDictionary props;
		QVariant varLog;
		SAlarmInfo sAlarmInfo;
		if (str.endsWith("摧毁", Qt::CaseSensitive) || str.endsWith("命中", Qt::CaseSensitive))
		{
			sAlarmInfo.level = static_cast<ELevel>(1);
		}
		else
		{
			sAlarmInfo.level = static_cast<ELevel>(0);
		}
		sAlarmInfo.time = QTime::currentTime();
		sAlarmInfo.data = QDate::currentDate();
		sAlarmInfo.info = msg.chDescript;
		varLog.setValue(sAlarmInfo);
		props[SFxAlarmInfoEventConstants::AlarmInfo().c_str()] = varLog;
		ctkEvent event = ctkEvent(SFxAlarmInfoEventConstants::Topic().c_str(), props);
		eventService->sendEvent(event);
	}
}

void CSimControlCenter::executeMsg(HitCommand& msg)
{
	std::string entityID = DataEleConvertor::getEntityID(msg.nEntityID);
	std::string targetID = DataEleConvertor::getEntityID(msg.nTargetId);

// 	osg::ref_ptr<FxSceneObject::CFxSceneObject> objEntity = getSceneObjService()->getObject(entityID);
// 	if (!objEntity) return;
// 	osg::ref_ptr<FxSceneObject::CFxSceneObject> objTarget = getSceneObjService()->getObject(targetID);
// 	if (!objTarget) return;

	// 创建击中特效
//	HGxObject::CGxExplosionEle* hitEle = new HGxObject::CGxExplosionEle;
//	hitEle->strID = DataEleConvertor::getHitEffectID(msg.nTargetId);
// 	hitEle->primaryEntityID() = targetID;
// 	hitEle->explosionTime() = 3;
// 	hitEle->autoSetExplosionSize() = true;
// 	hitEle->explosionRatio() = 3.0;
// 	hitEle->startExplosion() = true;
// 	hitEle->explosionMode() = FxEffectObject::CFxExplosionEle::CYCLE_EXPLOSION;
// 	getSceneObjService()->addObject(hitEle);
}

void CSimControlCenter::executeMsg(DestoryCommand& msg)
{
	if (m_bStopThreads) return;

	std::string entityID = DataEleConvertor::getEntityID(msg.nEntityID);

	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->destroyUnit(entityID);
	}
}

void CSimControlCenter::executeMsg( AimInfoMsg& msg )
{
	std::string entityID = DataEleConvertor::getEntityID(msg.nEntityId);
	std::string targetID = DataEleConvertor::getEntityID(msg.nTargetId);
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	std::string strID = DataEleConvertor::getAimEffectID(msg.nEntityId, msg.nTargetId);
	HGxObject::CGxObjectBase* pCGxObjectBase = HGxObject::CGxObjectManager::getInstance()->getObject(strID);
	if (pCGxObjectBase == NULL)
	{
		osg::ref_ptr<HGxObject::GxEarthLockEle> pLockEle = new HGxObject::GxEarthLockEle;
		pLockEle->strID = DataEleConvertor::getAimEffectID(msg.nEntityId, msg.nTargetId);
		pLockEle->sendEntityID() = entityID;
		pLockEle->receiveEntityID() = targetID;
		pLockEle->animationTime() = osg::minimum(msg.dContinueTime*0.2, 0.5);
		pLockEle->lockTime() = osg::maximum(msg.dContinueTime*0.2, 2.0);
		pLockEle->lockNum() = (msg.dContinueTime*0.6) / pLockEle->lockTime();
		pLockEle->lockColor() = osg::Vec4(0.0, 0, 1.0, 0.5);
		pLockEle->circleColor() = osg::Vec4(1, 1, 1, 0.01);

		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(pLockEle);
		}
	}
	else
	{
		if (!msg.bAim)
		{
			if (driveMgrInterface)
			{
				driveMgrInterface->destroyUnit(strID);
			}
		}
	}
}

void CSimControlCenter::executeMsg( ViewInfoMsg& msg )
{
	//界面不是自动漫游时就不发送视点信息
	if(!m_bNeedViewInfo)
	{
		return;	
	}
	switch(msg.enViewType)
	{
	case EN_LOCAT_VIEW:
		{
			ViewLocatCmd locatCmd;
			memcpy(&locatCmd, msg.chMsg, sizeof(ViewLocatCmd));
			emit setViewSignal(locatCmd);
		}
		break;
	case EN_PEER_VIEW:
		{
			PeerView peerView;
			memcpy(&peerView, msg.chMsg, sizeof(PeerView));
			emit setViewSignal(peerView);
		}
		break;
	case EN_PEERE2T_VIEW:
		{
			PeerE2TView peer2View;
			memcpy(&peer2View, msg.chMsg, sizeof(PeerE2TView));
			emit setViewSignal(peer2View);
		}
		break;
	case EN_TRACK_VIEW:
		{
			TracView tracView;
			memcpy(&tracView, msg.chMsg, sizeof(TracView));
			emit setViewSignal(tracView);
		}
		break;
	case EN_TRAVE_VIEW:
		{
			TraveView traveView;
			memcpy(&traveView,msg.chMsg,sizeof(TraveView));
			emit setViewTraveSignal(traveView);
		}
		break;
	case EN_TRAVETARGET_VIEW:
		{
			TraveTargetView traveTargetView;
			memcpy(&traveTargetView,msg.chMsg,sizeof(TraveTargetView));

			emit setViewTraveTargetSignal(traveTargetView);
		}
		break;
	default:
		break;
	}
}

void CSimControlCenter::executeMsg(DetectCommand& msg)
{
	std::string detectEffectID = DataEleConvertor::getDetectAttriID(msg.nEntityID, msg.nTargetId);

	// TRACK_STATE - 跟踪 UNTRACK_STATE-取消跟踪 
	switch (msg.enTrackState)
	{
	case TRACK_STATE:
		{

		}
		break;
	case UNTRACK_STATE:
		{

		}
		break;
	default:
		break;
	}
}

void CSimControlCenter::executeMsg(EntityStu& msg)
{
	SDriveObjectData sDriveObjectData;

	sDriveObjectData.dTime = msg.dTime;
	std::string strID = std::to_string(_Longlong(msg.nEntityID));
	strcpy(sDriveObjectData.strID, strID.c_str());
	sDriveObjectData.vecPosture[0] = msg.dPicthAngle;
	sDriveObjectData.vecPosture[1] = msg.dRolAngle;
	sDriveObjectData.vecPosture[2] = -msg.dAzAngle;

// 	osg::Vec3d vecLLH = osg::Vec3d(msg.dLon, msg.dLat, msg.dAlt);
// 	osg::Vec3d vecXYZ = osg::Vec3d(0, 0, 0);
// 	HGxAlgor::DegreeLLH2WGS84XYZ(vecLLH, vecXYZ);

	sDriveObjectData.vecPositionLLH[0] = msg.dLon;
	sDriveObjectData.vecPositionLLH[1] = msg.dLat;
	sDriveObjectData.vecPositionLLH[2] = msg.dAlt;

	sDriveObjectData.nDriveStyle = 0;

	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->driveUnit(sDriveObjectData);
	}
}

void CSimControlCenter::executeMsg(Senor& msg)
{
	std::string sensorID = DataEleConvertor::getSensorID(msg.nEntityID, msg.nSeorId);

	HGxObject::CGxObjectBase* pCGxObjectBase = HGxObject::CGxObjectManager::getInstance()->getObject(sensorID);
	if (pCGxObjectBase)
	{
		if (pCGxObjectBase->getObjectOption())
		{
			HGxObject::CGxRadarScanOption* radarEle = dynamic_cast<HGxObject::CGxRadarScanOption*>(pCGxObjectBase->getObjectOption());
			if (radarEle == NULL)
			{
				return;
			}
			radarEle->radius() = msg.dRaduis;
			radarEle->azMin() = msg.dAzStartAngle;
			radarEle->azMax() = msg.dAzEndAngle;
			radarEle->elevMin() = msg.dPicthStartAngle;
			radarEle->elevMax() = msg.dPicthEndAngle;
		}
	}
}

void CSimControlCenter::executeMsg(NewEntity& msg)
{
	SNewObjectData sNewObjectData;

	sNewObjectData = DataEleConvertor::getEntityAttri(msg);

	if (msg.nComPriId != 0)
	{

	}
	else
	{

	}

	if (msg.enEquType == AIRCAFT_EUT)
	{
// 		if (RED_CAMP == msg.enCamp)
// 		{
// 			entityAttri->modelEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("Model/AircraftRed.ive");
// 		}
// 		else
// 		{
// 			entityAttri->modelEle()->filePath() = HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("Model/AircraftBlue.ive");
// 		}
	}
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->addUnit(sNewObjectData);
	}

	if (sNewObjectData.enEquType == MISSILE_EUT)
	{

		osg::ref_ptr<HGxObject::CGxTrackLineEle> opWakeFlame = new HGxObject::CGxTrackLineEle;
		std::string strID = sNewObjectData.strID;
		opWakeFlame->entityID() = strID;
		strID = strID + "TrackLine";
		opWakeFlame->strID = strID;
		opWakeFlame->nameEle()->text() = strID;
		opWakeFlame->color() = osg::Vec4(sNewObjectData.nameColor[0], sNewObjectData.nameColor[1], sNewObjectData.nameColor[2], sNewObjectData.nameColor[3]);
		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(opWakeFlame);
		}
	}
	else if (sNewObjectData.enEquType == AIRCAFT_EUT)
	{
		osg::ref_ptr<HGxObject::CWakeRibbonEle> opWakeRibbon = new HGxObject::CWakeRibbonEle;
		std::string strID = sNewObjectData.strID;
		opWakeRibbon->entityID() = strID;
		opWakeRibbon->strID = strID + "WakeRibbon";
		opWakeRibbon->nameEle()->text() = strID;
		opWakeRibbon->size() = 5000;
		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(opWakeRibbon);
		}
		osg::ref_ptr<HGxObject::GxLabelEle> sLabelEle = new HGxObject::GxLabelEle;
		std::string strPara = sNewObjectData.strID;
		sLabelEle->entityID() = strPara;
		strPara = "Label_" + strPara;
		sLabelEle->strID = strPara;
		sLabelEle->nameEle()->text() = strPara;
		strPara = "角色：被观察点";
		sLabelEle->text() = strPara;
		sLabelEle->lineColor() = osg::Vec4(1.0, 0.1, 0.0, 1.0);
		sLabelEle->backgroundColor()= osg::Vec4(0.7, 0.7, 0.7, 1.0);
		sLabelEle->textColor() = osg::Vec4(0.0, 0.0, 0.0, 1.0);
		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(sLabelEle);
		}
	}

	// 传感器数量
	if (msg.nSenorNum > 0)
	{
		for (int n = 0; n < msg.nSenorNum; ++n)
		{
			osg::ref_ptr<HGxObject::CGxRadarScanOption> rpRadarScan = new HGxObject::CGxRadarScanOption;
			rpRadarScan->strID = "RadarScan";
			rpRadarScan->nameEle()->text() = QString::fromUtf8("RadarScan").toStdString();
			rpRadarScan->strID = DataEleConvertor::getSensorID(msg.senor[n].nEntityID, msg.senor[n].nSeorId); 
			rpRadarScan->entityID() = QString::number(msg.senor[n].nEntityID).toStdString();
			rpRadarScan->radius() = msg.senor[n].dRaduis;
			rpRadarScan->azMin() = msg.senor[n].dAzStartAngle;
			rpRadarScan->azMax() = msg.senor[n].dAzEndAngle;
			rpRadarScan->elevMin() = msg.senor[n].dPicthStartAngle;
			rpRadarScan->elevMax() = msg.senor[n].dPicthEndAngle;

			switch (msg.enCamp)
			{
			case 1://红方阵营
			{
				rpRadarScan->lineColor() = osg::Vec4(0.0,0.0,0.0,0.0);
				rpRadarScan->elfinColor() = redRadarSurfaceColor;
				rpRadarScan->color() = redRadarSurfaceColor;
			}
			break;
			case 2://蓝方阵营
			{
				rpRadarScan->lineColor() = osg::Vec4(0.0, 0.0, 0.0, 0.0);
				rpRadarScan->elfinColor() = blueRadarSurfaceColor;
				rpRadarScan->color() = blueRadarSurfaceColor;
			}
			break;
			default:
				break;
			}

			if (driveMgrInterface)
			{
				driveMgrInterface->addSensorUnit(rpRadarScan);
			}
		}
	}
}

void CSimControlCenter::executeMsg( SimInfo& msg )
{
	// 场景信息
	emit sig_simInfo(msg);
}

void CSimControlCenter::executeMsg( AirSpeceSectorInfo &msg )
{
	return;
// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (msg.enType == ADD_OP)
	{
		osg::ref_ptr<HGxObject::CGxAirSpaceSectorOption> rpAirSpaceSector = new HGxObject::CGxAirSpaceSectorOption;
		osg::Vec4  veccolor = DataEleConvertor::getModelColor(RED_CAMP);
		veccolor.a() = 0.8;
		rpAirSpaceSector->bottomColor() = veccolor;
		//rpAirSpaceSector->bottomLineStyle().color = DataEleConvertor::getModelColor(RED_CAMP);
		veccolor = DataEleConvertor::getModelColor(RED_CAMP);
		veccolor.a() = 0.2;
		rpAirSpaceSector->topColor() = veccolor;

		HGxObject::SLineStyle style;
		style.vecColor = veccolor;
		rpAirSpaceSector->topLineStyle() = style;
		rpAirSpaceSector->strID = DataEleConvertor::getAirSpaceID(msg.nEntityId);
		rpAirSpaceSector->isTerrain() = true;
		rpAirSpaceSector->center() = osg::Vec3d(msg.dLon, msg.dLat, msg.dAlt);
		rpAirSpaceSector->startAngle() = msg.dStartAngle;
		rpAirSpaceSector->swingAngle() = msg.dSwingAngle;
		rpAirSpaceSector->radius() = msg.dRadius;
		rpAirSpaceSector->height() = msg.dH;

		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(rpAirSpaceSector);
		}
	}
	else if (msg.enType == MOD_OP)
	{
		std::string sensorID = DataEleConvertor::getAirSpaceID(msg.nEntityId);

		HGxObject::CGxObjectBase* pCGxObjectBase = HGxObject::CGxObjectManager::getInstance()->getObject(sensorID);
		if (pCGxObjectBase)
		{
			if (pCGxObjectBase->getObjectOption())
			{
				HGxObject::CGxAirSpaceSectorOption* radarEle = dynamic_cast<HGxObject::CGxAirSpaceSectorOption*>(pCGxObjectBase->getObjectOption());
				if (radarEle == NULL)
				{
					return;
				}
				radarEle->center() = osg::Vec3d(msg.dLon, msg.dLat, msg.dAlt);
				radarEle->startAngle() = msg.dStartAngle;
				radarEle->swingAngle() = msg.dSwingAngle;
				radarEle->radius() = msg.dRadius;
				radarEle->height() = msg.dH;
			}
		}
	}
	else if (msg.enType == DEL_OP)
	{
		if (driveMgrInterface)
		{
			driveMgrInterface->destroyUnit(DataEleConvertor::getAirSpaceID(msg.nEntityId));
		}
	}
}

void CSimControlCenter::executeMsg( SpeareSubMsg &msg )
{
	
}


void CSimControlCenter::slot_msgDataHandler(const ComMsg&msgData)
{
	if (m_bStopThreads) return;

	switch (msgData.msgHead.type)
	{
		// 新建实体
	case NewEntityMsg:
		{
			NewEntity msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 实体状态
	case EntityStuMsg:
		{
			EntityStu msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 跟踪目标
	case DetecTargetMsg:
		{
			DetectCommand msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			//std::cout << "DetectCommand" << msgData.msgHead.dTime << "EntityId==" << msg.nEntityID << "TargetId==="<<msg.nTargetId;
			executeMsg(msg);
		}break;
		// 销毁命令
	case DestoryMsg:
		{
			DestoryCommand msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 显示端开始命令
	case UiStartMsg:
		{
		}break;
		// 事件消息
	case EnventMsg:
		{
			EventCommand msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 视点信息
	case ViewMsg:
		{
			ViewInfoMsg msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 时间信息
	case TimeCommandMsg:
		{
			TimeMsg msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 字幕消息
	case ScreenWordMsg:
		{
			ScreenWordInfo msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 击中事件
	case HitMsg:
		{
			HitCommand msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		// 传感器消息
	case SenorMsg:
		{
			Senor msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}break;
		//初始化命令
	case InitSimMsg:
		{
			//getSceneObjService()->removeAll();
			emit sig_init();
		}
		break;
		//锁定特效
	case AimMsg:
		{
			AimInfoMsg msg;
			memcpy(&msg, msgData.data, msgData.msgHead.dataLen);
			executeMsg(msg);
		}
		break;
	case AirSpaceMsg: // 空域
		{
			AirSpeceSectorInfo msg;
			memcpy(&msg,msgData.data,msgData.msgHead.dataLen);
			executeMsg(msg);
		}
		break;
	case SpeSubMsg: // 组合节点实体分离消息
		{
			SpeareSubMsg msg;
			memcpy(&msg,msgData.data,msgData.msgHead.dataLen);
			executeMsg(msg);
		}
		break;
	default:
		break;
	}
}

void CSimControlCenter::stopThreads()
{
	m_bStopThreads = true;
	if(m_dataParserThread->isRunning())
	{
		m_dataParserThread->Stop();
		m_dataParserThread->quit();
		//m_dataParserThread->wait();
	}
}

void CSimControlCenter::startThreads()
{
	m_bStopThreads = false;
	m_dataParserThread->Start();
}

void CSimControlCenter::setIpAndPort(char* ipAddr, unsigned int port)
{

}

CSimControlCenter::CSimControlCenter()
	:m_bTraksVisible(true)
	,m_bLinkLineVisible(true)
	,m_bRadarVisible(true)
	,m_bTrackVisible(true)
	,m_bNeedViewInfo(true)
	,m_bLabel(true)
	,m_bStopThreads(false)
{
	m_dataParserThread = new CMsgDataParserThread;
	REGISTER_NETWORK();
	HGxNet::CGxServicer* pNetwork = HGxNet::CGxNetworkManager::getInstance()->generateServicer(UDP_SERVICER_TYPE);
	if (pNetwork)
	{
		// 网络服务参数
		SUdpParam pParam;
		pParam.iCommuMode = UDP_SEND | UDP_RECV;   // 允许接受和发送
		pParam.iUdpType = UDP_MULTICAST;           // 使用组播
		pParam.strURL = "udp://192.168.5.14:5000"; // 本机IP和端口
		pParam.strGroupURL = "udp://230.0.0.1:5000"; // 组IP
		pParam.strInterURL = "udp://0.0.0.0:5000";   // 在中本机IP地址和端口
		pParam.bEchoBack = true;                      // 开启回环，否则接受不到自己发出的数据

													  // 初始化网络服务
		pNetwork->initialize(&pParam);
		// 连接对方，组播时发送的地址是组
		pNetwork->setConnect("udp://230.0.0.1:5000");
		SUdpReceiver *pUdpReceiver = new SUdpReceiver(pNetwork);
		// 设置接受处理函数
		pNetwork->setNetworkRecevier(pUdpReceiver);
		// 启动服务
		pNetwork->start();
		m_dataParserThread->connect(pUdpReceiver, SIGNAL(recvDataSignal(QByteArray)),SLOT(slot_receiveData(QByteArray)), Qt::QueuedConnection);
	}
	m_dataParserThread->Start();
	//m_dataParserThread->connect(m_udpRecvThread, SIGNAL(recvDataSignal(QByteArray)),
	//	SLOT(slot_receiveData(QByteArray)), Qt::QueuedConnection);

	this->connect(m_dataParserThread, SIGNAL(sig_parseData(const ComMsg&)),
		SLOT(slot_msgDataHandler(const ComMsg&)), Qt::DirectConnection);

	qRegisterMetaType<std::string>("std::string");
	qRegisterMetaType<ViewLocatCmd>("ViewLocatCmd");
	qRegisterMetaType<PeerView>("PeerView");
	qRegisterMetaType<PeerE2TView>("PeerE2TView");
	qRegisterMetaType<TracView>("TracView");
	qRegisterMetaType<SimInfo>("SimInfo");
	qRegisterMetaType<TraveView>("TraveView");
	qRegisterMetaType<TraveTargetView>("TraveTargetView");
}

void CSimControlCenter::synchronizationSettingsSlot( QString type, bool visible )
{
	if(type == "radar")
	{
		m_bRadarVisible = visible;
	}
	else if(type == "LinkLine")
	{
		m_bLinkLineVisible = visible;
	}
	//跟踪
	else if(type == "track")
	{
		m_bTrackVisible = visible;
	}
	//航迹
	else if(type == "tracks")
	{
		m_bTraksVisible = visible;
	}
	else if(type == "label")
	{
		m_bLabel = visible;
	}
}

void CSimControlCenter::whetherSendViewInfoSlot( bool flage )
{
	m_bNeedViewInfo = flage;
}


