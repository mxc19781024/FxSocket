#include <QPainter>
#include <FxGanttWidget.h>
#include <xutility>
#include "bdpUtil\GxSimulationTimer.h"
#include "bdpUtil\GxTimeManager.h"

CFxGanttWidget::CFxGanttWidget( CFxGanttChart* pGanttChart )
	:m_pGanttChart(pGanttChart)
{

}

CFxGanttWidget::~CFxGanttWidget()
{

}

void CFxGanttWidget::paintEvent( QPaintEvent* p )
{
	if (m_pGanttChart->m_dCurTotalTime > 0)
	{
		QPainter painter(this);
		//painter.fillRect(rect(), m_pGanttChart->m_colorBackGround);
		if (m_pGanttChart->m_nGanttTotalHeight>this->height())	
		{
			m_pGanttChart->m_bScrollBar = true;
		}
		else
		{
			m_pGanttChart->m_bScrollBar = false;
		}
		drawEvent(painter);
		drawPointer(painter);
		drawScrollBar(painter);
	}
	
	QWidget::paintEvent(p);
}

void CFxGanttWidget::wheelEvent( QWheelEvent* ev )
{
	m_pGanttChart->wheelEvent(ev);
	QWidget::wheelEvent(ev);
}

void CFxGanttWidget::mousePressEvent( QMouseEvent* ev )
{
	//if (m_pGanttChart->m_nBeginX < ev->pos().x())
	{
		if (m_pGanttChart->m_rectScrollBar.contains(ev->pos()))
		{
			m_pGanttChart->m_bDragScrollBar = true;
		}
		else
		{
			m_pGanttChart->m_bDragVertical = true;
		}
		m_pGanttChart->mousePressEvent(ev);
	}
	QWidget::mousePressEvent(ev);
}

void CFxGanttWidget::mouseReleaseEvent( QMouseEvent* ev )
{
	if (ev->button() == Qt::LeftButton)
	{
		m_pGanttChart->m_bDragVertical = false;
		m_pGanttChart->m_bDragScrollBar = false; 
	}
	QWidget::mouseReleaseEvent(ev);
}

void CFxGanttWidget::mouseMoveEvent( QMouseEvent* ev )
{
	m_pGanttChart->mouseMoveEvent(ev);
	QWidget::mouseMoveEvent(ev);
}

void CFxGanttWidget::mouseDoubleClickEvent( QMouseEvent* ev )
{
	m_pGanttChart->mouseDoubleClickEvent(ev);
	QWidget::mouseDoubleClickEvent(ev);
}

void CFxGanttWidget::drawEvent( QPainter& painter )
{
	double dInterval = double(m_pGanttChart->m_nObjectHeight);
	int nCurY = m_pGanttChart->m_nGanttY;
	int nBeginNum = - m_pGanttChart->m_nGanttY / dInterval;
	int nNum = ceil((double)this->height() / dInterval) + nBeginNum;
	int nCount = 0;
	for (auto itrObject = m_pGanttChart->m_mapIDToObjectInfo.begin(); itrObject != m_pGanttChart->m_mapIDToObjectInfo.end(); ++itrObject)
	{
		++nCount;
		if (nCount > nBeginNum)
		{
			itrObject->m_bDraw = true;
			painter.setPen(QColor(105, 106, 105));
			painter.setFont(QFont(QString::fromLocal8Bit("Microsoft YaHei"), 10, QFont::Normal));
			painter.drawText(0, nCurY, m_pGanttChart->m_nBeginX, m_pGanttChart->m_nObjectHeight, Qt::AlignCenter, itrObject->m_strName);
			if (m_pGanttChart->m_bGrid)
			{
				painter.drawLine(0, nCurY + m_pGanttChart->m_nObjectHeight, this->width(), nCurY + m_pGanttChart->m_nObjectHeight);
			}
			else
			{
				painter.drawLine(0, nCurY + m_pGanttChart->m_nObjectHeight, m_pGanttChart->m_nBeginX, nCurY + m_pGanttChart->m_nObjectHeight);
			}
			for (auto itrEvent = itrObject->m_mapIDToEventInfo.begin(); itrEvent != itrObject->m_mapIDToEventInfo.end(); ++itrEvent)
			{
				double dBeginTime = m_pGanttChart->JDToSecAtCurBeginTime(itrEvent->m_dBeginTime);
				double dEndTime = m_pGanttChart->JDToSecAtCurBeginTime(itrEvent->m_dEndTime);
				if (dEndTime < 0)
				{
					itrEvent->m_bDraw = false;
					continue;
				}
				else
				{
					dBeginTime = max(dBeginTime, 0.0);
					dEndTime = min(dEndTime, m_pGanttChart->m_dCurTotalTime*m_pGanttChart->m_nTotalGraduation / m_pGanttChart->m_nTimeGraduation);
					int nBeginX = m_pGanttChart->timeToX(dBeginTime);
					int nEndX = m_pGanttChart->timeToX(dEndTime);
					if (nBeginX > this->width())
					{
						itrEvent->m_bDraw = false;
						continue;
					}
					if (nEndX>(this->width() - m_pGanttChart->m_rectScrollBar.width())&&m_pGanttChart->m_bScrollBar)
					{
						nEndX -= m_pGanttChart->m_rectScrollBar.width();
					}
					itrEvent->m_bDraw = true;
					int nGanttY = nCurY + (m_pGanttChart->m_nObjectHeight - m_pGanttChart->m_nGanttHeight) * 0.5;
					itrEvent->m_rectDraw = QRect(nBeginX, nGanttY, nEndX - nBeginX, m_pGanttChart->m_nGanttHeight);
					painter.setPen(QColor(0, 0, 0, 255));
					painter.drawRect(itrEvent->m_rectDraw);
					if (m_pGanttChart->m_bGradients)
					{
						QLinearGradient linear(nBeginX, nGanttY, nEndX, nGanttY);        //Ë®Æ½½¥±ä
						linear.setColorAt(0.0, itrEvent->m_colorEvent.rgb());
						linear.setColorAt(0.5, QColor(255, 255, 255));
						linear.setColorAt(1.0, itrEvent->m_colorEvent.rgb());
						QBrush brush(linear);
						painter.fillRect(itrEvent->m_rectDraw, brush);
					}
					else
					{
						painter.fillRect(itrEvent->m_rectDraw, itrEvent->m_colorEvent.rgb());
					}

				}
			}
		}
		else
		{
			itrObject->m_bDraw = false;
		}
		nCurY += m_pGanttChart->m_nObjectHeight;
		if (nCount > nNum)
		{
			break;
		}
	}
	painter.setPen(QColor(105, 106, 105));
	painter.drawLine(0, 0, this->width(), 0);
	painter.drawLine(m_pGanttChart->m_nBeginX, 0, m_pGanttChart->m_nBeginX, this->height());
}

void CFxGanttWidget::drawPointer( QPainter& painter )
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
		painter.setPen(m_pGanttChart->m_colorPointer);
		painter.drawLine(nDrawX, 0, nDrawX, height());
	}
}

void CFxGanttWidget::drawScrollBar( QPainter& painter )
{
	if (m_pGanttChart->m_bScrollBar)
	{
		painter.setPen(QColor(0, 0, 0));
		QRect rect(width()-m_pGanttChart->m_rectScrollBar.width(),0,m_pGanttChart->m_rectScrollBar.width(),height());
		painter.drawRect(rect);
		painter.fillRect(rect, QColor(128, 128, 128));
		painter.fillRect(m_pGanttChart->m_rectScrollBar, QColor(0, 0, 0));
	}
}
