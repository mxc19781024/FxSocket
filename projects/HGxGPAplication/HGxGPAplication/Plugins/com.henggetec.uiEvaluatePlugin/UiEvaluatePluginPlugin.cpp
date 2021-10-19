#include "UiEvaluatePluginPlugin_p.h"
#include <QtPlugin>
#include "GxPluginFwLauncherInterface.h"
#include "GxUiFrameworkServiceInterface.h"
#include "GxPluginFrameworkCore.h"

UiEvaluatePluginPlugin* UiEvaluatePluginPlugin::instance = 0;

UiEvaluatePluginPlugin::UiEvaluatePluginPlugin()
  : context(0)
	, m_pEvaluateInfoWgt(nullptr)
{
}

UiEvaluatePluginPlugin::~UiEvaluatePluginPlugin()
{
  
}

void UiEvaluatePluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;
  // 在主程序创建菜单
  if (m_pEvaluateInfoWgt == nullptr)
  {
	  CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	  if (uiFrameworkService)
	  {
		  m_pEvaluateInfoWgt = new CEvaluateInfoWidget(context, uiFrameworkService->mainWindow());
		  m_pEvaluateInfoWgt->hide();
		  m_pEvaluateInfoWgt->move((1920 - m_pEvaluateInfoWgt->width()) - 20, 150);;
	  }
  }
}

void UiEvaluatePluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiEvaluatePluginPlugin* UiEvaluatePluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiEvaluatePluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiEvaluatePlugin, UiEvaluatePluginPlugin)
#endif

