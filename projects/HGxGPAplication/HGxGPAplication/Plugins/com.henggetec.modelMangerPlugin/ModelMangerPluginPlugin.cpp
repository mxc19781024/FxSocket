#include "ModelMangerPluginPlugin_p.h"
#include <QtPlugin>

ModelMangerPluginPlugin* ModelMangerPluginPlugin::instance = 0;

ModelMangerPluginPlugin::ModelMangerPluginPlugin()
  : context(0)
{
}

ModelMangerPluginPlugin::~ModelMangerPluginPlugin()
{
  
}

void ModelMangerPluginPlugin::start(ctkPluginContext* context)
{
  instance = this;
  this->context = context;

  m_pServiceObj = new FxDrive::CModelMgr;
  context->registerService<IModelMgrInterface>(m_pServiceObj);
}

void ModelMangerPluginPlugin::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

ModelMangerPluginPlugin* ModelMangerPluginPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* ModelMangerPluginPlugin::getPluginContext() const
{
  return context;
}

#if QT_VERSION < 0x50000 
 Q_EXPORT_PLUGIN2(com_henggetec_modelMangerPlugin, ModelMangerPluginPlugin)
#endif

