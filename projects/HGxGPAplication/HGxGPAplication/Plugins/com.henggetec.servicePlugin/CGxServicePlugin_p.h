#ifndef CGXSERVICEPLUGIN_P_H
#define CGXSERVICEPLUGIN_P_H

#include <ctkPluginActivator.h>

class CServiceObject;

class CGxServicePlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_servicePlugin")
  #endif

public:

  CGxServicePlugin();
  ~CGxServicePlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static CGxServicePlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static CGxServicePlugin* instance;
  ctkPluginContext* context;

  // 服务接口实现类对象
  CServiceObject					*m_pServiceObj;

}; // CGxServicePlugin

#endif // CGXSERVICEPLUGIN_P_H
