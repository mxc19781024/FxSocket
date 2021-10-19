#include "UiGanateChartPluginPlugin_p.h"
#include <QtPlugin>
#include "GxPluginFrameworkCore.h"
#include "GxPluginFwLauncherInterface.h"
#include "GxUiFrameworkServiceInterface.h"

UiGanateChartPluginPlugin* UiGanateChartPluginPlugin::instance = 0;

UiGanateChartPluginPlugin::UiGanateChartPluginPlugin()
  : context(0)
	, m_pGanateChartWgt(nullptr)
{
}

UiGanateChartPluginPlugin::~UiGanateChartPluginPlugin()
{
  
}

void UiGanateChartPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;
  // 在主程序创建菜单
  if (m_pGanateChartWgt == nullptr)
  {
	  CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	  if (uiFrameworkService)
	  {
		  m_pGanateChartWgt = new CFxGanttChart(context, uiFrameworkService->mainWindow());
		  m_pGanateChartWgt->hide();

		  m_pGanateChartWgt->setFixedSize(750, 150);
		  m_pGanateChartWgt->move((1920 - m_pGanateChartWgt->width()) - 35, 911);
	  }
  }
}

void UiGanateChartPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiGanateChartPluginPlugin* UiGanateChartPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiGanateChartPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiGanateChartPlugin, UiGanateChartPluginPlugin)
#endif

