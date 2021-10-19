#include <ctkPluginContext.h>
#include <service/event/ctkEventAdmin.h>
#include <service/event/ctkEventConstants.h>

//#include <FreeXFramework/FreeXFramework.h>
//#include <scene/SceneServiceUtil.h>
//#include <scene/IFxSceneService.h>
//#include <earth/IFxEarthManipulatorManager.h>
//#include <earth/IFxEarthScene.h>
//#include <earth_plot_ui/IEarthPlotUiService.h>
#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>
#include "CSceneTool.h"
//#include <scene_kits/IFxSceneKitsService.h>
//#include <scene_kits/FxScreenKitsTypes.h>
#include <FxLoadIndicator/CLoadIndicator.h>
#include <QtConcurrentMap>
//#include <ICore.h>
#include <QtConcurrentRun>
//#include <CMainWindowManager.h>
#include <functional>
#include <QFutureWatcher>
#include <QMetaType>
#include <iostream>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxSceneCtrl.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpTool/kits/GxKitsManager.h>
#include <bdpTool/tool/GxToolManager.h>

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>
#include <gdpScene/manipulator/GxEarthManipulator.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include "GxUiFrameworkServiceInterface.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
CSceneTool::CSceneTool(ctkPluginContext *context, QWidget *parent, Qt::WFlags flags)
#else
CSceneTool::CSceneTool(ctkPluginContext *context, QWidget *parent, Qt::WindowFlags flags)
#endif
	: QWidget(parent, flags)
	, m_pContext(context)
	, m_pRoamBtn(NULL)
	, m_pSelectWgt(NULL)
	, m_strSceneID("")
{
	qRegisterMetaType<std::string>("std::string");

	ui.setupUi(this);//toolWgt

	// 接收创造场景时发来的事件
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneCreated", HGxFWLauncherInstance.getPluginContext());

	// 接收场景切换事件
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneSwitch", HGxFWLauncherInstance.getPluginContext());

	// 接收视点同步事件
	HGxEventHelperInstance.subscribeTopic(this, "scene/synchviewpoint", HGxFWLauncherInstance.getPluginContext());

	// 接收删除场景事件
	HGxEventHelperInstance.subscribeTopic(this, "event/sceneRemove", HGxFWLauncherInstance.getPluginContext());

	// 接收场景激活事件
	HGxEventHelperInstance.subscribeTopic(this, "com_henggetec_uiFramework", HGxFWLauncherInstance.getPluginContext());

	m_vecToolItems.clear();

	// 初始化界面
	m_pSelectWgt = new CSceneToolSelectWidget(this);
	QVBoxLayout *pToolLayOut = new QVBoxLayout(ui.toolWgt);
	pToolLayOut->setContentsMargins(0,0,0,0);
	pToolLayOut->addWidget(m_pSelectWgt);
	ui.toolWgt->setLayout(pToolLayOut);

	//视点同步按钮初始化
	//initSyncBtnStatus();

	//加入工具按钮并设置样式
	QPixmap twoPixmapOff(":/Resources/images/reset.png");
	QPixmap twoPixmapOff1(":/Resources/images/reset_h.png");
	QPixmap twoPixmapOff2(":/Resources/images/reset_s.png");
	CSceneToolItem *pFwBtn = m_pSelectWgt->addPlotType(false, tr("reset"), twoPixmapOff, twoPixmapOff1, twoPixmapOff2);

	//QPixmap xzPixmap(":/Resources/images/select.png");
	//QPixmap xzPixmap1(":/Resources/images/select_h.png");
	//QPixmap xzPixmap2(":/Resources/images/select_s.png");
	//CSceneToolItem *pXzBtn = m_pSelectWgt->addPlotType(true, "选择", xzPixmap, xzPixmap1, xzPixmap2);

	//QPixmap myPixmap(":/Resources/images/roam.png");
	//QPixmap myPixmap1(":/Resources/images/roam_h.png");
	//QPixmap myPixmap2(":/Resources/images/roam_s.png");
	//m_pRoamBtn = m_pSelectWgt->addPlotType(true, "漫游", myPixmap, myPixmap1, myPixmap2);

	m_vecToolItems.push_back(pFwBtn);

	connect(pFwBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(sceneResetSlot()));
	//connect(pXzBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(xzBtnClickedSlot(bool)));
	connect(ui.synchPushBtn, SIGNAL(toggled(bool)), this, SLOT(viewSynchSlot(bool)));
	connect(this, SIGNAL(initRoamStatusSignal()), this, SLOT(initRoamStatusSlot()));
	connect(this, SIGNAL(setSyncBtnSignal()), this, SLOT(setSyncBtnSlot()));
	connect(ui.airViewPushBtn, SIGNAL(toggled(bool)), this, SLOT(airViewBtnSlot(bool)));
	connect(ui.navigatorPushBtn, SIGNAL(toggled(bool)), this, SLOT(navigatorBtnSlot(bool)));
	connect(ui.mapScalePushBtn, SIGNAL(toggled(bool)), this, SLOT(mapScaleBtnSlot(bool)));
	connect(ui.graticulePushBtn, SIGNAL(toggled(bool)), this, SLOT(graticuleBtnSlot(bool)));
	connect(ui.sceneInfoPushBtn, SIGNAL(toggled(bool)), this, SLOT(sceneInfoBtnSlot(bool)));

	connect(this, SIGNAL(activeSceneSignal(std::string)), this, SLOT(activeSceneSlot(std::string)));
}

