#ifndef CPLOTTYPEITEM_H
#define CPLOTTYPEITEM_H

#include <QPushButton>

/**
* @class	: CSceneToolItem
* @brief	: 标绘类型项
* @note		: 选择某个标绘类型，然后在场景中进行标绘
* @author	: kzm
**/
class CSceneToolItem : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool selected READ isSelected WRITE setSelected)

public:
	CSceneToolItem(QWidget *parent, bool bIsCheckable = true, const QString &name = QString(), const QPixmap &normalPixmap = QPixmap()
		, const QPixmap &hoverPixmap = QPixmap(), const QPixmap &selectedPixmap = QPixmap());
	~CSceneToolItem();

	// 是否选中
	bool isSelected() { return m_bSelected; }

	// 是否可选中
	bool isCheckable() { return m_bIsCheckable; }
	
	// 设置是否选中
	void setSelected(bool selected);

	// 获取场景工具名称
	QString getToolName();
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
	bool m_bIsCheckable;		// 是否可选中
};

#endif // CPLOTTYPEITEM_H
