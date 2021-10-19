#include "GxEarthManipulatorPlugin_p.h"
#include <QtPlugin>
#include <QIcon>

#include "CSceneToolUiPlugin.h"

#include <gdpScene/CommonGeo.h>
#include <gdpTool/GxToolCommon.h>

CGxEarthManipulatorPlugin* CGxEarthManipulatorPlugin::instance = 0;

CGxEarthManipulatorPlugin::CGxEarthManipulatorPlugin()
  : context(0)
  , m_pSceneToolPlugin(NULL)
{
}

CGxEarthManipulatorPlugin::~CGxEarthManipulatorPlugin()
{
  
}

void CGxEarthManipulatorPlugin::start(ctkPluginContext* context)
{
	instance = this;
	this->context = context;

	REGISTER_GEOGRAPHICAL();
	REGISTER_GEOGRAPHICAL_TOOL();

	m_pSceneToolPlugin = new CSceneToolUiPlugin(context); 
}

void CGxEarthManipulatorPlugin::stop(ctkPluginContext* context)
{
	Q_UNUSED(context)

	UNREGISTER_GEOGRAPHICAL();
	UNREGISTER_GEOGRAPHICAL_TOOL();
}

CGxEarthManipulatorPlugin* CGxEarthManipulatorPlugin::getInstance()
{
  return instance;
}

ctkPluginContext* CGxEarthManipulatorPlugin::getPluginContext() const
{
  return context;
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
Q_EXPORT_PLUGIN2(com_henggetec_earthManipulator, CGxEarthManipulatorPlugin)
#endif

