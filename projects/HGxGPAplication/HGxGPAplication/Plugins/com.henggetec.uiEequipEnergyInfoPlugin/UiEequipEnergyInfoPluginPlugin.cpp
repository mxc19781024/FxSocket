#include "UiEequipEnergyInfoPluginPlugin_p.h"
#include <QtPlugin>
#include "GxPluginFwLauncherInterface.h"
#include "GxUiFrameworkServiceInterface.h"
#include "GxPluginFrameworkCore.h"

UiEequipEnergyInfoPluginPlugin* UiEequipEnergyInfoPluginPlugin::instance = 0;

UiEequipEnergyInfoPluginPlugin::UiEequipEnergyInfoPluginPlugin()
  : context(0)
	, m_pEquipEnergyInfoWgt(nullptr)
{
}

UiEequipEnergyInfoPluginPlugin::~UiEequipEnergyInfoPluginPlugin()
{
  
}

void UiEequipEnergyInfoPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;


  // 在主程序创建菜单
  if (m_pEquipEnergyInfoWgt == nullptr)
  {
	  CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	  if (uiFrameworkService)
	  {
		  m_pEquipEnergyInfoWgt = new CEquipEnergyInfoWidget(context, uiFrameworkService->mainWindow());
		  m_pEquipEnergyInfoWgt->hide();

		  m_pEquipEnergyInfoWgt->move(20, 920);
	  }
  }
}

void UiEequipEnergyInfoPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiEequipEnergyInfoPluginPlugin* UiEequipEnergyInfoPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiEequipEnergyInfoPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiEequipEnergyInfoPlugin, UiEequipEnergyInfoPluginPlugin)
#endif

