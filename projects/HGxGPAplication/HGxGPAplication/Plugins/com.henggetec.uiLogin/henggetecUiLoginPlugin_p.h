#ifndef HENGGETECUILOGINPLUGIN_P_H
#define HENGGETECUILOGINPLUGIN_P_H

#include <ctkPluginActivator.h>

class henggetecUiLoginPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)
  #if QT_VERSION >= 0x50000
  Q_PLUGIN_METADATA(IID "com_henggetec_uiLogin")
  #endif

public:

  henggetecUiLoginPlugin();
  ~henggetecUiLoginPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static henggetecUiLoginPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static henggetecUiLoginPlugin* instance;
  ctkPluginContext* context;


}; // henggetecUiLoginPlugin

#endif // HENGGETECUILOGINPLUGIN_P_H
