#include "NetworkRecvPlugin_p.h"
#include <QtPlugin>
#include "SimControlCenter.h"


NetworkRecvPlugin* NetworkRecvPlugin::instance = 0;

NetworkRecvPlugin::NetworkRecvPlugin()
  : context(0)
{
}

NetworkRecvPlugin::~NetworkRecvPlugin()
{
  
}

void NetworkRecvPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;
  CSimControlCenter *pSimControlCenter = new CSimControlCenter();
}

void NetworkRecvPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

NetworkRecvPlugin* NetworkRecvPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* NetworkRecvPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_networkRecv, NetworkRecvPlugin)
#endif

