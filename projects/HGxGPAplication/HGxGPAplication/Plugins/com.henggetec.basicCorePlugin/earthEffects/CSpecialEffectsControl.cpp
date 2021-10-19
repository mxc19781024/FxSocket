#include <QFrame>
#include <QFile>

//#include <FreeXFramework/LoadInfo.h>

#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventAdmin.h>


//#include <FreeXFramework/FreeXFramework.h>
//#include <FreeXFramework/FxServiceQuerier.h>
#include <scene_ui/ISceneUiService.h>
//#include <workspace_ui/IFxWorkSpaceService.h>
#include <scene_ui/ISceneInterface.h>

#include <FxToast/CToast.h>
#include <FxMessageBox/CMessageBox.h>
//#include "ICore.h"
#include "CSpecialEffectsControl.h"
//#include <CMainWindowManager.h>
#include <QFutureWatcher>
#include <qtconcurrentrun.h>
#include <functional>
#include <FxLoadIndicator/CLoadIndicator.h>
#include <QSystemSemaphore>

//#include <earth_ocean/IFxOceanManager.h>
#include "CEffectsControlItem.h"
//#include <scene/IFxSceneService.h>

#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/scene/GxSceneBase.h>

#include <gdpScene/CommonGeo.h>
#include <gdpScene/layer/GxLayerEffect.h>
#include <gdpScene/layer/GxLayerManager.h>
#include <gdpTool/environment/GxEnvirementCom.h>
#include <gdpTool/environment/GxWeatherOption.h>
#include <gdpTool/environment/GxOceanOption.h>
#include <gdpTool/environment/GxSkyOption.h>
#include <gdpTool/environment/GxTerminatorOption.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
CSpecialEffectsControl::CSpecialEffectsControl(ctkPluginContext *context, QWidget *parent, Qt::WFlags flags)
#else
CSpecialEffectsControl::CSpecialEffectsControl(ctkPluginContext *context, QWidget *parent, Qt::WindowFlags flags)
#endif
	: QWidget(parent, flags)
	, m_pAttrWgt(NULL)
	, m_strSceneID("")
	, m_pSkyOption(NULL)
	, m_pOceanOption(NULL)
	, m_pWeatherOption(NULL)
{
	ui.setupUi(this);

	//读样式表文件
 	QFile file(":/qss/Resources/qss/freex_earth_effect_ui.css");
 	if (file.open(QIODevice::ReadOnly))
 	{
		QString str = file.readAll();
 		setStyleSheet(str);
 		file.close();
 	}

	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneSwitch", HGxFWLauncherInstance.getPluginContext());
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneRemove", HGxFWLauncherInstance.getPluginContext());

	initWgt();

	connect(ui.SettingPushBtn, SIGNAL(clicked()), this, SLOT(settingBtnClickedSlot()));
	connect(this, SIGNAL(initEnvSignal()), this, SLOT(initEnvSlot()));
	connect(this, SIGNAL(settingWidgetHideSignal()), this, SLOT(settingWidgetHideSlot()));
}

CSpecialEffectsControl::~CSpecialEffectsControl()
{
	m_rpEnviManager->close(FXEARTH_ENVIRORMENT_TYPE_SKY, m_strSceneID);

	m_rpEnviManager->close(FXEARTH_ENVIRORMENT_TYPE_OCEAN, m_strSceneID);

	m_rpEnviManager->close(FXEARTH_ENVIRORMENT_TYPE_WEATHER, m_strSceneID);

 	foreach(auto item, m_vecControlItem)
 	{
 		if (NULL != m_pSkyOption)
 		{
			m_pSkyOption->bNebulaVisible = false;
			m_pSkyOption->bSkyBoxVisible = false;
			m_pSkyOption->updateEnvirementNode();
		}
 		swichedSlotForItem(item->getItemName(), false);
 	}

	if (nullptr != m_pAttrWgt)
	{
		m_pAttrWgt->close();
		delete m_pAttrWgt;
		m_pAttrWgt = nullptr;
	}
}

