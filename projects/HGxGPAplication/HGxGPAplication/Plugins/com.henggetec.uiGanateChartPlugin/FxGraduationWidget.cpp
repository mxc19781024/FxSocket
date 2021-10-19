#include <QPainter>
#include <FxGraduationWidget.h>
#include<QStyleOption>
#include "bdpUtil\GxSimulationTimer.h"
#include "bdpUtil\GxTimeManager.h"
CFxGraduationWidget::CFxGraduationWidget( CFxGanttChart* pGanttChart )
	:m_pGanttChart(pGanttChart)
{
	
}

CFxGraduationWidget::~CFxGraduationWidget()
{

}

void CFxGraduationWidget::paintEvent( QPaintEvent* p )
{
	if (m_pGanttChart->m_dCurTotalTime > 0)
	{
		QPainter painter(this);
		drawGraduation(painter);
		drawEventPoint(painter);
		drawPointer(painter);

		QStyleOption opt;
		opt.init(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	}

	QWidget::paintEvent(p);

}

void CFxGraduationWidget::wheelEvent( QWheelEvent* ev )
{
	m_pGanttChart->wheelEvent(ev);
	QWidget::wheelEvent(ev);
}

void CFxGraduationWidget::mousePressEvent( QMouseEvent* ev )
{
	if (m_pGanttChart->m_polygonPointer.containsPoint(ev->pos(),Qt::OddEvenFill))
	{
		m_pGanttChart->m_bDragPointer = true;
	}
	else if (m_pGanttChart->m_nBeginX < ev->pos().x())
	{
		m_pGanttChart->m_bDragHorizontal = true;
	}		 
	m_pGanttChart->mousePressEvent(ev);
	QWidget::mousePressEvent(ev);
}

void CFxGraduationWidget::mouseReleaseEvent( QMouseEvent* ev )
{
	if (ev->button() == Qt::LeftButton)
	{
		m_pGanttChart->m_bDragHorizontal = false;
		m_pGanttChart->m_bDragPointer = false;
	}
	m_pGanttChart->mouseReleaseEvent(ev);
	QWidget::mouseReleaseEvent(ev);
}

void CFxGraduationWidget::mouseMoveEvent( QMouseEvent* ev )
{
	m_pGanttChart->mouseMoveEvent(ev);
	QWidget::mouseMoveEvent(ev);
}

void CFxGraduationWidget::drawGraduation( QPainter& painter )
{
	m_pGanttChart->m_nTotalGraduation = width() - m_pGanttChart->m_nBeginX;
	m_pGanttChart->m_nTimeGraduation = m_pGanttChart->m_nTotalGraduation / m_pGanttChart->m_nLongX*m_pGanttChart->m_nLongX;//计算总刻度
	//painter.fillRect(QRect(0, 0, this->width(), this->height()), QColor(255,255,0));
	int nDelta = m_pGanttChart->m_dCurTotalTime / m_pGanttChart->m_nTimeGraduation;//刻度时间间隔
	int nLongY = this->height() - m_pGanttChart->m_nLongY;
	int nShortY = this->height() - m_pGanttChart->m_nShortY;

	QDateTime dateTime = m_pGanttChart->convertJDToDateTime(m_pGanttChart->m_dInitialBeginTime);
	painter.setPen(QColor(105, 106, 105));
	painter.setFont(QFont(QString::fromLocal8Bit("Microsoft YaHei"), 10, QFont::Normal));
	painter.drawText(0, 0, m_pGanttChart->m_nBeginX, this->height(), Qt::AlignHCenter, dateTime.toString("yyyy/MM/dd\nhh:mm:ss"));//绘制仿真时间

	QVector<QLine> lines;
	for (int nX = m_pGanttChart->m_nBeginX; nX < width(); nX += m_pGanttChart->m_nShortX)
	{
		if ((nX - m_pGanttChart->m_nBeginX) % m_pGanttChart->m_nLongX == 0)
		{
			QLine lineLong(nX, this->height(), nX, nLongY);
			lines.push_back(lineLong);
			int nHour = 0, nMin = 0, nSec = 0;
			m_pGanttChart->convertSecToHMS(m_pGanttChart->m_dCurBeginTime + m_pGanttChart->m_dCurTotalTime*(nX - m_pGanttChart->m_nBeginX) / m_pGanttChart->m_nTimeGraduation, nHour, nMin, nSec);
			painter.setPen(QColor(105, 106, 105, 255));
			painter.setFont(QFont(QString::fromLocal8Bit("Microsoft YaHei"), 9, QFont::Normal));
			painter.drawText(nX - 50, 0, 100, this->height()-m_pGanttChart->m_nLongY, Qt::AlignHCenter, m_pGanttChart->HMSToString(nHour, nMin, nSec));
		}
		else
		{
			QLine lineShort(nX, this->height(), nX, nShortY);
			lines.push_back(lineShort);
		}
	}
	painter.setPen(QPen(m_pGanttChart->m_colorGraduation));
	painter.drawLines(lines);	//绘制刻度
}

void CFxGraduationWidget::drawPointer( QPainter& painter )
{
	double dTime = m_pGanttChart->JDToSecAtCurBeginTime(HGxUtil::CGxTimeManager::Instance()->getOrCreateSimuCtrl(0)->getSimulateTimeCurrent());
	bool bDraw = true;
	if (dTime<0)
	{
		bDraw = false;
	}
	if (bDraw)
	{
		int nDrawX = m_pGanttChart->timeToX(dTime);
		m_pGanttChart->m_polygonPointer.translate(nDrawX - m_pGanttChart->m_polygonPointer.at(1).x(), 0);//滑块移动
		QPainterPath pPath;
		pPath.addPolygon(m_pGanttChart->m_polygonPointer);
		painter.fillPath(pPath, m_pGanttChart->m_colorPointer);
	}
}

void CFxGraduationWidget::drawEventPoint(QPainter& painter)
{
	auto itrObject = m_pGanttChart->m_mapIDToObjectInfo.begin();
	for (;itrObject != m_pGanttChart->m_mapIDToObjectInfo.end();itrObject++)
	{
		for (auto itrEvent = itrObject->m_mapIDToEventInfo.begin(); itrEvent != itrObject->m_mapIDToEventInfo.end(); ++itrEvent)
		{
			double dBeginTime = m_pGanttChart->JDToSecAtCurBeginTime(itrEvent->m_dBeginTime);
			{
				int nBeginX = m_pGanttChart->timeToX(dBeginTime);
				if (nBeginX > this->width() ||  nBeginX < m_pGanttChart->m_nBeginX)
				{
					continue;
				}
				QPoint point(nBeginX-m_pGanttChart->m_mapImage[itrEvent->m_strType]->width()/2
					,this->height()-m_pGanttChart->m_mapImage[itrEvent->m_strType]->height());
				itrEvent->m_point = point;
				painter.drawPixmap(point,*m_pGanttChart->m_mapImage[itrEvent->m_strType]);

			}
		}
	}
}
