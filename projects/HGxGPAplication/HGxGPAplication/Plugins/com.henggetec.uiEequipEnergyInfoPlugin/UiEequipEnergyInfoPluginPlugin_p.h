#ifndef UIEEQUIPENERGYINFOPLUGINPLUGIN_P_H
#define UIEEQUIPENERGYINFOPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "CEquipEnergyInfoWidget.h"

class UiEequipEnergyInfoPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiEequipEnergyInfoPlugin")
  #endif

public:

  UiEequipEnergyInfoPluginPlugin();
  ~UiEequipEnergyInfoPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiEequipEnergyInfoPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static UiEequipEnergyInfoPluginPlugin* instance;
  ctkPluginContext* context;
  CEquipEnergyInfoWidget*	m_pEquipEnergyInfoWgt;

}; // UiEequipEnergyInfoPluginPlugin

#endif // UIEEQUIPENERGYINFOPLUGINPLUGIN_P_H
