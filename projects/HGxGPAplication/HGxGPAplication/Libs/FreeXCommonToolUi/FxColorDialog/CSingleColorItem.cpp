#include <QPainter>
#include "CSingleColorItem.h"


#define ITEM_EDGE_LENGTH 22

class CSingleColorItemPrivate
{
	Q_DECLARE_PUBLIC(CSingleColorItem)

public:
	explicit CSingleColorItemPrivate(CSingleColorItem *parent);

public:
	CSingleColorItem * const q_ptr;
	bool m_bMouseIn;
	QColor m_color;
};

CSingleColorItemPrivate::CSingleColorItemPrivate(CSingleColorItem *parent)
	: q_ptr(parent)
	, m_bMouseIn(false)
{

}


CSingleColorItem::CSingleColorItem(const QColor &c, QWidget *parent)
	: QWidget(parent)
	, d_ptr(new CSingleColorItemPrivate(this))
{
	setFixedSize(ITEM_EDGE_LENGTH, ITEM_EDGE_LENGTH);
	SetColor(c);
	setCursor(Qt::PointingHandCursor);
}

CSingleColorItem::~CSingleColorItem()
{

}

QColor CSingleColorItem::GetColor()
{
	Q_D(CSingleColorItem);
	return d->m_color;
}

void CSingleColorItem::paintEvent(QPaintEvent *ev)
{
	Q_D(CSingleColorItem);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(d->m_color);
	painter.drawRoundedRect(0, 0, width(), height(), 2, 2);
	
	QWidget::paintEvent(ev);
}

void CSingleColorItem::mousePressEvent(QMouseEvent *ev)
{
	Q_UNUSED(ev);
	Q_D(CSingleColorItem);
	emit ItemClickedSignal(d->m_color);
	QWidget::mousePressEvent(ev);
}

void CSingleColorItem::enterEvent(QEvent *)
{
	Q_D(CSingleColorItem);
	d->m_bMouseIn = true;
	update();
}

void CSingleColorItem::leaveEvent(QEvent *)
{
	Q_D(CSingleColorItem);
	d->m_bMouseIn = false;
	update();
}

void CSingleColorItem::SetColor(const QColor &c)
{
	Q_D(CSingleColorItem);
	d->m_color = c;
	update();
}
