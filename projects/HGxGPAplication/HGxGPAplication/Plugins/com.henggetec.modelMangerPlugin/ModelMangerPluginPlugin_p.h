#ifndef MODELMANGERPLUGINPLUGIN_P_H
#define MODELMANGERPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>
#include <ModelMgr.h>

class ModelMangerPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_modelMangerPlugin")
  #endif

public:

  ModelMangerPluginPlugin();
  ~ModelMangerPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static ModelMangerPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static ModelMangerPluginPlugin* instance;
  ctkPluginContext* context;
  FxDrive::CModelMgr *m_pServiceObj;

}; // ModelMangerPluginPlugin

#endif // MODELMANGERPLUGINPLUGIN_P_H
