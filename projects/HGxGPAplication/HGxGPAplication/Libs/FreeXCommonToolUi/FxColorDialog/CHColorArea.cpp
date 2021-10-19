/**************************************************************************************************
* @file		: CHColorArea.cpp
* @note		: 色调区域实现文件
* @author	: kzm
* @data		: 2018-8
**************************************************************************************************/

#include <QPainter>
#include <QMouseEvent>
#include <QLinearGradient>
#include "CHColorArea.h"


class CHColorAreaPrivate
{
	Q_DECLARE_PUBLIC(CHColorArea)
public:
	explicit CHColorAreaPrivate(CHColorArea *parent);

public:
	CHColorArea * const q_ptr;	// q指针
	double m_dfHue;				// 色调值(0.0 - 1.0)
	int m_iHue;					// 色调值(0 - 360)
	int m_iBorderMargin;		// 两端的边距
	int m_iSideMargin;			// 指针标记的宽度
};

CHColorAreaPrivate::CHColorAreaPrivate(CHColorArea *parent)
	: q_ptr(parent)
	, m_iSideMargin(8)
	, m_iBorderMargin(10)
{

}

CHColorArea::CHColorArea(QWidget *parent, int iHue)
	: QWidget(parent)
	, d_ptr(new CHColorAreaPrivate(this))
{
	Q_D(CHColorArea);
	d->m_iHue = iHue;
	d->m_dfHue = qRound((double)iHue / 360);
}

CHColorArea::~CHColorArea()
{

}

void CHColorArea::SetHueValue(int iHue)
{
	Q_D(CHColorArea);
	d->m_iHue = iHue;
	d->m_dfHue = (double)iHue / 360;
	update();
}


int CHColorArea::GetHueValue()
{
	Q_D(CHColorArea);
	return d->m_iHue;
}

void CHColorArea::paintEvent(QPaintEvent *event)
{
	Q_D(CHColorArea);

	QRect r = event->rect();

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);

	int iColorAreaWidth = r.width() - d->m_iBorderMargin * 2;
	// 画渐变区域
	QLinearGradient hueGradient(0, 0, iColorAreaWidth, 0);
	for (double i = 0; i < 1.0; i += 1.0 / 16)
	{
		hueGradient.setColorAt(i, QColor::fromHsvF(i, 1, 1));
	}
	hueGradient.setColorAt(1, QColor::fromHsvF(1, 1, 1));
	painter.setBrush(QBrush(hueGradient));
	painter.drawRoundedRect(d->m_iBorderMargin, d->m_iSideMargin, iColorAreaWidth, r.height() - d->m_iSideMargin * 2, 5, 5);

	QPointF selecPos = QPointF(d->m_iBorderMargin + qRound(d->m_dfHue * iColorAreaWidth), r.height() / 2);

	painter.setBrush(QColor::fromHsv(d->m_iHue, 255, 255));
	painter.setPen(QPen(Qt::white, 2));
	painter.drawEllipse(selecPos, 8, 8);
}

void CHColorArea::mousePressEvent(QMouseEvent *ev)
{
	Q_D(CHColorArea);

	int iColorAreaWidth = width() - d->m_iBorderMargin * 2;
	double dfTempValue = (double)(ev->pos().x() - d->m_iBorderMargin) / iColorAreaWidth;
	d->m_dfHue = qBound(0.0, dfTempValue, 1.0);
	update();
	d->m_iHue = qBound(0, qRound(d->m_dfHue * 360), 359);
	emit HueManualChangedSignal(d->m_iHue);
}

void CHColorArea::mouseMoveEvent(QMouseEvent *ev)
{
	Q_D(CHColorArea);

	int iColorAreaWidth = width() - d->m_iBorderMargin * 2;
	double dfTempValue = (double)(ev->pos().x() - d->m_iBorderMargin) / iColorAreaWidth;
	d->m_dfHue = qBound(0.0, dfTempValue, 1.0);
	update();
	d->m_iHue = qBound(0, qRound(d->m_dfHue * 360), 359);
	emit HueManualChangedSignal(d->m_iHue);
}