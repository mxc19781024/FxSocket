#ifndef ISCENE_TOOL2D_UI_SERVICE_H
#define ISCENE_TOOL2D_UI_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class ISceneTool2dUiService
{
public:
	// 创建场景
	virtual QWidget* getWgt() = 0;

	/**
	* @brief	: 停止场景操作
	*/
	virtual void stopManipulation() = 0;
};

Q_DECLARE_INTERFACE(ISceneTool2dUiService, "freex_scene_tool2d_ui_service")

#endif
