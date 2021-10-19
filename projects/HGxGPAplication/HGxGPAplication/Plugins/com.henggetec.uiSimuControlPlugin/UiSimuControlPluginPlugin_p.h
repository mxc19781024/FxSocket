#ifndef UISIMUCONTROLPLUGINPLUGIN_P_H
#define UISIMUCONTROLPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "CSimularControlUiPlugin.h"

class UiSimuControlPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiSimuControlPlugin")
  #endif

public:

  UiSimuControlPluginPlugin();
  ~UiSimuControlPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiSimuControlPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;
  // 服务插件UI对象
  CBizSimuCtrlUiPlugin*			m_pSimuControlObj;

private:

  static UiSimuControlPluginPlugin* instance;
  ctkPluginContext* context;


}; // UiSimuControlPluginPlugin

#endif // UISIMUCONTROLPLUGINPLUGIN_P_H
