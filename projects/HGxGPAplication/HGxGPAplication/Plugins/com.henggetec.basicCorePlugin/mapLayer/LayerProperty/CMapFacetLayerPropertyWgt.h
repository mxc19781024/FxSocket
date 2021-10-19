#ifndef CMapFacetLayerPropertyWgt_H
#define CMapFacetLayerPropertyWgt_H

#include <QWidget>
//#include <map_layer/IFxMapLayer.h>
//#include <scene/IFxScene.h>
#include "ui_CMapFacetLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerModel;
	class CGxSceneBase;
}

class CMapFacetLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CMapFacetLayerPropertyWgt(QWidget *parent = 0);
	~CMapFacetLayerPropertyWgt();

	// 设置当前图层
	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

private:
	//初始化信号和槽函数
	void initSignalAndSlots();

	HGxScene::CGxSceneBase* getCurrentScene();

	// 初始化比例尺
	void initScaleRange(QComboBox *pComboBox);

	// 设置可见比例尺的范围
	void setVisualScale(int, int);

private slots:
	// 图层名称改变槽函数
	void layerNameChangedSlot(const QString &name);
	// 数据路径改变槽函数
	void dataPathChangedSlot(const QString &path);

	// 比例尺范围可见改变槽函数
	void scaleVisualChangedSlot(bool);
	// 最小比例尺改变槽函数
	void minEditTextChangedSlot(QString strText);  
	// 最大比例尺改变槽函数
	void maxEditTextChangedSlot(QString strText);
	// 获取当前比例尺
	void getCurrentScaleSlot();

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

signals:
	// 图层名称改变信号
	void layerNameChangedSignal(const QString &);

private:
	Ui::CMapFacetLayerPropertyWgt ui;

	//记录视点范围上一次的值 用于超过最大值时恢复
	double m_lastMinScaleValue;
	double m_lastMaxScaleValue;

	HGxScene::CGxLayerModel* m_pModelLayer;
};

#endif // CMapFacetLayerPropertyWgt_H
