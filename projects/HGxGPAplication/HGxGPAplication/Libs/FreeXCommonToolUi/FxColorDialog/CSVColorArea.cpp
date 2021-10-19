#include <QPainter>
#include <QMouseEvent>
#include "CSVColorArea.h"


CSVColorArea::CSVColorArea(QWidget *parent)
	: QWidget(parent)
	, m_iHue(0)
	, m_iSaturation(0)
	, m_iBrightness(0)
	, m_iBorderMargin(10)
{
}

CSVColorArea::~CSVColorArea()
{

}

void CSVColorArea::SetHue(int h)
{
	m_iHue = h;
	update();
}

void CSVColorArea::SetSaturation(int s)
{
	m_iSaturation = s;
	update();
}

void CSVColorArea::SetBrightness(int v)
{
	m_iBrightness = v;
	update();
}

void CSVColorArea::SetHsv(int h, int s, int v)
{
	m_iHue = h;
	m_iSaturation = s;
	m_iBrightness = v;
	update();
}

void CSVColorArea::setColor(const QColor &c)
{
	m_iHue = c.hue();
	m_iSaturation = c.saturation();
	m_iBrightness = c.value();
	update();
}

void CSVColorArea::paintEvent(QPaintEvent *event)
{
	QRect r = this->rect();

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setRenderHint(QPainter::Antialiasing);

	// 水平方向饱和度变化
	QColor hColor;
	hColor.setHsv(m_iHue, 255, 255);
	int iColorAreaWidth = r.width() - m_iBorderMargin * 2;
	int iColorAreaHeight = r.height() - m_iBorderMargin * 2;
	QLinearGradient hGradient(0, 0, iColorAreaWidth, 0);
	hGradient.setColorAt(1, hColor);
	hGradient.setColorAt(0, QColor("#ffffff"));
	painter.setBrush(QBrush(hGradient));
	painter.drawRoundedRect(m_iBorderMargin, m_iBorderMargin, iColorAreaWidth, iColorAreaHeight, 5, 5);

	// 垂直方向亮度变化(可通过透明度设置)
	QLinearGradient vGradient(0, 0, 0, iColorAreaHeight);
	vGradient.setColorAt(0, QColor(0, 0, 0, 0));
	vGradient.setColorAt(1, QColor(0, 0, 0, 255));
	painter.setBrush(QBrush(vGradient));
	painter.drawRoundedRect(m_iBorderMargin, m_iBorderMargin, iColorAreaWidth, iColorAreaHeight, 5, 5);

	painter.setPen(QPen(m_iBrightness > 128 ? Qt::black : Qt::white, 2));
	painter.setBrush(Qt::transparent);
	QPointF selecPos = QPointF(m_iBorderMargin + (double)m_iSaturation / 255 * iColorAreaWidth, m_iBorderMargin + (double)(255 - m_iBrightness) / 255 * iColorAreaHeight);
	painter.drawEllipse(selecPos, 8, 8);
}

void CSVColorArea::mousePressEvent(QMouseEvent *ev)
{
	QRect r = this->rect();
	int iColorAreaWidth = r.width() - m_iBorderMargin * 2;
	int iColorAreaHeight = r.height() - m_iBorderMargin * 2;
	m_iSaturation = qBound(0, qRound((double)(ev->x() - m_iBorderMargin) / iColorAreaWidth * 255), 255);
	m_iBrightness = qBound(0, 255 - qRound((double)(ev->y() - m_iBorderMargin) / iColorAreaHeight * 255), 255);
	emit SvManualChangedSignal(m_iHue, m_iSaturation, m_iBrightness);
	update();
}

void CSVColorArea::mouseMoveEvent(QMouseEvent *ev)
{
	QRect r = this->rect();
	int iColorAreaWidth = r.width() - m_iBorderMargin * 2;
	int iColorAreaHeight = r.height() - m_iBorderMargin * 2;
	m_iSaturation = qBound(0, qRound((double)(ev->x() - m_iBorderMargin) / iColorAreaWidth * 255), 255);
	m_iBrightness = qBound(0, 255 - qRound((double)(ev->y() - m_iBorderMargin) / iColorAreaHeight * 255), 255);
	emit SvManualChangedSignal(m_iHue, m_iSaturation, m_iBrightness);
	update();
}


void CSVColorArea::hueChangedSlot(int h)
{
	m_iHue = h;
	update();
}