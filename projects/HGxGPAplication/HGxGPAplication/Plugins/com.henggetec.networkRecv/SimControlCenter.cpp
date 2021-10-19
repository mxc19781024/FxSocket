#include "SimControlCenter.h"
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
#include "service/event/ctkEventAdmin.h"
#include "common/UnitEventCommon.h"
#include "PlatformPlugin/object/GxRadarScanOption.h"
#include "PlatformPlugin/object/GxAirSpaceSectorOption.h"
#include "PlatformPlugin/object/GxMarkEffectLockOption.h"
#include "PlatformPlugin/object/GxTrackLineCommon.h"
#include "PlatformPlugin/object/GxWakeRibbonCommon.h"
#include "PlatformPlugin/object/GxLabelCommon.h"
#include "bdpCommon/GxStrUtil.h"
#include "QSettings"
#include "DataEleConvertor.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
//qt5 中文乱码
#pragma execution_character_set("utf-8")
#endif

//////////////////////////////////////////////////////////////////////////
const osg::Vec4 redRadarSurfaceColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarConeColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarSideColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarSurfaceLineColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarConeLineColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarSideLineColor(1, 106.0 / 255, 106.0 / 255, 0.3);
const osg::Vec4 redRadarOutLineColor(1, 106.0 / 255, 106.0 / 255, 1);

const osg::Vec4 blueRadarSurfaceColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarConeColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarSideColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarSurfaceLineColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarConeLineColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarSideLineColor(106.0 / 255, 165.0 / 255, 1, 0.3);
const osg::Vec4 blueRadarOutLineColor(106.0 / 255, 165.0 / 255, 1, 1);

CSimControlCenter::CSimControlCenter()
	:m_bTraksVisible(true)
	, m_bLinkLineVisible(true)
	, m_bRadarVisible(true)
	, m_bTrackVisible(true)
	, m_bLabel(true)
	, m_bStopThreads(false)
{
	m_dataParserThread = new CMsgDataParserThread;
	REGISTER_NETWORK();
	HGxNet::CGxServicer* pNetwork = HGxNet::CGxNetworkManager::getInstance()->generateServicer(UDP_SERVICER_TYPE);
	if (pNetwork)
	{
		QString DataFileDir = HGxDB::CGxRegister::getInstance()->getConfig().getDataPath().data();
		QSettings setings(DataFileDir + "config/NetCfg.ini", QSettings::Format::IniFormat);

		setings.beginGroup("sendNetInfo");
		QString qsendNetInfo;
		qsendNetInfo = setings.value("IP").toString() + ":" + setings.value("Port").toString();
		setings.endGroup();

		setings.beginGroup("recvNetInfo");
		QString qrecvNetInfo;
		qrecvNetInfo = setings.value("IP").toString() + ":" + setings.value("Port").toString();
		setings.endGroup();

		// 网络服务参数
		SUdpParam pParam;
		pParam.iCommuMode = UDP_SEND | UDP_RECV;   // 允许接受和发送
		pParam.iUdpType = UDP_MULTICAST;           // 使用组播
		pParam.strURL = "udp://" + qrecvNetInfo.toStdString(); // 本机IP和端口192.168.5.49:5000
		pParam.strGroupURL = "udp://" + qsendNetInfo.toStdString(); // 组IP230.0.0.1:5000
		pParam.strInterURL = "udp://0.0.0.0:5000";   // 在中本机IP地址和端口
		pParam.bEchoBack = true;                      // 开启回环，否则接受不到自己发出的数据

		// 初始化网络服务
		pNetwork->initialize(&pParam);
		// 连接对方，组播时发送的地址是组
		pNetwork->setConnect("udp://" + qsendNetInfo.toStdString());
		SUdpReceiver *pUdpReceiver = new SUdpReceiver(pNetwork);
		// 设置接受处理函数
		pNetwork->setNetworkRecevier(pUdpReceiver);
		// 启动服务
		pNetwork->start();
		m_dataParserThread->connect(pUdpReceiver, SIGNAL(recvDataSignal(QByteArray)), SLOT(slot_receiveData(QByteArray)), Qt::QueuedConnection);
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
	std::string entityID = CDataEleConvertor::getInstance()->getInstance()->getEntityID(msg.nEntityID);
	std::string targetID = CDataEleConvertor::getInstance()->getEntityID(msg.nTargetId);

// 	osg::ref_ptr<FxSceneObject::CFxSceneObject> objEntity = getSceneObjService()->getObject(entityID);
// 	if (!objEntity) return;
// 	osg::ref_ptr<FxSceneObject::CFxSceneObject> objTarget = getSceneObjService()->getObject(targetID);
// 	if (!objTarget) return;

	// 创建击中特效
//	HGxObject::CGxExplosionEle* hitEle = new HGxObject::CGxExplosionEle;
//	hitEle->strID = CDataEleConvertor::getInstance()->getHitEffectID(msg.nTargetId);
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

	std::string entityID = CDataEleConvertor::getInstance()->getEntityID(msg.nEntityID);

	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->destroyUnit(entityID);
	}
}

