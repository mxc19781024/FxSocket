#ifndef UIGANATECHARTPLUGINPLUGIN_P_H
#define UIGANATECHARTPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "FxGanttChartWidget.h"

class UiGanateChartPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiGanateChartPlugin")
  #endif

public:

  UiGanateChartPluginPlugin();
  ~UiGanateChartPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiGanateChartPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static UiGanateChartPluginPlugin* instance;
  ctkPluginContext* context;
  CFxGanttChart *m_pGanateChartWgt;

}; // UiGanateChartPluginPlugin

#endif // UIGANATECHARTPLUGINPLUGIN_P_H
