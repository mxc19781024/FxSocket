#ifndef IEARTHLAYER_WGT_SERVICE_H
#define IEARTHLAYER_WGT_SERVICE_H

#include <QWidget>
#include <QtPlugin>

class IEarthLayerWgtService
{
public:
	/**
	* @brief 获取环境插件的窗口指针
	* @return 环境插件的窗口指针
	*/
	virtual QWidget* getWgt() = 0;

	/**
	* @brief 获取三维地球图层属性管理
	* @return 图层属性管理
	*/
	virtual QWidget* getLayerMgrWgt() = 0;
};

Q_DECLARE_INTERFACE(IEarthLayerWgtService, "freex_earthlayer_wgt_service")

#endif
