#ifndef NETWORKRECVPLUGIN_P_H
#define NETWORKRECVPLUGIN_P_H

#include <ctkPluginActivator.h>

class NetworkRecvPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_networkRecv")
  #endif

public:

  NetworkRecvPlugin();
  ~NetworkRecvPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static NetworkRecvPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static NetworkRecvPlugin* instance;
  ctkPluginContext* context;


}; // NetworkRecvPlugin

#endif // NETWORKRECVPLUGIN_P_H