CSceneTool::~CSceneTool()
{
	stopManipulation();
	
	HGxTool::CGxKitsManager::getInstance()->clearScence(m_strSceneID);

 	//场景工具插件卸载关闭视点同步
 	viewSynchSlot(false);
}

void CSceneTool::stopManipulation()
{
	auto iter = m_vecToolItems.begin();
	for (; iter != m_vecToolItems.end(); ++iter)
	{
		CSceneToolItem *pToolItem = *iter;
		if (pToolItem)
		{
			pToolItem->setSelected(false);
		}
	}
	// 获取二维地图窗口指针
	QWidget *pEarthSceneRenderWgt = getEarthSceneRenderWgt();
	if (pEarthSceneRenderWgt)
	{
		pEarthSceneRenderWgt->setCursor(Qt::ArrowCursor);
	}
}

void CSceneTool::sceneResetSlot()
{
	HGxScene::CGxEarthManipulator* pEarthMani = static_cast<HGxScene::CGxEarthManipulator*>(HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID)->getManipulatorSystem()->getCurrentManipulator());

	if (pEarthMani)
	{
		pEarthMani->locateHome(2);
	}
}

void CSceneTool::viewSynchSlot( bool bSynch )
{
	int iControllerID = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID)->getController();
	HGxScene::CGxControllerMgr::getInstance()->setViewSync(bSynch, iControllerID);

 	// 发送信号给二维场景工具插件，使其界面同步更新
	ctkDictionary message;
	message.insert("SynchViewpoint", bSynch);

	//异步发送事件
	HGxEventHelperInstance.postEvent("scene/synchviewpoint", message);
}

void CSceneTool::handleEvent( const ctkEvent& event )
{
	if (event.getTopic() == "event/sceneCreated")
	{
		QString strSceneType = event.getProperty("SceneType").toString();
		std::string sceneId = event.getProperty("SceneID").toString().toStdString();
 		if (strSceneType == GX_EARTH_SCENE_TYPE)
 		{
 			m_strSceneID = sceneId;
 
 			// todo 在底层没有将鼠标状态改好，暂时不用将鼠标置为手状态, 注释掉此信号
 			emit initRoamStatusSignal();
 		
			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_SCREENINFO, sceneId);

			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_MAPSCALE, sceneId);

			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_AIRVIEW, sceneId);

			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_NAVIGATOR, sceneId);

			//bool bRes = HGxTool::CGxToolManager::getInstance()->create(SCREEN_KITS_TYPE_GRATICULE, sceneId);
		}
 		// 在存在一个场景时，将视点同步按钮置为不可用状态
 		emit setSyncBtnSignal();
	}
	else if (event.getTopic() == "event/sceneSwitch")
	{
		QString strSceneType = event.getProperty("SceneType").toString();
		std::string sceneId = event.getProperty("SceneID").toString().toStdString();
		if (strSceneType == GX_EARTH_SCENE_TYPE)
		{
			m_strSceneID = sceneId;

			//通过工作空间创建场景
			QString strCondition = event.getProperty("Condition").toString();
			if (strCondition == "CreatedFromFile")
			{
				emit setSyncBtnSignal();
			}
		}
	}
	else if (event.getTopic() == "scene/synchviewpoint")
	{
		bool bSynchInfo = event.getProperty("SynchViewpoint").toBool();

		// 只是使界面同步信息保持一致，不需要调用底层视点同步接口,因此在这里需要锁住信号
		ui.synchPushBtn->blockSignals(true);

		ui.synchPushBtn->setChecked(bSynchInfo);

		ui.synchPushBtn->blockSignals(false);
	}
	else if (event.getTopic() == "event/sceneRemove")
	{
		// 初始化视点同步按钮状态
		setSyncBtnSlot();
	}
	else if (event.getTopic() == "com_henggetec_uiFramework")
	{
		QString sceneId = event.getProperty("id").toString();
		//W00085
// 		CGxUiFrameworkServiceInterface* pServiceUI = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());
// 
// 		if (pServiceUI)
// 		{
// 			std::string strSceneType = pServiceUI->dockUserData(sceneId, Qt::UserRole).toString().toStdString();
// 
// 			if (strSceneType == GX_EARTH_SCENE_TYPE)
// 			{
// 				emit activeSceneSignal(sceneId.toStdString());
// 			}
// 		}
	}
}

