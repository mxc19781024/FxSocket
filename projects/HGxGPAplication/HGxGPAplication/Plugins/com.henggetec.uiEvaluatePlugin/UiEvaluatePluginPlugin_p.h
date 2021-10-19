#ifndef UIEVALUATEPLUGINPLUGIN_P_H
#define UIEVALUATEPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "CEvaluateInfoWidget.h"

class UiEvaluatePluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiEvaluatePlugin")
  #endif

public:

  UiEvaluatePluginPlugin();
  ~UiEvaluatePluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiEvaluatePluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static UiEvaluatePluginPlugin* instance;
  ctkPluginContext* context;
  CEvaluateInfoWidget *m_pEvaluateInfoWgt;

}; // UiEvaluatePluginPlugin

#endif // UIEVALUATEPLUGINPLUGIN_P_H
