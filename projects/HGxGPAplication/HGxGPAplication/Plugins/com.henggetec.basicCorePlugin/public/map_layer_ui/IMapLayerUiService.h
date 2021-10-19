#ifndef IMAPLAYER_UI_SERVICE_H
#define IMAPLAYER_UI_SERVICE_H

#include "layer_ui/CLayerManager.h"

class IMapLayerUiService
{
public:
	// 从本地文件添加二维图层
	virtual void addMapLayerFromLocalFile(const QString &filePath) = 0;

	// 从服务添加二维图层
	virtual void addMapLayerFromService(const QString &url, const QString &layerName, const QString &driverType, CLayerConfig::LayerType layerType, const QString &layerFormat = "" ) = 0;
};

Q_DECLARE_INTERFACE(IMapLayerUiService, "freex_maplayer_ui_service")

#endif
