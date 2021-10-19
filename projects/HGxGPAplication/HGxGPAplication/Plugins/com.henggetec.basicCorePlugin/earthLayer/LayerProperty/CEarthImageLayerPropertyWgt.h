#ifndef IMAGE_LAYER_PROPERTY_WGT_H
#define IMAGE_LAYER_PROPERTY_WGT_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>

#include <ui_CEarthImageLayerPropertyWgt.h>

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerImage;
}

class CEarthImageLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CEarthImageLayerPropertyWgt(QWidget *parent = 0);
	~CEarthImageLayerPropertyWgt();

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
	Ui::CEarthImageLayerPropertyWgt ui;

	HGxScene::CGxLayerImage* m_pImageLayer;
};

#endif // IMAGE_LAYER_PROPERTY_WGT_H
