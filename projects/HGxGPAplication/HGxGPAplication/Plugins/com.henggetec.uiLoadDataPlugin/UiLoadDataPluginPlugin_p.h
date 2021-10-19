#ifndef UILOADDATAPLUGINPLUGIN_P_H
#define UILOADDATAPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>


class CSceneControlWidget;

class UiLoadDataPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiLoadDataPlugin")
  #endif

public:

  UiLoadDataPluginPlugin();
  ~UiLoadDataPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiLoadDataPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static UiLoadDataPluginPlugin* instance;
  ctkPluginContext* context;
  CSceneControlWidget *m_pSceneControlWgt;

}; // UiLoadDataPluginPlugin

#endif // UILOADDATAPLUGINPLUGIN_P_H
