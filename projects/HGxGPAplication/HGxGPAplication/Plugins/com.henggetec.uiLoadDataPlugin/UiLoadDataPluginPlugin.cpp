#include "UiLoadDataPluginPlugin_p.h"
#include <QtPlugin>
#include "GxPluginFwLauncherInterface.h"
#include "GxUiFrameworkServiceInterface.h"
#include "GxPluginFrameworkCore.h"
#include "SceneControlWidget.h"


UiLoadDataPluginPlugin* UiLoadDataPluginPlugin::instance = 0;

UiLoadDataPluginPlugin::UiLoadDataPluginPlugin()
	: context(0)
	, m_pSceneControlWgt(nullptr)
{
}

UiLoadDataPluginPlugin::~UiLoadDataPluginPlugin()
{
  
}

void UiLoadDataPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  // 在主程序创建菜单
  if (m_pSceneControlWgt == nullptr)
  {
	  CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	  if (uiFrameworkService)
	  {
		  m_pSceneControlWgt = new CSceneControlWidget(context, uiFrameworkService->mainWindow());
		  m_pSceneControlWgt->hide();
		  m_pSceneControlWgt->move(750, 925);
	  }
  }
}

void UiLoadDataPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiLoadDataPluginPlugin* UiLoadDataPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiLoadDataPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiLoadDataPlugin, UiLoadDataPluginPlugin)
#endif

