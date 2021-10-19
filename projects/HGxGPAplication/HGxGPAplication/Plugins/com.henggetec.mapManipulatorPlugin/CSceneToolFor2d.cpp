#include <QColorDialog>

#include <ctkPluginContext.h>

#include <service/event/ctkEventAdmin.h>
#include <service/event/ctkEventConstants.h>
//#include <FreeXFramework/FreeXFramework.h>
//#include <scene/SceneServiceUtil.h>
//#include <map_plot_ui/IMapPlotUiService.h>
//#include <map/IMapManipulatorManager.h>

#include "CSceneToolFor2d.h"
#include <FxColorDialog/FxColorPickerWidget.h>
#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>
#include "CSceneToolSelectWidget.h"
#include "CSceneToolItem.h"
#include <QDebug>
#include <QMetaType>
#include <iostream>
//#include <scene_kits/IFxSceneKitsService.h>
//#include <scene_kits/FxScreenKitsTypes.h>
//#include <map_plot_ui/IMapPlotUiService.h>
//#include <map_measure_ui/IMapMeasureUiService.h>
//#include <scene/IFxScene.h>
//#include <scene/IFxSceneService.h>

#include <bdpScene/scene/GxSceneBase.h>
#include <bdpScene/scene/GxSceneCtrl.h>
#include <bdpScene/scene/GxControlManager.h>
#include <bdpScene/manipulator/GxManipulatorManager.h>
#include <bdpTool/kits/GxKitsManager.h>
#include <bdpTool/tool/GxToolManager.h>

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>
#include <gdpScene/manipulator/GxMapManipulator.h>

#include <gdpTool/environment/GxEnvirementCom.h>
#include <bdpTool/environment/GxEnviManager.h>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include "GxUiFrameworkServiceInterface.h"

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
CSceneToolFor2d::CSceneToolFor2d(ctkPluginContext *context, QWidget *parent, Qt::WFlags flags)
#else
CSceneToolFor2d::CSceneToolFor2d(ctkPluginContext *context, QWidget *parent, Qt::WindowFlags flags)
#endif
	: QWidget(parent, flags)
	, m_pContext(context)
	, m_pColorDialog(NULL)
	, m_pSelectWgt(NULL)
	, m_strSceneID("")
{
	qRegisterMetaType<std::string>("std::string");

 	ui.setupUi(this);
 
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

 	initWgt();	
}

CSceneToolFor2d::~CSceneToolFor2d()
{
 	stopManipulation();
 
	HGxTool::CGxKitsManager::getInstance()->clearScence(m_strSceneID);
 
 	//场景工具插件卸载关闭视点同步
 	viewSynchSlot(false);
}

void CSceneToolFor2d::stopManipulation()
{
 	HGxScene::CGxManipulatorManager* pMapManipManager = getMapManipulatorManager();
 	if (pMapManipManager)
 	{
 		pMapManipManager->activeManipulator(FREE_MAP_MANIPULATOR_KEY);
 	}
 	
 	auto iter = m_vecToolItems.begin();
 	for (; iter != m_vecToolItems.end(); ++iter)
 	{
 		CSceneToolItem *pToolItem = *iter;
 		pToolItem->setSelected(false);
 	}
 	if(nullptr != m_pRoamBtn)
 	{
        m_pRoamBtn ->setSelected(true);
 	   // 获取二维地图窗口指针
 	   QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 	   if (pMapSceneRenderWgt)
 	   {
 		   //x0038 modifity
 		   //pMapSceneRenderWgt->setCursor(Qt::OpenHandCursor);
 		   pMapSceneRenderWgt->setCursor(Qt::ArrowCursor);
 	   }
 
 	   HGxScene::CGxManipulatorManager* pMapManipManager = getMapManipulatorManager();
 	   if (pMapManipManager)
 	   {
 		   pMapManipManager->activeManipulator(FREE_MAP_MANIPULATOR_KEY);
 	   }
 	}
 	
 	// 获取二维地图窗口指针
 	QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 	if (pMapSceneRenderWgt)
 	{
 		pMapSceneRenderWgt->setCursor(Qt::ArrowCursor);
 	}
}

