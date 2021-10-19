#include "ProgressContainer.h"

#include <QPainter>
#include <QStyleOption>
#include <QtCore\qmath.h>
#include "spline.h"

#define Margin 2

ProgressContainer::ProgressContainer(QWidget *parent)
	: QWidget(parent)
	, m_type(SimpleCircle1)
	, m_dTotal(100), m_dValue(50)
	, m_ringBackColor(QColor("#163240")), m_ringColor(QColor("#20929c"))
	, m_rippleBackColor(QColor("#163240")), m_rippleColor(QColor("#47c9f3"))
	, m_rippleBorderPen(Qt::NoPen), m_ringBorderPen(Qt::NoPen)
	, m_bRingGradient(false), m_ringStartColor(QColor("#224e9d")), m_ringEndColor(QColor("#20929c"))
	, m_bRippleGradient(false), m_rippleStartColor(QColor("#47c9f3")), m_rippleEndColor(QColor("#134d62"))
	, m_font(QFont("微软雅黑"))
	, m_iOffset(0)
	, m_iDensity(2)
	, m_rollIndex1(0), m_rollIndex2(0)
{
	startTimer(80);

	m_points1.resize(4 * m_iDensity + 1);
	m_points2.resize(4 * m_iDensity + 1);
}

ProgressContainer::~ProgressContainer()
{
}

void ProgressContainer::SetContainerType(BallType type)
{
	m_type = type;

	update();
}

void ProgressContainer::SetTotal(double dTotal)
{
	m_dTotal = dTotal;

	update();
}

void ProgressContainer::SetValue(double dValue)
{
	m_dValue = dValue;

	update();
}

void ProgressContainer::SetRingColor(const QColor &ringBackColor, const QColor &ringColor)
{
	m_ringBackColor = ringBackColor;
	m_ringColor = ringColor;

	update();
}

void ProgressContainer::SetRippleColor(const QColor &rippleBackColor, const QColor &rippleColor)
{
	m_rippleBackColor = rippleBackColor;
	m_rippleColor = rippleColor;

	update();
}

void ProgressContainer::SetRingBorderPen(const QPen &pen)
{
	m_ringBorderPen = pen;

	update();
}

void ProgressContainer::SetRippleBorderPen(const QPen &pen)
{
	m_rippleBorderPen = pen;

	update();
}

void ProgressContainer::SetRingGradient(bool bOpen, const QColor &startColor, const QColor &endColor)
{
	m_bRingGradient = bOpen;
	m_ringStartColor = startColor;
	m_ringEndColor = endColor;

	update();
}

void ProgressContainer::SetRippleGradient(bool bOpen, const QColor &startColor, const QColor &endColor)
{
	m_bRippleGradient = bOpen;
	m_rippleStartColor = startColor;
	m_rippleEndColor = endColor;

	update();
}

void ProgressContainer::setWaveDensity(int iDensity)
{
	if (iDensity > 0 && iDensity != m_iDensity)
	{
		m_points1.resize(4 * iDensity + 1);
		m_points2.resize(4 * iDensity + 1);
		m_iDensity = iDensity;
		update();
	}
}

void ProgressContainer::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);

	switch (m_type)
	{
	case SimpleCircle1:
		DrawSimpleCircle1(painter);
		break;
	case SimpleCircle2:
		DrawSimpleCircle2(painter);
		break;
	case SimpleSquare1:
	case SimpleSquare2:
		DrawSimpleSquare(painter);
		break;
	}
}

void ProgressContainer::resizeEvent(QResizeEvent *event)
{
	m_curve.clear();

	switch (m_type)
	{
	case SimpleCircle1:
	{
		int iRadius = (width() > height() ? height() : width()) / 2;		// Բ�İ뾶��ȡ ��/�� ��Сֵ
		int iLineWidth = iRadius / 12;		// ��������ܿ�ȵı���
		m_iActualRadius = iRadius - iLineWidth * 3;

	}
	break;
	case SimpleCircle2:
	{
		int iRadius = (width() > height() ? height() : width()) / 2 - Margin;		// Բ�İ뾶��ȡ ��/�� ��Сֵ
		int iLineWidth = iRadius * (180 - 144) / 180;		// ��������ܿ�ȵı���
		m_iActualRadius = iRadius - iLineWidth * 3 / 2.0 - 2;

	}
	break;
	case SimpleSquare1:
	case SimpleSquare2:
	{
		int iRadius = (width() > height() ? height() : width()) / 2;		// Բ�İ뾶��ȡ ��/�� ��Сֵ
		m_iActualRadius = width() / 2;

	}
	break;
	}

	double dWidth = M_PI * m_iDensity / m_iActualRadius;
	double dRectHeight = m_iActualRadius / 10.0;
	double dBaseHeight = (m_dTotal - m_dValue) * m_iActualRadius * 2.0 / m_dTotal;
	double dPercent = m_iActualRadius / m_iDensity / 2.0;
	for (int i = 0; i < m_points1.count(); i++)
	{
		m_points1[i].setX(i * dPercent + width() / 2 - m_iActualRadius);
		m_points2[i].setX(i * dPercent + width() / 2 - m_iActualRadius);

		double dHeight = dBaseHeight + dRectHeight * qSin(dWidth * (m_points1[i].x() - dPercent * 2 + m_iOffset));
		m_points1[i].setY(dHeight + height() / 2 - m_iActualRadius);

		dHeight = dBaseHeight + dRectHeight * qSin(dWidth * (m_points1[i].x() - m_iOffset));
		m_points2[i].setY(dHeight + height() / 2 - m_iActualRadius);
	}
}

