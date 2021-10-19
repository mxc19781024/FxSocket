#include <QPainter>
#include "CSingleColorPickerItem.h"

#define ITEM_EDGE_LENGTH 18


class CSingleColorPickerItemPrivate
{
	Q_DECLARE_PUBLIC(CSingleColorPickerItem)

public:
	CSingleColorPickerItemPrivate(CSingleColorPickerItem *parent);

public:
	CSingleColorPickerItem * const q_ptr;
	bool m_bMouseIn;
	QColor m_color;
};

CSingleColorPickerItemPrivate::CSingleColorPickerItemPrivate(CSingleColorPickerItem *parent)
	: q_ptr(parent)
	, m_bMouseIn(false)
{

}


CSingleColorPickerItem::CSingleColorPickerItem(const QColor &c, QWidget *parent)
	: QWidget(parent)
	, d_ptr(new CSingleColorPickerItemPrivate(this))
{
	setFixedSize(ITEM_EDGE_LENGTH, ITEM_EDGE_LENGTH);
	SetColor(c);
	setCursor(Qt::PointingHandCursor);
}

CSingleColorPickerItem::~CSingleColorPickerItem()
{

}

QColor CSingleColorPickerItem::GetColor()
{
	Q_D(CSingleColorPickerItem);
	return d->m_color;
}

void CSingleColorPickerItem::paintEvent(QPaintEvent *ev)
{
	Q_D(CSingleColorPickerItem);
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(d->m_color);
	painter.drawRoundedRect(0, 0, width(), height(), 2, 2);
	//painter.drawRect(0, 0, width(), height());
	/*if (d->m_bMouseIn)
	{
		painter.setPen(QPen(Qt::white, 3));
		painter.drawRect(1, 1, ITEM_EDGE_LENGTH - 3, ITEM_EDGE_LENGTH - 3);
	}*/
	QWidget::paintEvent(ev);
}

void CSingleColorPickerItem::mousePressEvent(QMouseEvent *ev)
{
	Q_UNUSED(ev);
	Q_D(CSingleColorPickerItem);
	int h = d->m_color.hue();
	int s = d->m_color.saturation();
	int v = d->m_color.value();
	emit ItemClickedSignal(d->m_color);
	QWidget::mousePressEvent(ev);
}

void CSingleColorPickerItem::enterEvent(QEvent *)
{
	Q_D(CSingleColorPickerItem);
	d->m_bMouseIn = true;
	update();
}

void CSingleColorPickerItem::leaveEvent(QEvent *)
{
	Q_D(CSingleColorPickerItem);
	d->m_bMouseIn = false;
	update();
}

void CSingleColorPickerItem::SetColor(const QColor &c)
{
	Q_D(CSingleColorPickerItem);
	d->m_color = c;
	update();
}
