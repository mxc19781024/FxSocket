#ifndef CPLOTTYPESELECTWIDGET_H
#define CPLOTTYPESELECTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "FxFlowLayout/CFlexibleFlowLayout.h"

class CSceneToolItem;
class FlowLayout;
class CFloatScrollBar;

class CSceneToolSelectWidget : public QScrollArea
{
	Q_OBJECT

public:
	CSceneToolSelectWidget(QWidget *parent);
	~CSceneToolSelectWidget();
	// 添加新的工具类型
	CSceneToolItem *addPlotType(bool bIsCheckable = true, const QString &name = QString(), const QPixmap &normalPixmap = QPixmap()
		, const QPixmap &hoverPixmap = QPixmap(), const QPixmap &selectedPixmap = QPixmap());

	/** 
	* @brief 设置当前选中的场景工具
	* @note add by w00025
	*/
	void setCurrentItem(CSceneToolItem*	pCurSceneToolItem);

protected:

private slots:
	// 标绘类型点击槽函数
	void sceneToolClickedSlot(bool);

private:
	// 流布局
	CFlexibleFlowLayout*			m_pLayout;
	// 滚动条
	CFloatScrollBar*				m_pScrollBar;
	// 标记当前的ITEM
	CSceneToolItem*					m_pCurSceneToolItem;
};

#endif // CPLOTTYPESELECTWIDGET_H
