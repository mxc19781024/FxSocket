#ifndef GXHENGGETECSIMUCONTROLPLUGIN_P_H
#define GXHENGGETECSIMUCONTROLPLUGIN_P_H

#include <ctkPluginActivator.h>

class CSimuControlUiPlugin;

class CGxhenggetecSimuControlPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  Q_PLUGIN_METADATA(IID "com_henggetec_simuControlPlugin")
#endif

public:

  CGxhenggetecSimuControlPlugin();
  ~CGxhenggetecSimuControlPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static CGxhenggetecSimuControlPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static CGxhenggetecSimuControlPlugin* instance;
  ctkPluginContext* context;

  // 服务插件UI对象
  CSimuControlUiPlugin*			m_pSimuControlObj;


}; // CGxhenggetecSimuControlPlugin

#endif // GXHENGGETECSIMUCONTROLPLUGIN_P_H
