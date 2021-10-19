#include "CSceneToolItem.h"

#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

#define ITEM_HEIGHT 105

CSceneToolItem::CSceneToolItem(QWidget *parent, bool bIsCheckable, const QString &name, const QPixmap &normalPixmap, const QPixmap &hoverPixmap, const QPixmap &selectedPixmap)
	: QWidget(parent)
	, m_normalPixmap(normalPixmap)
	, m_hoverPixmap(hoverPixmap)
	, m_selectedPixmap(selectedPixmap)
	, m_bMouseHover(false)
	, m_bSelected(false)
	, m_bIsCheckable(bIsCheckable)
{
	setMinimumHeight(ITEM_HEIGHT);
	setMaximumHeight(ITEM_HEIGHT);
	setToolTip(name);
	if (name.size() <= 4)
	{
		m_strName = name;
	}
	else
	{
		m_strName = name.left(4).append("...");
	}
}

CSceneToolItem::~CSceneToolItem()
{

}

void CSceneToolItem::setSelected(bool bSelected)
{
	m_bSelected = bSelected;
	style()->unpolish(this);
	style()->polish(this);
	update();
}

void CSceneToolItem::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	if (m_bSelected)
	{
		painter.drawPixmap((event->rect().width() - m_selectedPixmap.width()) / 2, ITEM_HEIGHT / 5, m_selectedPixmap);
		QPen pen(Qt::white, 1);
		painter.setPen(pen);
	}
	else
	{
		if (m_bMouseHover)
		{
			painter.drawPixmap((event->rect().width() - m_hoverPixmap.width()) / 2, ITEM_HEIGHT / 5, m_hoverPixmap);
			QPen pen(Qt::white, 1);
			painter.setPen(pen);
		}
		else
		{
			painter.drawPixmap((event->rect().width() - m_normalPixmap.width()) / 2, ITEM_HEIGHT / 5, m_normalPixmap);
			QPen pen(QColor("#999999"), 1);
			painter.setPen(pen);
		}
	}
	painter.drawText(0, ITEM_HEIGHT * 3 / 5, event->rect().width(), 100, Qt::AlignHCenter, m_strName);
}

void CSceneToolItem::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_bIsCheckable)
		{
			setSelected(!m_bSelected);
		}
		else
		{
			if (m_strName != "选择")
			{
				// 放大、缩小、漫游操作属于切换
				setSelected(true);
			}
			else
			{
				// 选择即标绘拾取工具为可点击选中可点击取消
				setSelected(!m_bSelected);
			}
		}
		
		emit itemClickedSignal(m_bSelected);
	}
	QWidget::mouseReleaseEvent(event);
}

void CSceneToolItem::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if(!m_bIsCheckable)
		{
			setSelected(!m_bSelected);
		}
	}
	QWidget::mousePressEvent(event);
}

void CSceneToolItem::enterEvent(QEvent *event)
{
	m_bMouseHover = true;
	update();
	QWidget::enterEvent(event);
}

void CSceneToolItem::leaveEvent(QEvent *event)
{
	m_bMouseHover = false;
	update();
	QWidget::leaveEvent(event);
}

QString CSceneToolItem::getToolName()
{
	return m_strName;
}

