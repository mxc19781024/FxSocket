#pragma once

#include <QWidget>
#include "qcustomplot.h"

class FxTimePlot : public QCustomPlot
{
	Q_OBJECT

public:
	FxTimePlot(QWidget *parent = 0);
	~FxTimePlot();

	int addLineGraph(const QString &title, const QString &key, const QString &value, const QColor &color = QColor("#0aca22"));

	void setValueRange(int index, qreal min, qreal max);
	void setTimeRange(int index, const QDateTime &min, const QDateTime &max);

	void addPoint(int index, const QDateTime &key, qreal value);

private:
	QVector<QCPGraph*> m_pLineGraphs;
};