void CSceneToolFor2d::preStart()
{
	/***************暂时不处理标绘*********************/

// 	IMapPlotUiService*pPlotService =getService<IMapPlotUiService>();
// 	if (pPlotService)
// 	{
// 		pPlotService->stopPlotting();
// 	}
// 	IMapMeasureUiService *pMeasureService = getService<IMapMeasureUiService>();
// 	if (pMeasureService)
// 	{
// 		pMeasureService->stopMeasureting();
// 	}
}

void CSceneToolFor2d::initWgt()
{
 	m_vecToolItems.clear();
 
 	//加入工具界面并设置其样式
 	m_pSelectWgt = new CSceneToolSelectWidget(this);
 	QVBoxLayout *pToolLayOut = new QVBoxLayout(ui.toolWgt);
 	pToolLayOut->setContentsMargins(0,0,0,0);
 	pToolLayOut->addWidget(m_pSelectWgt);
 	ui.toolWgt->setLayout(pToolLayOut);
 
 	//加入工具按钮并设置样式
 	QPixmap fwPixmap(":/map_manipulator/Resources/images/reset.png");
 	QPixmap fwPixmap1(":/map_manipulator/Resources/images/reset_h.png");
 	QPixmap fwPixmap2(":/map_manipulator/Resources/images/reset_s.png");
 	CSceneToolItem *pFwBtn = m_pSelectWgt->addPlotType(false, tr("reset"), fwPixmap, fwPixmap1, fwPixmap2);
 	QPixmap xzPixmap(":/map_manipulator/Resources/images/select.png");
 	QPixmap xzPixmap1(":/map_manipulator/Resources/images/select_h.png");
 	QPixmap xzPixmap2(":/map_manipulator/Resources/images/select_s.png");
 	//CSceneToolItem *pXzBtn = m_pSelectWgt->addPlotType(true, "选择", xzPixmap, xzPixmap1, xzPixmap2);
 	QPixmap myPixmap(":/map_manipulator/Resources/images/roam.png");
 	QPixmap myPixmap1(":/map_manipulator/Resources/images/roam_h.png");
 	QPixmap myPixmap2(":/map_manipulator/Resources/images/roam_s.png");
 	m_pRoamBtn = m_pSelectWgt->addPlotType(true, "漫游", myPixmap, myPixmap1, myPixmap2);
 	QPixmap fdPixmap(":/map_manipulator/Resources/images/zoomIn.png");
 	QPixmap fdPixmap1(":/map_manipulator/Resources/images/zoomIn_h.png");
 	QPixmap fdPixmap2(":/map_manipulator/Resources/images/zoomIn_s.png");
 	CSceneToolItem *pFdBtn = m_pSelectWgt->addPlotType(true, "放大", fdPixmap, fdPixmap1, fdPixmap2);
 	QPixmap sxPixmap(":/map_manipulator/Resources/images/zoomOut.png");
 	QPixmap sxPixmap1(":/map_manipulator/Resources/images/zoomOut_h.png");
 	QPixmap sxPixmap2(":/map_manipulator/Resources/images/zoomOut_s.png");
 	CSceneToolItem *pSxBtn = m_pSelectWgt->addPlotType(true, "缩小", sxPixmap, sxPixmap1, sxPixmap2);
 	QPixmap bjPixmap(":/map_manipulator/Resources/images/mapBackColor.png");
 	QPixmap bjPixmap1(":/map_manipulator/Resources/images/mapBackColor_h.png");
 	QPixmap bjPixmap2(":/map_manipulator/Resources/images/mapBackColor_s.png");
 	CSceneToolItem *pBjBtn = m_pSelectWgt->addPlotType(false, "地图背景色", bjPixmap, bjPixmap1, bjPixmap2);
 	
 	m_vecToolItems.push_back(m_pRoamBtn);
 	m_vecToolItems.push_back(pFdBtn);
 	m_vecToolItems.push_back(pSxBtn);
 
 	//信号槽连接
 	connect(pFwBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(sceneResetSlot()));
 	connect(pFdBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(zoomInSlot(bool)));
 	connect(m_pRoamBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(roamSlot(bool)));
 	connect(pSxBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(zoomOutSlot(bool)));
 	//connect(pXzBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(selectSlot(bool)));
 	connect(pBjBtn, SIGNAL(itemClickedSignal(bool)), this, SLOT(backgroundColorSlot()));
 
 	connect(ui.synchPushBtn, SIGNAL(toggled(bool)), this, SLOT(viewSynchSlot(bool)));
 	connect(this, SIGNAL(initRoamStatusSignal()), this, SLOT(initRoamStatusSlot()), Qt::QueuedConnection);
 	connect(this, SIGNAL(setSyncBtnSignal()), this, SLOT(setSyncBtnSlot()), Qt::QueuedConnection);
 
 	connect(ui.airViewPushBtn, SIGNAL(toggled(bool)), this, SLOT(airViewBtnSlot(bool)));
 	connect(ui.navigatorPushBtn, SIGNAL(toggled(bool)), this, SLOT(navigatorBtnSlot(bool)));
 	connect(ui.mapScalePushBtn, SIGNAL(toggled(bool)), this, SLOT(mapScaleBtnSlot(bool)));
 	connect(ui.graticulePushBtn, SIGNAL(toggled(bool)), this, SLOT(graticuleBtnSlot(bool)));
	connect(ui.sceneInfoPushBtn, SIGNAL(toggled(bool)), this, SLOT(sceneInfoBtnSlot(bool)));
	connect(ui.twilightPushBtn, SIGNAL(toggled(bool)), this, SLOT(twilightBtnSlot(bool)));

	connect(this, SIGNAL(activeSceneSignal(std::string)), this, SLOT(activeSceneSlot(std::string)));
}

 HGxScene::CGxManipulatorManager* CSceneToolFor2d::getMapManipulatorManager()
 {
 	return HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID)->getManipulatorSystem();
 }

 HGxScene::CGxSceneBase* CSceneToolFor2d::getMapScene()
 {
 	return HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID);
 }

