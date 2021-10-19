#include "GxProcessor.h"


#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

#include <QStackedWidget>
#include <QDebug>
#include <QDateTime>

#include <FxLoadIndicator/CLoadIndicator.h>

#include "QtitanDocking.h"

#define  WITHOSG		1
#if WITHOSG
#include "GxUiFrameworkServiceInterface.h"
#include <simu_control_ui/ISimuControlUiService.h>
#include <earth_manipulator_ui/ISceneToolUiService.h>
#include <map_tool_ui/ISceneTool2dUiService.h>
#include <earth_effects_ui/IEffectsUiService.h>
#include <earth_layer_ui/I3DRealSceneUiService.h>
#include <scene_ui/ISceneUiService.h>
#include <earth_layer_ui/IEarthLayerWgtService.h>
#include <map_layer_ui/IMapLayerWgtService.h>
#include <service_ui/IServiceUiInterface.h>
#include <bdpTool/kits/GxKitsManager.h>
#include <bdpTool/tool/GxToolManager.h>

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>
#include <gdpScene/manipulator/GxMapManipulator.h>
#endif

#include <FxLoadIndicator/CLoadIndicator.h>

#include "QtitanDocking.h"

#define  WITHOSG		1
#if WITHOSG
#include "GxUiFrameworkServiceInterface.h"
#include <simu_control_ui/ISimuControlUiService.h>
#include <earth_manipulator_ui/ISceneToolUiService.h>
#include <map_tool_ui/ISceneTool2dUiService.h>
#include <earth_effects_ui/IEffectsUiService.h>
#include <earth_layer_ui/I3DRealSceneUiService.h>
#include <scene_ui/ISceneUiService.h>
#include <earth_layer_ui/IEarthLayerWgtService.h>
#include <map_layer_ui/IMapLayerWgtService.h>
#include <service_ui/IServiceUiInterface.h>
#include <bdpTool/kits/GxKitsManager.h>
#include <bdpTool/tool/GxToolManager.h>

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>
#include <gdpScene/manipulator/GxMapManipulator.h>
#endif
#include "scene_ui/ISceneInterface.h"
#include "bdpScene/scene/GxSceneBase.h"
#include "osg/Camera"
#include "osg/Geode"
#include "osg/Geometry"
#include "osgDB/ReadFile"
#include "osg/Texture2D"
#include "bdpScene/scene/GxSceneBase.h"
#include <osgEarth/Registry>
#include "bdpDB/GxRegister.h"
using namespace HGxPFWCore;

