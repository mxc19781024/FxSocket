#ifndef DRIVEMGRINTERFACEPLUGINPLUGIN_P_H
#define DRIVEMGRINTERFACEPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include "DriveMgr.h"

class DriveMgrInterfacePluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_driveMgrInterfacePlugin")
  #endif

public:

  DriveMgrInterfacePluginPlugin();
  ~DriveMgrInterfacePluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static DriveMgrInterfacePluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static DriveMgrInterfacePluginPlugin* instance;
  ctkPluginContext* context;
  FxDrive::CDriveMgr *m_pServiceObj;

}; // DriveMgrInterfacePluginPlugin

#endif // DRIVEMGRINTERFACEPLUGINPLUGIN_P_H