void CSceneToolFor2d::sceneResetSlot()
{
	HGxScene::CGxMapManipulator* pMapMani = static_cast<HGxScene::CGxMapManipulator*>(HGxScene::CGxControllerMgr::getInstance()->getScene(m_strSceneID)->getManipulatorSystem()->getCurrentManipulator());
	
	if (pMapMani)
	{
		pMapMani->locateHome(2);
	}
}

void CSceneToolFor2d::zoomInSlot(bool bZoomIn)
{
 	preStart();
 	if(m_pRoamBtn && m_pRoamBtn->isSelected())
 	{
 		m_pRoamBtn ->setSelected(false);
 		roamSlot(false);
 	}	
 	// 获取二维地图窗口指针
 	QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 	if (pMapSceneRenderWgt)
 	{
 		 pMapSceneRenderWgt->setCursor(QCursor(QPixmap(":/map_manipulator/Resources/images/zoomin_cursor.png")));
 	}
 
 	HGxScene::CGxManipulatorManager* pMapManipulator = getMapManipulatorManager();
 	if (pMapManipulator)
 	{
 		pMapManipulator->activeManipulator(ZOOM_IN_MANIPULATOR_KEY);
 	}
}

void CSceneToolFor2d::zoomOutSlot(bool bZoomOut)
{
	preStart();
	if (m_pRoamBtn && m_pRoamBtn->isSelected())
	{
		m_pRoamBtn->setSelected(false);
		roamSlot(false);
	}
	if (getMapSceneRenderWgt())
	{
		getMapSceneRenderWgt()->setCursor(QCursor(QPixmap(":/map_manipulator/Resources/images/zoomout_cursor.png")));
	}

	HGxScene::CGxManipulatorManager* pMapManipulator = getMapManipulatorManager();
	if (pMapManipulator)
	{
		pMapManipulator->activeManipulator(ZOOM_OUT_MANIPULATOR_KEY);
	}
}

