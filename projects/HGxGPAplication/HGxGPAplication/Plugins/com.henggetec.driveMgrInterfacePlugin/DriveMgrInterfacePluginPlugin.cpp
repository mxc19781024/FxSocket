#include "DriveMgrInterfacePluginPlugin_p.h"
#include <QtPlugin>

DriveMgrInterfacePluginPlugin* DriveMgrInterfacePluginPlugin::instance = 0;

DriveMgrInterfacePluginPlugin::DriveMgrInterfacePluginPlugin()
  : context(0)
{
}

DriveMgrInterfacePluginPlugin::~DriveMgrInterfacePluginPlugin()
{
  
}

void DriveMgrInterfacePluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  m_pServiceObj = new FxDrive::CDriveMgr;
  context->registerService<IDriveMgrInterface>(m_pServiceObj);
}

void DriveMgrInterfacePluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

DriveMgrInterfacePluginPlugin* DriveMgrInterfacePluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* DriveMgrInterfacePluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_driveMgrInterfacePlugin, DriveMgrInterfacePluginPlugin)
#endif

