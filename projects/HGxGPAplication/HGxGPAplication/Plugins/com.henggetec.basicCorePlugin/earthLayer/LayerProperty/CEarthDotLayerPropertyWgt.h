#ifndef CEarthDotLayerPropertyWgt_H
#define CEarthDotLayerPropertyWgt_H

#include <QWidget>
//#include <earth_layer/IFxEarthLayer.h>
#include "ui_CEarthDotLayerPropertyWgt.h"
#include <layer_ui/CLayerManager.h>
//#include <scene/IFxScene.h>

namespace HGxScene
{
	class CGxGlobalLayer;
	class CGxLayerModel;
	class CGxSceneBase;
}

class CEarthDotLayerPropertyWgt : public QWidget
{
	Q_OBJECT

public:
	CEarthDotLayerPropertyWgt(QWidget *parent = 0);
	~CEarthDotLayerPropertyWgt();

	void setCurrentLayer(HGxScene::CGxGlobalLayer* pLayer);
	
	HGxScene::CGxGlobalLayer* getCurrentLayer();

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
	// 获取当前视点范围
	void getCurrentViewScopeSlot();

signals:
	void layerNameChangedSignal(const QString &);

private:
	Ui::CEarthDotLayerPropertyWgt ui;

	//记录可见范围上一次的值 用于超过最大值时恢复
	double m_lastMinScaleValue;
	double m_lastMaxScaleValue;

	HGxScene::CGxLayerModel *m_pModelLayer;
};

#endif // CEarthDotLayerPropertyWgt_H
