#include <CBizDataInterface.h>
#include <service/event/ctkEventHandler.h>
#include <service/event/ctkEventConstants.h>
#include <bdpUtil/GxTimeManager.h>
#include <bdpDB/GxRegister.h>

#include <QStyleOption>
#include <QPainter>
#include <QString>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QUdpSocket>
#include <QMessageBox>
#include <QByteArray>

#include "FxModelConfigReader.h"
#include "GxEventHelperInterface.h"
#include "GxPluginFrameworkCore.h"
#include "public/IDriveMgrInterface.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxServiceQuerier.h"
#include "bdpObject/GxObjectManager.h"
#include "PlatformPlugin/object/GxAircraftOption.h"



CBizDataInterface::CBizDataInterface(ctkPluginContext *context, QObject *parent)
	: QObject(parent)
	, m_pContext(context)
	, m_pSceneParser(nullptr)
	, m_pBizDataSend(nullptr)
{
	FxModelSet::CFxModelConfigReader wr;
	if (!wr.loadFromFile(HGxDB::CGxRegister::getInstance()->getConfig().getFullPath("/UserConfig/modelconfig.xml")))
	{
		//FREEX_LOG_WARN << "load model list file falied: " << FeFileReg->GetDataPath() + "/UserConfig/modelconfig.xml";
	}
	m_pSceneParser = new CBizDataParse("");
	m_pSceneParser->SetEquipInfoData(wr.GetLoadData());
	m_pBizDataSend = new CBizDataSend(m_pSceneParser);
	m_pBizDataSend->start();

	connect(m_pSceneParser, SIGNAL(SignalFileParseFinish(bool)), this, SLOT(SlotFileParseFinish(bool)));
	connect(m_pSceneParser, SIGNAL(SignalsendDriverData()), this,SLOT(SlotReceiveDriverData()));
}

CBizDataInterface::~CBizDataInterface()
{

}

void CBizDataInterface::LoadFile(std::string strPath)
{
	ClearSceneData();
	//启动解析线程
 	m_pSceneParser->SetFilePath(strPath);
 	m_pSceneParser->start();

}

bool CBizDataInterface::LoadData(QByteArray qbyteData)
{
	if (qbyteData.isEmpty())
		return false;

	char* cData = qbyteData.data();
	return false;
}


void CBizDataInterface::ClearSceneData()
{
	m_pSceneParser->ClearData();
	m_pBizDataSend->ClearData();
	// 获取数据管理服务
	HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
	if (driveMgrInterface)
	{
		driveMgrInterface->clearUnit();
	}
	ClearScene();
}

void CBizDataInterface::ClearScene()
{

}

void CBizDataInterface::SlotFileParseFinish(bool bState)
{
	//异步发送事�?
	ctkDictionary event;
	event.insert("LoadFileState", bState);
	HGxEventHelperInstance.postEvent("event/LoadFileFinished", event);
	if(bState)
	{	
		HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->stop();
		if(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0))
		{
			HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->start();
		}
		emit SignalAllTimeAixBegin(bState);
	}
}

void CBizDataInterface::SlotReceiveDriverData()
{
	if (m_pSceneParser->GetMapIndexToVecDriverData().size() <= 0)
	{
		return;
	}
	if (m_pBizDataSend)
	{
		m_pBizDataSend->SetData(m_pSceneParser->GetMapIndexToVecDriverData(), m_pSceneParser->GetMapIndexRoleCommandData(),
			m_pSceneParser->GetMapIndexSenorCommandData());
	}
}

