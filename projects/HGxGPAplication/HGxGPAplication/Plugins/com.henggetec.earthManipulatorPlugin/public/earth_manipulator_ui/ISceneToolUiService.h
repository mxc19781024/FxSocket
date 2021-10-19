#ifndef ISCENE_TOOL_UI_SERVICE_H
#define ISCENE_TOOL_UI_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class ISceneToolUiService
{
public:

	/**
	* @brief	: 创建场景
	*/
	virtual QWidget* getWgt() = 0;

	/**
	* @brief	: 停止场景操作
	*/
	virtual void stopManipulation() = 0;
};

Q_DECLARE_INTERFACE(ISceneToolUiService, "freex_scene_tool_ui_service")

#endif