CEnvirAttrSetting* CSpecialEffectsControl::getAttrWgt()
{
	return m_pAttrWgt;
}

void CSpecialEffectsControl::initWgt()
{
	if(m_pAttrWgt == NULL)
	{
		m_pAttrWgt = new CEnvirAttrSetting(this);
		connect(m_pAttrWgt, SIGNAL(envirAttrChangedSignal()), this, SLOT(envirChangedSlot()));
	}
	QList<std::pair<QString, QString>> lst3dItem;
	lst3dItem << std::make_pair(tr("sun"), "taiyang") << std::make_pair(tr("atmosphere"), "daqi") << std::make_pair(tr("star"), "xingchen") << std::make_pair(tr("shadow"), "yingyin") << std::make_pair(tr("map of relief"), "xuanyun")
		<< std::make_pair(tr("cloud layer"), "yuntu") << std::make_pair(tr("rain effect"), "yu") << std::make_pair(tr("snow effect"), "xue") << std::make_pair(tr("fog effect"), "wu") << std::make_pair(tr("ocean"), "haiyang");
	addControlItem(lst3dItem);

	ui.scrollArea->viewport()->setStyleSheet("background:transparent");
	m_pAttrWgt->startTimer(true);
}

void CSpecialEffectsControl::addControlItem(QList<std::pair<QString, QString>> lstItem)
{
	clearControlItem();
	QVBoxLayout *pLayout = new QVBoxLayout();
	pLayout->setContentsMargins(0,0,0,0);
	pLayout->setSpacing(20);
	for(int i = 0; i < lstItem.count(); i++)
	{
		CEffectsControlItem *pItem = new CEffectsControlItem(this);
		pItem->setItemName(lstItem.at(i).first, lstItem.at(i).second);
		if(lstItem.at(i).first == "map of relief")
		{
            QString btnName = tr("turn on shadows for better results");
            pItem->setBtnToolTip(btnName);
		}

		pLayout->addWidget(pItem);
		m_vecControlItem.push_back(pItem);

		connect(pItem, SIGNAL(ItemChecked(const QString &, bool)), this, SLOT(swichedSlotForItem(const QString &, bool)));
	}	
	pLayout->addStretch(1);
	ui.ControlWgt->setLayout(pLayout);
}

void CSpecialEffectsControl::clearControlItem()
{
	QObjectList lstObj = ui.ControlWgt->children();
	for(int i = 0; i < lstObj.count(); i++)
	{
		QObject *pObj = lstObj.at(i);
		delete pObj;
		pObj = NULL;
	}
	m_vecControlItem.clear();
}

void CSpecialEffectsControl::sendModifiedEvent(bool bModified)
{
	ctkDictionary message;
	message.insert("changeStatus", bModified);

	HGxEventHelperInstance.postEvent("event/pluginStatus", message);
}

void CSpecialEffectsControl::initOceanForQtFive()
{
// 	//只是做一下初始化工作，满足QT5不支持多线程渲染问题
// 	getOceanNode();
}

HGxTool::CGxOceanOption* CSpecialEffectsControl::getOceanOption()
{
	return m_pOceanOption;
}

HGxTool::CGxWeatherOption* CSpecialEffectsControl::getWeatherOption()
{
	return m_pWeatherOption;
}

HGxTool::CGxSkyOption* CSpecialEffectsControl::getSkyOption()
{
	return m_pSkyOption;
}

