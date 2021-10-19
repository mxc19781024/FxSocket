#include "CSceneToolUiPlugin.h"
#include "CSceneTool.h"
//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>
//#include <FreeXFramework/FxServiceReference.h>




CSceneToolUiPlugin::CSceneToolUiPlugin(ctkPluginContext *context)
	: m_pContext(context)
	, m_pSceneTool(NULL)
{
	context->registerService<ISceneToolUiService>(this);

	if (!m_pSceneTool)
	{
		m_pSceneTool = new CSceneTool(context);
	}
}

CSceneToolUiPlugin::~CSceneToolUiPlugin()
{

}

QWidget* CSceneToolUiPlugin::getWgt()
{
	if (m_pSceneTool)
	{
		return m_pSceneTool;
	}
	return NULL;
}

void CSceneToolUiPlugin::stopManipulation()
{
	if (m_pSceneTool)
	{
		m_pSceneTool->stopManipulation();
	}
}