#ifndef GXOGCLAYERSETPROWINDOW_H
#define GXOGCLAYERSETPROWINDOW_H

#include <QWidget>
#include <QListWidget>

//#include <FreeXServiceUtilUi/Common.h>
#include "public/Common.h"

#include "ui_GxOGCLayerSetProWindow.h"


/**
* class CGxOGCLayerSetProWindow
* 展示OGC类型的服务的属性窗口
* 包含服务名称(可编辑，相当于重命名)
* 服务地址
* 服务类型
* 服务版本
* 图层信息
*/

class CGxOGCLayerSetProWindow : public QWidget
{
	Q_OBJECT

public:
	CGxOGCLayerSetProWindow(QWidget *parent = 0);
	~CGxOGCLayerSetProWindow();

private:
	// 刷新界面信息
	void updateUi();

public:
	// 添加服务属性
	void  setServerProPerty(SServerPropertyStruct stServerProPerty);
	// 更改姓名
	void setServerName(const QString &strName);
signals:
	// 通过属性编辑更改服务名称
	void reSetByProEditSignal(const SServerPropertyStruct &stServerPropertyStruct);

private slots:
	// 查看具体图层信息
	void checkLayerInfoSlot(QListWidgetItem *item);
	// 服务名字编辑结束信号槽
	//void on_pServerNameEdit_editingFinished();
	// 服务名字编辑结束信号槽
	void on_pServerNameEdit_textChanged(const QString &strText);

private:
	Ui::CGxOGCLayerSetProWindow ui;
	SServerPropertyStruct m_stServerProperty;
};

#endif // GXOGCLAYERSETPROWINDOW_H