void CBizDataInterface::TimerUpdateSlot()
{
// 	QUdpSocket udpsocket;
// 
// 	std::map<int,VectorDriverAircraftObjectsData> mapIndexToVecDriverData = /*m_mapIndexToVecDriverData;//*/ m_pSceneParser->GetMapIndexToVecDriverData();
// 
// 	double dCurrentSimuTime = FxSimulationTimerInstance()->getSimulateTimeCurrent();
// 	double dDifferentTime = (dCurrentSimuTime - m_pSceneParser->GetSimuStartTime())*86400.0;
// 	double dFirstTimerSec = mapIndexToVecDriverData.begin()->second.at(0).dTime;
// 	double dEndTimerSec = mapIndexToVecDriverData.rbegin()->second.at(0).dTime;
// 	int	   nIndex = dDifferentTime/(dEndTimerSec - dFirstTimerSec)*mapIndexToVecDriverData.size();
// 	//static int nIndex = 0;
// 
// 	if (mapIndexToVecDriverData.count(nIndex))
// 	{
// 		VectorDriverAircraftObjectsData pRecvData = mapIndexToVecDriverData[nIndex];
// 		int nStructSize = sizeof(SDriverAircraftObjectData);
// 		int nSize = nStructSize * pRecvData.size();
// 		char* pData = new char[nSize];
// 		for(int i=0;i<pRecvData.size();++i)
// 		{
// 			memcpy(pData + nStructSize*i, &(pRecvData.at(i)), nStructSize);
// 		}
// 
// 		QByteArray dataArray = QByteArray::fromRawData(pData,nSize);
// 		std::string m_strAddr = "127.0.0.1";
// 		QHostAddress  addr(m_strAddr.c_str());
// 		udpsocket.writeDatagram(dataArray,addr/*QHostAddress::LocalHost*/,8888);
// 		delete[] pData;
// 		pData = nullptr;
// 	}
	//nIndex++;
}


void CBizDataInterface::LoadFlyPathData()
{
	std::vector<osg::Vec3d> startPos;
	std::vector<osg::Vec3d> endPos;

	startPos.push_back(osg::Vec3d(120.736344,23.329826,10000));   
	startPos.push_back(osg::Vec3d(120.846344,23.369826,10000));
	startPos.push_back(osg::Vec3d(120.936344,23.329826,10000));   
	startPos.push_back(osg::Vec3d(121.036344,23.389826,10000));
	startPos.push_back(osg::Vec3d(121.136344,23.389826,10000));
	startPos.push_back(osg::Vec3d(121.246344,23.389826,10000));


// 	FxScene::IFxSceneService *pSceneService = FreeXFrameworkInstance.getService<FxScene::IFxSceneService>().get();
// 	if (pSceneService == nullptr)
// 	{
// 		return ;
// 	}
// 	//绘制服务
// 	FxCore::CFxServiceReference plotServiceRef = pSceneService->getServiceReference<FxPlot::IFxPlotService>(0);
// 	if (!plotServiceRef)
// 	{
// 		return;
// 	}
// 
// 	//m_pPlotService = qobject_cast<FxPlot::IFxPlotService*>(FreeXFrameworkInstance.getService(plotServiceRef));
// 	FxPlot::IFxPlotService* pPlotService = plotServiceRef.getServiceObj<FxPlot::IFxPlotService>();
// 	pPlotService->stopPick();
// 	if (pPlotService == nullptr)
// 	{
// 		return;
// 	}
// 	std::vector<CMotionStateInfo> vecOutput;
// 	FlightInterpolationAndSphereSmooth(pPlotService->getPlotContext(), startPos, vecOutput, 50, 4000, 50);
// 
// 	for (int i = 0;i < vecOutput.size(); ++i)
// 	{
// 		  VectorDriverAircraftObjectsData vecData;
// 		  SDriverAircraftObjectData sPreData;
// 		  sPreData.dTime = i * 0.02;
// 		  std::string strID = std::to_string(_Longlong(100));
// 		  strcpy(sPreData.strID, strID.c_str());
// 		  sPreData.vecPosture[0] = vecOutput.at(i).vecPosture.x();
// 		  sPreData.vecPosture[1] = vecOutput.at(i).vecPosture.y();
// 		  sPreData.vecPosture[2] = vecOutput.at(i).vecPosture.z();
// 
// 		  sPreData.vecPositionLLH[0] = vecOutput.at(i).vecLLH.x();
// 		  sPreData.vecPositionLLH[1] = vecOutput.at(i).vecLLH.y();
// 		  sPreData.vecPositionLLH[2] = vecOutput.at(i).vecLLH.z();
// 		  vecData.push_back(sPreData);
// 		  sPreData.nDriveStyle = 0;
// 		  m_mapIndexToVecDriverData[i] = vecData;
// 	}
}

