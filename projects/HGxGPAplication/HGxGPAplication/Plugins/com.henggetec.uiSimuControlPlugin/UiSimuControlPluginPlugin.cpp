#include "UiSimuControlPluginPlugin_p.h"
#include <QtPlugin>

UiSimuControlPluginPlugin* UiSimuControlPluginPlugin::instance = 0;

UiSimuControlPluginPlugin::UiSimuControlPluginPlugin()
  : context(0)
{
}

UiSimuControlPluginPlugin::~UiSimuControlPluginPlugin()
{
  
}

void UiSimuControlPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  m_pSimuControlObj = new CBizSimuCtrlUiPlugin(context);
  ctkDictionary dic;
  context->registerService<ISimuControlUiService>(m_pSimuControlObj, dic);
}

void UiSimuControlPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

UiSimuControlPluginPlugin* UiSimuControlPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* UiSimuControlPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_uiSimuControlPlugin, UiSimuControlPluginPlugin)
#endif

