#ifndef CSCENETOOLUIPLUGIN_H
#define CSCENETOOLUIPLUGIN_H

#include <QObject>
#include <ctkPluginContext.h>
#include <service/event/ctkEventHandler.h>

#include <ctkPluginContext.h>
//#include <FreeXCore/IFxDocument.h>
#include <earth_manipulator_ui/ISceneToolUiService.h>

class CSceneTool;

class CSceneToolUiPlugin : public QObject
	, public ISceneToolUiService
{
	Q_OBJECT
	Q_INTERFACES(ISceneToolUiService)

public:
	CSceneToolUiPlugin(ctkPluginContext *context);
	~CSceneToolUiPlugin();
	
	/**
	* @brief	: 获取场景工具窗口
	*/
	QWidget* getWgt();

	/**
	* @brief	: 停止场景操作
	*/
	void stopManipulation();

private:
	// 插件上下文
	ctkPluginContext*				m_pContext;

	// 三维场景工具指针
	CSceneTool*						m_pSceneTool;
};

#endif // CSCENETOOLUIPLUGIN_H