CBizDataSend::CBizDataSend(CBizDataParse*	pSceneParser)
	:m_pSceneParser(pSceneParser)
	,m_bDown(true)
{

}

CBizDataSend::~CBizDataSend()
{

}

void CBizDataSend::SetData( std::map<int,VectorDriverAircraftObjectsData> mapIndexToVecDriverData, 
	std::map<float, RoleCommandVec> mapIndexRoleCommandData,
	std::map<float, vecZaijiRadarState> mapZaijiRadarState)
{
	if (m_mutex.tryLock())
	{
		 m_mapIndexToVecDriverData = mapIndexToVecDriverData;
		 m_mapIndexRoleCommandData = mapIndexRoleCommandData;
		 m_mapZaijiRadarStateData = mapZaijiRadarState;
		 //m_bDown = true;
		 m_mutex.unlock();
	 }
}

void CBizDataSend::ClearData()
{
	if (m_mutex.tryLock())
	{
		m_mapIndexToVecDriverData.clear();
		m_mapIndexRoleCommandData.clear();
		m_mapZaijiRadarStateData.clear();
		//m_bDown = false;
		m_mutex.unlock();
	}
}

void CBizDataSend::run()
{
	while (true)
	{
		
		while (m_bDown)
		{
			m_mutex.lock();
			if (m_mapIndexToVecDriverData.size() > 0)
			{
				double dCurrentSimuTime = HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent();
				double dDifferentTime = (dCurrentSimuTime - m_pSceneParser->GetSimuStartTime())*86400.0;
				double dFirstTimerSec = m_mapIndexToVecDriverData.begin()->second.at(0).dTime;
				double dEndTimerSec = m_mapIndexToVecDriverData.rbegin()->second.at(0).dTime;
				int	   nIndex = dDifferentTime/(dEndTimerSec - dFirstTimerSec)*m_mapIndexToVecDriverData.size();

				if (m_mapIndexToVecDriverData.count(nIndex))
				{
					VectorDriverAircraftObjectsData pRecvData = m_mapIndexToVecDriverData[nIndex];
					// 获取数据管理服务
					HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
					if (driveMgrInterface)
					{
						driveMgrInterface->driveAircraftUnit(pRecvData);
					}
				}
				if (m_mapIndexRoleCommandData.count(int(dDifferentTime)))
				{
					RoleCommandVec vecRoleCommand = m_mapIndexRoleCommandData[int(dDifferentTime)];
					for (int n = 0; n < vecRoleCommand.size(); ++n)
					{
						if (vecRoleCommand.at(n).C1 == 69)//C1==69,飞机停算,飞机消失
						{
							// 获取数据管理服务
							HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
							if (driveMgrInterface)
							{
								//driveMgrInterface->destroyUnit(vecRoleCommand.at(n).recplatfmID);
							}
						}
						else if (vecRoleCommand.at(n).C1 == 39)//飞机被导弹击�?飞机导弹消失
						{
							// 获取数据管理服务
							HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());
							if (driveMgrInterface)
							{
								//39/2,39/3不设置爆炸效果，1,2,3让导弹都消失
								if (vecRoleCommand.at(n).C2 == 1)
								{

									//driveMgrInterface->destroyUnit(vecRoleCommand.at(n).WorkState);
								}
								//driveMgrInterface->destroyUnit(vecRoleCommand.at(n).recplatfmID);
							}
						}
					}

				}
				if (m_mapZaijiRadarStateData.count(int(dDifferentTime)))
				{
					vecZaijiRadarState vecZaijiRadar = m_mapZaijiRadarStateData[int(dDifferentTime)];
					for (int i = 0; i < vecZaijiRadar.size(); ++i)
					{
						std::string strID = "Radar" + vecZaijiRadar.at(i).IDbelong;
						if (HGxObject::CGxObjectManager::getInstance()->getObject(strID) != NULL)
						{
							if (HGxObject::CGxObjectManager::getInstance()->getObject(vecZaijiRadar.at(i).IDbelong) != NULL)
							{
								HGxObject::CGxObjectBase* pCGxObjectBase = HGxObject::CGxObjectManager::getInstance()->getObject(vecZaijiRadar.at(i).IDbelong);
								if (pCGxObjectBase)
								{
									if (pCGxObjectBase->getObjectOption())
									{
										HGxObject::CGxAircraftOption* ObjectEle = dynamic_cast<HGxObject::CGxAircraftOption*>(pCGxObjectBase->getObjectOption());
										HGxObject::CGxRadarScanOption* RadarScan = dynamic_cast<HGxObject::CGxRadarScanOption*>(HGxObject::CGxObjectManager::getInstance()->getObject(strID)->getObjectOption());;
										if (ObjectEle != NULL && RadarScan != NULL)
										{
											switch (ObjectEle->camp())
											{
											case 2://红方阵营
											{
												RadarScan->lineColor() = osg::Vec4(0.0, 0.0, 0.0, 0.0);
												RadarScan->elfinColor() = osg::Vec4(1, 106.0 / 255, 106.0 / 255, 0.3);
												RadarScan->color() = osg::Vec4(1, 106.0 / 255, 106.0 / 255, 0.3);
											}
											break;
											case 1://蓝方阵营
											{
												RadarScan->lineColor() = osg::Vec4(0.0, 0.0, 0.0, 0.0);
												RadarScan->elfinColor() = osg::Vec4(106.0 / 255, 165.0 / 255, 1, 0.3);
												RadarScan->color() = osg::Vec4(106.0 / 255, 165.0 / 255, 1, 0.3);;
											}
											break;
											default:
												break;
											}
										}
									}
								}
							}
						}
						else
						{
							// 获取数据管理服务
							HGxPFWCore::CGxServiceQuerier<IDriveMgrInterface> driveMgrInterface(HGxFWLauncherInstance.getPluginContext());

							osg::ref_ptr<HGxObject::CGxRadarScanOption> rpRadarScan = new HGxObject::CGxRadarScanOption;
							rpRadarScan->strName = QString::fromUtf8("RadarScan").toStdString();
							rpRadarScan->strID = strID;
							rpRadarScan->parent() = vecZaijiRadar.at(i).IDbelong;
							rpRadarScan->entityID() = vecZaijiRadar.at(i).IDbelong;
							rpRadarScan->radius() = vecZaijiRadar.at(i).dectout/4;
							rpRadarScan->azMin() = -25.0;//vecZaijiRadar.at(i).dAzStartAngle;
							rpRadarScan->azMax() = 25.0;//vecZaijiRadar.at(i).dAzEndAngle;
							rpRadarScan->elevMin() =-5.0;
							rpRadarScan->elevMax() = 5.0;
							rpRadarScan->lineColor() = osg::Vec4(0.0, 0.0, 0.0, 0.0);
							rpRadarScan->elfinColor() = osg::Vec4(106.0 / 255, 165.0 / 255, 1, 0.3);
							rpRadarScan->color() = osg::Vec4(106.0 / 255, 165.0 / 255, 1, 0.3);;
							if (driveMgrInterface)
							{
								driveMgrInterface->addSensorUnit(rpRadarScan);
							}
						}

					}
				}
// 					int nStructSize = sizeof(SDriverAircraftObjectData);
// 					int nSize = nStructSize * pRecvData.size();
// 					char* pData = new char[nSize];
// 					for(int i=0;i<pRecvData.size();++i)
// 					{
// 						memcpy(pData + nStructSize*i, &(pRecvData.at(i)), nStructSize);
// 					}

// 					QByteArray dataArray = QByteArray::fromRawData(pData,nSize);
// 					std::string m_strAddr = "127.0.0.1";
// 					QHostAddress  addr(m_strAddr.c_str());
// 					udpsocket.writeDatagram(dataArray,addr/*QHostAddress::LocalHost*/,8888);

//					delete[] pData;
//					pData = nullptr;
//				}
			}
			m_mutex.unlock();
			OpenThreads::Thread::microSleep(30);
		}

	}


}
