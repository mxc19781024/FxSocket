#include "CCustomColorItem.h"

#include <QPainter>


CCustomColorItem::CCustomColorItem(const QColor &color, QWidget *parent)
	: CSingleColorItem(color, parent)
{
	SetColor(Qt::transparent);
	setCursor(Qt::ArrowCursor);
	setObjectName("XUI_CustomColorItem");
}

CCustomColorItem::~CCustomColorItem()
{
}

void CCustomColorItem::SetColor(const QColor &c)
{
	CSingleColorItem::SetColor(c);
	setCursor(Qt::PointingHandCursor);
}

void CCustomColorItem::paintEvent(QPaintEvent *event)
{
	CSingleColorItem::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(QPen(QColor("#4f76f4"), 2));
	painter.setBrush(Qt::transparent);
	painter.drawRoundedRect(1, 1, width() - 2, height() - 2, 1, 1);
}

void CCustomColorItem::mousePressEvent(QMouseEvent *event)
{
	if (GetColor() == Qt::transparent)
	{
		return;
	}
	CSingleColorItem::mousePressEvent(event);
}
