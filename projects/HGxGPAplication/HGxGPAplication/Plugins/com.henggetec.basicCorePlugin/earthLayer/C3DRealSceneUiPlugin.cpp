#include "C3DRealSceneUiPlugin.h"
#include "C3DRealScene.h"

C3DRealSceneUiPlugin::C3DRealSceneUiPlugin(ctkPluginContext *context)
	: m_pContext(context)
	, m_p3DRealScene(NULL)
{
	context->registerService<I3DRealSceneUiService>(this);

	if (!m_p3DRealScene)
	{
		m_p3DRealScene = new C3DRealScene(context);
	}
}

C3DRealSceneUiPlugin::~C3DRealSceneUiPlugin()
{

}

QWidget* C3DRealSceneUiPlugin::getWgt()
{
	if (m_p3DRealScene)
	{
		return m_p3DRealScene;
	}
	return NULL;
}