void CSpecialEffectsControl::initEnvWhenStart()
 {
	// 创建环境管理
	m_rpEnviManager = HGxTool::CGxEnviManager::getInstance();

	// 创建天空、海洋、天气
	m_rpEnviManager->create(FXEARTH_ENVIRORMENT_TYPE_SKY, m_strSceneID);
	m_rpEnviManager->open(FXEARTH_ENVIRORMENT_TYPE_SKY, m_strSceneID);

	m_rpEnviManager->create(FXEARTH_ENVIRORMENT_TYPE_OCEAN, m_strSceneID);
	m_rpEnviManager->open(FXEARTH_ENVIRORMENT_TYPE_OCEAN, m_strSceneID);

	m_rpEnviManager->create(FXEARTH_ENVIRORMENT_TYPE_WEATHER, m_strSceneID);
	m_rpEnviManager->open(FXEARTH_ENVIRORMENT_TYPE_WEATHER, m_strSceneID);

	m_pWeatherOption = dynamic_cast<HGxTool::CGxWeatherOption*>(m_rpEnviManager->getEnvirmentOption(FXEARTH_ENVIRORMENT_TYPE_WEATHER));

	m_pOceanOption = dynamic_cast<HGxTool::CGxOceanOption*>(m_rpEnviManager->getEnvirmentOption(FXEARTH_ENVIRORMENT_TYPE_OCEAN));

	m_pSkyOption = dynamic_cast<HGxTool::CGxSkyOption*>(m_rpEnviManager->getEnvirmentOption(FXEARTH_ENVIRORMENT_TYPE_SKY));
 }
 
//   FxEarth::IFxEarthLayerService* CSpecialEffectsControl::getEarthLayerService()
//   {
//   	FxScene::IFxSceneService *pSceneService = getService<FxScene::IFxSceneService>();
//   	if (!pSceneService)
//   	{
//   		return NULL;
//   	}
//   
//   // 	ctkServiceReference serviceRef = pSceneService->getServiceReference<FxEarth::IFxEarthLayerService>(getSceneID());
//   // 	
//   // 	FxEarth::IFxEarthLayerService* pEarthLayerService = NULL;
//   // 	if (!serviceRef)
//   // 	{
//   // 		return NULL;
//   // 	}
//   // 	pEarthLayerService = FreeXFrameworkInstance.getService<FxEarth::IFxEarthLayerService>(serviceRef);
//   	
//   //	return pEarthLayerService;
//   
//   	return pSceneService->getService<FxEarth::IFxEarthLayerService>(getSceneID()).get();
//   }

void CSpecialEffectsControl::swichedSlotForItem( const QString &strName, bool bChecked)
{
	if (m_strSceneID.empty())
	{
		return;
	}

	if (m_pWeatherOption)
	{
		if (strName == tr("rain effect"))
		{
			m_pWeatherOption->bRainVisible = bChecked;
		}
		else if (strName == tr("snow effect"))
		{
			m_pWeatherOption->bSnowVisible = bChecked;
		}
		else if (strName == tr("fog effect"))
		{
			m_pWeatherOption->bFogVisible = bChecked;
		}

		m_pWeatherOption->updateEnvirementNode();
	}

	if (strName == tr("ocean"))
	{
		if (m_pOceanOption)
		{
			m_pOceanOption->bOceanVisible = bChecked;
			m_pOceanOption->updateEnvirementNode();	
		}
	}

	if (m_pSkyOption)
	{
		if (strName == tr("sun"))
		{
			m_pSkyOption->bSunVisible = bChecked;
		}
		else if(strName == tr("atmosphere"))
		{
			m_pSkyOption->bAtmosVisible = bChecked;
		}
		else if(strName == tr("star"))
		{
			m_pSkyOption->bStarVisible = bChecked;
		}
		else if (strName == tr("cloud layer"))
		{
			m_pSkyOption->bCloudLayer = bChecked;
		}

		m_pSkyOption->updateEnvirementNode();
	}

 	if (strName == tr("shadow"))
 	{
 		if(bChecked)
 		{
 			QVector<CEffectsControlItem *>::const_iterator itorItem = m_vecControlItem.begin();
 			for(; itorItem != m_vecControlItem.end(); itorItem++)
 			{
 				CEffectsControlItem *pItem = *itorItem;
 				if(pItem->getItemName() == tr("sun"))
 				{
 					if(!pItem->getSwitchState())
 					{
 						// 将开关置为开启
 						pItem->setSwitchOn(true);
 						// 调用底层接口打开太阳关照
 						if (m_pSkyOption)
 						{
							m_pSkyOption->bSunVisible = bChecked;
							m_pSkyOption->updateEnvirementNode();
 						}

						/******************此处需要主窗口*****************/
 						//CToast::information(tr("tips"), tr("The sun light effect has started syncronously"), NULL/*,   ICore::mainWindowManager()->sceneWindow()*/);
						CMessageBox::information(tr("The sun light effect has started syncronously"), NULL);
					}
 					break;
 				}
 			}
 		}

  		//w00224 2020-5-27
		HGxScene::CGxLayerEffect* pEffectLayer = HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->getEffectLayer(EFFECT_LAYER_SHADOW);
		if (pEffectLayer)
  		{
			pEffectLayer->setVisible(bChecked);
  		}
 	}
 	else if(strName == tr("map of relief"))
 	{
  		//w00224 2020-5-27   
		HGxScene::CGxLayerEffect* pEffectLayer = HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->getEffectLayer(EFFECT_LAYER_CONTOUR);
  		if (pEffectLayer)
  		{
			pEffectLayer->setVisible(bChecked);
  		}
 	}

	// 发送插件变化事件
 	sendModifiedEvent(true);
}

