#ifndef HENGGETECUIEQUIPMENTINFOPLUGINPLUGIN_P_H
#define HENGGETECUIEQUIPMENTINFOPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "CEquipmentInfoWidget.h"

class UiEquipmentInfoPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiEquipmentInfoPlugin")
  #endif

public:

  UiEquipmentInfoPluginPlugin();
  ~UiEquipmentInfoPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static UiEquipmentInfoPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static UiEquipmentInfoPluginPlugin* instance;
  ctkPluginContext* context;
  CEquipmentInfoWidget*	m_pEquipmentInfoWgt;

}; // UiEquipmentInfoPluginPlugin

#endif // HENGGETECUIEQUIPMENTINFOPLUGINPLUGIN_P_H
