#ifndef CPLOTTYPESELECTWIDGET_H
#define CPLOTTYPESELECTWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "FxFlowLayout/CFlexibleFlowLayout.h"

class CSceneToolItem;
class FlowLayout;
class CFloatScrollBar;

/*#ifdef FXSCENETOOL_LIB
# define FXSCENETOOL_EXPORT __declspec(dllexport)
#else
# define FXSCENETOOL_EXPORT __declspec(dllimport)
#endif*/

class /*FXSCENETOOL_EXPORT*/ CSceneToolSelectWidget : public QScrollArea
{
	Q_OBJECT

public:
	CSceneToolSelectWidget(QWidget *parent);
	~CSceneToolSelectWidget();
	// 添加新的工具类型
	CSceneToolItem *addPlotType(bool bIsCheckable = true, const QString &name = QString(), const QPixmap &normalPixmap = QPixmap()
		, const QPixmap &hoverPixmap = QPixmap(), const QPixmap &selectedPixmap = QPixmap());

public:
	/** 
	* @brief 设置当前选中的场景工具
	* @note add by w00025
	*/
	void setCurrentItem(CSceneToolItem*	pCurSceneToolItem);


private slots:
	// 标绘类型点击槽函数
	void sceneToolClickedSlot(bool);

private:
	CFlexibleFlowLayout *m_pLayout;				// 流布局
	CFloatScrollBar *m_pScrollBar;		// 滚动条
	CSceneToolItem *m_pCurSceneToolItem;	// 标记当前的ITEM
};

#endif // CPLOTTYPESELECTWIDGET_H
