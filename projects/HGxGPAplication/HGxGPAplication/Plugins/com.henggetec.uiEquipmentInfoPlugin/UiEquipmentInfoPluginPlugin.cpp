#include "UiEquipmentInfoPluginPlugin_p.h"
#include <QtPlugin>
#include "GxUiFrameworkServiceInterface.h"
#include <GxPluginFrameworkCore.h>
#include <GxPluginFwLauncherInterface.h>
#include <GxEventHelperInterface.h>

UiEquipmentInfoPluginPlugin* UiEquipmentInfoPluginPlugin::instance = 0;

UiEquipmentInfoPluginPlugin::UiEquipmentInfoPluginPlugin()
  : context(0)
	, m_pEquipmentInfoWgt(nullptr)
{
}

UiEquipmentInfoPluginPlugin::~UiEquipmentInfoPluginPlugin()
{
  
}

void UiEquipmentInfoPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  // 在主程序创建菜单
  if (m_pEquipmentInfoWgt == nullptr)
  {
	  CGxUiFrameworkServiceInterface* uiFrameworkService = HGXPFCoreInstance.getService<CGxUiFrameworkServiceInterface>(HGxFWLauncherInstance.getPluginContext());

	  if (uiFrameworkService)
	  {
		  m_pEquipmentInfoWgt = new CEquipmentInfoWidget(context, uiFrameworkService->mainWindow());
		  m_pEquipmentInfoWgt->hide();
		  m_pEquipmentInfoWgt->move(20, 150);
		  // 	  ctkDictionary props;
		  // 	  props.insert(ctkPluginConstants::SERVICE_RANKING, 0);
		  // 	  context->registerService<CGxUiFrameworkServiceInterface>(m_pServiceImp, props);
	  }
  }

}

void UiEquipmentInfoPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiEquipmentInfoPluginPlugin* UiEquipmentInfoPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiEquipmentInfoPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiEquipmentInfoPlugin, UiEquipmentInfoPluginPlugin)
#endif

