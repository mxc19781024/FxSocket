#ifndef GXEARTHMANIPULATORPLUGIN_P_H
#define GXEARTHMANIPULATORPLUGIN_P_H

#include <ctkPluginActivator.h>

#include "earth_manipulator_ui/ISceneToolUiService.h"

class CGxEarthManipulatorPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  Q_PLUGIN_METADATA(IID "com_henggetec_earthManipulator")
#endif

public:

  CGxEarthManipulatorPlugin();
  ~CGxEarthManipulatorPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static CGxEarthManipulatorPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static CGxEarthManipulatorPlugin* instance;
  ctkPluginContext* context;

  // 三维场景工具服务指针
  ISceneToolUiService*			m_pSceneToolPlugin;

}; // CGxEarthManipulatorPlugin

#endif // GXEARTHMANIPULATORPLUGIN_P_H