void CSceneToolFor2d::roamSlot(bool bRoam)
{
 	preStart();
 	// 获取二维地图窗口指针
 	QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 	if (pMapSceneRenderWgt)
 	{
 		//x00038 modifity
 		//pMapSceneRenderWgt->setCursor(Qt::OpenHandCursor);
 		pMapSceneRenderWgt->setCursor(Qt::ArrowCursor);
 	}
 
 	HGxScene::CGxManipulatorManager* pMapManipulator = getMapManipulatorManager();
 	if (pMapManipulator)
 	{
 		pMapManipulator->activeManipulator(FREE_MAP_MANIPULATOR_KEY);
 	}
}

void CSceneToolFor2d::selectSlot(bool bSelected)
{
 	//preStart();
 	// 获取二维地图窗口指针
 	QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 	if (pMapSceneRenderWgt)
 	{
 		bSelected == true ? pMapSceneRenderWgt->setCursor(QCursor(QPixmap(":/map_manipulator/Resources/images/select_cursor.png")))
 			: pMapSceneRenderWgt->setCursor(Qt::ArrowCursor);
 	}
 
 	/***************暂时不处理标绘*********************/
 
 // 	IMapPlotUiService *pMapPlotUiService = getService<IMapPlotUiService>();
 // 
 // 	if (pMapPlotUiService)
 // 	{
 // 		bSelected == true ? pMapPlotUiService->startPicking()
 // 			: pMapPlotUiService->stopPicking();
 // 	}
}

void CSceneToolFor2d::backgroundColorSlot()
{
 	preStart();
 
 	HGxScene::CGxSceneBase* pMapScene = getMapScene();
 	if (pMapScene == nullptr)
 	{
 		return;
 	}
 
 	if (m_pColorDialog == nullptr)
 	{
 		m_pColorDialog = new FxColorPickerWidget(this);
 	}
 
    //获取当前背景颜色
    osg::Vec4d vecColor = getMapScene()->getBackground() * 255;
    m_pColorDialog->SetCurrentColor(QColor(vecColor.r(), vecColor.g(), vecColor.b(), vecColor.a()));
    
    FxColorPickerWidget::ButtonRole role = m_pColorDialog->showDialog();
    if (role == FxColorPickerWidget::Yes)
    {
    	QColor color = m_pColorDialog->GetCurrentColor();
    	osg::Vec4d Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  	getMapScene()->setBackground(Color);
    }
}