CGxProcessor::CGxProcessor(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<std::string>("std::string");

	CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (uiFrameworkService)
	{
		uiFrameworkService->resize(QSize(1920, 1080));
		uiFrameworkService->move(QPoint(0, 0));

		//uiFrameworkService->framelessWindowHint();

		ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());

		ISimuControlUiService* pSimuControlService = HGXPFCoreInstance.getService<ISimuControlUiService>(HGxFWLauncherInstance.getPluginContext());

		ISceneToolUiService* pEarthSceneToolService = HGXPFCoreInstance.getService<ISceneToolUiService>(HGxFWLauncherInstance.getPluginContext());

		ISceneTool2dUiService* pMapSceneToolService = HGXPFCoreInstance.getService<ISceneTool2dUiService>(HGxFWLauncherInstance.getPluginContext());

		IEffectsUiService* pEffectsService = HGXPFCoreInstance.getService<IEffectsUiService>(HGxFWLauncherInstance.getPluginContext());

		I3DRealSceneUiService* p3DRealService = HGXPFCoreInstance.getService<I3DRealSceneUiService>(HGxFWLauncherInstance.getPluginContext());

		IEarthLayerWgtService* pEarthLayerWgtService = HGXPFCoreInstance.getService<IEarthLayerWgtService>(HGxFWLauncherInstance.getPluginContext());

		IMapLayerWgtService* pMapLayerWgtService = HGXPFCoreInstance.getService<IMapLayerWgtService>(HGxFWLauncherInstance.getPluginContext());

		IServiceUiInterface* pServiceUI = HGXPFCoreInstance.getService<IServiceUiInterface>(HGxFWLauncherInstance.getPluginContext());

		QWidget* pWidget = NULL;
		QWidget* pDockWidget = NULL;

		CLoadIndicator::addUseCount(uiFrameworkService->mainWindow());

		//编译出错时   可以尝试将动态库全部重新生成一下
		// 二三维场景服务
		if (pSceneUiService)
		{
			pDockWidget = uiFrameworkService->addSceneWidget("2D", QStringLiteral("二维"), pSceneUiService->createScene("2D", GX_MAP_SCENE_TYPE));
			pSceneUiService->saveSceneDockWidget("2D", pDockWidget);
			pWidget = uiFrameworkService->addSceneWidget("3D", QStringLiteral("三维"), pSceneUiService->createScene("3D", GX_EARTH_SCENE_TYPE));
			pSceneUiService->saveSceneDockWidget("3D", pWidget);
		}
		//	// 二维操作服务
		//	if (pMapSceneToolService)
		//	{
		//		m_pSceneToolStackedWgt->addWidget(pMapSceneToolService->getWgt());
		//	}

		//	// 三维操作服务
		//	if (pEarthSceneToolService)
		//	{
		//		m_pSceneToolStackedWgt->addWidget(pEarthSceneToolService->getWgt());
		//		m_pSceneToolStackedWgt->setCurrentWidget(pEarthSceneToolService->getWgt());
		//		pDockWidget = uiFrameworkService->addDockWidget("SceneTool", QStringLiteral("场景工具"),m_pSceneToolStackedWgt, Qtitan::DockPanelArea::RightDockPanelArea);
		//		
		//	}

		//	// 仿真控制服务

		//	// 三维实景服务
		//	if (p3DRealService)
		//	{
		//		uiFrameworkService->addDockWidget("3DReal", QStringLiteral("三维实景"), p3DRealService->getWgt(), Qtitan::DockPanelArea::InsideDockPanelArea, pDockWidget);
		//	}

		//	// 特效窗口服务
		//	if (pEffectsService)
		//	{
		//		uiFrameworkService->addDockWidget("Effects", QStringLiteral("环境特效"), pEffectsService->getWgt(), Qtitan::DockPanelArea::InsideDockPanelArea, pDockWidget);
		//	}

		//	// 二维图层服务
		//	if (pMapLayerWgtService)
		//	{
		//		m_pLayerTreeStackedWgt->addWidget(pMapLayerWgtService->getWgt());
		//	}

		//	// 三维图层服务
		//	if (pEarthLayerWgtService)
		//	{
		//		m_pLayerTreeStackedWgt->addWidget(pEarthLayerWgtService->getWgt());
		//		m_pLayerTreeStackedWgt->setCurrentWidget(pEarthLayerWgtService->getWgt());
		//		m_pLayerTreeStackedWgt->hide();
		//		pDockWidget = uiFrameworkService->addDockWidget("SceneLayer", QStringLiteral("场景图层"), m_pLayerTreeStackedWgt, Qtitan::DockPanelArea::LeftDockPanelArea);
		//	}

	 //		// 服务
	 //		if (pServiceUI)
	 //		{
	 //			uiFrameworkService->addDockWidget("Service", QStringLiteral("图层服务"), pServiceUI->getWgt(), Qtitan::DockPanelArea::BottomDockPanelArea, pDockWidget);
	 //		}

	 //		// 二维图层属性面板
	 //		if (pMapLayerWgtService)
	 //		{
	 //			uiFrameworkService->addDockWidget("MapLayerProperty", QStringLiteral("属性"), pMapLayerWgtService->getLayerMgrWgt(), Qtitan::DockPanelArea::RightDockPanelArea);
	 //			uiFrameworkService->closeDockWidget("MapLayerProperty");
	 //		}

	 //		// 三维图层属性面板
	 //		if (pEarthLayerWgtService)
	 //		{
	 //			uiFrameworkService->addDockWidget("EarthLayerProperty", QStringLiteral("属性"), pEarthLayerWgtService->getLayerMgrWgt(), Qtitan::DockPanelArea::RightDockPanelArea);
	 //			uiFrameworkService->closeDockWidget("EarthLayerProperty");
	 //		}
		//}
	}
	//connect(this, SIGNAL(dockActiveSignal(std::string)), this, SLOT(activeDockSlot(std::string)));

	//HGxEventHelperInstance.subscribeTopic(this, "com_henggetec_uiFramework", HGxFWLauncherInstance.getPluginContext());
	
}
void CGxProcessor::handleEvent(const ctkEvent &event)
{
	if (event.getTopic() == "com_henggetec_uiFramework")
	{
		qDebug() << QDateTime::currentDateTime() << "to handled topic " << event.getTopic();

		QString id = event.getProperty("id").toString();
		QObject* obj = event.getProperty("widget").value<QObject*>();
		bool actived = event.getProperty("active").toBool();
		QString type = event.getProperty("type").toString();
	}
}