void ProgressContainer::timerEvent(QTimerEvent *event)
{
	// �ı�ƫ��ֵ
	if (++m_rollIndex1 > 2 * m_iActualRadius)
	{
		m_rollIndex1 = 0;
	}

	if (--m_rollIndex2 < 0)
	{
		m_rollIndex2 = 2 * m_iActualRadius;
	}

	int iRadius = (width() > height() ? height() : width()) / 2;		// Բ�İ뾶��ȡ ��/�� ��Сֵ

	double dWidth = M_PI * m_iDensity / m_iActualRadius;
	double dRectHeight = m_iActualRadius / 10.0;
	double dBaseHeight = (m_dTotal - m_dValue) * m_iActualRadius * 2.0 / m_dTotal;
	double dPercent = m_iActualRadius / m_iDensity / 2.0;
	for (int i = 0; i < m_points1.count(); i++)
	{

		double dHeight = dBaseHeight + dRectHeight * qSin(dWidth * (m_points1[i].x() - dPercent * 2 + m_iOffset));
		m_points1[i].setY(dHeight + height() / 2 - m_iActualRadius);

		dHeight = dBaseHeight + dRectHeight * qSin(dWidth * (m_points1[i].x() - m_iOffset));

		m_points2[i].setY(dHeight + height() / 2 - m_iActualRadius);
	}

	m_iOffset += iRadius / 10 / m_iDensity;
	if (m_iOffset > iRadius * 2)
	{
		m_iOffset -= iRadius * 2;
	}

	update();
}

