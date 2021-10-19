#include "RadarChart.h"

#include <QPainter>
#include <QtCore\qmath.h>

RadarChart::RadarChart(QWidget *parent)
	: QWidget(parent)
	, m_ringColor1(QColor(110, 110, 110))
	, m_ringColor2(QColor(255, 122, 50))
	, m_lastColor(QColor("#06161e"))
	, m_insidePen(QPen(QColor(110, 110, 110,100), 2, Qt::SolidLine))
	, m_latPen(QPen(QColor(110, 110, 110, 100), 3, Qt::DotLine))
	, m_backStartColor(QColor("#005298")), m_backEndColor(QColor("#00ffd8"))
	, m_lineStartColor(QColor("#3a6ece")), m_lineEndColor(QColor("#2fd0e3"))
	, m_bTextBorderOpen(true)
	, m_iRingCount(3)
{
	m_backStartColor.setAlpha(0.4*255);
	m_backEndColor.setAlpha(0.4 * 255);
}

RadarChart::~RadarChart()
{
}

void RadarChart::SetRingBackColor(const QColor &color1, const QColor &color2, const QColor &color3)
{
	m_ringColor1 = color1;
	m_ringColor2 = color2;
	m_lastColor = color3;

	update();
}

void RadarChart::SetPen(const QPen &pen1, const QPen &pen2)
{
	m_insidePen = pen1;
	m_latPen = pen2;

	update();
}

void RadarChart::SetBackGradient(const QColor &startColor, const QColor &endColor)
{
	m_backStartColor = startColor;
	m_backEndColor = endColor;

	update();
}

void RadarChart::SetLineGradient(const QColor &startColor, const QColor &endColor)
{
	m_lineStartColor = startColor;
	m_lineEndColor = endColor;

	update();
}

void RadarChart::SetTextBorderOpen(bool bOpen)
{
	m_bTextBorderOpen = bOpen;

	update();
}

void RadarChart::SetData(const QVector<RadarNode> &vecData)
{
	m_vecData = vecData;

	update();
}

void RadarChart::SetData(SEquipCap sEquipCap)
{
	m_vecData.clear();
	// 设置反锯齿
	RadarNode node;

	node.dScale = sEquipCap.dHit;
	node.qName = "打击";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	node.dScale = sEquipCap.dCruise;
	node.qName = "巡航";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	node.dScale = sEquipCap.dPerformance;
	node.qName = "性能";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	node.dScale = sEquipCap.dEndurance;
	node.qName = "续航";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	node.dScale = sEquipCap.dCoordinate;
	node.qName = "协调";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	node.dScale = sEquipCap.dProbe;
	node.qName = "探测";
	node.fontColor = QColor(110, 110, 110);
	m_vecData.push_back(node);

	update();
}

void RadarChart::SetRingCount(int iCount)
{
	m_iRingCount = iCount;

	update();
}

