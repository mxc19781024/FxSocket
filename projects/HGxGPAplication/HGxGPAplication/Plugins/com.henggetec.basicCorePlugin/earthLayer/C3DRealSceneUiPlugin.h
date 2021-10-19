/**************************************************************************************************
* @file C3DRealSceneUiPlugin.h
* @note 三维实景插件
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef C3D_REAL_SCENE_UI_PLUGIN_H
#define C3D_REAL_SCENE_UI_PLUGIN_H

#include <QObject>
#include <ctkPluginContext.h>
#include <service/event/ctkEventHandler.h>

#include <ctkPluginContext.h>
#include <earth_layer_ui/I3DRealSceneUiService.h>

class C3DRealScene;

/**
  * @class C3DRealSceneUiPlugin
  * @brief 三维实景插件类
  * @author w00040
*/
class C3DRealSceneUiPlugin : public QObject, 
	public I3DRealSceneUiService
{
	Q_OBJECT
	Q_INTERFACES(I3DRealSceneUiService)

public:
	C3DRealSceneUiPlugin(ctkPluginContext *context);
	~C3DRealSceneUiPlugin();
	
	/**
	* @brief	: 获取窗口
	*/
	QWidget* getWgt();

private:
	// 插件上下文
	ctkPluginContext*				m_pContext;

	// 三维实景指针
	C3DRealScene*					m_p3DRealScene;
};

#endif
