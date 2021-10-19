/**************************************************************************************************
* @file C3DRealSceneSelectWidget.h
* @note 三维实景选择界面
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef C3D_REAL_SCENE_SELECT_WIDGET_H
#define C3D_REAL_SCENE_SELECT_WIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "FxFlowLayout/CFlexibleFlowLayout.h"

class C3DRealSceneItem;
class CGxFlowLayout;

class C3DRealSceneSelectWidget : public QScrollArea
{
	Q_OBJECT

public:
	C3DRealSceneSelectWidget(QWidget *parent);
	~C3DRealSceneSelectWidget();
	// 添加新的工具类型
	C3DRealSceneItem *addPlotType(bool bIsCheckable = true, const QString &name = QString(), const QPixmap &normalPixmap = QPixmap()
		, const QPixmap &hoverPixmap = QPixmap(), const QPixmap &selectedPixmap = QPixmap());

public:
	/** 
	* @brief 设置当前选中的场景工具
	* @note add by w00025
	*/
	void setCurrentItem(C3DRealSceneItem*	pCurSceneToolItem);


private slots:
	// 标绘类型点击槽函数
	void sceneToolClickedSlot(bool);

private:
	//CFlexibleFlowLayout *m_pLayout;				// 流布局
	//CFloatScrollBar *m_pScrollBar;		// 滚动条

	CFlexibleFlowLayout* m_pLayout;		// 流布局
	C3DRealSceneItem *m_pCurSceneToolItem;	// 标记当前的ITEM
};

#endif // CPLOTTYPESELECTWIDGET_H
