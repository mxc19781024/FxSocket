#include "DataMgrPluginPlugin_p.h"
#include <QtPlugin>
#include "CBizDataInterface.h"

DataMgrPluginPlugin* DataMgrPluginPlugin::instance = 0;

DataMgrPluginPlugin::DataMgrPluginPlugin()
  : context(0)
{
}

DataMgrPluginPlugin::~DataMgrPluginPlugin()
{
  
}

void DataMgrPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  CBizDataInterface* pDataPlugin = new CBizDataInterface(context);
  context->registerService<IFxBizDataMgrService>(pDataPlugin);

}

void DataMgrPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

DataMgrPluginPlugin* DataMgrPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* DataMgrPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_dataMgrPlugin, DataMgrPluginPlugin)
#endif

