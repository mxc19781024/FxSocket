#ifndef GXOGCLAYERPROWINDOW_H
#define GXOGCLAYERPROWINDOW_H

#include <QWidget>

//#include <FreeXServiceUtilUi/Common.h>
#include "public/Common.h"

#include "ui_GxOGCLayerProWindow.h"


/**
* class CGxOGCLayerProWindow
* 展示OGC类型的图层的属性窗口
* 包含服务名称
* 服务地址
* 服务类型
* 图层信息
*/

class CGxOGCLayerProWindow : public QWidget
{
	Q_OBJECT

public:
	CGxOGCLayerProWindow(QWidget *parent = 0);
	~CGxOGCLayerProWindow();

private:
	// 刷新界面信息
	void updateUi();

public:
	// 添加图层属性
	void  setLayerProPerty(SLayerPropertyStruct stLayerProPerty);

signals:
	// 通过属性编辑更改图层信息
	void reLayerSetByProEditSignal(const SLayerPropertyStruct &stLayerPropertyStruct);

private slots:
	// 界面可编辑部分编辑结束信号槽
	//void on_pServerNameEdit_editingFinished();
	//void on_pServerWebsiteEdit_editingFinished();
	//服务格式改变信号槽
	void formatComboBoxChangedSlot(QString strFormat);

private:
	Ui::CGxOGCLayerProWindow ui;
	SLayerPropertyStruct m_stLayerProperty;           // 图层属性
};

#endif // GXOGCLAYERPROWINDOW_H
