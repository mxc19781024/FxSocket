/**************************************************************************************************
* @file CSimuListWidget.h
* @note 仿真列表界面
* @author b00066
* @date 2019-9-24
**************************************************************************************************/

#ifndef SIMU_LIST_WIDGET_H
#define SIMU_LIST_WIDGET_H
#include <ctkPluginContext.h>
#include <service/event/ctkEventHandler.h>
#include "qtreewidget.h"


namespace Ui {
	class CSimuListWidget;
}

class CSimuListWidget : public QWidget, public ctkEventHandler 
{
	Q_OBJECT
	Q_INTERFACES(ctkEventHandler)
public:
	CSimuListWidget(ctkPluginContext *context, QWidget *parent = nullptr);
	~CSimuListWidget();

protected:
	// 业务单元服务插件注册事件处理
	virtual void handleEvent(const ctkEvent& event);

	// 插件启动时初始化
	void init();

	void initStyle();

	bool getService();

	void updateParentItem(QTreeWidgetItem* pItem);

signals:
	// 信号：移除单元服务
	void removeServiceSignal();

	// 信号：更新单元服务
	void updateServiceSignal();

	void changeModeSignal(bool);

protected slots:

	// 槽：右键菜单
	void contextMenuSlot(const QPoint&);

	// 槽：控制单元对象显隐
	void itemChangedSlot(QTreeWidgetItem *, int);

	// 槽：菜单点击事件
	void addClickedSlot(bool);
	
	// 槽：菜单点击事件
	void clearClickedSlot(bool);

	// 槽：菜单点击事件
	void removeClickedSlot(bool);

	// 槽：菜单点击事件
	void traceClickedSlot(bool);

	// 槽：菜单点击事件
	void propertyClickedSlot(bool);

	void removeItem( QTreeWidgetItem* pItem );

	// 槽：移除单元服务
	void removeServiceSlot();

	// 槽：更新单元服务
	void updateServiceSlot();

	void updateTree();

	void updateTree(const QString& strServiceID);

	void closeWgtSlot();
protected:
	Ui::CSimuListWidget*                         ui;
	ctkPluginContext*                            m_pContext;
	QAction*                                     m_pAddAction;
	QAction*                                     m_pClearAction;
	QAction*                                     m_pRemoveAction;
	QAction*                                     m_pTraceAction;
	QAction*                                     m_pPropertyActiton;
	QMap<QString,QTreeWidgetItem*>               m_mapID2Item;
	QMap<QString,QMap<QString,QTreeWidgetItem*>> m_mapType2Factory;
	QMenu*                                       m_pMenu;
	QTreeWidgetItem*                             m_pCurItem;
	//FxBizUnit::IFxBizUnitService*                m_pBizUnitService;
	bool										 m_bSimuState;
};

#endif