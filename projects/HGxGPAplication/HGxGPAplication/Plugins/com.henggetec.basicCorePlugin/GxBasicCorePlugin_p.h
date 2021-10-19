#ifndef GXBASICCOREPLUGINPLUGIN_P_H
#define GXBASICCOREPLUGINPLUGIN_P_H

#include <QObject>
#include <QTimer>
#include <ctkPluginActivator.h>

#include "earth_layer_ui/I3DRealSceneUiService.h"
#include "earth_effects_ui/IEffectsUiService.h"

#include "osg/ref_ptr"

namespace HGxObject
{
	class CGxObjectManager;
}

class CGxBasicCorePlugin :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
  Q_PLUGIN_METADATA(IID "com_henggetec_basicCorePlugin")
#endif

public:

  CGxBasicCorePlugin();
  ~CGxBasicCorePlugin();

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

  static CGxBasicCorePlugin* getInstance();

  ctkPluginContext* getPluginContext() const;

private:

  static CGxBasicCorePlugin* instance;
  ctkPluginContext* context;

  QObject *m_pSceneObj;    // 场景对象

  I3DRealSceneUiService* m_p3DRealScenePlugin;		  // 三维实景服务

  QObject*	m_pEffectPlugin;	// 三维环境插件服务实现类指针

}; // CGxBasicCorePlugin

#endif // GXBASICCOREPLUGINPLUGIN_P_H