void CSpecialEffectsControl::settingBtnClickedSlot()
{
	m_pAttrWgt->show();
}

void CSpecialEffectsControl::updateUI( const SEarthEnvInfo &stEarthEnvInfo )
{
	// 1.设置太阳、大气、星辰、阴影、晕渲、云图
	auto iter = m_vecControlItem.begin();

	for (; iter != m_vecControlItem.end(); ++iter)
	{
		QString strControlName = (*iter)->getItemName();

		auto iterMap = stEarthEnvInfo.mapEnvSet.find(strControlName);
		
		// 找到对应的环境特效
		if (iterMap != stEarthEnvInfo.mapEnvSet.end() && iterMap.key() == strControlName)
		{
			(*iter)->setSwitchOn(iterMap.value());
		}
	}

	// 2.设置
	if (m_pAttrWgt)
	{
		// 为防止打开工作空间时初始化环境设置界面所造成的工作空间状态改变的情况，在这里锁住发送给
		// 工作空间告知状态变更的信号
		m_pAttrWgt->blockSignals(true);
		m_pAttrWgt->setSunLightIntensity(stEarthEnvInfo.fSunIntensity);
		m_pAttrWgt->spinBoxRainConcentrationSlot(stEarthEnvInfo.fRainConcentration);
		m_pAttrWgt->spinBoxRainHeightSlot(stEarthEnvInfo.fRainHeight);
		m_pAttrWgt->spinBoxSnowConcentrationSlot(stEarthEnvInfo.fSnowConcentration);
		m_pAttrWgt->spinBoxSnowHeightSlot(stEarthEnvInfo.fSnowHeight);
		m_pAttrWgt->spinBoxYunTuSlot(stEarthEnvInfo.fCloudLowHeight);
		m_pAttrWgt->spinBoxWuHuaSlot(stEarthEnvInfo.fFogFactor);
		m_pAttrWgt->spinBoxOceanHeightSlot(stEarthEnvInfo.fSeaHeight);
		m_pAttrWgt->spinBoxWindVecSlot(stEarthEnvInfo.fSeaWindSeep);
		m_pAttrWgt->spinBoxWindDirSlot(stEarthEnvInfo.fSeaWindDirection);
		m_pAttrWgt->blockSignals(false);
	}
}

void CSpecialEffectsControl::initData()
{
	QFutureWatcher<bool> *pInitDataWatcher = new QFutureWatcher<bool>();
	connect(pInitDataWatcher, SIGNAL(finished()), this, SLOT(getNodeFinishedSlot()));
	// 调用底层数据接口获取界面初始化数据

	//x00038 modifity 海洋不在线程中初始化
	
	if (HGxScene::CGxControllerMgr::getInstance())
	{
		// 暂停三维渲染
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, true);
	}

	if (m_pSkyOption)
	{
		m_pSkyOption->bNebulaVisible = true;
		m_pSkyOption->bSkyBoxVisible = true;
		m_pSkyOption->updateEnvirementNode();
	}
	//end

	pInitDataWatcher->setFuture(QtConcurrent::run(std::bind(&CSpecialEffectsControl::getNodeInThread, this)));
}

