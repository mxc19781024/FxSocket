#include "FxTimePlot.h"
#include <QDateTime>

#pragma execution_character_set("utf-8")

FxTimePlot::FxTimePlot(QWidget *parent)
	: QCustomPlot(parent)
{
	setAttribute(Qt::WA_OpaquePaintEvent, false);
	setBackground(Qt::transparent);
}

FxTimePlot::~FxTimePlot()
{
}

int FxTimePlot::addLineGraph(const QString & title, const QString & key, const QString & value, const QColor &color)
{
	int index = m_pLineGraphs.size();

	if (axisRectCount() <= index)
	{
		QCPAxisRect * bottomRect = new QCPAxisRect(this);
		plotLayout()->addElement(1, 0, bottomRect);
	}

	QCPGraph * pLineGraph = addGraph(axisRect(index)->axis(QCPAxis::atBottom), axisRect(index)->axis(QCPAxis::atLeft));
	QSharedPointer<QCPAxisTickerDateTime> keyTicker = QSharedPointer<QCPAxisTickerDateTime>(new QCPAxisTickerDateTime);
	keyTicker.data()->setDateTimeFormat(QString("hh:mm"));
	pLineGraph->keyAxis()->setTicker(keyTicker);
	pLineGraph->keyAxis()->setRange(QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime()), QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime().addDays(1)));
	pLineGraph->keyAxis()->setScaleType(QCPAxis::stLinear);
	setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	axisRect(index)->setRangeZoomAxes(pLineGraph->keyAxis(), NULL);

	QFont font(QString::fromLocal8Bit("微软雅黑"));
	font.setPixelSize(12);

	pLineGraph->keyAxis()->setBasePen(Qt::NoPen);
	pLineGraph->keyAxis()->setTickLabelColor(Qt::white);
	pLineGraph->keyAxis()->setLabelColor(Qt::white);
	pLineGraph->keyAxis()->setLabelFont(font);
	pLineGraph->keyAxis()->setTickLabelFont(font);
	pLineGraph->keyAxis()->setTickPen(Qt::NoPen);
	pLineGraph->keyAxis()->setSubTicks(false);
	pLineGraph->valueAxis()->setBasePen(Qt::NoPen);
	pLineGraph->valueAxis()->setTickLabelColor(Qt::white);
	pLineGraph->valueAxis()->setLabelColor(Qt::white);
	pLineGraph->valueAxis()->setTickPen(Qt::NoPen);
	pLineGraph->valueAxis()->setSubTicks(false);
	pLineGraph->valueAxis()->setLabelFont(font);
	pLineGraph->valueAxis()->setTickLabelFont(font);
	pLineGraph->keyAxis()->grid()->setPen(QPen(QColor("#2c5d77"), 1.0, Qt::DashLine));
	pLineGraph->valueAxis()->grid()->setPen(QPen(QColor("#2c5d77"), 1.0, Qt::DashLine));

	// 线样式
	QColor lineColor = color;
	QPen linePen(color, 2.0);
	pLineGraph->setPen(linePen);
	lineColor.setAlpha(128);

	// 节点样式
	QCPScatterStyle scatterStyle(QCPScatterStyle::ssCircle, linePen, Qt::white, 8.0);
	pLineGraph->setScatterStyle(scatterStyle);

	// 填充样式
	QLinearGradient gradient(0, 0, 0, 1);
	gradient.setColorAt(0, lineColor);
	gradient.setColorAt(1.0, Qt::transparent);
	gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
	pLineGraph->setBrush(gradient);

	// 设置轴名称
	pLineGraph->keyAxis()->setLabel(key);
	pLineGraph->valueAxis()->setLabel(value);

	// 标题
	QCPAxis *rightAxis = axisRect(index)->addAxis(QCPAxis::atRight);
	rightAxis->setBasePen(Qt::NoPen);
	rightAxis->setTicks(false);
	rightAxis->setSubTicks(false);
	rightAxis->setTickLabels(false);
	rightAxis->setLabelColor(QColor("#ffba03"));
	font.setPixelSize(14);
	rightAxis->setLabelFont(font);
	rightAxis->setLabel(title);

	m_pLineGraphs.append(pLineGraph);

	return index;
}

void FxTimePlot::setValueRange(int index, qreal min, qreal max)
{
	if (index < m_pLineGraphs.size())
	{
		m_pLineGraphs[index]->valueAxis()->setRange(min, max);
	}
}

void FxTimePlot::setTimeRange(int index, const QDateTime & min, const QDateTime & max)
{
	if (index < m_pLineGraphs.size())
	{
		m_pLineGraphs[index]->keyAxis()->setRange(QCPAxisTickerDateTime::dateTimeToKey(min), QCPAxisTickerDateTime::dateTimeToKey(max));
	}
}

void FxTimePlot::addPoint(int index, const QDateTime &key, qreal value)
{
	if (index < m_pLineGraphs.size())
	{
		m_pLineGraphs[index]->addData(QCPAxisTickerDateTime::dateTimeToKey(key), value);
		replot();
	}
}
