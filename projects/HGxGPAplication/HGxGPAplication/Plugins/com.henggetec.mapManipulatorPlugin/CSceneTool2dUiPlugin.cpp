#include "CSceneTool2dUiPlugin.h"
#include "CSceneToolFor2d.h"
//#include <scene/IFxSceneService.h>
//#include <FreeXFramework/FxServiceReference.h>
//#include <scene/IFxScene.h>
//#include <FreeXFramework/FreeXFramework.h>
#include <scene_ui/ISceneUiService.h>
#include <scene_ui/ISceneInterface.h>


CSceneTool2dUiPlugin::CSceneTool2dUiPlugin(ctkPluginContext *context)
	: m_pContext(context)
	, m_pSceneTool(NULL)
{
	context->registerService<ISceneTool2dUiService>(this);

	if (!m_pSceneTool)
	{
		m_pSceneTool = new CSceneToolFor2d(context);
	}
}

CSceneTool2dUiPlugin::~CSceneTool2dUiPlugin()
{

}

QWidget* CSceneTool2dUiPlugin::getWgt()
{
	if (m_pSceneTool)
	{
		return m_pSceneTool;
	}
	return NULL;
}

void CSceneTool2dUiPlugin::stopManipulation()
{
	if (m_pSceneTool)
	{
		m_pSceneTool->stopManipulation();
	}
}