void ProgressContainer::DrawSimpleCircle1(QPainter & painter)
{
	int iRadius = (width() > height() ? height() : width()) / 2;		// Բ�İ뾶��ȡ ��/�� ��Сֵ
	int iLineWidth = iRadius / 12;		// ��������ܿ�ȵı���

	QPointF center(width() / 2, height() / 2);		// Բ��

	double dScale = m_dValue / m_dTotal;

	painter.save();
	painter.translate(center);

	// ���ƻ�
	QBrush brush;
	if (m_bRingGradient)	// ���价�����ϵ��£�
	{
		QLinearGradient gradient(0, -iRadius, 0, iRadius);
		gradient.setColorAt(0, m_ringStartColor);
		gradient.setColorAt(1, m_ringEndColor);
		brush = QBrush(gradient);
	}
	else
	{
		brush = m_ringColor;
	}

	painter.setPen(QPen(brush, iLineWidth));
	painter.drawEllipse(QPointF(0, 0), iRadius - iLineWidth / 2, iRadius - iLineWidth / 2);


	// ����Բ��
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_rippleBackColor);
	painter.drawEllipse(QPointF(0, 0), iRadius - iLineWidth * 2, iRadius - iLineWidth * 2);
	painter.restore();

	painter.save();
	QPainterPath ball;
	QRectF rect(width() / 2 - m_iActualRadius, height() / 2 - m_iActualRadius, 2 * m_iActualRadius, 2 * m_iActualRadius);
	ball.addEllipse(rect);


	// ������͸����Զ����͸��
	QBrush brush1;
	QBrush brush2;
	//painter.setBrush(Qt::red);
	//painter.drawRect(rect);
	if (m_bRippleGradient)
	{
		qreal qTopY = rect.bottomLeft().y() - dScale * m_iActualRadius * 2 - m_iActualRadius / 10.0;

		// ��������
		m_rippleStartColor.setAlphaF(0.6);
		m_rippleEndColor.setAlphaF(0.6);
		QLinearGradient gradient1(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient1.setColorAt(0, m_rippleStartColor);
		gradient1.setColorAt(1, m_rippleEndColor);
		brush1 = QBrush(gradient1);

		// Զ������
		QLinearGradient gradient2(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient2.setColorAt(0, m_rippleStartColor);
		gradient2.setColorAt(1, m_rippleEndColor);
		brush2 = QBrush(gradient2);
	}
	else
	{
		m_rippleColor.setAlphaF(0.6);
		brush1 = QBrush(m_rippleColor);
		m_rippleColor.setAlphaF(1);

		brush2 = QBrush(m_rippleColor);
	}


	if (dScale > 0 && dScale < 1)
	{
		QVector<QPointF> contrlPoints = calculateControlPoints(m_points1);
		QPainterPath path1;
		path1.moveTo(m_points1.first());
		for (int i = 0; i < m_points1.size() - 1; i++)
		{
			QPointF pos = m_points1.at(i + 1);
			path1.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}

		path1.lineTo(rect.bottomRight());
		path1.lineTo(rect.bottomLeft());
		path1.closeSubpath();
		painter.setPen(Qt::NoPen);
		//QColor color = m_waveColor;
		//color.setAlpha(255 * 0.8);
		painter.setBrush(brush2);
		painter.drawPath(path1.intersected(ball));

		contrlPoints = calculateControlPoints(m_points2);
		QPainterPath path2;
		path2.moveTo(m_points2.first());
		for (int i = 0; i < m_points2.size() - 1; i++)
		{
			QPointF pos = m_points2.at(i + 1);
			path2.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}

		path2.lineTo(rect.bottomRight());
		path2.lineTo(rect.bottomLeft());
		path2.closeSubpath();
		//color.setAlpha(255 * 0.6);
		painter.setBrush(brush1);
		painter.drawPath(path2.intersected(ball));

	}
	else
	{
		if (dScale == 1)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(brush2);
			painter.drawEllipse(rect);
		}
	}
	painter.restore();

	// ����ֵ
	painter.save();
	painter.translate(center);
	painter.setPen(Qt::white);
	QString str = QString("%1%").arg((int)(dScale * 100));
	m_font.setPixelSize(m_iActualRadius / 2);
	painter.setFont(m_font);
	QRectF textRect(-m_iActualRadius, -m_iActualRadius, 2 * m_iActualRadius, 2 * m_iActualRadius);
	painter.drawText(textRect, Qt::AlignCenter, str);
	painter.restore();
}

