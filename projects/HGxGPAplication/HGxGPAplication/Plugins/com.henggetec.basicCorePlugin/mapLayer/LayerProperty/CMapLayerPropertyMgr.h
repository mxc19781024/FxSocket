#ifndef MAP_LAYER_PROPERTY_MGR_H
#define MAP_LAYER_PROPERTY_MGR_H

#include <QObject>
#include <QPointer>
#include <QWidget>

#include "CMapImageLayerPropertyWgt.h"
#include "CMapDotLayerPropertyWgt.h"
#include "CMapLineLayerPropertyWgt.h"
#include "CMapFacetLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
}

class QHBoxLayout;
class CGxUiFrameworkServiceInterface;

class CMapLayerPropertyMgr :/* public QObject*/public QWidget
{
	Q_OBJECT

public:
	CMapLayerPropertyMgr(QWidget *parent = nullptr);
	~CMapLayerPropertyMgr();

	// 显示属性窗口
	void showLayerProperty(HGxScene::CGxGlobalLayer* pLayer);

	// 当前图层发生改变(如果属性窗口所在的Dock已创建并且可见，则更新；否则不做任何处理)
	void changeCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	// 删除图层属性
	void deleteLayerProperty(HGxScene::CGxGlobalLayer* layer);

Q_SIGNALS:
	void layerNameChangedSignal(const QString &);

private:
	QPointer<CMapImageLayerPropertyWgt> m_pImgLayerProp;	// 影像图层
	QPointer<CMapDotLayerPropertyWgt> m_pDotLayerProp;		// 矢量点图层
	QPointer<CMapLineLayerPropertyWgt> m_pLineLayerProp;	// 矢量线图层
	QPointer<CMapFacetLayerPropertyWgt> m_pFacetLayerProp;	// 矢量面图层

	CGxUiFrameworkServiceInterface* m_pUiFrameworkService; // 主界面插件服务
	QHBoxLayout* m_pHLayout;
};

#endif // MAP_LAYER_PROPERTY_MGR_H
