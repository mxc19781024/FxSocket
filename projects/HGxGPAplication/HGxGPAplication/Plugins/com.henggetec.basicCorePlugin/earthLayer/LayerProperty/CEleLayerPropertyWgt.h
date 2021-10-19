#ifndef CELELAYERPROPERTYWGT_H
#define CELELAYERPROPERTYWGT_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>

#include "ui_CEleLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerElevation;
	class CGxSceneBase;
}

class CEleLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CEleLayerPropertyWgt(QWidget *parent = 0);
	~CEleLayerPropertyWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

private slots:
	// 图层名称改变槽函数
	void layerNameChangedSlot(const QString &name);
	// 数据路径改变槽函数
	void dataPathChangedSlot(const QString &path);
	// 是否允许缓存改变槽函数
	void cacheEnableChangedSlot(bool);

signals:
	void layerNameChangedSignal(const QString &);

private:
	Ui::CEleLayerPropertyWgt ui;

	HGxScene::CGxLayerElevation* m_pEleLayer;
};

#endif // CELELAYERPROPERTYWGT_H
