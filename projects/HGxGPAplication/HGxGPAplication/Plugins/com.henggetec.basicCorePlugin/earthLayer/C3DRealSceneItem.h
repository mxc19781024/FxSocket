/**************************************************************************************************
* @file C3DRealSceneItem.h
* @note 三维实景类型项
* @author w00040
* @data 2019-5-10
**************************************************************************************************/
#ifndef C3D_REAL_SCENE_ITEM_H
#define C3D_REAL_SCENE_ITEM_H

#include <QPushButton>

/**
  * @class C3DRealSceneItem
  * @brief 实现三维实景类型
  * @author w00040
*/
class C3DRealSceneItem : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
	C3DRealSceneItem(QWidget *parent, bool bIsCheckable = true, const QString &name = QString(), const QPixmap &normalPixmap = QPixmap()
		, const QPixmap &hoverPixmap = QPixmap(), const QPixmap &selectedPixmap = QPixmap());
	~C3DRealSceneItem();

	// 是否选中
	bool isSelected() { return m_bSelected; }

	// 是否可选中
	bool isCheckable() { return m_bIsCheckable; }
	
	// 设置是否选中
	void setSelected(bool selected);

protected:
	// 绘图事件
	void paintEvent(QPaintEvent *) override;
	// 鼠标松开事件
	void mouseReleaseEvent(QMouseEvent *) override;
	// 鼠标点击事件
	void mousePressEvent(QMouseEvent *) override;
	// 鼠标进入事件
	void enterEvent(QEvent *) override;
	// 鼠标离开事件
	void leaveEvent(QEvent *) override;

signals:
	void itemClickedSignal(bool);

private:
	// 标记是否选中该标绘类型
	bool m_bSelected;

private:
	QString m_strName;			// 工具类型名称
	QPixmap m_normalPixmap;		// 正常的图标
	QPixmap m_hoverPixmap;		// 鼠标悬浮时的图标
	QPixmap m_selectedPixmap;	// 选中时的图标
	bool m_bMouseHover;			// 标记鼠标是否悬浮于上面
	bool m_bIsCheckable;		//是否可选中
};

#endif
