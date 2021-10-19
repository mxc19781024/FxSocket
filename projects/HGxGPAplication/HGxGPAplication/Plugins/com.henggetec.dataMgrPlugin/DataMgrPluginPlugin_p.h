#ifndef DATAMGRPLUGINPLUGIN_P_H
#define DATAMGRPLUGINPLUGIN_P_H

#include <ctkPluginActivator.h>

class DataMgrPluginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_dataMgrPlugin")
  #endif

public:

  DataMgrPluginPlugin();
  ~DataMgrPluginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static DataMgrPluginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static DataMgrPluginPlugin* instance;
  ctkPluginContext* context;


}; // DataMgrPluginPlugin

#endif // DATAMGRPLUGINPLUGIN_P_H