void CSceneTool::selectSlot( bool bSelected)
{
 	if (getEarthSceneRenderWgt())
 	{
 		bSelected == true ? getEarthSceneRenderWgt()->setCursor(QCursor(QPixmap(":/map_manipulator/Resources/images/select_cursor.png")))
 			: getEarthSceneRenderWgt()->setCursor(Qt::OpenHandCursor);
 	}

	/****************暂时不处理标绘*****************/

// 	IEarthPlotUiService *pEarthPlotUiService = getService<IEarthPlotUiService>();
// 
// 	if (pEarthPlotUiService)
// 	{
// 		bSelected == true ? pEarthPlotUiService->startPicking()
// 			: pEarthPlotUiService->stopPicking();
// 	}
}

QWidget* CSceneTool::getEarthSceneRenderWgt()
{
	/****************此处需要获取服务**************/

 	QWidget *pRenderWgt = NULL;
// 	ISceneUiService *pSceneUiService = getService<ISceneUiService>();
// 	if (pSceneUiService)
// 	{
// 		ISceneInterface *pInterface = pSceneUiService->scene(pSceneUiService->activeScene());
// 
// 		if (pInterface)
// 		{
// 			pRenderWgt = pInterface->getSceneRenderWgt();
// 		}
// 	}

	return pRenderWgt;
}

void CSceneTool::stopPlot()
{
	/***************暂时不支持标绘****************/

// 	IEarthPlotUiService *pService = getService<IEarthPlotUiService>();
// 	if (pService)
// 	{
// 		pService->stopPlotting();
// 	}
}

void CSceneTool::initSyncBtnStatus()
{
	/***************此处需要获取视点同步状态*********************/

	ui.synchPushBtn->blockSignals(true);

	int id = HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID)->getController();

	HGxScene::CGxControllerMgr::getInstance()->getViewSync(id) == true ? ui.synchPushBtn->setChecked(true) : ui.synchPushBtn->setChecked(false);

	ui.synchPushBtn->blockSignals(false);
}

void CSceneTool::initRoamStatusSlot()
{
	// 获取三维地球窗口指针
	QWidget *pEarthSceneRenderWgt = getEarthSceneRenderWgt();
	if (pEarthSceneRenderWgt)
	{
		//x0038 modifity
		//pEarthSceneRenderWgt->setCursor(Qt::OpenHandCursor);
		pEarthSceneRenderWgt->setCursor(Qt::ArrowCursor);
	}

	// 漫游的时候停止标绘
	stopPlot();
// 	if (m_pRoamBtn)
// 	{
// 		m_pRoamBtn->setSelected(true);
// 		if (m_pSelectWgt)
// 		{
// 			m_pSelectWgt->setCurrentItem(m_pRoamBtn);
// 		}
// 
// 		if (pEarthSceneRenderWgt)
// 		{
// 			pEarthSceneRenderWgt->setCursor(Qt::OpenHandCursor);
// 		}
// 	}
}

void CSceneTool::setSyncBtnSlot()
{

}

void CSceneTool::airViewBtnSlot(bool bVisiabled)
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_AIRVIEW, m_strSceneID);
}

void CSceneTool::navigatorBtnSlot(bool bVisiabled)
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_NAVIGATOR, m_strSceneID);
}

void CSceneTool::mapScaleBtnSlot(bool bVisiabled)
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_MAPSCALE, m_strSceneID);
}

void CSceneTool::graticuleBtnSlot(bool bVisiabled)
{
	if(bVisiabled)
	{
		//CLoadIndicator::addUseCount(ICore::mainWindowManager()->mainWindow());

		bool bCreate =  HGxTool::CGxToolManager::getInstance()->create(SCREEN_KITS_TYPE_GRATICULE, m_strSceneID);

		QFutureWatcher<void>* pWatcher = new QFutureWatcher<void>();
		pWatcher->setFuture(QtConcurrent::run(std::bind(&CSceneTool::setKitsStatue, this, bVisiabled, SCREEN_KITS_TYPE_GRATICULE, "", true)));
	}

	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_GRATICULE, m_strSceneID);
}

