#include "GxhenggetecMapManipulatorPlugin_p.h"
#include <QtPlugin>
#include <QIcon>

#include "CSceneTool2dUiPlugin.h"

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>

CGxhenggetecMapManipulatorPlugin* CGxhenggetecMapManipulatorPlugin::instance = 0;

CGxhenggetecMapManipulatorPlugin::CGxhenggetecMapManipulatorPlugin()
  : context(0)
  , m_pSceneToolSerive(NULL)
  , m_pMapToolPluginObj(NULL)
{
}

CGxhenggetecMapManipulatorPlugin::~CGxhenggetecMapManipulatorPlugin()
{
  
}

void CGxhenggetecMapManipulatorPlugin::start(ctkPluginContext* context)
{
	instance = this;
	this->context = context;

	REGISTER_GEOGRAPHICAL();
	REGISTER_GEOGRAPHICAL_TOOL();

	m_pSceneToolSerive = new CSceneTool2dUiPlugin(context);
}

void CGxhenggetecMapManipulatorPlugin::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)

	UNREGISTER_GEOGRAPHICAL();
	UNREGISTER_GEOGRAPHICAL_TOOL();

	m_pSceneToolSerive = NULL;
	m_pMapToolPluginObj = NULL;
}

CGxhenggetecMapManipulatorPlugin* CGxhenggetecMapManipulatorPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* CGxhenggetecMapManipulatorPlugin::getPluginContext() const
{
  return context;
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(com_henggetec_mapManipulatorPlugin, CGxhenggetecMapManipulatorPlugin)
#endif


