#ifndef IMAGE_LAYER_PROPERTY_WGT_H
#define IMAGE_LAYER_PROPERTY_WGT_H

#include <QWidget>
//#include <map_layer/IFxMapLayer.h>

#include "ui_CMapImageLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerImage;
}

class CMapImageLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CMapImageLayerPropertyWgt(QWidget *parent = 0);
	~CMapImageLayerPropertyWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

private slots:
	void layerNameChangedSlot(const QString &name);
	void dataPathChangedSlot(const QString &path);
	void transparencyChangedSlot(int);
	void cacheEnableChangedSlot(bool);

signals:
	void layerNameChangedSignal(const QString &);

private:
	Ui::CMapImageLayerPropertyWgt ui;

	HGxScene::CGxLayerImage* m_pImageLayer;
};

#endif // IMAGE_LAYER_PROPERTY_WGT_H
