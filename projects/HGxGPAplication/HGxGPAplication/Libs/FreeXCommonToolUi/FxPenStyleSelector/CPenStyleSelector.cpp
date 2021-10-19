#include "FxPenStyleSelector/CPenStyleSelector.h"

#include <QPen>
#include <QPainter>
#include <QStyledItemDelegate>



CPenStyleSelector::CPenStyleSelector(QWidget *parent)
	: QComboBox(parent)
{
	setItemDelegate(new QStyledItemDelegate(this));
	setIconSize(QSize(110, 14));

	for (int i = Qt::SolidLine; i <= Qt::DashDotDotLine; ++i)
	{
		QPixmap pixmap(110, 14);
		pixmap.fill(Qt::transparent);
		QBrush brush(Qt::white);
		QPen pen(brush, 2, (Qt::PenStyle)i);

		QPainter painter(&pixmap);
		painter.setPen(pen);
		painter.drawLine(0, 7, 110, 7);

		addItem(QIcon(pixmap), "");
	}
}

CPenStyleSelector::~CPenStyleSelector()
{
}

void CPenStyleSelector::setLineStyle(uint iLineStyle)
{
	switch (iLineStyle)
	{
	case 0xffff:
		setCurrentIndex(0);
		break;
	case 0x3f3f:
		setCurrentIndex(1);
		break;
	case 0x1111:
		setCurrentIndex(2);
		break;
	case 0x27ff:
		setCurrentIndex(3);
		break;
	case 0x24ff:
		setCurrentIndex(4);
		break;
	default:
		setCurrentIndex(0);
		break;
	}
}

uint CPenStyleSelector::lineStyle()
{
	switch (currentIndex())
	{
	case 0:
		return 0xffff;
		break;
	case 1:
		return 0x3f3f;
		break;
	case 2:
		return 0x1111;
		break;
	case 3:
		return 0x27ff;
		break;
	case 4:
		return 0x24ff;
		break;
	}
	return 0xffff;
}
