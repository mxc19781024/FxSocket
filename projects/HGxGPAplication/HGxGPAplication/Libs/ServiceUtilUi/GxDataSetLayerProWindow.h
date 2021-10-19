#ifndef GXDATASETLAYERPROWINDOW_H
#define GXDATASETLAYERPROWINDOW_H

#include <QWidget>
#include "ui_GxDataSetLayerProWindow.h"
//#include <FreeXServiceUtilUi/Common.h>
#include "public/Common.h"

/**
* class CGxDataSetLayerProWindow
* 展示Tile类型的图层的属性窗口
* 包含服务名称
* 服务地址
* 服务类型
*/

class CGxDataSetLayerProWindow : public QWidget
{
	Q_OBJECT

public:
	CGxDataSetLayerProWindow(QWidget *parent = 0);
	~CGxDataSetLayerProWindow();

private:
	// 刷新界面信息
	void updateUi();

public:
	// 添加服务属性
	void  setLayerProPerty(SLayerPropertyStruct stLayerProPerty);
	// 更改姓名
	void setServerName(const QString &strName);
signals:
	// 通过属性编辑更改图层信息
	void reLayerSetByProEditSignal(const SLayerPropertyStruct &stLayerPropertyStruct);

private slots:
	// 图层类型更改槽函数
	void on_pLayerTypeCBBox_currentIndexChanged(const QString &strText);

	//void on_pDataSetNameEdit_editingFinished();

	void on_pDataSetNameEdit_textChanged(const QString &strText);
private:
	Ui::CGxDataSetLayerProWindow ui;
	SLayerPropertyStruct m_stLayerProperty;           // 图层属性
};

#endif // GXDATASETLAYERPROWINDOW_H
