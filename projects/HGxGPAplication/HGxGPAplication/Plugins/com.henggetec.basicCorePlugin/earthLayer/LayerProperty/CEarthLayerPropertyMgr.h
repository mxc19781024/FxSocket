#ifndef EARTH_LAYER_PROPERTY_MGR_H
#define EARTH_LAYER_PROPERTY_MGR_H

#include <QObject>
#include <QPointer>
//#include <earth_layer/IFxEarthLayer.h>

#include "CEarthImageLayerPropertyWgt.h"
#include "CEarthDotLayerPropertyWgt.h"
#include "CEarthLineLayerPropertyWgt.h"
#include "CEarthFacetLayerPropertyWgt.h"
#include "CEleLayerPropertyWgt.h"
#include "CTiltPropertyWgt.h"

class CGxUiFrameworkServiceInterface;

namespace HGxScene
{
	class CGxGlobalLayer;
}

class QHBoxLayout;

class CEarthLayerPropertyMgr : public /*QObject*/QWidget
{
	Q_OBJECT

public:
	CEarthLayerPropertyMgr(QWidget *parent = nullptr);
	~CEarthLayerPropertyMgr();

	// 显示属性窗口
	void showLayerProperty(HGxScene::CGxGlobalLayer* layer);

	// 当前图层发生改变(如果属性窗口所在的Dock已创建并且可见，则更新；否则不做任何处理)
	void changeCurrentLayer(HGxScene::CGxGlobalLayer* player);

	// 删除图层属性
	void deleteLayerProperty(HGxScene::CGxGlobalLayer* layer);

Q_SIGNALS:
	void layerNameChangedSignal(const QString &);

private:
	QPointer<CEarthImageLayerPropertyWgt >m_pImgLayerProp;	// 影像图层
	QPointer<CEarthDotLayerPropertyWgt> m_pDotLayerProp;		// 矢量点图层
	QPointer<CEarthLineLayerPropertyWgt> m_pLineLayerProp;	// 矢量线图层
	QPointer<CEarthFacetLayerPropertyWgt> m_pFacetLayerProp;	// 矢量面图层
	QPointer<CEleLayerPropertyWgt> m_pEleLayerProp;		// 高程图层
	QPointer<CTiltPropertyWgt> m_pLiveLayerProp;			// 倾斜摄影图层

	CGxUiFrameworkServiceInterface* m_pUiFrameworkService; // 主界面插件服务
	QHBoxLayout* m_pHBoxLayout;
};

#endif // EARTH_LAYER_PROPERTY_MGR_H
