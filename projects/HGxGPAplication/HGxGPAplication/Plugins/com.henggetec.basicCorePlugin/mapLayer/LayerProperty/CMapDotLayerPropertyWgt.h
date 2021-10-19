#ifndef CMapDotLayerPropertyWgt_H
#define CMapDotLayerPropertyWgt_H

#include <QWidget>
//#include <map_layer/IFxMapLayer.h>
#include <layer_ui/CLayerManager.h>
//#include <scene/IFxScene.h>

#include "ui_CMapDotLayerPropertyWgt.h"

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerModel;
	class CGxSceneBase;
}

class CMapDotLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CMapDotLayerPropertyWgt(QWidget *parent = 0);
	~CMapDotLayerPropertyWgt();

	// 设置当前图层
	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);

	HGxScene::CGxGlobalLayer* getCurrentLayer();

private:
	//初始化信号和槽函数
	void initSignalAndSlots();

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

	// 字体类型改变槽函数
	void fontFamilyChangedSlot();
	// 字体大小改变槽函数
	void fontSizeChangedSlot(int);
	// 字体颜色改变槽函数
	void fontColorChangedSlot(const QColor &);
	// 图标路径改变槽函数
	void iconPathChangedSlot(const QString &);
	// 图标大小改变槽函数
	void iconSizeChangedSlot(int);
	// 选择图标按钮点击槽函数
	void selectIconPathSlot();

signals:
	// 图层名称改变信号
	void layerNameChangedSignal(const QString &);

private:
	HGxScene::CGxSceneBase* getCurrentScene();

private:
	Ui::CMapDotLayerPropertyWgt ui;

	//记录二维比例尺上一次的值,用于超过最大值时恢复
	double m_lastMinScaleValue;
	double m_lastMaxScaleValue;

	// 底层二维图层对信息
	HGxScene::CGxLayerModel* m_pModelLayer;
};

#endif // CMapDotLayerPropertyWgt_H
