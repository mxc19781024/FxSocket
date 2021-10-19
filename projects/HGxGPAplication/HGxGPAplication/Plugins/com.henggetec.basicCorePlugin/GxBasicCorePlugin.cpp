#include "GxBasicCorePlugin_p.h"
#include <QtPlugin>
#include <QIcon>

#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>

#include "GxUiFrameworkServiceInterface.h"

#include "scene/CSceneUiPlugin.h"

#include "earthLayer/CEarthLayerWidget.h"
#include "earthLayer/C3DRealSceneUiPlugin.h"

#include "mapLayer/CMapLayerWidget.h"

#include "earthEffects/CEffectsUiPlugin.h"

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>

CGxBasicCorePlugin* CGxBasicCorePlugin::instance = 0;

CGxBasicCorePlugin::CGxBasicCorePlugin()
  : context(0)
{

}

CGxBasicCorePlugin::~CGxBasicCorePlugin()
{
  
}

void CGxBasicCorePlugin::start(ctkPluginContext* context)
{
	instance = this;
	this->context = context;

	REGISTER_GEOGRAPHICAL();
	REGISTER_GEOGRAPHICAL_TOOL();

	// 场景
	m_pSceneObj = new CSceneUiPlugin(context);
 
	context->registerService<ISceneUiService>(m_pSceneObj);
 
	// 创建三维图层界面，并添加到主界面中
	CEarthLayerWidget *pEarthLayerWgt = new CEarthLayerWidget(context);
    
	// 初始化界面
	pEarthLayerWgt->initWhenPluginStart();

	// 创建二维图层界面，并添加到主界面中
	CMapLayerWidget *pMapLayerWgt = new CMapLayerWidget(context);

	// 初始化界面
	pMapLayerWgt->initWhenPluginStart();

	// 创建三维实景工具界面
	m_p3DRealScenePlugin = new C3DRealSceneUiPlugin(context);

	m_pEffectPlugin = new CEffectsUiPlugin(context);
}

void CGxBasicCorePlugin::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)

	UNREGISTER_GEOGRAPHICAL();
	UNREGISTER_GEOGRAPHICAL_TOOL();

   // 删除场景
   delete m_pSceneObj;
 
   // 删除三维图层
   m_p3DRealScenePlugin->getWgt()->deleteLater();

   if (m_p3DRealScenePlugin)
   {
	   delete m_p3DRealScenePlugin;
	   m_p3DRealScenePlugin = NULL;
   }
 
   if (m_pEffectPlugin)
   {
	   delete m_pEffectPlugin;
	   m_pEffectPlugin = NULL;
   }
}

CGxBasicCorePlugin* CGxBasicCorePlugin::getInstance()
{
  return instance;
}

ctkPluginContext* CGxBasicCorePlugin::getPluginContext() const
{
  return context;
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(com_henggetec_basicCorePlugin, CGxBasicCorePlugin)
#endif

