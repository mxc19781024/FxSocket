#ifndef GXTILELAYERPROWINDOW_H
#define GXTILELAYERPROWINDOW_H

#include <QWidget>
#include "ui_GxTileLayerProWindow.h"
//#include "FreeXServiceUtilUi/Common.h"
#include "public/Common.h"

/**
* class CGxTileLayerProWindow
* 展示Tile类型的图层的属性窗口
* 包含服务名称
* 服务地址
* 服务类型
*/

class CGxTileLayerProWindow : public QWidget
{
	Q_OBJECT

public:
	CGxTileLayerProWindow(QWidget *parent = 0);
	~CGxTileLayerProWindow();

private:
	// 刷新界面信息
	void updateUi();

public:
	// 添加服务属性
	void  setLayerProPerty(SLayerPropertyStruct stLayerProPerty);
	// 更改姓名
	void setServerName(const QString &strName);

private slots:
	// 图层类型更改槽函数
	void on_pLayerTypeCBBox_currentIndexChanged(const QString &strText);

	//void on_pServerNameEdit_editingFinished();

	void on_pServerNameEdit_textChanged(const QString &strText);

signals:
	// 通过属性编辑更改图层信息
	void reLayerSetByProEditSignal(const SLayerPropertyStruct &stLayerPropertyStruct);

private:
	Ui::CGxTileLayerProWindow ui;
	SLayerPropertyStruct m_stLayerProperty;           // 图层属性
};

#endif // GXTILELAYERPROWINDOW_H
