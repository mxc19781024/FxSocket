#ifndef GXHENGGETECMAPMANIPULATORPLUGIN_P_H
#define GXHENGGETECMAPMANIPULATORPLUGIN_P_H

#include <ctkPluginActivator.h>

#include "map_tool_ui/ISceneTool2dUiService.h"

class CGxhenggetecMapManipulatorPlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  Q_PLUGIN_METADATA(IID "com_henggetec_mapManipulatorPlugin")
#endif

public:

  CGxhenggetecMapManipulatorPlugin();
  ~CGxhenggetecMapManipulatorPlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static CGxhenggetecMapManipulatorPlugin* getInstance();

  ctkPluginContext* getPluginContext() const;


private:

  static CGxhenggetecMapManipulatorPlugin* instance;
  ctkPluginContext* context;

  // 二维场景工具服务指针
  ISceneTool2dUiService*		m_pSceneToolSerive;

  // 二维场景工具界面容器指针
  QObject*					m_pMapToolPluginObj;

}; // CGxhenggetecMapManipulatorPlugin

#endif // GXHENGGETECMAPMANIPULATORPLUGIN_P_H
