#ifndef IMAPLAYER_WGT_SERVICE_H
#define IMAPLAYER_WGT_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class IMapLayerWgtService
{
public:
	/**
	* @brief 获取环境插件的窗口指针
	* @return 环境插件的窗口指针
	*/
	virtual QWidget* getWgt() = 0;

	/**
	* @brief 获取二维地图图层属性管理
	* @return 图层属性管理
	*/
	virtual QWidget* getLayerMgrWgt() = 0;
};

Q_DECLARE_INTERFACE(IMapLayerWgtService, "freex_maplayer_wgt_service")

#endif