void CSceneToolFor2d::viewSynchSlot( bool bSynch )
{
 	preStart();
 
 	int iControllerID = getMapScene()->getController();
 	HGxScene::CGxControllerMgr::getInstance()->setViewSync(bSynch, iControllerID);
 
 	// 发送信号给三维场景工具插件，使其界面同步更新
 	ctkDictionary message;
 	message.insert("SynchViewpoint", bSynch);
 
	//异步发送事件
	HGxEventHelperInstance.postEvent("scene/synchviewpoint", message);
}
void CSceneToolFor2d::handleEvent( const ctkEvent& event )
{
 	if (event.getTopic() == "event/sceneCreated")
 	{
 		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
 		std::string sceneId = event.getProperty("SceneID").toString().toStdString();

 		if (strSceneType == GX_MAP_SCENE_TYPE)
 		{
			m_strSceneID = sceneId;

			//视点同步按钮初始化
			initSyncBtnStatus();

 			emit initRoamStatusSignal();

			HGxTool::CGxKitsManager* pp = HGxTool::CGxKitsManager::getInstance();

 			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_SCREENINFO, sceneId);
 
 			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_MAPSCALE, sceneId);
 
 			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_AIRVIEW, sceneId);
 
 			HGxTool::CGxKitsManager::getInstance()->create(SCREEN_KITS_TYPE_NAVIGATOR, sceneId);
 
 			bool bRes = HGxTool::CGxToolManager::getInstance()->create(SCREEN_KITS_TYPE_GRATICULE, sceneId);
 
 			bRes = HGxTool::CGxEnviManager::getInstance()->create(FREE_MAP_TERMINSTOR_KEY, sceneId);
 		}
 		emit setSyncBtnSignal();
 	}
	else if (event.getTopic() == "scene/synchviewpoint")
	{
		bool bSynchInfo = event.getProperty("SynchViewpoint").toBool();

		// 只是使界面同步信息保持一致，不需要调用底层视点同步接口,因此在这里需要锁住信号
		ui.synchPushBtn->blockSignals(true);

		ui.synchPushBtn->setChecked(bSynchInfo);

		ui.synchPushBtn->blockSignals(false);
	}
	else if (event.getTopic() == "event/sceneSwitch")
	{
		std::string strSceneType = event.getProperty("SceneType").toString().toStdString();
		std::string sceneId = event.getProperty("SceneID").toString().toStdString();
		if (strSceneType != GX_MAP_SCENE_TYPE)
		{
			if (m_pColorDialog != nullptr)
			{
				if (m_pColorDialog->isVisible())
				{
					m_pColorDialog->hide();
				}
			}
		}
		m_strSceneID = sceneId;

		//通过工作空间创建场景
		QString strCondition = event.getProperty("Condition").toString();
		if (strCondition == "CreatedFromFile")
		{
			emit setSyncBtnSignal();
		}
	}
	else if (event.getTopic() == "event/sceneRemove")
	{
		// 初始化视点同步按钮状态
		setSyncBtnSlot();
	}
	else if (event.getTopic() == "com_henggetec_uiFramework")
	{
		QString sceneId = event.getProperty("id").toString();
	}
}

void CSceneToolFor2d::initRoamStatusSlot()
{
 	if (m_pRoamBtn)
 	{
 		m_pRoamBtn->setSelected(true);
 		if (m_pSelectWgt)
 		{
 			m_pSelectWgt->setCurrentItem(m_pRoamBtn);
 		}
 		// 获取二维地图窗口指针
 		QWidget *pMapSceneRenderWgt = getMapSceneRenderWgt();
 
 		if (pMapSceneRenderWgt)
 		{
 			//x0038 modifity
 			//pMapSceneRenderWgt->setCursor(Qt::OpenHandCursor);
 			pMapSceneRenderWgt->setCursor(Qt::ArrowCursor);
 		}
 	}
}

QWidget* CSceneToolFor2d::getMapSceneRenderWgt()
{
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

void CSceneToolFor2d::stopPlot()
{

	/**************暂时不处理标绘***************/

// 	IMapPlotUiService *pService = getService<IMapPlotUiService>();
// 	if (pService)
// 	{
// 		pService->stopPlotting();
// 	}
}

void CSceneToolFor2d::initSyncBtnStatus()
{
  	ui.synchPushBtn->blockSignals(true);
 
 	int id = getMapScene()->getController();
 
 	HGxScene::CGxControllerMgr::getInstance()->getViewSync(id) == true ? ui.synchPushBtn->setChecked(true) : ui.synchPushBtn->setChecked(false);
 
  	ui.synchPushBtn->blockSignals(false);
}

void CSceneToolFor2d::setSyncBtnSlot()
{
	// 只有一个场景时，不处理按钮状态
}

void CSceneToolFor2d::setKitsStatue( bool bVisiabled, const std::string strKey, std::string sceneId)
{
	if (bVisiabled)
	{
		if (strKey == SCREEN_KITS_TYPE_GRATICULE)
		{
			HGxTool::CGxToolManager::getInstance()->open(strKey, sceneId);
		}
		else if (strKey == FREE_MAP_TERMINSTOR_KEY)
		{
			HGxTool::CGxEnviManager::getInstance()->open(strKey, sceneId);
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
		else if (strKey == FREE_MAP_TERMINSTOR_KEY)
		{
			HGxTool::CGxEnviManager::getInstance()->close(strKey, sceneId);
		}
		else
		{
			HGxTool::CGxKitsManager::getInstance()->close(strKey, sceneId);
		}
	}
}

void CSceneToolFor2d::airViewBtnSlot( bool bVisiabled )
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_AIRVIEW, m_strSceneID);
}

