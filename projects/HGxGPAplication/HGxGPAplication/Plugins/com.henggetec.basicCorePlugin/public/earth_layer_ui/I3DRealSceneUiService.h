/**************************************************************************************************
* @file I3DRealSceneUiService.h
* @note 三维实景服务
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef I3D_REAL_SCENE_UI_SERVICE_H
#define I3D_REAL_SCENE_UI_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class I3DRealSceneUiService
{
public:
	virtual ~I3DRealSceneUiService() {}

	/**
	* @brief	: 获取场景
	*/
	virtual QWidget* getWgt() = 0;
};

Q_DECLARE_INTERFACE(I3DRealSceneUiService, "freex_3d_real_scene_ui_service")

#endif