void CSimControlCenter::executeMsg(AimInfoMsg& msg )
{
	std::string entityID = CDataEleConvertor::getInstance()->getEntityID(msg.nEntityId);
	std::string targetID = CDataEleConvertor::getInstance()->getEntityID(msg.nTargetId);
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	std::string strID = CDataEleConvertor::getInstance()->getAimEffectID(msg.nEntityId, msg.nTargetId);
	
	if (driveMgrInterface)
	{
		if (!driveMgrInterface->getUnitEle(strID))
		{
			osg::ref_ptr<HGxObject::GxEarthLockEle> pLockEle = new HGxObject::GxEarthLockEle;
			pLockEle->strID = CDataEleConvertor::getInstance()->getAimEffectID(msg.nEntityId, msg.nTargetId);
			pLockEle->sendEntityID() = entityID;
			pLockEle->receiveEntityID() = targetID;
			pLockEle->animationTime() = osg::minimum(msg.dContinueTime*0.2, 0.5);
			pLockEle->lockTime() = osg::maximum(msg.dContinueTime*0.2, 2.0);
			pLockEle->lockNum() = (msg.dContinueTime*0.6) / pLockEle->lockTime();
			pLockEle->lockColor() = osg::Vec4(0.0, 0, 1.0, 0.5);
			pLockEle->lockCircleRatio() = 5.0;
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
	std::string sensorID = CDataEleConvertor::getInstance()->getSensorID(msg.nEntityID, msg.nSeorId);
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		if (driveMgrInterface->getUnitEle(sensorID))
		{
			HGxObject::CGxRadarScanOption* radarEle = dynamic_cast<HGxObject::CGxRadarScanOption*>(driveMgrInterface->getUnitEle(sensorID));
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
	if (msg.enSpaceType == 1)
	{
		return;
	}	
	SNewObjectData sNewObjectData;
	sNewObjectData = CDataEleConvertor::getInstance()->getEntityAttri(msg);

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
		opWakeFlame->parent() = strID;
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
		opWakeRibbon->parent() = strID;
		opWakeRibbon->strID = strID + "WakeRibbon";
		opWakeRibbon->strName = strID;
		opWakeRibbon->size() = 5000;
		opWakeRibbon->width() = 10;
		if (driveMgrInterface)
		{
			driveMgrInterface->addSensorUnit(opWakeRibbon);
		}
	}
	osg::ref_ptr<HGxObject::GxLabelEle> sLabelEle = new HGxObject::GxLabelEle;
	std::string strPara = sNewObjectData.strID;
	sLabelEle->entityID() = strPara;
	sLabelEle->parent() = strPara;
	strPara = "Label_" + strPara;
	sLabelEle->strID = strPara;
	strPara = sNewObjectData.strName;
	sLabelEle->strName = strPara;
	strPara = "装备名称：" + strPara + ";";
	if (sNewObjectData.enCamp == 1)
	{
		strPara = strPara + "阵营：红方";
		//sLabelEle->lineColor()
		sLabelEle->lineColor() = osg::Vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (sNewObjectData.enCamp == 2)
	{
		strPara = strPara + "阵营：蓝方";
		sLabelEle->lineColor() = osg::Vec4(0.0, 0.0, 1.0, 1.0);
	}
	else
	{
		strPara = strPara + "阵营：白方";
		sLabelEle->lineColor() = osg::Vec4(0.0, 0.1, 0.0, 1.0);
	}

	sLabelEle->text() = strPara;

	sLabelEle->backgroundColor() = osg::Vec4(1, 1, 1, 1.0);
	sLabelEle->textColor() = osg::Vec4(0.0, 0.0, 0.0, 1.0);
	sLabelEle->width() = 3.0;
	sLabelEle->fontSize() = 10;
	if (driveMgrInterface)
	{
		driveMgrInterface->addSensorUnit(sLabelEle);
	}

	// 传感器数量
	if (msg.nSenorNum > 0)
	{
		for (int n = 0; n < msg.nSenorNum; ++n)
		{
			osg::ref_ptr<HGxObject::CGxRadarScanOption> rpRadarScan = new HGxObject::CGxRadarScanOption;
			rpRadarScan->strID = "RadarScan";
			rpRadarScan->strName = QString::fromUtf8("RadarScan").toStdString();
			rpRadarScan->strID = CDataEleConvertor::getInstance()->getSensorID(msg.senor[n].nEntityID, msg.senor[n].nSeorId); 
			rpRadarScan->entityID() = QString::number(msg.senor[n].nEntityID).toStdString();
			rpRadarScan->parent() = QString::number(msg.senor[n].nEntityID).toStdString();
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

void CSimControlCenter::executeMsg(AirSpeceSectorInfo &msg )
{
// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (msg.enType == ADD_OP)
	{
		osg::ref_ptr<HGxObject::CGxAirSpaceSectorOption> rpAirSpaceSector = new HGxObject::CGxAirSpaceSectorOption;
		osg::Vec4  veccolor = CDataEleConvertor::getInstance()->getModelColor(RED_CAMP);
		veccolor.a() = 0.8;
		rpAirSpaceSector->bottomColor() = veccolor;
		//rpAirSpaceSector->bottomLineStyle().color = CDataEleConvertor::getInstance()->getModelColor(RED_CAMP);
		veccolor = CDataEleConvertor::getInstance()->getModelColor(RED_CAMP);
		veccolor.a() = 0.2;
		rpAirSpaceSector->topColor() = veccolor;

		HGxObject::SLineStyle style;
		style.vecColor = veccolor;
		rpAirSpaceSector->topLineStyle() = style;
		rpAirSpaceSector->strID = CDataEleConvertor::getInstance()->getAirSpaceID(msg.nEntityId);
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
		std::string sensorID = CDataEleConvertor::getInstance()->getAirSpaceID(msg.nEntityId);

		// 获取数据管理服务
		if (driveMgrInterface)
		{
			if (driveMgrInterface->getUnitEle(sensorID))
			{
				HGxObject::CGxAirSpaceSectorOption* radarEle = dynamic_cast<HGxObject::CGxAirSpaceSectorOption*>(driveMgrInterface->getUnitEle(sensorID));
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
			driveMgrInterface->destroyUnit(CDataEleConvertor::getInstance()->getAirSpaceID(msg.nEntityId));
		}
	}
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
	case ClearMsg: // 清除场景
	{
		HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());

		if (driveMgrInterface)
		{
			driveMgrInterface->clearUnit();
		}
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