void ProgressContainer::DrawSimpleCircle2(QPainter & painter)
{
	int iRadius = (width() > height() ? height() : width()) / 2 - Margin;		// Բ�İ뾶��ȡ ��/�� ��Сֵ
	int iLineWidth = iRadius * (180 - 144) / 180;		// ��������ܿ�ȵı���

	QPointF center(width() / 2, height() / 2);		// Բ��

	painter.save();
	painter.translate(center);

	double dScale = m_dValue / m_dTotal;

	// ���ƻ�
	QPainterPath ringPath;
	ringPath.addEllipse(QPointF(0, 0), iRadius, iRadius);
	ringPath.addEllipse(QPointF(0, 0), iRadius - iLineWidth, iRadius - iLineWidth);
	QBrush brush;
	if (m_bRingGradient)	// ���价�����ϵ��£�
	{
		QLinearGradient gradient(0, -iRadius, 0, iRadius);
		gradient.setColorAt(0, m_ringStartColor);
		gradient.setColorAt(1, m_ringEndColor);
		brush = QBrush(gradient);
	}
	else
	{
		brush = m_ringColor;
	}

	QRectF rect(-iRadius + iLineWidth / 2, -iRadius + iLineWidth / 2, 2 * iRadius - iLineWidth, 2 * iRadius - iLineWidth);
	painter.setPen(QPen(brush, iLineWidth, Qt::SolidLine, Qt::FlatCap));
	painter.drawArc(rect, 270 * 16, -dScale * 360 * 16);

	painter.setPen(m_ringBorderPen);
	painter.drawPath(ringPath);

	// ����Բ��
	painter.setPen(m_rippleBorderPen);
	painter.setBrush(m_rippleBackColor);
	painter.drawEllipse(QPointF(0, 0), iRadius - iLineWidth * 3 / 2.0, iRadius - iLineWidth * 3 / 2.0);
	painter.restore();


	painter.save();
	QPainterPath ball;
	rect = QRectF(width() / 2 - m_iActualRadius, height() / 2 - m_iActualRadius, 2 * m_iActualRadius, 2 * m_iActualRadius);
	ball.addEllipse(rect);


	// ������͸����Զ����͸��
	QBrush brush1;
	QBrush brush2;
	//painter.setBrush(Qt::red);
	//painter.drawRect(rect);
	if (m_bRippleGradient)
	{
		qreal qTopY = rect.bottomLeft().y() - dScale * m_iActualRadius * 2 - m_iActualRadius / 10.0;

		// ��������
		m_rippleStartColor.setAlphaF(0.6);
		m_rippleEndColor.setAlphaF(0.6);
		QLinearGradient gradient1(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient1.setColorAt(0, m_rippleStartColor);
		gradient1.setColorAt(1, m_rippleEndColor);
		brush1 = QBrush(gradient1);

		// Զ������
		QLinearGradient gradient2(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient2.setColorAt(0, m_rippleStartColor);
		gradient2.setColorAt(1, m_rippleEndColor);
		brush2 = QBrush(gradient2);
	}
	else
	{
		m_rippleColor.setAlphaF(0.6);
		brush1 = QBrush(m_rippleColor);
		m_rippleColor.setAlphaF(1);

		brush2 = QBrush(m_rippleColor);

	}

	if (dScale > 0 && dScale < 1)
	{
		QVector<QPointF> contrlPoints = calculateControlPoints(m_points1);
		QPainterPath path1;
		path1.moveTo(m_points1.first());
		for (int i = 0; i < m_points1.size() - 1; i++)
		{
			QPointF pos = m_points1.at(i + 1);
			path1.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}


		path1.lineTo(rect.bottomRight());
		path1.lineTo(rect.bottomLeft());
		path1.closeSubpath();
		painter.setPen(Qt::NoPen);
		//QColor color = m_waveColor;
		//color.setAlpha(255 * 0.8);
		painter.setBrush(brush2);
		painter.drawPath(path1.intersected(ball));

		contrlPoints = calculateControlPoints(m_points2);
		QPainterPath path2;
		path2.moveTo(m_points2.first());
		for (int i = 0; i < m_points2.size() - 1; i++)
		{
			QPointF pos = m_points2.at(i + 1);
			path2.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}

		path2.lineTo(rect.bottomRight());
		path2.lineTo(rect.bottomLeft());
		path2.closeSubpath();
		//color.setAlpha(255 * 0.6);
		painter.setBrush(brush1);
		painter.drawPath(path2.intersected(ball));

	}
	else
	{
		if (dScale == 1)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(brush2);
			painter.drawEllipse(rect);
		}
	}
	painter.restore();

	// ����ֵ
	painter.save();
	painter.translate(center);
	painter.setPen(Qt::white);
	QString str = QString("%1%").arg((int)(dScale * 100));
	m_font.setPixelSize(m_iActualRadius / 2);
	painter.setFont(m_font);
	QRectF textRect(-m_iActualRadius, -m_iActualRadius, 2 * m_iActualRadius, 2 * m_iActualRadius);
	painter.drawText(textRect, Qt::AlignCenter, str);
	painter.restore();
}

void ProgressContainer::DrawSimpleSquare(QPainter & painter)
{
	QPointF center(width() / 2, height() / 2);		// Բ��

													//painter.translate(center);

	double dScale = m_dValue / m_dTotal;
	QRectF rect(0, 0, width(), height());

	painter.save();
	// ���Ƶ�
	painter.setPen(m_rippleBorderPen);
	painter.setBrush(m_rippleBackColor);

	// ���ˣ���QPainterPath���󽻹��ܣ�intersect)������������sin����
	QPainterPath roundedRect;

	// ���Ƶ�
	if (m_type == SimpleSquare1)
	{
		painter.drawRect(rect);
		roundedRect.addRect(rect);
	}
	else if (m_type == SimpleSquare2)
	{
		painter.drawRoundedRect(rect, 10, 10);
		roundedRect.addRoundedRect(rect, 10, 10);
	}


	// ������͸����Զ����͸��
	QBrush brush1;
	QBrush brush2;
	if (m_bRippleGradient)
	{
		qreal qTopY = rect.bottomLeft().y() - dScale * m_iActualRadius * 2 - m_iActualRadius / 10.0;

		// ��������
		m_rippleStartColor.setAlphaF(0.6);
		m_rippleEndColor.setAlphaF(0.6);
		QLinearGradient gradient1(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient1.setColorAt(0, m_rippleStartColor);
		gradient1.setColorAt(1, m_rippleEndColor);
		brush1 = QBrush(gradient1);

		// Զ������
		QLinearGradient gradient2(width() / 2, qTopY, width() / 2, rect.bottomLeft().y());
		gradient2.setColorAt(0, m_rippleStartColor);
		gradient2.setColorAt(1, m_rippleEndColor);
		brush2 = QBrush(gradient2);
	}
	else
	{
		m_rippleColor.setAlphaF(0.6);
		brush1 = QBrush(m_rippleColor);
		m_rippleColor.setAlphaF(1);

		brush2 = QBrush(m_rippleColor);

	}

	if (dScale > 0 && dScale < 1)
	{
		QVector<QPointF> contrlPoints = calculateControlPoints(m_points1);
		QPainterPath path1;
		path1.moveTo(m_points1.first());
		for (int i = 0; i < m_points1.size() - 1; i++)
		{
			QPointF pos = m_points1.at(i + 1);
			path1.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}


		path1.lineTo(rect.bottomRight());
		path1.lineTo(rect.bottomLeft());
		path1.closeSubpath();
		painter.setPen(Qt::NoPen);
		//QColor color = m_waveColor;
		//color.setAlpha(255 * 0.8);
		painter.setBrush(brush2);
		painter.drawPath(path1.intersected(roundedRect));

		contrlPoints = calculateControlPoints(m_points2);
		QPainterPath path2;
		path2.moveTo(m_points2.first());
		for (int i = 0; i < m_points2.size() - 1; i++)
		{
			QPointF pos = m_points2.at(i + 1);
			path2.cubicTo(contrlPoints[2 * i], contrlPoints[2 * i + 1], pos);
		}

		path2.lineTo(rect.bottomRight());
		path2.lineTo(rect.bottomLeft());
		path2.closeSubpath();
		//color.setAlpha(255 * 0.6);
		painter.setBrush(brush1);
		painter.drawPath(path2.intersected(roundedRect));

	}
	else
	{
		if (dScale == 1)
		{
			painter.setPen(Qt::NoPen);
			painter.setBrush(brush2);
			painter.drawRect(rect);
		}
	}
	painter.restore();


	// ����ֵ
	painter.setPen(Qt::white);
	QString str = QString("%1%").arg((int)(dScale * 100));
	m_font.setPixelSize(m_iActualRadius / 2);
	painter.setFont(m_font);
	QRectF textRect(rect);
	painter.drawText(textRect, Qt::AlignCenter, str);
}

QVector<QPointF> ProgressContainer::calculateControlPoints(const QVector<QPointF> &points)
{
	QVector<QPointF> controlPoints;
	controlPoints.resize(points.count() * 2 - 2);

	int iCount = points.count() - 1;
	if (iCount == 1)
	{
		controlPoints[0].setX((2 * points[0].x() + points[1].x()) / 3);
		controlPoints[0].setY((2 * points[0].y() + points[1].y()) / 3);
		controlPoints[1].setX(2 * controlPoints[0].x() - points[0].x());
		controlPoints[1].setY(2 * controlPoints[0].y() - points[0].y());
		return controlPoints;
	}

	QVector<double> vector;
	vector.resize(iCount);

	vector[0] = points[0].x() + 2 * points[1].x();
	for (int i = 1; i < iCount - 1; i++)
	{
		vector[i] = 4 * points[i].x() + 2 * points[i + 1].x();
	}

	vector[iCount - 1] = (8 * points[iCount - 1].x() + points[iCount].x()) / 2.0;
	QVector<double> xControl = firstControlPoints(vector);

	vector[0] = points[0].y() + 2 * points[1].y();
	for (int i = 1; i < iCount - 1; i++)
	{
		vector[i] = 4 * points[i].y() + 2 * points[i + 1].y();
	}

	vector[iCount - 1] = (8 * points[iCount - 1].y() + points[iCount].y()) / 2.0;
	QVector<double> yControl = firstControlPoints(vector);

	for (int i = 0, j = 0; i < iCount; ++i, ++j)
	{
		controlPoints[j].setX(xControl[i]);
		controlPoints[j].setY(yControl[i]);

		j++;

		if (i < iCount - 1)
		{
			controlPoints[j].setX(2 * points[i + 1].x() - xControl[i + 1]);
			controlPoints[j].setY(2 * points[i + 1].y() - yControl[i + 1]);
		}
		else
		{
			controlPoints[j].setX((points[iCount].x() + xControl[iCount - 1]) / 2);
			controlPoints[j].setY((points[iCount].y() + yControl[iCount - 1]) / 2);
		}
	}

	return controlPoints;
}