void RadarChart::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	
	int iCount = m_vecData.count();		// 节点数目

	if (iCount != 6)
	{
		return;
	}

	int iRadius = (width() > height() ? height() : width() - 40) / 2;		// 圆的半径，取 宽/2 / 高 最小值
	QPointF center(width() / 2, height() / 2);
	painter.translate(center);

	int iCountGrid = 72;
	qreal qAngle = 360.0 / iCountGrid;//iCount;
	

	// 绘制环形
	int iGridWidth = (iRadius * 2 / 3) / (m_iRingCount - 1 + 0.5);

	// 绘制内框（从外至内）
	for (int i = 0; i < m_iRingCount + 1; i++)
	{
		QPolygonF polygon;

		if (i == 0)		// 画边框
		{
			for (int j = 0; j < iCountGrid; j++)
			{

				polygon << CalPosition((90 + j * qAngle), iRadius);
			}
			painter.setPen(m_latPen);
			painter.setBrush(m_lastColor);
		}
		else
		{
			for (int j = 0; j < iCountGrid; j++)
			{

				polygon << CalPosition((90 + j * qAngle), iRadius - iGridWidth / 2 - (i - 1) * iGridWidth);
			}

			painter.setPen(m_insidePen);
			if (i == 1)			// 除去边框的第一层没有线
			{
				painter.setPen(Qt::NoPen);
			}

			if (i % 2 == 0)		// 隔行换色
			{
				painter.setBrush(m_ringColor1);
			}
			else
			{
				painter.setBrush(m_ringColor2);
			}
		}

		
		painter.drawPolygon(polygon);
	}
	qreal qAngleLine = 360.0 / iCount;
	// 绘制扩散线
	for (int i = 0; i < iCount; i++)
	{
		QPointF pos = CalPosition((90 - i * qAngleLine), iRadius);
		painter.drawLine(QPointF(0, 0), pos);
	}

	// 绘制数据
	QPainterPath path;

	path.moveTo(CalPosition((90), iRadius * m_vecData[0].dScale));

	painter.setPen(m_insidePen);
	for (int i = 1; i < iCount; i++)
	{
		path.lineTo(CalPosition((90 - i * qAngleLine), iRadius * m_vecData[i].dScale));
	}

	path.lineTo(CalPosition((90), iRadius * m_vecData[0].dScale));		// 闭合，所以需要再连接到起始点


	// 计算渐变极点（坐下到右上）
	QPointF bottomLeft = CalPosition((90 - 4 * qAngleLine), iRadius * m_vecData[4].dScale);
	QPointF topRight = CalPosition((90 - qAngleLine), iRadius * m_vecData[1].dScale);
	for (int i = 0; i < iCount; i++)
	{
		double d = qAbs(qSin((90 - i * qAngleLine) * M_PI / 180));
		double dValue = iRadius * m_vecData[i].dScale * d;			// 隐射到 右上-左下

		if (i < iCount / 2)		// 右上部分
		{
			if (dValue > iRadius * m_vecData[1].dScale)
			{
				topRight = CalPosition((90 - qAngleLine), dValue);
			}
		}
		else					// 左下部分
		{
			if (dValue > iRadius * m_vecData[4].dScale)
			{
				bottomLeft = CalPosition((90 - 4 * qAngleLine), dValue);
			}
		}
	}
	// 线渐变
	QLinearGradient penGradient(bottomLeft, topRight);
	penGradient.setColorAt(0, m_lineStartColor);
	penGradient.setColorAt(1, m_lineEndColor);
	painter.setPen(QPen(QBrush(penGradient), 2));

	// 填充渐变	
	QLinearGradient brushGradient(bottomLeft, topRight);
	brushGradient.setColorAt(0, m_backStartColor);
	brushGradient.setColorAt(1, m_backEndColor);
	painter.setBrush(QBrush(brushGradient));

	painter.drawPath(path);


	// 绘制数值
	QFont font("微软雅黑");
	font.setPixelSize(12);
	painter.setFont(font);
	QFontMetrics fm(font);

	if (m_bTextBorderOpen)		// 有边框、有底色
	{
		for (int i = 0; i < iCount; i++)
		{
			QPointF pos = CalPosition((90 - i * qAngleLine), iRadius * m_vecData[i].dScale);
			QString str = m_vecData[i].qName;//QString("%1%").arg((int)(m_vecData[i].dScale * 100));

			qreal qTextW = fm.width(str);

			qreal qX = pos.x() - (qTextW + qTextW/ 2) / 2;		// 预留文字宽度的一半用来画圈
			qreal qY = pos.y() - fm.height() / 2;

			if (qX < -iRadius)
			{
				qX = iRadius;
			}
			else if (qX + qTextW + qTextW / 2 > iRadius)
			{
				qX = iRadius - qTextW - qTextW / 2;
			}

			if (qY < -iRadius)
			{
				qY = -iRadius;
			}
			else if (qY + fm.height() > iRadius)
			{
				qY = iRadius - fm.height();
			}

			QRectF rect(qX, qY, qTextW * 3 / 2, fm.height());

			// 绘制底
			painter.setPen(m_vecData[i].borderColor);
			painter.setBrush(m_vecData[i].backColor);
			painter.drawRect(rect);

			int iPointRadius = qTextW / 12;
			// 绘制点
			painter.setPen(Qt::NoPen);
			painter.setBrush(m_vecData[i].backColor);
			painter.drawEllipse(QPointF(qX + iPointRadius*3, qY + fm.height()/2), iPointRadius, iPointRadius);

			// 绘制文字
			painter.setPen(m_vecData[i].fontColor);
			rect = QRectF(qX + iPointRadius * 6, qY, qTextW, fm.height());
			painter.drawText(rect, Qt::AlignCenter, str);
		}
	}
	else
	{
		for (int i = 0; i < iCount; i++)
		{

			QPointF pos = CalPosition((90 - i * qAngleLine), iRadius * m_vecData[i].dScale);
			painter.setPen(m_backStartColor);
			painter.setBrush(m_backStartColor);
			painter.drawEllipse(pos, iGridWidth / 10, iGridWidth / 10);

			QPointF posText = CalPosition((90 - i * qAngleLine), iRadius);
			// 绘制文字
			QString str = m_vecData[i].qName;//QString("%1%").arg((int)(m_vecData[i].dScale * 100));
			qreal qTextW = fm.width(str);
			qreal qX = posText.x() - qTextW / 2;		// 预留文字宽度的一半用来画圈
			qreal qY = posText.y() - fm.height() / 2;


			QRectF rect(qX, qY, qTextW * 3 / 2, fm.height());
			painter.setPen(m_vecData[i].fontColor);
			painter.setBrush(m_vecData[i].fontColor);
			painter.drawEllipse(posText, iGridWidth / 7, iGridWidth / 7);
			painter.setPen(m_vecData[i].fontColor);
			painter.drawText(rect, Qt::AlignCenter, str);
// 			if (qX < -iRadius)
// 			{
// 				qX = iRadius;
// 			}
// 			else if (qX + qTextW + qTextW / 2 > iRadius)
// 			{
// 				qX = iRadius - qTextW - qTextW / 2;
// 			}
// 
// 			if (qY < -iRadius)
// 			{
// 				qY = -iRadius;
// 			}
// 			else if (qY + fm.height() > iRadius)
// 			{
// 				qY = iRadius - fm.height();
// 			}
// 
// 			if ((int)(qAngle * i) % 180 == 0)
// 			{
// 				if (qX < 0)
// 				{
// 					qX += qTextW / 2;
// 				}
// 				else if (qX > 0)
// 				{
// 					qX -= qTextW / 2;
// 				}
// 			}
// 			else
// 			{
// 				if (qY < 0)
// 				{
// 					qY += fm.height() / 2;
// 				}
// 				else if (qY > 0)
// 				{
// 					qY -= fm.height() / 2;
// 				}
// 			}

// 			
// 
// 			QRectF rect(qX, qY, qTextW * 3 / 2, fm.height());
// 
// 			painter.setPen(m_vecData[i].fontColor);
// 			painter.drawText(rect, Qt::AlignCenter, str);
		}
	}
	

}

QPointF RadarChart::CalPosition(double dAngle, int iRadius)
{
	qreal qX = iRadius * qSin(dAngle * M_PI / 180);
	qreal qY = -iRadius * qCos(dAngle * M_PI / 180);
	return QPointF(qX, qY);
}