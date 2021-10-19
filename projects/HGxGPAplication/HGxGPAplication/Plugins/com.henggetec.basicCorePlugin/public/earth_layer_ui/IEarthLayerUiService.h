#ifndef IEARTHLAYER_UI_SERVICE_H
#define IEARTHLAYER_UI_SERVICE_H

#include "layer_ui/CLayerManager.h"
//#include "earth_layer/IFxLayer.h"

#include <gdpScene/layer/GxLayerLive.h>

class IEarthLayerUiService
{
public:
	virtual void addImageLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true, const QString &layerFormat = "") = 0;
	virtual void addEleLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true) = 0;
	virtual void addVectorLayer(const QString &name, const QString &path, const QString &driverType, bool fromLocal = true) = 0;

	virtual void addLiveLayer(HGxScene::CGxGlobalLayer *pLiveLayer) = 0;
	
	/// 添加海图图层 add by yq 海图图层也可以拖拽添加 
	virtual void addSeaLayer(const QString &name, const QString &path,const QString &driver="s57",bool fromlocal = true){};
};

Q_DECLARE_INTERFACE(IEarthLayerUiService, "freex_earthlayer_ui_service")

#endif
