#ifndef CSCENETOOL2DUIPLUGIN_H
#define CSCENETOOL2DUIPLUGIN_H

#include <QObject>

#include <ctkPluginContext.h>
//#include <FreeXCore/IFxDocument.h>
#include <map_tool_ui/ISceneTool2dUiService.h>

class CSceneToolFor2d;
class CSceneTool2dUiPlugin : public QObject, public ISceneTool2dUiService
{
	Q_OBJECT
	Q_INTERFACES(ISceneTool2dUiService)

public:
	CSceneTool2dUiPlugin(ctkPluginContext *context);
	~CSceneTool2dUiPlugin();
	
	QWidget* getWgt();

	/**
	* @brief	: 停止场景操作
	*/
	void stopManipulation();

private:
	// 插件上下文
	ctkPluginContext*				m_pContext;

	// 三维场景工具指针
	CSceneToolFor2d*				m_pSceneTool;
};

#endif // CSCENETOOL2DUIPLUGIN_H
