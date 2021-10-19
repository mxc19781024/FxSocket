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
#include "bdpDB/GxLogFunc.h"
#include "bdpDB/GxTinyXml.h"
#include "gdpScene/layer/GxLayerManager.h"
#include "GxServiceQuerier.h"
#include "bdpTool/environment/GxEnviManager.h"
#include "gdpTool/environment/GxEnvirementCom.h"
#include "gdpTool/environment/GxSkyOption.h"
#include "service/event/ctkEventAdmin.h"
#include <gdpScene/layer/GxLayerConfigFactory.h>
using namespace HGxPFWCore;

CGxProcessor::CGxProcessor(QObject *parent)
    : QObject(parent)
{

}

void CGxProcessor::load()
{

	qRegisterMetaType<std::string>("std::string");

	CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	if (uiFrameworkService)
	{
		uiFrameworkService->resize(QSize(1920, 1080));
		uiFrameworkService->move(QPoint(0, 0));
		uiFrameworkService->hide();
		//uiFrameworkService->framelessWindowHint();

		ISceneUiService* pSceneUiService = HGXPFCoreInstance.getService<ISceneUiService>(HGxFWLauncherInstance.getPluginContext());
		ISimuControlUiService* pSimuControlService = HGXPFCoreInstance.getService<ISimuControlUiService>(HGxFWLauncherInstance.getPluginContext());
		ISceneToolUiService* pEarthSceneToolService = HGXPFCoreInstance.getService<ISceneToolUiService>(HGxFWLauncherInstance.getPluginContext());
		IEarthLayerWgtService* pEarthLayerWgtService = HGXPFCoreInstance.getService<IEarthLayerWgtService>(HGxFWLauncherInstance.getPluginContext());

		IServiceUiInterface* pServiceUI = HGXPFCoreInstance.getService<IServiceUiInterface>(HGxFWLauncherInstance.getPluginContext());

		QWidget* pWidget = NULL;

		//CLoadIndicator::addUseCount(uiFrameworkService->mainWindow());
		// 二三维场景服务
		if (pSceneUiService)
		{
			//w00085
			HGxPFWCore::CGxServiceQuerier<ctkEventAdmin> eventService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());
			if (eventService)
			{
				ctkDictionary sceneCreatedDic;
				sceneCreatedDic.insert("iValue", 84);
				sceneCreatedDic.insert("strMessage", "Load 3D Scene!");
				ctkEvent event = ctkEvent("event/Progerss", sceneCreatedDic);
				eventService->sendEvent(event);
			}
			pWidget = uiFrameworkService->addSceneWidget("3D", QStringLiteral("三维"), pSceneUiService->createScene("3D", GX_EARTH_SCENE_TYPE));

			if (eventService)
			{
				ctkDictionary sceneCreatedDic;
				sceneCreatedDic.insert("iValue", 90);
				sceneCreatedDic.insert("strMessage", "Finish 3D Scene!");
				ctkEvent event = ctkEvent("event/Progerss", sceneCreatedDic);
				eventService->sendEvent(event);
			}
		}

		HGxTool::CGxSkyOption* pSkyOption = dynamic_cast<HGxTool::CGxSkyOption*>(HGxTool::CGxEnviManager::getInstance()->getEnvirmentOption(FXEARTH_ENVIRORMENT_TYPE_SKY));
		if (pSkyOption)
		{
			//pSkyOption->bSunVisible = false;
			pSkyOption->bCloudLayer = false;
			//pSkyOption->bMoonVisible = false;
			pSkyOption->bNebulaVisible = false;
			pSkyOption->bStarVisible = true;
			pSkyOption->bAtmosVisible = true;
			pSkyOption->dDensity = 0.01;
			HGxTool::CGxEnviManager::getInstance()->setEnvirmentOption(FXEARTH_ENVIRORMENT_TYPE_SKY, dynamic_cast<const HGxTool::SEnviOption&>(*pSkyOption));
		}
	}

	HGxEventHelperInstance.subscribeTopic(this, "com_henggetec_uiFramework", HGxFWLauncherInstance.getPluginContext());
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

void CGxProcessor::slotPluginStart()
{
	LOG(E_LOG_NOTICE) << "加载时间管理器";
	//异步发送事件
	ctkDictionary event;
	HGxEventHelperInstance.postEvent("event/LoginStarted", event);

	HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->startAll();
	HGxPFWCore::CGxServiceQuerier<CGxUiFrameworkServiceInterface> uiFrameworkService(HGxPFWCore::CGxPluginFrameworkCore::fwLauncher()->getPluginContext());

	if (!uiFrameworkService/* || !uiFrameworkService.valid()*/)
	{
		qDebug() << "ui framework not valid";
		exit(-1);
	}
	load();
}