void CSpecialEffectsControl::setSceneName(const std::string &strSceneName)
{
	HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);

	if (NULL == pScene)
	{
		return;
	}

	pScene->setSceneName(strSceneName);
}

std::string CSpecialEffectsControl::getSceneID()
{
	return m_strSceneID;
}

std::string CSpecialEffectsControl::getSceneName()
{
	HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);

	if (NULL == pScene)
	{
		return "";
	}

	std::string strSceneName = pScene->getObjectName();

	return strSceneName;
}

std::string CSpecialEffectsControl::getSceneName( std::string unSceneID )
{
	HGxScene::CGxSceneBase* pScene = HGxScene::CGxControllerMgr::getInstance()->getScene(unSceneID);

	if (NULL == pScene)
	{
		return "";
	}

	std::string strSceneName = pScene->getObjectName();

	return strSceneName;
}

bool CSpecialEffectsControl::getNodeInThread()
{
	if (HGxScene::CGxControllerMgr::getInstance())
	{
		// 暂停三维渲染
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, true);
	}

	if (!m_pSkyOption)
	{
		return false;
	}

 	return true;
}

void CSpecialEffectsControl::handleEvent( const ctkEvent& event )
{
	// 场景创建结束后
	if (event.getTopic() == "event/sceneCreated")
	{
		QString strSceneType = event.getProperty("SceneType").toString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			// 记录当前场景id
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();

			// 如果是打开文件创建场景，则不会立刻创建默认图层，而是通过load加载到插件时初始化图层
			QString strCondition = event.getProperty("Condition").toString();
			if (strCondition == "CreatedFromFile")
			{
				return;
			}
			//z00011 增加任务数量
			//FxCore::CLoadInfo::Instance()->addCount();
			//end 

			emit initEnvSignal();
		}
	}
	// 场景切换时
	else if (event.getTopic() == "event/sceneSwitch")
	{
		QString strSceneType = event.getProperty("SceneType").toString();

		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			m_strSceneID = event.getProperty("SceneID").toString().toStdString();
		}
		else
		{
			emit settingWidgetHideSignal();
		}
	}
	else if ("event/sceneRemove" == event.getTopic())
	{
		emit settingWidgetHideSignal();
	}
}

void CSpecialEffectsControl::initEnvSlot()
{
	/*********************此处需要获取主窗口指针******************************/
	//CLoadIndicator::addUseCount(ICore::mainWindowManager()->mainWindow());
	
	initEnvWhenStart();

	initData();
}