void CSceneTool::sceneInfoBtnSlot(bool bVisiabled)
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_SCREENINFO, m_strSceneID);
}

void CSceneTool::activeSceneSlot(std::string str)
{
	HGxScene::CGxControllerMgr::getInstance()->activeSyncScene(str);
}

void CSceneTool::setKitsStatue( bool bVisiabled, const std::string strKey, std::string sceneId, bool bThreadCall/*=false*/)
{
	if(bThreadCall)
	{
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(sceneId, true);
	}

	if (bVisiabled)
	{
		if (strKey == SCREEN_KITS_TYPE_GRATICULE)
		{
			HGxTool::CGxToolManager::getInstance()->open(strKey, sceneId);
		}
		else
		{
			HGxTool::CGxKitsManager::getInstance()->open(strKey, sceneId);
		}
	}
	else
	{
		if (strKey == SCREEN_KITS_TYPE_GRATICULE)
		{
			HGxTool::CGxToolManager::getInstance()->close(strKey, sceneId);
		}
		else
		{
			HGxTool::CGxKitsManager::getInstance()->close(strKey, sceneId);
		}
	}

	if(bThreadCall)
	{
		//CLoadIndicator::removeUseCount();
		// 线程中处理恢复渲染
		HGxScene::CGxControllerMgr::getInstance()->pauseSimulation(sceneId, false);
	}
}

std::string CSceneTool::getToolsOpen()
{
	std::string list;

	if (ui.airViewPushBtn->isChecked())
	{
		list += (SCREEN_KITS_TYPE_AIRVIEW);
	}

	if (ui.navigatorPushBtn->isChecked())
	{
		list += ",";
		list += SCREEN_KITS_TYPE_NAVIGATOR;
	}

	if (ui.mapScalePushBtn->isChecked())
	{
		list += ",";
		list += SCREEN_KITS_TYPE_MAPSCALE;
	}

	if (ui.graticulePushBtn->isChecked())
	{
		list += ",";
		list += SCREEN_KITS_TYPE_GRATICULE;
	}

	if (ui.sceneInfoPushBtn->isChecked())
	{
		list += ",";
		list += SCREEN_KITS_TYPE_SCREENINFO;
	}

	return list;
}

void CSceneTool::initToolUI( QStringList strTools )
{
	ui.airViewPushBtn->blockSignals(true);
	ui.navigatorPushBtn->blockSignals(true);
	ui.mapScalePushBtn->blockSignals(true);
	ui.graticulePushBtn->blockSignals(true);
	ui.sceneInfoPushBtn->blockSignals(true);

	//初始化时保证所有按钮的状态都为false
	ui.airViewPushBtn->setChecked(false);
	ui.navigatorPushBtn->setChecked(false);
	ui.mapScalePushBtn->setChecked(false);
	ui.graticulePushBtn->setChecked(false);
	ui.sceneInfoPushBtn->setChecked(false);

	for (int i=0; i<strTools.size(); ++i)
	{
		std::string tool = strTools.at(i).toStdString();
		if (SCREEN_KITS_TYPE_AIRVIEW == tool)
		{
			ui.airViewPushBtn->setChecked(true);
		}
		else if (SCREEN_KITS_TYPE_NAVIGATOR == tool)
		{
			ui.navigatorPushBtn->setChecked(true);
		}
		else if (SCREEN_KITS_TYPE_MAPSCALE == tool)
		{
			ui.mapScalePushBtn->setChecked(true);
		}
		else if (SCREEN_KITS_TYPE_GRATICULE == tool)
		{
			ui.graticulePushBtn->setChecked(true);
		}
		else if (SCREEN_KITS_TYPE_SCREENINFO == tool)
		{
			ui.sceneInfoPushBtn->setChecked(true);
		}
	}

	//w00224 2020-6-11 工作空间切换后，场景信息的按钮需要和场景中信息显示相对应
	airViewBtnSlot(ui.airViewPushBtn->isChecked());
	navigatorBtnSlot(ui.navigatorPushBtn->isChecked());
	mapScaleBtnSlot(ui.mapScalePushBtn->isChecked());
	graticuleBtnSlot(ui.graticulePushBtn->isChecked());
	sceneInfoBtnSlot(ui.sceneInfoPushBtn->isChecked());


	ui.airViewPushBtn->blockSignals(false);
	ui.navigatorPushBtn->blockSignals(false);
	ui.mapScalePushBtn->blockSignals(false);
	ui.graticulePushBtn->blockSignals(false);
	ui.sceneInfoPushBtn->blockSignals(false);
}