void CSceneToolFor2d::navigatorBtnSlot( bool bVisiabled )
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_NAVIGATOR, m_strSceneID);
}

void CSceneToolFor2d::mapScaleBtnSlot( bool bVisiabled )
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_MAPSCALE, m_strSceneID);
}

void CSceneToolFor2d::graticuleBtnSlot( bool bVisiabled )
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_GRATICULE, m_strSceneID);
}

void CSceneToolFor2d::sceneInfoBtnSlot( bool bVisiabled )
{
	setKitsStatue(bVisiabled, SCREEN_KITS_TYPE_SCREENINFO, m_strSceneID);
}

void CSceneToolFor2d::twilightBtnSlot(bool bVisiabled)
{
	setKitsStatue(bVisiabled, FREE_MAP_TERMINSTOR_KEY, m_strSceneID);
}

void CSceneToolFor2d::activeSceneSlot(std::string str)
{
	HGxScene::CGxControllerMgr::getInstance()->activeSyncScene(str);
}

std::string CSceneToolFor2d::getToolsOpen()
{
	std::string list;

	if (ui.airViewPushBtn->isChecked())
	{
		list += SCREEN_KITS_TYPE_AIRVIEW;
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

	if (ui.twilightPushBtn->isChecked())
	{
		list += ",";
		list += FREE_MAP_TERMINSTOR_KEY;
	}

	return list;
}

void CSceneToolFor2d::initToolUI( QStringList strTools )
{
	ui.airViewPushBtn->blockSignals(true);
	ui.navigatorPushBtn->blockSignals(true);
	ui.mapScalePushBtn->blockSignals(true);
	ui.graticulePushBtn->blockSignals(true);
	ui.sceneInfoPushBtn->blockSignals(true);
	ui.twilightPushBtn->blockSignals(true);

	//初始化时保证所有按钮的状态都为false
	ui.airViewPushBtn->setChecked(false);
	ui.navigatorPushBtn->setChecked(false);
	ui.mapScalePushBtn->setChecked(false);
	ui.graticulePushBtn->setChecked(false);
	ui.sceneInfoPushBtn->setChecked(false);
	ui.twilightPushBtn->setChecked(false);

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
		else if (FREE_MAP_TERMINSTOR_KEY == tool)
		{
			ui.twilightPushBtn->setChecked(true);
		}
	}

	//w00224 2020-6-11 工作空间切换后，场景信息的按钮需要和场景中信息显示相对应
	airViewBtnSlot(ui.airViewPushBtn->isChecked());
	navigatorBtnSlot(ui.navigatorPushBtn->isChecked());
	mapScaleBtnSlot(ui.mapScalePushBtn->isChecked());
	graticuleBtnSlot(ui.graticulePushBtn->isChecked());
	sceneInfoBtnSlot(ui.sceneInfoPushBtn->isChecked());
	twilightBtnSlot(ui.twilightPushBtn->isChecked());

	ui.airViewPushBtn->blockSignals(false);
	ui.navigatorPushBtn->blockSignals(false);
	ui.mapScalePushBtn->blockSignals(false);
	ui.graticulePushBtn->blockSignals(false);
	ui.sceneInfoPushBtn->blockSignals(false);
	ui.twilightPushBtn->blockSignals(false);
}
