#include "CFloatScrollBar.h"

#include <QPainter>




CFloatScrollBar::CFloatScrollBar(QWidget *parent)
    : QScrollBar(parent)
{
	setContextMenuPolicy(Qt::PreventContextMenu);
	setMouseTracking(true);
	setStyleSheet("QScrollBar:vertical { margin: 0; }"
		"QScrollBar::add-line:vertical { height: 0; margin: 0; }"
		"QScrollBar::sub-line:vertical { height: 0; margin: 0; }");
}

CFloatScrollBar::~CFloatScrollBar()
{
}


QSize CFloatScrollBar::sizeHint() const
{
	if (Qt::Horizontal == orientation()) {
		return QSize(1, 10);
	}
	else {
		return QSize(7, 1);
	}
}


void CFloatScrollBar::setScrollBarOpacity(double dfOpacity)
{
	m_dfOpacity = dfOpacity;
}

void CFloatScrollBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

	QRect ss = rect();

	painter.fillRect(rect(), QColor("#333333"));

    int x, y, w, h;
    rect().getRect(&x, &y, &w, &h);

    QMargins margins(0, 2, 0, 2);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor("#333333"));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);

	painter.setOpacity(m_dfOpacity);
	
	QRect curRect = rect();
    QRect trimmed(rect()/*.adjusted(2, 2, 2, 2)*/);

    QPainterPath path;
    path.addRoundedRect(trimmed, 2, 2);
    painter.setClipPath(path);

    painter.drawRect(trimmed);

    const qreal q = (Qt::Horizontal == orientation() ? w : h) /
            static_cast<qreal>(maximum()-minimum()+pageStep()-1);

    QRect handle = Qt::Horizontal == orientation()
            ? QRect(sliderPosition()*q, y, pageStep()*q, h)
            : QRect(x, sliderPosition()*q, w, pageStep()*q);

    painter.setBrush(QColor("#636363"));

    painter.drawRoundedRect(handle, 2, 2);
}
