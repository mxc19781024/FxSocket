#ifndef CEarthLineLayerPropertyWgt_H
#define CEarthLineLayerPropertyWgt_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>
#include "ui_CEarthLineLayerPropertyWgt.h"
#include <layer_ui/CLayerManager.h>
//#include <scene/IFxScene.h>

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerModel;
	class CGxSceneBase;
}

class CEarthLineLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CEarthLineLayerPropertyWgt(QWidget *parent = 0);
	~CEarthLineLayerPropertyWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

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
	// 透明度改变槽函数
	void transparencyChangedSlot(int);
	// 线宽改变槽函数
	void lineWidthChangedSlot(int);
	// 线型改变槽函数
	void lineTypeChangedSlot(int);
	// 获取当前视点范围槽函数
	void getCurrentViewScopeSlot();

private:
	//初始化信号和槽函数
	void initSignalAndSlots();

	HGxScene::CGxSceneBase* getCurrentScene();

signals:
	void layerNameChangedSignal(const QString &);

private:
	Ui::CEarthLineLayerPropertyWgt ui;

	//记录可见范围上一次的值 用于超过最大值时恢复
	double m_lastMinScaleValue;
	double m_lastMaxScaleValue;

	HGxScene::CGxLayerModel* m_pModelLayer;
};

#endif // CEarthLineLayerPropertyWgt_H