bool CSpecialEffectsControl::getEarthEnvData(SEarthEnvInfo &stEarthEnvInfo)
{
	if (!m_pSkyOption)
	{
		// 	//z00011 增加任务数量
		// 	FxCore::CLoadInfo::Instance()->removeCount();
		// 	//end 
		 return false;
	}

	if (!m_pWeatherOption)
	{
		// 	//z00011 增加任务数量
		// 	FxCore::CLoadInfo::Instance()->removeCount();
		// 	//end 
		return false;
	}

	if (!m_pOceanOption)
	{
		// 	//z00011 增加任务数量
		// 	FxCore::CLoadInfo::Instance()->removeCount();
		// 	//end 
		return false;
	}

 	QMap<QString, bool>	mapEnvSet;
 	mapEnvSet.clear();
 
 	if (m_pSkyOption)
 	{
 		mapEnvSet.insert(tr("sun"), m_pSkyOption->bSunVisible);
 		mapEnvSet.insert(tr("atmosphere"), m_pSkyOption->bAtmosVisible);
 		mapEnvSet.insert(tr("star"), m_pSkyOption->bStarVisible);
 		//w00224 2020-5-28
 		mapEnvSet.insert(tr("cloud layer"), m_pSkyOption->bCloudLayer);
 		stEarthEnvInfo.fCloudLowHeight = m_pSkyOption->dCloudMinDis / 1000.0;
 
		stEarthEnvInfo.fSunIntensity = m_pSkyOption->dDensity;
		{
			ctkDictionary sceneSpaceExistMsg;
			sceneSpaceExistMsg.insert("SceneID", getSceneID().c_str());
			sceneSpaceExistMsg.insert("SceneType", GX_EARTH_SCENE_TYPE);

			HGxEventHelperInstance.postEvent("event/EarthEnvCreate", sceneSpaceExistMsg);
		}
 	}

 	if (m_pWeatherOption)
 	{
		mapEnvSet.insert(tr("rain effect"), m_pWeatherOption->bRainVisible);
		stEarthEnvInfo.fRainConcentration = m_pWeatherOption->fRainIntensity;
		stEarthEnvInfo.fRainHeight = m_pWeatherOption->fRainHeight;

		mapEnvSet.insert(tr("snow effect"), m_pWeatherOption->bSnowVisible);
		stEarthEnvInfo.fSnowConcentration = m_pWeatherOption->fSnowIntensity;
		stEarthEnvInfo.fSnowHeight = m_pWeatherOption->fSnowHeight;

		mapEnvSet.insert(tr("fog effect"), m_pWeatherOption->bFogVisible);
		stEarthEnvInfo.fFogFactor = m_pWeatherOption->fFogIntensity;
 	}

  	if (!HGxScene::CGxControllerMgr::getInstance())
  	{
//    		//z00011 增加任务数量
//    		FxCore::CLoadInfo::Instance()->removeCount();
//    		//end 
  
  		return false;
  	}

  	// 通过三维图层服务获取地球阴影和晕渲效果的初始状态

  	//w00224 2020-5-27

	HGxScene::CGxLayerEffect* pNormalEffectLayer = HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->getEffectLayer(EFFECT_LAYER_SHADOW);
  	if (pNormalEffectLayer)
  	{
  		mapEnvSet.insert(tr("shadow"), pNormalEffectLayer->getVisible());
  	}
  
	HGxScene::CGxLayerEffect* pContourEffectLayer = HGxScene::CGxLayerManager::getOrCreateLayerManager(m_strSceneID)->getEffectLayer(EFFECT_LAYER_CONTOUR);
  	if (pContourEffectLayer)
  	{
  		mapEnvSet.insert(tr("map of relief"), pContourEffectLayer->getVisible());
  	}
  		
  	if(m_pOceanOption)
  	{
  		stEarthEnvInfo.fSeaHeight = m_pOceanOption->dSeaLevel;
  		stEarthEnvInfo.fSeaWindSeep = m_pOceanOption->dWindSpeed;
  		stEarthEnvInfo.fSeaWindDirection = m_pOceanOption->dWindDirection;
  		mapEnvSet.insert(tr("ocean"), m_pOceanOption->bOceanVisible);
  	}
 
 	stEarthEnvInfo.mapEnvSet = mapEnvSet;

	return true;
}

bool CSpecialEffectsControl::getNodeFinishedSlot()
{
	if (HGxScene::CGxControllerMgr::getInstance())
	{
		// 暂停三维渲染
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(m_strSceneID, false);
	}

	QFutureWatcher<bool> *pFuture = static_cast<QFutureWatcher<bool> *>(sender());

	if (pFuture->result())
	{
		// 获取环境数据来初始化界面
		SEarthEnvInfo stEarthEnvInfo;
		getEarthEnvData(stEarthEnvInfo);
		updateUI(stEarthEnvInfo);
	}

	pFuture->deleteLater();
	pFuture = NULL;

	// 引用计数减一
	//CLoadIndicator::removeUseCount();

	//z00011 增加任务数量
	//FxCore::CLoadInfo::Instance()->removeCount();

	return true;
}

void CSpecialEffectsControl::envirChangedSlot()
{
	// 发送插件变化事件
  	sendModifiedEvent(true);
}

void CSpecialEffectsControl::settingWidgetHideSlot()
{
	//二维界面隐藏环境设置
	if (nullptr != m_pAttrWgt)
	{
		m_pAttrWgt->hide();
	}
}
