#ifndef CEarthFacetLayerPropertyWgt_H
#define CEarthFacetLayerPropertyWgt_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>
//#include <scene/IFxScene.h>

#include "ui_CEarthFacetLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerModel;
	class CGxSceneBase;
}


class CEarthFacetLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CEarthFacetLayerPropertyWgt(QWidget *parent = 0);
	~CEarthFacetLayerPropertyWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

signals:
	void layerNameChangedSignal(const QString &);

private:
	//初始化信号和槽函数
	void initSignalAndSlots();

	HGxScene::CGxSceneBase* getCurrentScene();

private slots:
	// 图层名称改变槽函数
	void layerNameChangedSlot(const QString &name);
	// 数据路径改变槽函数
	void dataPathChangedSlot(const QString &path);
	// 是否所有视点范围均可见改变槽函数
	void allViewScopeChangedSlot(bool);
	// 最小、最大视点范围改变槽函数
	void viewScopeChangedSlot();
	// 线色改变槽函数
	void lineColorChangedSlot(const QColor &color);
	// 线不透明度改变槽函数
	void lineTranChangedSlot(int);
	// 线宽改变槽函数
	void lineWidthChangedSlot(int);
	// 线型改变槽函数
	void lineTypeChangedSlot(int);
	// 填充色改变槽函数
	void fillColorChangedSlot(const QColor &color);
	// 填充不透明度改变槽函数
	void fillTranChangedSlot(int);
	// 获取当前视点范围
	void getCurrentViewScopeSlot();

private:
	Ui::CEarthFacetLayerPropertyWgt ui;

	//记录视点范围上一次的值 用于超过最大值时恢复
	double m_lastMinScaleValue;
	double m_lastMaxScaleValue;

	HGxScene::CGxLayerModel *m_pModelLayer;
};

#endif // CEarthFacetLayerPropertyWgt_H
